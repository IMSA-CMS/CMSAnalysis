#ifndef HISTVARIABLE_HH
#define HISTVARIABLE_HH

class HistVariable 
{
    public:
	static HistVariable GenSimSameSignMass(std::string histName) {return HistVariable("GenSim Same Sign Inv Mass", histName);}
	static HistVariable SameSignMass(std::string histName) {return HistVariable("Same Sign Inv Mass", histName);}
	static HistVariable InvariantMass(std::string histName) {return HistVariable("Invariant Mass", histName);}
	static HistVariable GenSimPt(std::string histName) {return HistVariable("GenSim pT", histName);}
	static HistVariable Pt(std::string histName) {return HistVariable("pT", histName);}
	static HistVariable Eta(std::string histName) {return HistVariable("Eta", histName);}
	static HistVariable Phi(std::string histName) {return HistVariable("Phi", histName);}
	static HistVariable MET(std::string histName) {return HistVariable("MET", histName);}
	std::string getName() {return name;}
	std::string getHistName() {return graphName;}
    
    private:
	HistVariable(std::string iName, std::string iGraphName) : name(iName), graphName(iGraphName) {}
	std::string name;
	std::string graphName;
};

#endif
