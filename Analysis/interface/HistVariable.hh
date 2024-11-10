#ifndef HISTVARIABLE_HH
#define HISTVARIABLE_HH

#include <string>

class HistVariable 
{
    public:
	/*
	static HistVariable genSimSameSignMass(std::string histName) {return HistVariable("GenSim Same Sign Inv Mass", histName);}
	static HistVariable sameSignMass(std::string histName) {return HistVariable("Same Sign Inv Mass", histName);}
	static HistVariable invariantMass(std::string histName) {return HistVariable("Invariant Mass", histName);}
	static HistVariable genSimPt(std::string histName) {return HistVariable("GenSim pT", histName);}
	static HistVariable pt(std::string histName) {return HistVariable("pT", histName);}
	static HistVariable eta(std::string histName) {return HistVariable("Eta", histName);}
	static HistVariable phi(std::string histName) {return HistVariable("Phi", histName);}
	static HistVariable mET(std::string histName) {return HistVariable("MET", histName);}
	static HistVariable firstPt(std::string histName) {return HistVariable("firstPt", histName);}
	static HistVariable secondPt(std::string histName) {return HistVariable("secondPt", histName);}
	static HistVariable thirdPt(std::string histName) {return HistVariable("thirdPt", histName);}
	static HistVariable fourthPt(std::string histName) {return HistVariable("fourthPt", histName);}
	*/
	std::string getName() {return name;}
	//std::string getHistName() {return graphName;}
    //HistVariable(std::string iName, std::string placeholder) : name(iName) , graphName(placeholder) {}
	HistVariable(std::string iName) : name(iName) {}

    private:
	std::string name;
	//std::string graphName;
};

#endif

/*
#ifndef HISTVARIABLE_HH
#define HISTVARIABLE_HH

#include <string>

class HistVariable 
{
    public:
	static HistVariable genSimSameSignMass(std::string histName) {return HistVariable("GenSim Same Sign Inv Mass", histName);}
	static HistVariable sameSignMass(std::string histName) {return HistVariable("Same Sign Inv Mass", histName);}
	static HistVariable invariantMass(std::string histName) {return HistVariable("Invariant Mass", histName);}
	static HistVariable genSimPt(std::string histName) {return HistVariable("GenSim pT", histName);}
	static HistVariable pt(std::string histName) {return HistVariable("pT", histName);}
	static HistVariable eta(std::string histName) {return HistVariable("Eta", histName);}
	static HistVariable phi(std::string histName) {return HistVariable("Phi", histName);}
	static HistVariable mET(std::string histName) {return HistVariable("MET", histName);}
	static HistVariable firstPt(std::string histName) {return HistVariable("firstPt", histName);}
	static HistVariable secondPt(std::string histName) {return HistVariable("secondPt", histName);}
	static HistVariable thirdPt(std::string histName) {return HistVariable("thirdPt", histName);}
	static HistVariable fourthPt(std::string histName) {return HistVariable("fourthPt", histName);}
	std::string getName() {return name;}
	std::string getHistName() {return graphName;}
    HistVariable(std::string iName, std::string iGraphName) : name(iName), graphName(iGraphName) {}

    private:
	std::string name;
	std::string graphName;
};

#endif
*/
