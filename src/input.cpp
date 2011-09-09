#include <input.h>

// Constructor
Input::Input(std::string const &fileName) :
m_valid(false),
m_fileName(fileName)                                        
{
   // Open up the input image
   m_image = cv::imread(m_fileName.c_str());

   // Determine whether or not the read was succesful
   if( !m_image.data ) 
   {
      m_valid = false;
      std::cout << "failed to open: " << m_fileName << std::endl;
   }
   m_valid = true;
}

