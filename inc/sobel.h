#ifndef __SOBEL_H__
#define __SOBEL_H__

#include <cv.h>
#include <energy.h>

// Sobel Energy Filter
class Sobel : public Energy
{
public:
   Sobel( int const & kernelSize );

   ~Sobel(){}

   void runEnergyFilter(cv::Mat const & input);

private:
   int      m_kernelSize;    // Kernel Size 

};

#endif
