#include "CIAnalysis/CIStudies/interface/TriggerModule.hh"

#include "CIAnalysis/CIStudies/interface/Trigger.hh"
#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"

TriggerModule::TriggerModule(std::shared_ptr<RecoIdentificationModule> iRecoMod) :
  recoMod(iRecoMod),
  passAny(0),
  total(0)
{
}

bool TriggerModule::process()
{
  bool passAnyTrigger = false;       // True if the event passes any of the triggers
  bool passCurrentTrigger;           // True if the event passes the current trigger

  //keeps track of exactly which triggers the event passes
  std::vector<bool> passSingleTrigger;

  for (auto trigger : triggers)
  {
    passCurrentTrigger = trigger->checkEvent(recoMod);

    // Set passAnyTrigger to true once the event passes one trigger
    // Since passAnyTrigger is automatically false, if the event doesn't pass any triggers, passAnyTrigger remains false
    if (passCurrentTrigger)  // if (passCurrentTrigger == true)  
    {
      passAnyTrigger = true;
      passSingleTrigger.push_back(true);
    }
    else
    {
      passSingleTrigger.push_back(false);
    }
  }

  
  if (overlap.find(passSingleTrigger) == overlap.end())
  {
    //if not in there, add vector to map, make value 1
    overlap.insert({passSingleTrigger,1}); 
  }
  else
  {
    //if in there, just increment 1
    overlap[passSingleTrigger]++;
  }

  // Increment the counters (passAny is incremented only if passAnyTrigger is true)
  if (passAnyTrigger)
  {
    ++passAny;
    //std::cout << passAny << '\n';
  }

  ++total;
  //std::cout << total << "\n\n";

  return passAnyTrigger;
}

int TriggerModule::twoTriggerOverlap(int index1, int index2)
{
  int counter = 0;
  for (auto event : overlap)
  {
    if (event.first[index1] == true || event.first[index2] == true)
    {
      counter += event.second;
    }
  }
  return counter;
}

int TriggerModule::nTriggerOverlap(std::vector<int> indices)
{
  int counter = 0;
  for (auto event : overlap)
  {
    bool anyTriggered = false;
    for (auto index : indices)
    {
      if (event.first[index] == true)
      {
        anyTriggered = true;
        break;
      }
    }
    if (anyTriggered)
    {
      counter += event.second;
    }
  }
  return counter;
}

void TriggerModule::callTriggerCombo()
{
  int total = triggers.size();
  std::vector<int> combination;
  std::vector<int> nTriggers;

  //populate vector with numbers for indices
  for (int i = 0; i < total; ++i)
  {
    nTriggers.push_back(i);
  }

  //find combinations for n-1 combos, where n is total num of triggers
  for (int i = 1; i < total; ++i)
  {
    triggerCombinations(0, i, combination, nTriggers);
  }
}

void TriggerModule::triggerCombinations(int offset, int nCombo, std::vector<int>& combination, std::vector<int>& nTriggers)
{
  
  if (nCombo == 0)
  {
    trigCombos.push_back(combination);
    return;
  }
  for (size_t i = offset; i <= nTriggers.size() - nCombo; ++i)
  {
    combination.push_back(nTriggers[i]);
    triggerCombinations(i+1, nCombo-1, combination, nTriggers);
    combination.pop_back();
  }
}

void TriggerModule::findMostEfficientCombo(double efficiency, int nPass, std::vector<int> combo)
{
  if (nPass > highestPass)
  {
    highestEfficiency = efficiency;
    highestPass = nPass;
    highestEffCombo.clear();
    for (size_t i = 0; i < combo.size(); ++i)
    {
      highestEffCombo.push_back(combo[i]);
    }
  }
}

void TriggerModule::finalize()
{
  //for (auto trigger : triggers)
  //{
  //  trigger->printTriggerResult();
  //  std::cout << '\n';
  //}

  std::cout << '\n';

  callTriggerCombo();
  for (size_t i = 0; i < trigCombos.size(); ++i)
  {
    int passCombo = nTriggerOverlap(trigCombos[i]);
    printComboEfficiency(passCombo, trigCombos[i]);
  }

  printOverallEfficiency();
}

void TriggerModule::printComboEfficiency(int passCombo, std::vector<int> trigIndices)
{
  //std::cout << trigIndices.size() << '\n';
  if (trigIndices.size() > 1)
  {
    std::cout << "Triggers combined:" << '\n';
  }
  for (size_t i = 0; i < trigIndices.size(); ++i)
  {
    int index = trigIndices[i];
    std::cout << triggers[index]->getName() << '\n';
  }
  std::cout << "Total event count: " << total << '\n';
  if (trigIndices.size() > 1)
  {
    std::cout << "Number of events passing at least one trigger: " << passCombo << '\n';
  }
  else
  {
    std::cout << "Total passed: " << passCombo << '\n';
  }
  double efficiency = static_cast<double>(passCombo) / total;
  findMostEfficientCombo(efficiency, passCombo, trigIndices);
  std::cout << "Overall Efficiency: " << efficiency << '\n' << std::endl;
}

void TriggerModule::printOverallEfficiency()
{
  //std::cout << "Total event count: " << total << '\n';
  //std::cout << "Number of events passing at least one trigger: " << passAny << '\n';
  //std::cout << "Overall Efficiency: " << static_cast<double>(passAny) / total << '\n';


  std::cout << "Most efficient combination:" << '\n';
  for (size_t i = 0; i < highestEffCombo.size(); ++i)
  {
    int index = highestEffCombo[i];
    std::cout << triggers[index]->getName() << '\n';
  }
  std::cout << "Total event count: " << total << '\n';
  std::cout << "Number of events passing at least one trigger: " << highestPass << '\n';
  std::cout << "Overall Efficiency: " << highestEfficiency << '\n' << std::endl;
  
}
