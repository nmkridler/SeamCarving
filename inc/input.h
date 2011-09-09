#ifndef __INPUT_H__
#define __INPUT_H__

#include <cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <string>
#include <boost/shared_ptr.hpp>

class Input
{
   public:
      // Constructor
      Input(std::string const & fileName);

      // Destructor
      ~Input(){} 

      // Image getter
      cv::Mat const &getImage() { return m_image;}

      // Did the image load properly?
      bool const &success(){ return m_valid; }
     
   private:
      bool         m_valid;      ///< Valid image
      std::string  m_fileName;   ///< file name
      cv::Mat      m_image;      ///< Image data
};

typedef boost::shared_ptr<Input> InputPtr;
#endif
