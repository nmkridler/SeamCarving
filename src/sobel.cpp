#include <sobel.h>

Sobel::Sobel( int const & kernelSize) : 
Energy(),
m_kernelSize(kernelSize)
{
}      

void Sobel::runEnergyFilter(cv::Mat const & input)
{
   // Assumes input is already grayscaled
   // Create a temporary matrix
   cv::Mat tmpMat;
   cv::Sobel(input ,tmpMat  ,input.depth(),1,0,m_kernelSize);
   cv::Sobel(tmpMat,m_energy,input.depth(),0,1,m_kernelSize);

}                  
