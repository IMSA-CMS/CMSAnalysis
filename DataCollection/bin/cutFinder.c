//imports
#include <cmath>
#include <iostream>
#include <algorithm>
#include "TROOT.h"
#include "TFile.h"
#include "TH1.h"
#include "TF1.h"
#include <iostream>
#include <string>
#include <new>
#include <string>

//Copied from superimpose.c to open the graph
TFile *file;
TH1F *plot;

TH1F* getHist(std::string name, TFile* file);

TH1F* getHist(std::string name, TFile* file)
{
  auto hist = dynamic_cast<TH1F*>(file->Get(name.c_str()));
  if (!hist)
    throw std::runtime_error(name + " not found!");
    return hist;
}

//Gets the integral
double getIntegral(string filePath, string histPath, double min) {
  double total;
  file = TFile::Open(filePath.c_str());
  plot = getHist(histPath.c_str(), file);   
  total = plot->Integral(min, 500);
  return total;
}

//Significance calculating function
double calculateSignificance(double signalEvents, double backgroundEvents) {
  double foundSignificance;
  foundSignificance = signalEvents / sqrt(backgroundEvents);
  return foundSignificance;
}

//Event calculating function
double calculateEvents(double crossSection, double selected, double generated, double l) {
  return l * crossSection * (selected/generated);
}

