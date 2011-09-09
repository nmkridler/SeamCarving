#ifndef __TRANSPORTSIMPLE_H__
#define __TRANSPORTSIMPLE_H__

#include <cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>
#include <seam.h>
#include <sobel.h>
#include <algorithm>
#include <boost/shared_ptr.hpp>
#include <list>
#include <filters.h>

///////////////////////////////////////////////////////////
//
//  Calculates a transport map to determine the optimal 
// seams to carve from the image. 
//
///////////////////////////////////////////////////////////
class TransportSimple
{
public:
   // Constructor
   TransportSimple( int const & row, int const & col );

   // Destructor
   ~TransportSimple(){}

   // Apply removals
   virtual bool retargetImage( cv::Mat const &input, 
                               int     const &colReduc,
                               int     const &rowRedec,
                               cv::Mat       &output);

   // Calculate the cost of removing a seam
   void calculateSeamCost( cv::Mat const & input,
                           cv::Mat       & output,
                           cv::Mat       & path,
                           float         & minEnergy,
                           bool    const & vertical,
                           cv::Size      & size);

protected:
   int                      m_rowReduc;         // Total reduction in rows
   int                      m_colReduc;         // Total reduction in rows
   EnergyPtr                m_energyFilter;     // Energy calculation
   SeamPtr                  m_seam;             // Seam calculations
   cv::Mat                  m_gray;             // Grayscale image
};

typedef boost::shared_ptr<TransportSimple> TransportBasePtr;

#endif
   
  
