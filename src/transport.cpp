#include <transport.h>

Transport::Transport( int const & row, int const & col) :
TransportSimple(row,col),
m_mapExists(false)
{
   m_path.clear();          // Reset the path vector
}

bool Transport::formMap( cv::Mat const & input )
{
   // Check bounds, only let it resize by 75%
   if( 3*input.rows/4 < m_rowReduc || 3*input.cols/4 < m_colReduc)
      return false;

   // Initialize the transport map to be all zeros
   m_map      = cv::Mat::zeros(cv::Size(m_colReduc+1,m_rowReduc+1),CV_32F);
   m_decision = cv::Mat::zeros(cv::Size(m_colReduc+1,m_rowReduc+1),CV_8U);
 
   // Create the grayscale
   cv::cvtColor(input, m_gray, CV_RGB2GRAY);

   // Work image
   cv::Mat workImgRow;
   m_gray.copyTo(workImgRow);

   // For each row, we need a vector containing the images 
   // of the previous row
   std::vector<cv::Mat> prevRow;

   // Initialize the path
   m_path.clear();
   m_path.push_back(workImgRow.row(0)); // This is just a place holder

   // Start with T(0,0) = 0
   for( int i = 0; i <= m_rowReduc; ++i)
   {
      // We also need to save the image at the start of the row
      cv::Mat startRowImg;

      // On the edge there is no choice, so calculate the cost
      if( i > 0 )
      {
         float hCost = 0;
         cv::Mat cutImg, path;
         cv::Size inSize(input.rows-i+1,input.cols);
         calculateSeamCost( workImgRow, cutImg, path, hCost, false, inSize);
         m_map.at<float>(i,0) = m_map.at<float>(i-1,0) + hCost;
         m_decision.at<uint8_t>(i,0) = 1; // 1 indicates horizontal
         m_path.push_back(path);

         // We need to save the cutImg
         cutImg.copyTo(workImgRow);
         cutImg.copyTo(startRowImg);
      }
      else
      {
         workImgRow.copyTo(startRowImg);
      }

      std::vector<cv::Mat> thisRow;
      thisRow.clear();
      // Loop over columns
      for( int j=1; j <= m_colReduc; ++j)
      {
         if( i == 0) // vertical cost only
         {  
            float vCost = 0;
            cv::Mat cutImg, path; 
            cv::Size inSize(input.cols-j+1,input.rows);
            calculateSeamCost( workImgRow, cutImg, path, vCost , true,inSize); 
            
            // Update the map and the path
            m_path.push_back(path);
            m_map.at<float>(i,j) = m_map.at<float>(i,j-1) + vCost;
            m_decision.at<uint8_t>(i,j) = 0; // 0 indicates vertical

            // Copy the smaller image into the work buffer
            cutImg.copyTo(workImgRow);
            thisRow.push_back(cutImg);
         }
         else
         {
            // Calculate cost of a vertical cut of an image
            // of size rows - i, cols - j + 1
            float vCost = 0;
            cv::Mat vCutImg, vPath;
            cv::Size inSize(input.cols-j+1,input.rows-i);
            calculateSeamCost( workImgRow, vCutImg, vPath, vCost , true,inSize); 
            vCost += m_map.at<float>(i,j-1);
            vCutImg.copyTo(workImgRow);

            // Calculate the cost of a horizontal cut
            // or size rows - i + 1, cols - j
            float hCost = 0;
            cv::Mat hCutImg, hPath;
            inSize.width = input.rows-i+1;
            inSize.height = input.cols-j;
            calculateSeamCost( prevRow[j-1], hCutImg, hPath, hCost , false,inSize);
            hCost += m_map.at<float>(i-1,j);

            // Every time we make a horizontal cut we need to save
            // the image for our next calculation
            if( hCost < vCost )
            {
               m_map.at<float>(i,j) = hCost;
               m_decision.at<uint8_t>(i,j) = 1;
               m_path.push_back(hPath);
            }
            else
            {
               m_map.at<float>(i,j) = vCost;
               m_decision.at<uint8_t>(i,j) = 0;
               m_path.push_back(vPath);
            }
            thisRow.push_back(hCutImg);
         } //End Else

      }  // End loop over j
      prevRow.clear();
      prevRow = thisRow;
      startRowImg.copyTo(workImgRow);
   }
   return true; 
   
}

bool Transport::retargetImage(cv::Mat const &input,
                              int     const &colReduc, 
                              int     const &rowReduc,
                              cv::Mat       &output)
{
   
   // Form the map first
   if( !m_mapExists ) formMap(input);

   // m_decision holds our decisions
   // Start at the entry we want and traverse
   if( colReduc > m_colReduc || colReduc < 0 ||
       rowReduc > m_rowReduc || rowReduc < 0 )
      return false;

   // Start at (rowReduc-1,colReduc-1)
   int rowStart = rowReduc;
   int colStart = colReduc;
   
   // Iterate until we have done all of the cuts
   std::list<cv::Point> node;
   while( rowStart > 0 || colStart > 0)
   {
      uint8_t decision = m_decision.at<uint8_t>(rowStart,colStart);
      if( decision > 0 ) // We made a horizontal cut
      {
         node.push_front(cv::Point(colStart,rowStart));
         --rowStart;
      }
      else // we made a vertical cut
      {
         node.push_front(cv::Point(colStart,rowStart));
         --colStart;
      }
   }

   // Make a new seam object so we can delete the seams
   cv::Mat outImg;
   input.copyTo(outImg);

   // Loop through the nodes we found starting at the end (0,0)
   std::list<cv::Point>::iterator nIter = node.begin();

   int nCols = m_decision.cols;
   cv::Point lastPt = cv::Point(0,0);
   cv::Size newSize(input.size().width,input.size().height);
      
   cv::Mat path;
  
   // Create a window to display the results
   cv::namedWindow("Carved",1);
   while( nIter != node.end() )
   {
      cv::Point thisPt = *nIter;
      cv::Mat   tmpImg;
      cv::Mat   tmpOut;
      
      int i = thisPt.y;
      int j = thisPt.x;
      outImg.copyTo(tmpImg);
      
      int colDiff = j - lastPt.x;
      bool horizontal = true;
      if( colDiff > 0 ) horizontal = false;

      // Transpose if horizontal cut
      if( horizontal )
      {
         cv::transpose(tmpImg,tmpImg);
         int tmpVal = newSize.width;
         newSize.width = newSize.height;
         newSize.height = tmpVal;
      }
     
      // Get the path that corresponds to this grid point 
      m_path[ j + i*nCols ].copyTo(path);
      newSize.width--;
      Filters::removeSeamRGB(tmpImg,path,tmpOut,newSize); 
      if( horizontal )
      {
         cv::transpose(tmpOut,tmpOut);
         int tmpVal = newSize.width;
         newSize.width = newSize.height;
         newSize.height = tmpVal;
      }
      
      // Udate the output image
      tmpOut.copyTo(outImg);
      lastPt = thisPt;
      cv::imshow("Carved",outImg);
      cv::waitKey(5);
      ++nIter;  // Move to the next node
   }
   // Copy only the truncated portion to the output
   cv::Rect roi(0,0,newSize.width,newSize.height);
   cv::Mat  tmpOut = outImg(roi);
   tmpOut.copyTo(output);
   return true;
} 

