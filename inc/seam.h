#ifndef __SEAM_H__
#define __SEAM_H__

#include <cv.h>
#include <opencv2/core/core.hpp>
#include <iostream>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <filters.h>

typedef boost::shared_ptr<cv::Mat> MatPtr;

/////////////////////////////////////////////////////////////////////
//
// Seam
//
// Takes an input energy filtered image and determines the minimal
// seam.  Horizontal or Vertical seams can be found by transposing
// the input image.
/////////////////////////////////////////////////////////////////////
class Seam
{
public:
   // Constructor
   Seam(cv::Size const &inSize);

    // Destructor
   ~Seam(){}

   // Accumulate energy
   void accumulateEnergy(cv::Mat const &input);

   // Backtrack for the path
   void calculatePath();

   // Getter for the cost map
   cv::Mat const &getCostMap() const { return m_cost; }
   
   // Getter for the path
   cv::Mat const &getPath()    const { return m_path; }

   // Get minimum energy
   float   const &getEnergy()  const { return m_energy; }
private:
   float          m_energy;      // Minimum energy
   cv::Mat        m_cost;        // Cumulative minimal energy
   cv::Mat        m_index;       // Save the indices chosen
   cv::Mat        m_path;        // Optimal path
   cv::Size       m_size;        // Size of the image
};

typedef boost::shared_ptr<Seam> SeamPtr;

#endif
