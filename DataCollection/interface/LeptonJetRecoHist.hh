#ifndef LEPTONJETRECOHIST_HH

#define LEPTONJETRECOHIST_HH

 

#include "CMSAnalysis/DataCollection/interface/HistogramPrototype1D.hh"

//#include "CMSAnalysis/DataCollection/interface/MiniAODEventFile.hh"

//#include "CMSAnalysis/DataCollection/interface/LeptonJetReconstructionModule.hh"

//#include "CMSAnalysis/DataCollection/interface/MatchingModule.hh"

//#include "MatchingModule.hh"

 

class MatchingModule;

 

class LeptonJetReconstructionModule;

 

class LeptonJetRecoHist : public HistogramPrototype1D

{

    public:

        LeptonJetRecoHist(std::shared_ptr<MatchingModule> iRecoMatchingModule,bool elec,std::shared_ptr<LeptonJetReconstructionModule> ileptonJetReco, const std::string& iname, int iNBins, double iminimum, double imaximum);

        std::vector<double> value() const override;

    private:

        std::shared_ptr<MatchingModule> recoMatchingModule;

        std::shared_ptr<LeptonJetReconstructionModule> leptonJetRecoModule;

        bool elec;

        //std::vector<double> counter;

};

 

#endif