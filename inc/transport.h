#ifndef __TRANSPORT_H__
#define __TRANSPORT_H__

#include <transportsimple.h>

///////////////////////////////////////////////////////////
//
//  Calculates a transport map to determine the optimal 
// seams to carve from the image. 
//
///////////////////////////////////////////////////////////
class Transport : public TransportSimple
{
public:
   // Constructor
   Transport( int const & row, int const & col );

   // Destructor
   ~Transport(){}

   // Form transport map
   bool formMap( cv::Mat const & input );

   // Apply removals
   bool retargetImage( cv::Mat const &input, 
                       int     const &colReduc,
                       int     const &rowRedec,
                       cv::Mat       &output);


private:
   bool                     m_mapExists;        // Does the map exist?
   cv::Mat                  m_map;              // Transport map
   cv::Mat                  m_decision;         // Decision map
   std::vector<cv::Mat>     m_path;             // Pointers to the paths
};

typedef boost::shared_ptr<Transport> TransportPtr;

#endif
   
  
