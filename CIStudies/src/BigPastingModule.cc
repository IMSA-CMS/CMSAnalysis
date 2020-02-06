#include "CIAnalysis/CIStudies/interface/BigPastingModule.hh"
#include "CIAnalysis/CIStudies/interface/PastingModule.hh"

BigPastingModule::BigPastingModule(PastingModule& pastingModule) :
  pasting(pastingModule)
{
}

void BigPastingModule::initialize()
{
  outputFile.open("histogramTables.txt");
  outputFile << "Process" << "\t" << "Year" << "\t" << "Particle" << "\t" << "Helicity" << "\t" << "Interference" << "\t" << "Lambda" << "\t" << "Mass" << "\t" << "Bin Contents" << std::endl;
}

bool BigPastingModule::process(const edm::EventBase& event)
{
  if (isNewHistogram())
    {
      printCurrentHistogram();
      clearCurrentHistogram();
    }

  return true;
}

void BigPastingModule::finalize()
{
  outputFile.close();
}

void BigPastingModule::printCurrentHistogram()
{
  pasting.finalize();

  std::string completeRowOutput;

  auto genSimHistogram = pasting.getHistogram("GenSim Invariant Mass Pasted");
  //auto recoHistogram = getHistogram("Reco Invariant Mass Pasted");

  for (int i = 1; i <= pasting.histBins; ++i)
    {
      auto massBin = pasting.minMassCut + (i - 1) * 100;
      auto binContents = genSimHistogram->GetBinContent(i);

      completeRowOutput = outputRow + "\t" + std::to_string(massBin) + "\t" + std::to_string(binContents);
      outputFile << completeRowOutput << std::endl;
    }

  pasting.massBins.clear();
}

void BigPastingModule::clearCurrentHistogram()
{
  pasting.getHistogram("GenSim Invariant Mass Pasted")->Reset();
  pasting.getHistogram("Reco Invariant Mass Pasted")->Reset();

  previousFileParams = getFileParams().getAllValues();
  outputRow = getFileParams().getTabSeparatedValues();
}

bool BigPastingModule::isNewHistogram()
{
  auto currentFileParams = getFileParams().getAllValues();
  const int massIndex = 4;

  for (unsigned int i = 0; i < currentFileParams.size(); ++i)
    {
      if (previousFileParams.empty())
	{
	  previousFileParams = getFileParams().getAllValues();
	  outputRow = getFileParams().getTabSeparatedValues();
	}

      if (i == massIndex)
	{
	  continue;
	}

      if (previousFileParams[i] != currentFileParams[i])
	{
	  return true;
	}
    }

  return false;
}
