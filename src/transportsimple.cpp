#include <transportsimple.h>

TransportSimple::TransportSimple( int const & row, int const & col) :
m_rowReduc(row),
m_colReduc(col)
{
   m_seam.reset();          // Reset the seam object
   m_energyFilter.reset();  // Reset the energy filter
}

bool TransportSimple::retargetImage(cv::Mat const &input,
                                    int     const &colReduc, 
                                    int     const &rowReduc,
                                    cv::Mat       &output)
{
   // Create the grayscale
   cv::cvtColor(input, m_gray, CV_RGB2GRAY);
   cv::Mat workImg;
   cv::Mat workRGB;
   input.copyTo(workRGB);
   m_gray.copyTo(workImg);

   // Start at zero and evaluate every option
   // until we get to the required number of cuts
   int colCount = 0;
   int rowCount = 0;

   // Keep track of the size
   cv::Size size(input.cols,input.rows);
   cv::Size inSize(0,0);
   cv::Size hSize(0,0);

   // Create some extra helpers
   cv::Mat hCutImg, vCutImg;
   cv::Mat hPath,   vPath;
   float   hCost = 0;
   float   vCost = 0;
   
   // Create a window
   cv::namedWindow("Carved",1);
   cv::Mat tmpRGB, hTmp;
   // Iterate until one of the counters is zero
   while( colCount < colReduc && rowCount < rowReduc )
   {
      // Calculate a vertical cut
      inSize.width  = input.cols-colCount;
      inSize.height = input.rows-rowCount;
      calculateSeamCost( workImg, vCutImg, vPath, vCost ,true,inSize);

      // Calculate a horizontal cut
      inSize.width  = input.rows-rowCount;
      inSize.height = input.cols-colCount;
      calculateSeamCost( workImg, hCutImg, hPath, hCost ,false,inSize);

      if( hCost < vCost ) // take the horizontal cut
      {
         hCutImg.copyTo(workImg);
         --size.height;

         // Transpose
         cv::transpose(workRGB,tmpRGB);
         hSize.width  = size.height;
         hSize.height = size.width;
         Filters::removeSeamRGB(tmpRGB,hPath,workRGB,hSize); 
         cv::transpose(workRGB,tmpRGB);
         ++rowCount;
      }
      else
      {
         vCutImg.copyTo(workImg);
         --size.width;
         Filters::removeSeamRGB(workRGB,vPath,tmpRGB,size); 
         ++colCount;
      }
      tmpRGB.copyTo(workRGB);
      cv::imshow("Carved",workRGB);
      cv::waitKey(5);
   }

   // One of them hit zero, which was it?
   while( colCount < colReduc )
   {
      // Calculate a vertical cut
      inSize.width  = input.cols-colCount;
      inSize.height = input.rows-rowCount;
      calculateSeamCost( workImg, vCutImg, vPath, vCost ,true,inSize);
      vCutImg.copyTo(workImg);
  
      // Update the size
      ++colCount;
      --size.width;
  
      Filters::removeSeamRGB(workRGB,vPath,tmpRGB,size);
      tmpRGB.copyTo(workRGB);
 
      // Display the result
      cv::imshow("Carved",workRGB);
      cv::waitKey(5);
   }
   while( rowCount < rowReduc )
   {
      // Calculate a vertical cut
      inSize.height = input.cols-colCount;
      inSize.width = input.rows-rowCount;
      calculateSeamCost( workImg, hCutImg, hPath, hCost ,false,inSize);
      hCutImg.copyTo(workImg);

      // Update the size
      ++rowCount;
      --size.height;

      // Transpose
      cv::transpose(workRGB,tmpRGB);
      hSize.width  = size.height;
      hSize.height = size.width;
      Filters::removeSeamRGB(tmpRGB,hPath,workRGB,hSize); 
      cv::transpose(workRGB,tmpRGB);
      tmpRGB.copyTo(workRGB);

      // Display
      cv::imshow("Carved",workRGB);
      cv::waitKey(5);

   }

   cv::Rect roi(0,0,size.width,size.height);
   cv::Mat  tmpOut = workRGB(roi);
   tmpOut.copyTo(output);
   return true;
} 

void TransportSimple::calculateSeamCost( cv::Mat const & input,
                                   cv::Mat       & output,
                                   cv::Mat       & path,
                                   float         & minEnergy,
                                   bool    const & vertical,
                                   cv::Size      & size)
{
   // The input should be the grayscaled image
   cv::Mat cost;
   cost = input.clone();
  
   // Energy calculation
   m_energyFilter.reset( new Sobel(3) );  // Reset the energy filter

   // Transpose if necessary 
   if( !vertical ) cv::transpose(cost,cost);

   // Run the energy filter
   m_energyFilter->runEnergyFilter(cost);

   // Initalize the seam object
   m_seam.reset( new Seam(size)   );

   // Find the path that has the minimal cumulative energy
   m_seam->accumulateEnergy(m_energyFilter->getEnergy());
   minEnergy = m_seam->getEnergy();
   
   // Add the path to the list
   m_seam->getPath().copyTo(path);

   // carve out the seam
   cv::Size newSize(size.width-1,size.height);
   Filters::removeSeamGray(cost,path,output,newSize);
  
   // Transpose if necessary
   if( !vertical ) cv::transpose(output,output);

   // Clean up
   m_seam.reset();
   m_energyFilter.reset(); 
}


