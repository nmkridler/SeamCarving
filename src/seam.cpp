#include <seam.h>
#include <opencv2/highgui/highgui.hpp>
#include <algorithm>

Seam::Seam(cv::Size const &inSize) : 
m_size(inSize)
{
}

void Seam::accumulateEnergy(cv::Mat const &input) 
{

   // Convert the input to CV_32F
   cv::Mat fMat;
   input.convertTo(fMat,CV_32F);
   
   // Initalize the cost and state mats
   fMat.copyTo(m_cost);
   m_index = cv::Mat::zeros(fMat.size(),CV_32S);
   for(int j = 0; j < m_index.cols; ++j)
   {
      m_index.at<int>(0,j) = j;
   }

   // Image size
   int nCols = m_size.width;
   int nRows = m_size.height;

   // Initialize min/max variables
   double minVal = 0;
   double maxVal = 0;
   cv::Point minLoc(0,0);

   // Loop over the rows
   cv::Mat prevRow;
   for( int i = 1; i < nRows; ++i)
   {
      // Left Boundary Condition
      m_cost.row(i-1).copyTo(prevRow);
      cv::minMaxLoc(prevRow.colRange(cv::Range(0,1)),&minVal,&maxVal,&minLoc);
      m_cost.at<float>(i,0) = fMat.at<float>(i,0) + minVal;
      m_index.at<int>(i,0)  = minLoc.x;
  
      // Loop over the interior columns
      for( int j = 1; j < nCols-1; ++j)
      {
         cv::minMaxLoc(prevRow.colRange(cv::Range(j-1,j+1)),&minVal,&maxVal,&minLoc);
         m_cost.at<float>(i,j) = fMat.at<float>(i,j) + minVal; 
         m_index.at<int>(i,j) = j + minLoc.x - 1;
      }
      
      // Right Boundary condition
      cv::minMaxLoc(prevRow.colRange(cv::Range(nCols-2,nCols-1)),&minVal,&maxVal,&minLoc);
      m_cost.at<float>(i,nCols-1) = fMat.at<float>(i,nCols-1) + minVal;
      m_index.at<int>(i,nCols-1) = nCols + minLoc.x - 2;
   }

   // Calculate the optimal path
   calculatePath(); 

}

void Seam::calculatePath()
{
   // Determine where the min occured in the last row
   double minVal = 0;
   double maxVal = 0;
   cv::Point minLoc(0,0);  
   cv::Mat lastRow;
   m_cost.row(m_size.height-1).colRange(cv::Range(0,m_size.width-1)).copyTo(lastRow);
   cv::minMaxLoc(lastRow,&minVal,&maxVal,&minLoc);

   // Minimum energy
   m_energy = minVal;

   // Copy into the path mat
   m_index.col(0).copyTo(m_path);
   m_path.at<int>(m_size.height-1) = minLoc.x;
   
   int lastIdx = m_path.at<int>(m_size.height-1);
   // Now pull out the path
   for( int idx = m_size.height-1; idx > 0; --idx)
   {
      int thisIdx = m_index.at<int>(idx,lastIdx);
      m_path.at<int>(idx-1) = thisIdx;
      lastIdx = thisIdx;
   }

}

