#include "CIAnalysis/CIStudies/interface/TextOutputModule.hh"

#include <stdexcept>

TextOutputModule::TextOutputModule(const std::string& outputFilename) :
  baseFilename(outputFilename)
{}

TextOutputModule::~TextOutputModule()
{
  if (!filesDeleted)
    deleteFiles();
}

void TextOutputModule::writeAll()
{
  deleteFiles();
  filesDeleted = true;
}

void TextOutputModule::writeLine(const std::string& line)
{
  writeToFile(line + '\n');
}

void TextOutputModule::writeToFile(const std::string& str)
{
  getFileStream(getFullName()) << str;
  //  std::flush(getFileStream(getFullName()));
}

std::string TextOutputModule::getFullName() const
{
  return getFilter() + baseFilename;
}

std::ofstream& TextOutputModule::getFileStream(const std::string& fullName)
{
  auto it = files.find(fullName);
  if (it == files.end())
    {
      auto newFile = new std::ofstream(fullName);
      if (!newFile || !*newFile)
	{
	  throw std::runtime_error("Unable to create file " + fullName + " !");
	}
      auto response = files.insert({fullName, newFile});
      it = response.first;
    }

  return *(it->second);
}

void TextOutputModule::deleteFiles() noexcept
{
  for (auto& entry : files)
    {
      delete entry.second;
    }
}
