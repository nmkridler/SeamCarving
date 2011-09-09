#include <settings.h>

Settings::Settings( std::string const & fileName):
m_settingsFile(fileName)
{
}

//////////////////////////////////////////////////////////
//
// readSettings
//
// This function opens up the settings file and reads the
// list of input images to process.
// 
// The file should be formatted as follows
// inFile  file path
// rScale  row scaling
// cScale  col scaling
// outFile file path
//////////////////////////////////////////////////////////
bool Settings::readSettings()
{
   // Create a map<string,string> for the files
   fileMap fMap;

   // Create an input stream
   std::ifstream inFile(m_settingsFile.c_str());
   if( !inFile ) return false;

   // Read the file
   std::string fileLine;
   std::string tmpStr;
   while( std::getline(inFile,fileLine) )
   {
      std::stringstream lineStream(fileLine);
      std::vector<std::string> strVector;
      while( getline(lineStream,tmpStr,' ') ) strVector.push_back(tmpStr);

      // Exit if the line is invalid
      if( strVector.size() < 2 ) break;

      // Add to the map
      fMap[strVector[0]] = strVector[1];
         
   }  // End loop over file
   inFile.close();

   // Create the appropriate lists
   m_inputFile   = fMap["inFile"];
   m_outFileName = fMap["outFile"];
   m_rowScale    = std::atof(fMap["rowScale"].c_str());
   m_colScale    = std::atof(fMap["colScale"].c_str());
 
   return true;
}
