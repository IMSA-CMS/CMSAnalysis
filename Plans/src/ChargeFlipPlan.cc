#include "CMSAnalysis/Plans/interface/ChargeFlipPlan.hh"
#include <memory>
#include "CMSAnalysis/Modules/interface/EventModule.hh"
#include "CMSAnalysis/Filters/interface/HiggsSelector.hh"
#include "CMSAnalysis/Modules/interface/SignFlipModule.hh"
#include "CMSAnalysis/Modules/interface/MatchingModule.hh"
#include "CMSAnalysis/Modules/interface/HistogramOutputModule.hh"
//#include "CMSAnalysis/Modules/interface/HistogramOutputModule.hh"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"

using std::make_shared;


void ChargeFlipPlan::initialize()
{
    auto& modules = getModules();


    auto eventMod = make_shared<EventModule>();
    eventMod->addSelector(make_shared<HiggsSelector>());


    auto matchMod = make_shared<MatchingModule>();
    //matchMod->setInput(eventMod->getEventInput());


    auto signFlip = make_shared<SignFlipModule>(matchMod);
    modules.addProductionModule(eventMod);
    modules.addProductionModule(matchMod);
    modules.addAnalysisModule(signFlip);



//Initializing Histograms, histBins: number of bins
/*defHist = new TH1F("Unweighted", "Unweighted", histBins, low, high);
for (int i = 0; i < 100; ++i){
  
   std::string s = "PDFWeighted" + std::to_string(i);
   const char* thName = s.c_str();
   //Stupid histograms won’t accept normal strings tho i think there’s a THString obj	
   TH1* newHist = new TH1F(thName, thName, histBins, low, high); 
 }
    //Editing Histograms
    highHist->SetName("Charge Flip Rates");
    //This is the name that will appear in root file 
    defHist->Fill(value, weight);
    //Adding raw values to fill up the bars
    highHist->SetBinContent(index, 5);
    //Changing the bars directly 
    highHist->Write();
    //Above is how you add them to the ROOT file when you’re done
    
    //modules.addAnalysisModule(ChargeFlipHist);*/
    
}


 