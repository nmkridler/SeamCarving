#include <seamengine.h>

SeamEngine::SeamEngine(std::string const &inputFile,
                       std::string const &outputFile,
                       double      const &rScale,
                       double      const &cScale) :
m_inputFile(inputFile),
m_outputFile(outputFile),
m_rScale(rScale),
m_cScale(cScale)
{

   // Create the input object
   m_input.reset( new Input(m_inputFile) );
   
}

bool SeamEngine::runEngine()
{
   if( m_input->success() )
   {
      // Set the number of pixels to reduce the image by
      double dRow = static_cast<double>(m_input->getImage().rows);
      double dCol = static_cast<double>(m_input->getImage().cols);
      int rowReduce = static_cast<int>(std::floor(dRow)*m_rScale);
      int colReduce = static_cast<int>(std::floor(dCol)*m_cScale);

      // Dump some info about the run
      std::cout << "Reducing image by: ";
      std::cout << rowReduce << " rows and ";
      std::cout << colReduce << " cols." << std::endl;
      if( rowReduce == 0 && colReduce == 0)
      {
         std::cout << "No reduction requested" << std::endl;
         return false;
      }

      // Create the transport map
      //m_transport.reset( new Transport(rowReduce,colReduce) );
      m_transport.reset( new TransportSimple(rowReduce,colReduce) );

      cv::Mat outImg;
      // Get the retargeted image
      // Now that the transport map exists, we can easily
      // carve out the seams
      m_transport->retargetImage( m_input->getImage(),
                                  colReduce, rowReduce, outImg);

      // Write the image to file
      cv::imwrite(m_outputFile.c_str(),outImg);
  
      // Display the image
      cv::namedWindow("Retarget",1);
      cv::imshow("Retarget",outImg);
      cv::waitKey(0);
   } else return false;
   return true;

}

