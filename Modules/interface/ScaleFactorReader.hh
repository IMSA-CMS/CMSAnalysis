#ifndef SCALEFACTORREADER_HH
#define SCALEFACTORREADER_HH

#include <map>
#include <memory>
#include "CMSAnalysis/Utility/interface/ScaleFactor.hh"

class ScaleFactorKey;


class ScaleFactorReader
{


public:
	virtual ~ScaleFactorReader() = default;
	virtual std::map<std::string, ScaleFactor::ScaleFactorSet> readData() = 0;
	// virtual std::string getKey(const EventInput* input) const = 0;

};

#endif