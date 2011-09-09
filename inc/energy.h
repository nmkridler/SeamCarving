#ifndef __ENERGY_H__
#define __ENERGY_H__

#include <cv.h>
#include <boost/shared_ptr.hpp>

class Energy
{
public:
   // Constructor
   Energy(){}

   // Destructor
   virtual ~Energy(){}

   // Energy getter
   cv::Mat const &getEnergy() const {return m_energy;}

   // Energy calculation
   virtual void runEnergyFilter(cv::Mat const & input)=0;

protected:
   // Energy-Filtered image
   cv::Mat   m_energy;
};

typedef boost::shared_ptr<Energy> EnergyPtr;


#endif




