#ifndef CROSSSECTIONREADER_HH
#define CROSSSECTIONREADER_HH

#include <unordered_map>

class CrossSectionReader
{
	public:
		CrossSectionReader(std::string fileAddress);
		double getCrossSection(std::string key) const;
	private:
  		std::unordered_map<std::string, double> weights;
};


#endif