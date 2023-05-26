#ifndef TEXTOUTPUTMODULE_HH
#define TEXTOUTPUTMODULE_HH

#include <fstream>
#include <string>

#include "AnalysisModule.hh"

// This module outputs a text file
// If there are multiple filter strings, it makes multiple text files
class TextOutputModule : public AnalysisModule
{
public:
  // The base name of the output file is needed
  TextOutputModule(const std::string& outputFilename);
  virtual ~TextOutputModule() override;

  virtual void finalize() override;

protected:
  // Write one line (with a newline character) to the file.
  // Provided for convenience.
  void writeLine(const std::string& line);

  // Write a string (with no newline) to a file.  This will automatically
  // switch to the appropriate file based on the current filter string.
  void writeToFile(const std::string& str);

private:
  // This holds all open files, one per filter string
  std::unordered_map<std::string, std::ofstream*> files;

  std::string baseFilename;

  // This indicates whether the files have been deleted (closed) already
  bool filesDeleted = false;

  // This returns the full name of the text file based on the filter string
  std::string getFullName() const;

  // This creates a new file if needed, and returns the file for writing
  std::ofstream& getFileStream(const std::string& fullName);

  // Closes all open text files and deletes their pointers
  void deleteFiles() noexcept;
};

#endif
