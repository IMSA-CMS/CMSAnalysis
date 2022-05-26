#include "CMSAnalysis/DataCollection/interface/TriggerSimModule.hh"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Framework/interface/Event.h"
#include "CMSAnalysis/DataCollection/interface/EventLoader.hh"


TriggerSimModule::TriggerSimModule(std::string _subProcess) :
  subProcess(_subProcess)
{}

// For use to add needed triggers
void TriggerSimModule::addTrigger(const EnumTriggers triggerNameEnum)
{
  std::vector<std::string> tmp_nameVector = triggerEnumNameMap.find(triggerNameEnum)->second;
  for (std::string tmp_triggerName : tmp_nameVector)
  {
    addTrigger(tmp_triggerName);
  }
}

void TriggerSimModule::addTrigger(const std::vector<EnumTriggers> triggerNameEnums)
{
  for (EnumTriggers triggerEnum : triggerNameEnums)
  {
    addTrigger(triggerEnum);
  }
}

void TriggerSimModule::addTrigger(const std::string _triggerName)
{
  // Only adds the trigger if it is not already listed
  if (std::find(triggerNames.begin(), triggerNames.end(), _triggerName) == triggerNames.end())
  {
    triggerNames.push_back(_triggerName);
  }
}

void TriggerSimModule::addTrigger(const std::vector<std::string> _triggerNames)
{
  for (std::string trigger : _triggerNames)
  {
    addTrigger(trigger);
  }
}

bool TriggerSimModule::removeTrigger(const EnumTriggers triggerNameEnum)
{
  bool passed = true;

  std::vector<std::string> tmp_nameVector = triggerEnumNameMap.find(triggerNameEnum)->second;
  for (std::string tmp_triggerName : tmp_nameVector)
  {
    if (!removeTrigger(tmp_triggerName))
    {
      passed = false;
    }
  }

  return passed;
}

bool TriggerSimModule::removeTrigger(const std::vector<EnumTriggers> triggerNameEnums)
{
  bool passed = true;
  
  for (EnumTriggers triggerEnum : triggerNameEnums)
  {
    if (!removeTrigger(triggerEnum))
    {
      passed = false;
    }
  }

  return passed;
}

bool TriggerSimModule::removeTrigger(const std::string _triggerName)
{
  // Removes the trigger, if it's listed
  auto located_iterator = std::find(triggerNames.begin(), triggerNames.end(), _triggerName);
  if (located_iterator != triggerNames.end())
  {
    triggerNames.erase(located_iterator);
    return true;
  }
  else
  {
    return false;
  }
}

bool TriggerSimModule::removeTrigger(const std::vector<std::string> _triggerNames)
{
  bool passed;

  for (std::string trigger : _triggerNames)
  {
    if (!removeTrigger(trigger))
    {
      passed = false;
    }
  }

  return passed;
}

// For use as a production module
void TriggerSimModule::finalize()
{
  // Prints out a summary of the results
  std::cout << std::endl;
  auto iter = triggerResultsData.begin();
  while (iter != triggerResultsData.end())
  {
    std::cout << iter->first << " : Passed " << iter->second.passed << " : Total " << iter->second.total << " : Passed/Total "
        << iter->second.passed / (double)iter->second.total << "\n";
    iter++;
  }
  std::cout << std::endl;
}

bool TriggerSimModule::process()
{
  //std::cerr << "Notice: TriggerSimModule::process is running\n";
  passedTriggers.clear();

  // Get the triggers regarding the event
  const auto triggerResults = getInput()->getTriggerResults(subProcess);
    
  const auto names = getInput()->getTriggerNames(subProcess);

/*
  // Example code from the twiki on trigger bits
  // Available here: https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookMiniAOD2017#Trigger
  for (unsigned int i = 0, n = triggerBits->size(); i < n; ++i)
  {
    std::cout << "Trigger " << names.triggerName(i) <<
        ", prescale " << triggerPrescales->getPrescaleForIndex(i) <<
        ": " << (triggerBits->accept(i) ? "PASS" : "fail (or not run)")
        << std::endl;
  }
*/

  bool passAnyTrigger = false;       // True if the event passes any of the triggers
  bool passCurrentTrigger;           // True if the event passes the current trigger

  for (unsigned int i = 0, n = triggerResults.size(); i < n; ++i)
  {
    // Checks if the current trigger is meant to be analyzed
    if (enableAll || std::find(triggerNames.begin(), triggerNames.end(), names[i])
        != triggerNames.end())
    {
      // This line is where the trigger is actually checked
      passCurrentTrigger = triggerResults[i];

      // Set passAnyTrigger to true once the event passes one trigger
      // Since passAnyTrigger is automatically false, if the event doesn't pass any triggers
      //    then passAnyTrigger remains false
      if (passCurrentTrigger) 
      {
        passAnyTrigger = true;
      }
      
      if (triggerResultsData.find(names[i]) == triggerResultsData.end())
      {
        // if not in there, add a log to the map, make value 1 if it passed
        triggerResultsData.insert({names[i], {(int)passCurrentTrigger, 1}}); 
      }
      else
      {
        // if in there, just increment 1 if it passed
        triggerResultsData[names[i]].passed += (int)passCurrentTrigger;
        ++triggerResultsData[names[i]].total;
      }

      if (passCurrentTrigger)
      {
        passedTriggers.push_back(names[i]);
      }
    }
  }

  return passAnyTrigger;
  return true;
}

bool TriggerSimModule::checkTrigger(EnumTriggers name)
{
  //std::cerr << "Notice: TriggerSimModule::checkTrigger is running\n";
  bool passed = false;
  
  std::vector<std::string> nameVector = triggerEnumNameMap.find(name)->second;

  // For every trigger that passed
  for (std::string trigger : passedTriggers)
  {
    //std::cerr << trigger << std::endl;
    
    // If the trigger is one of the specified ones
    if (enableAll || std::find(nameVector.begin(), nameVector.end(), trigger) != nameVector.end())
    {
      passed = true;
    }
  }

  return passed;
  return true;
}


// For use within a trigger object [Defunct]
std::map<std::string, bool> TriggerSimModule::checkEvent(std::shared_ptr<InputModule> input)
{
  // // Get the triggers regarding the event
  // const edm::TriggerResults triggerResults = input->getTriggerResults(subProcess);
    
  // const edm::TriggerNames &triggerNames = input->getTriggerNames(subProcess);
  std::map<std::string, bool> results;

  // for (unsigned int i = 0, n = triggerResults->size(); i < n; ++i)
  // {
  //   // Checks if the current trigger is meant to be analyzed
  //   if (enableAll || std::find(triggerNames.begin(), triggerNames.end(), triggerNames.triggerName(i))
  //       != triggerNames.end())
  //   {      
  //     // Log the results of the trigger
  //     results.insert({triggerNames.triggerName(i), triggerResults->accept(i)}); 
  //   }
  // }

  return results;
}