#ifndef __FILTERS_H__
#define __FILTERS_H__

#include <cv.h>
#include <opencv2/core/core.hpp>
#include <algorithm>

namespace Filters
{
   // Remove the seam and output a new image
   void removeSeamRGB(cv::Mat  const &input,
                      cv::Mat  const &path,
                      cv::Mat        &output,
                      cv::Size const &inSize);

   // Remove the seam and output a new image
   void removeSeamGray(cv::Mat   const &input,
                       cv::Mat   const &path,
                       cv::Mat         &output,
                       cv::Size  const &inSize);    

}
#endif

