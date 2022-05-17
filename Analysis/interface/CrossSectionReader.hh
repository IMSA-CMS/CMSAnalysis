#ifndef CROSSSECTIONREADER_HH
#define CROSSSECTIONREADER_HH

class CrossSectionReader
{
	public:
		CrossSectionReader(std::string fileAddress);
		double getWeight(std::string key) const;
	private:
  		std::unordered_map<std::string, double> weights;
};


#endif