//Main program
void cutFinder()
{
  //Declare variables
  //Luminosity
  double luminosity = 3000000;
  //Cross sections
  double higgsCross = 0.000000389452827;
  double QCDCross = 1.087;
  double QCD2Cross = 6.053;
  double QCD3Cross = 274;
  double DYCross = 5711;
  double TTCross = 67.75;
  double ZZCross = 16.523;
  //Generated events (5 files I believe)
  double higgsGenerated = 2500;
  double QCDGenerated = 70568;
  double QCD2Generated = 84468;
  double QCD3Generated = 75732;
  double DYGenerated = 72851;
  double TTGenerated = 70598;
  double ZZGenerated = 180780;
  //These events are not used, they are just histogram generated events
  /*//Invariant mass / same-sign invariant mass generated
  double higgsGenerated = 1078;
  double QCDGenerated = 1519;
  double QCD2Generated = 1419;
  double QCD3Generated = 882;
  double DYGenerated = 58.4027;
  double TTGenerated = 7327;
  double ZZGenerated = 5840;*/
  /*//MET generated
  double higgsGenerated = 1023;
  double QCDGenerated = 56688;
  double QCD2Generated = 84468;
  double QCD3Generated = 75732;
  double DYGenerated = 5073.85;
  double TTGenerated = 35486;
  double ZZGenerated = 154476;*/
  /*//Leading lepton pT generated
  double higgsGenerated = 1002;
  double QCDGenerated = 1159;
  double QCD2Generated = 84449;
  double QCD3Generated = 75732;
  double DYGenerated = 37.7805;
  double TTGenerated = 6900;
  double ZZGenerated = 4814;*/
  //More variables
  double background;
  double signal;
  double significance;
  double significantcut;
  //Starting the iteration and low/high bounds (bins are length 2, so 500 is actually 1000 GeV)
  double cut = 0.0;
  double iteration = 5.0;
  double previousSignificance = 0.0;
  double lowBound = 0.0;
  double highBound = 500.0;
  double higgsSelected;
  double QCDSelected;
  double QCD2Selected;
  double QCD3Selected;
  double DYSelected;
  double TTSelected;
  double ZZSelected;
  double significantSelected;
  double eventpercent;
  double zeroSignificance = 0.0;
  double binSize = 2.0;
  //While the iterator iterates over more than 2 GeV
  while(iteration >= 1)
  {
    //starts the cut at the low boundary
    cut = lowBound;
    //while the cut is less than the highest boundary
    while(cut < highBound) {
      //Finding the number of selected events w/ a cut
      //MET
      higgsSelected = getIntegral("higgsMET.root", "Cut4MET", cut);
      QCDSelected = getIntegral("QCDMET.root", "Cut0MET", cut);
      QCD2Selected = getIntegral("QCDMET2.root", "MET", cut);
      QCD3Selected = getIntegral("QCDMET3.root", "MET", cut);
      DYSelected = getIntegral("DYMET.root", "Cut0MET", cut);
      TTSelected = getIntegral("TTBarMET.root", "Cut0MET", cut);
      ZZSelected = getIntegral("ZZMET.root", "Cut0MET", cut);
      /*//Leading lepton pT
      higgsSelected = getIntegral("higgsMET.root", "Cut4Leading lepton pT", cut);
      QCDSelected = getIntegral("QCDMET.root", "Cut2Leading lepton pT", cut);
      QCD2Selected = getIntegral("QCDMET2.root", "Leading lepton pT", cut);
      QCD3Selected = getIntegral("QCDMET3.root", "Leading lepton pT", cut);
      DYSelected = getIntegral("DYMET.root", "Cut2Leading lepton pT", cut);
      TTSelected = getIntegral("TTBarMET.root", "Cut2Leading lepton pT", cut);
      ZZSelected = getIntegral("ZZMET.root", "Cut2Leading lepton pT", cut);*/
      /*//Same-sign Invariant Mass
      higgsSelected = getIntegral("higgsMass.root", "4same_Sign_Invariant_Mass", cut);
      QCDSelected = getIntegral("QCDMass.root", "2same_Sign_Invariant_Mass", cut);
      QCD2Selected = getIntegral("QCDMass2.root", "2same_Sign_Invariant_Mass", cut);
      QCD3Selected = getIntegral("QCDMass3.root", "2same_Sign_Invariant_Mass", cut);
      DYSelected = getIntegral("DYMass.root", "2same_Sign_Invariant_Mass", cut);
      TTSelected = getIntegral("TTBarMass.root", "2same_Sign_Invariant_Mass", cut);
      ZZSelected = getIntegral("ZZMass.root", "2same_Sign_Invariant_Mass", cut);*/
      /*//Invariant Mass
      higgsSelected = getIntegral("higgsMass.root", "4invariant_Mass", cut);
      QCDSelected = getIntegral("QCDMass.root", "2invariant_Mass", cut);
      QCD2Selected = getIntegral("QCDMass2.root", "2invariant_Mass", cut);
      QCD3Selected = getIntegral("QCDMass3.root", "2invariant_Mass", cut);
      DYSelected = getIntegral("DYMass.root", "2invariant_Mass", cut);
      TTSelected = getIntegral("TTBarMass.root", "2invariant_Mass", cut);
      ZZSelected = getIntegral("ZZMass.root", "2invariant_Mass", cut);*/
      //Finding significance
      signal = calculateEvents(higgsCross, higgsSelected, higgsGenerated, luminosity);
      //std::cout << cut << std::endl;
      //std::cout << higgsSelected << std::endl;
      background = calculateEvents(QCDCross, QCDSelected, QCDGenerated, luminosity) + calculateEvents(QCD2Cross, QCD2Selected, QCD2Generated, luminosity) + calculateEvents(QCD3Cross, QCD3Selected, QCD3Generated, luminosity) + calculateEvents(DYCross, DYSelected, DYGenerated, luminosity) + calculateEvents(TTCross, TTSelected, TTGenerated, luminosity) + calculateEvents(ZZCross, ZZSelected, ZZGenerated, luminosity);
      significance = calculateSignificance(signal, background);
      //Just checks if the cut is most significant if it's the first loop over the data
      if(iteration == 5.0) {
        if(significance > previousSignificance) {
          std::cout << "new highest significance" << std::endl;
          std::cout << previousSignificance << std::endl;
          std::cout << significance << std::endl;
          previousSignificance = significance;
          significantcut = binSize * cut;
          //significantSelected = getIntegral("higgsMET.root", "Cut4MET", cut);
          significantSelected = getIntegral("higgsMET.root", "Cut4Leading lepton pT", cut);
          //significantSelected = getIntegral("higgsMass.root", "4same_Sign_Invariant_Mass", cut);
          //significantSelected = getIntegral("higgsMass.root", "4invariant_Mass", cut);
          //std::cout << significantcut << std::endl;
        }
        //std::cout << significance << endl;
      }
      //If it's not the first loop, checks for a 10% difference in between cuts.
      else {
        if(significance >= 1.1 * previousSignificance) {
          std::cout << "new highest significance" << std::endl;
          std::cout << previousSignificance << std::endl;
          std::cout << significance << std::endl;
          previousSignificance = significance;
          significantcut = binSize * cut;
          //significantSelected = getIntegral("higgsMET.root", "Cut4MET", cut);
          significantSelected = getIntegral("higgsMET.root", "Cut4Leading lepton pT", cut);
          //significantSelected = getIntegral("higgsMass.root", "4same_Sign_Invariant_Mass", cut);
          //significantSelected = getIntegral("higgsMass.root", "4invariant_Mass", cut);
        } 
      }
      if(cut == 0) {
        zeroSignificance = significance;
      }
      cut = cut + iteration;
    }
    //Sets the high and low bounds to loop over next. Here, they are the found largest cut +/- the iteration, because it hasn't looked within that range.
    highBound = (significantcut / binSize) + iteration;
    lowBound = (significantcut / binSize) - iteration;
    iteration = iteration / 2;
  }
  double percentDifference;
  percentDifference = ((previousSignificance - zeroSignificance) / zeroSignificance) * 100;
  std::cout << "The cut is:" << endl;
  std::cout << significantSelected << " events selected" << std::endl;
  eventpercent = significantSelected / higgsGenerated * 100;
  std::cout << eventpercent << " percent efficiency" << std::endl;
  std::cout << significantcut << " GeV" << std::endl;
  std::cout << percentDifference << " Significance percent differerence" << std::endl;
  //std::cout << "If running invariant mass divide cut GeV by 2 and multiply by 5" << std::endl;
}
