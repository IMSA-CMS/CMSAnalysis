#ifndef TEXTOUTPUTMODULE_HH
#define TEXTOUTPUTMODULE_HH

#include <fstream>
#include <string>

#include "AnalysisModule.hh"

class TextOutputModule : public AnalysisModule
{
public:
  TextOutputModule(const std::string& outputFilename);
  ~TextOutputModule();

  virtual void writeAll() override;

protected:
  void writeLine(const std::string& line);
  void writeToFile(const std::string& str);

private:
  std::unordered_map<std::string, std::ofstream*> files;
  std::string baseFilename;
  bool filesDeleted = false;

  std::string getFullName() const;
  std::ofstream& getFileStream(const std::string& fullName);
  void deleteFiles() noexcept;
};

#endif
