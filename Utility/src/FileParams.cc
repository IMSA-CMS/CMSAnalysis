#include "CMSAnalysis/Utility/interface/FileParams.hh"
#include "CMSAnalysis/Utility/interface/Utility.hh"
#include <sstream>
#include <fstream>

FileParams::FileParams(std::string iprocess, std::map<std::string, std::string> iparameters):
process(iprocess),
parameters(iparameters)
{

}
// call getFileName(), open that file, read from the list into a vector of strings and return