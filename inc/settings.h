#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <fstream>
#include <boost/shared_ptr.hpp>
#include <cstdlib>

typedef std::map<std::string,std::string > fileMap;

class Settings
{
   public:
      // Constructor
      Settings(std::string const & fileName);

      // Destructor
      ~Settings(){}

      // Settings reader
      bool readSettings();

      // Get file list
      std::string const &getInputFile() const { return m_inputFile; }

      // Get output file name
      std::string const &outputFile() const {return m_outFileName;}

      // Get row scaling
      double const & getRowScale() const {return m_rowScale; }

      // Get col scaling
      double const & getColScale() const {return m_colScale; }

   private:

      std::string                m_settingsFile; // Text file containing settings
      std::string                m_inputFile;    // Input file
      std::string                m_outFileName;  // Output file name
      double                     m_rowScale;     // row scaling 
      double                     m_colScale;     // column scaling
     
};

typedef boost::shared_ptr<Settings> SettingsPtr;
#endif
