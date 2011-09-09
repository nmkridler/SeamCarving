#include <filters.h>

// Make the image smaller by removing a seam
void Filters::removeSeamRGB(cv::Mat  const  & input, 
                            cv::Mat  const  & path,
                            cv::Mat         & output,
                            cv::Size const  & inSize)
{

   // Initialize the output
   output = cv::Mat::zeros(inSize,input.type());
   
   // Loop through the rows
   for( int i = 0; i < inSize.height; ++i)
   {
      // Get the column pixel we want to delete
      int colPix = static_cast<int>(path.at<uint16_t>(i));
         
      for( int j = 0; j < colPix; ++j)
      {
         output.at<cv::Vec3b>(i,j) = input.at<cv::Vec3b>(i,j);
      }
      for( int j = colPix; j < inSize.width-1; ++j)
      {
         output.at<cv::Vec3b>(i,j) = input.at<cv::Vec3b>(i,j+1);
      }
 
   }
}

// Make the image smaller by removing a seam
void Filters::removeSeamGray(cv::Mat  const & input, 
                             cv::Mat  const & path,
                             cv::Mat        & output,
                             cv::Size const & inSize)
{

   // Initialize the output
   output = cv::Mat::zeros(inSize,input.type());
   
   // Loop through the rows
   for( int i = 0; i < inSize.height; ++i)
   {
      // Get the column pixel we want to delete
      int colPix = static_cast<int>(path.at<uint16_t>(i));
         
      for( int j = 0; j < colPix; ++j)
      {
         output.at<uchar>(i,j) = input.at<uchar>(i,j);
      }
      for( int j = colPix; j < inSize.width-1; ++j)
      {
         output.at<uchar>(i,j) = input.at<uchar>(i,j+1);
      }
 
   }
}


