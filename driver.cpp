#include <iostream>
#include <string>
#include <seamengine.h>
#include <cstdlib>
#include <settings.h>

int main( int argc, char **argv)
{
   if( argc < 1)
   {
      std::cout << "Usage: " << std::endl;
      std::cout << "./seam filename" << std::endl;
      return 0;
   }
    
   // Input file name
   std::string inputFile(argv[argc-1]);

   std::cout << " Running Seam Engine " << std::endl;
   std::cout << " Processing Settings File: " << inputFile << std::endl;

   Settings seamSettings(inputFile);
   if( seamSettings.readSettings() ) 
   {
      // Image file
      std::string imageFile = seamSettings.getInputFile();
      std::string outFile   = seamSettings.outputFile();
   
      double rScale = seamSettings.getRowScale();
      double cScale = seamSettings.getColScale();

      // Create the mosaic engine
      SeamEngine engine(imageFile,outFile,rScale,cScale);

      engine.runEngine();
   }
   else std::cout << "Failed to read settings" << std::endl;
   return 0;
}
