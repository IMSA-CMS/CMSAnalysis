#include "CMSAnalysis/Modules/interface/TriggerSimModule.hh"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Framework/interface/Event.h"
#include "CMSAnalysis/DataCollection/interface/EventLoader.hh"

TriggerSimModule::TriggerSimModule(std::string _subProcess) : subProcess(_subProcess){};

// For use to add needed triggers
void TriggerSimModule::addTrigger(const EnumTriggers triggerNameEnum)
{
  std::vector<std::string> tmp_nameVector = triggerEnumNameMap.find(triggerNameEnum)->second;
  for (std::string tmp_triggerName : tmp_nameVector)
  {
    addTrigger(tmp_triggerName);
  }
};

void TriggerSimModule::addTrigger(const std::vector<EnumTriggers> triggerNameEnums)
{
  for (EnumTriggers triggerEnum : triggerNameEnums)
  {
    addTrigger(triggerEnum);
  }
};

void TriggerSimModule::addTrigger(const std::string _triggerName)
{
  // Only adds the trigger if it is not already listed
  if (std::find(triggerNames.begin(), triggerNames.end(), _triggerName) == triggerNames.end())
  {
    triggerNames.push_back(_triggerName);
  }
};

void TriggerSimModule::addTrigger(const std::vector<std::string> _triggerNames)
{
  for (std::string trigger : _triggerNames)
  {
    addTrigger(trigger);
  }
};

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
};

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
};

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
};

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
};

// ?
void TriggerSimModule::initialize()
{
  for (auto name : triggerNames)
  {
    triggerResultsData.insert({name, {0, 0}});
  }
}

// For use as a production module
void TriggerSimModule::finalize()
{
  // Prints out a summary of the results
  std::cout << std::endl;
  auto iter = triggerResultsData.begin();
  //Do every other trigger because the entire list of triggers is too long to fit in the terminal
  //iter++;
  while (iter != triggerResultsData.end())
  {
    std::cout << iter->first << " : Passed " << iter->second.passed << " : Total " << iter->second.total << " : Passed/Total "
              << iter->second.passed / (double)iter->second.total << "\n";
    iter++;
    if (iter != triggerResultsData.end()) 
    {
      iter++;
    }
  }
  std::cout << std::endl;
};

bool TriggerSimModule::process()
{
  // std::cerr << "Notice: TriggerSimModule::process is running\n";
  passedTriggers.clear();

  // Get the triggers regarding the event
  // const auto triggerResults = getInput()->getTriggerResults(subProcess);

  // const auto names = getInput()->getTriggerNames(subProcess);


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

  // bool passAnyTrigger = false; // True if the event passes any of the triggers

  if (enableAll) 
  {
    auto allTriggerNames = getInput()->getTriggerNames(subProcess);
    
    auto results = getInput()->getTriggerResults(subProcess);

    for (unsigned int i = 0, n = allTriggerNames.size(); i < n; ++i)
    {
      // std::cout << allTriggerNames[i];
      // this creates a map key if it doesn't already exist
      triggerResultsData[allTriggerNames[i]];
      
      
      if (results[i]) 
      {
        ++triggerResultsData[allTriggerNames[i]].passed;
        passedTriggers.push_back(allTriggerNames[i]);
      }
      
      // if (getInput()->checkTrigger(allTriggerNames[i], subProcess))
      // {
        
      // }
    }
  } 
  else 
  {
    for (unsigned int i = 0, n = triggerNames.size(); i < n; ++i)
    {
      // Checks if the current trigger is meant to be analyzed 
      
      // if (enableAll || std::find(triggerNames.begin(), triggerNames.end(), names[i]) != triggerNames.end())
      // if (enableAll || std::find(triggerNames.begin(), triggerNames.end(), names[i].substr(0, names[i].find("_v") + 2)) != triggerNames.end())
      
      if (getInput()->checkTrigger(triggerNames[i], subProcess))
      {
        //std::cout << names[i] << "\n";
      
        // This line is where the trigger is actually checked

        // Set passAnyTrigger to true once the event passes one trigger
        // Since passAnyTrigger is automatically false, if the event doesn't pass any triggers
        //    then passAnyTrigger remains false
        // if (passCurrentTrigger)
        // {
        //   passAnyTrigger = true;
        // }

        // if in there, just increment 1 if it passed
        ++triggerResultsData[triggerNames[i]].passed;
        //std::cout << triggerNames[i] << " : " << triggerResultsData[triggerNames[i]].passed << "\n";
        passedTriggers.push_back(triggerNames[i]);
      }
    }
  }

  for (auto& pair : triggerResultsData)
  {
    ++pair.second.total;
  }

  // return passAnyTrigger;
  return true;
};

bool TriggerSimModule::checkTrigger(EnumTriggers name)
{
  bool passed = false;

  std::vector<std::string> nameVector = triggerEnumNameMap.find(name)->second;

  // For every trigger that passed
  for (std::string trigger : passedTriggers)
  {
    // If the trigger is one of the specified ones
    if (enableAll || std::find(nameVector.begin(), nameVector.end(), trigger) != nameVector.end())
    {
      passed = true;
    }
  }

  return passed;
};

// For use within a trigger object [Defunct]
std::map<std::string, bool> TriggerSimModule::checkEvent(std::shared_ptr<EventInput> input)
{
  // // Get the triggers regarding the event
  // const edm::TriggerResults triggerResults = input->getTriggerResults(subProcess);

  // const edm::TriggerNames &triggerNames = input->getTriggerNames(subProcess);
  std::map<std::string, bool> results;

  for (auto name : triggerNames)
  {
    results.insert({name, getInput()->checkTrigger(name, subProcess)});
  }
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
};