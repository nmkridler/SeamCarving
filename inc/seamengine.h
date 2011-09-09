#ifndef __SEAMENGINE_H__
#define __SEAMENGINE_H__

#include <cv.h>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <boost/shared_ptr.hpp>
#include <input.h>
#include <iostream>
#include <transportsimple.h>
#include <transport.h>
#include <cmath>

//////////////////////////////////////////////////////////////////
//
// Engine for seam carving.  Generates a transport map and then
// retargets the image at the desired level.
//
//////////////////////////////////////////////////////////////////
class SeamEngine
{
   public:
      // Constructor
      SeamEngine(std::string const &inputFile,
                 std::string const &outputFile, 
                 double      const &rScale,
                 double      const &cScale);

      // Destructor
      ~SeamEngine(){}

      // Execute
      bool runEngine();

   private:
      
      std::string           m_inputFile;    // Input image
      std::string           m_outputFile;   // Output file
      double                m_rScale;       // Row Scale Factor
      double                m_cScale;       // Col Scale Factor
      InputPtr              m_input;        // Input image
      TransportBasePtr      m_transport;    // Transport object
};


#endif
