#ifndef TRIGGERSIMMODULE_HH
#define TRIGGERSIMMODULE_HH

#include <memory>
#include <algorithm>

#include "ProductionModule.hh"


class Trigger;

class TriggerSimModule : public ProductionModule
{
public:
  enum class EnumTriggers {singleMuonTriggers, doubleMuonTriggers, singleElectronTriggers, doubleElectronTriggers,
    isoSingleMuonTriggers, isoDoubleMuonTriggers, isoSingleElectronTriggers, isoDoubleElectronTriggers, debug};

  TriggerSimModule(std::string _subProcess = "HLT"); // Defaults to HLT -- There are three main sub processes: HLT, SIM, and RECO

  // Methods to only enable specific triggers
  void addTrigger(const EnumTriggers triggerNameEnum);
  void addTrigger(const std::vector<EnumTriggers> triggerNameEnums);
  void addTrigger(const std::string _triggerName);
  void addTrigger(const std::vector<std::string> _triggerNames);
  
  // Returns false if a trigger couldn't be found
  bool removeTrigger(const EnumTriggers triggerNameEnum);
  bool removeTrigger(const std::vector<EnumTriggers> triggerNameEnums);
  bool removeTrigger(const std::string _triggerName);
  bool removeTrigger(const std::vector<std::string> _triggerNames);

  void enableAllTriggers() {enableAll = true;}
  void disableAllTriggers() {enableAll = false;}

  // Methods for use as production module
  virtual void finalize() override;
  bool processEvent() {return process();} // Event Counter that calls virtual process function
  bool checkTrigger(EnumTriggers name);

  // Methods for use as trigger object
  // [Defunct]
  virtual std::map<std::string, bool> checkEvent(std::shared_ptr<InputModule> input);
  virtual std::vector<std::string> getNames() {return triggerNames;}
  // [/Defunct]

protected:
  virtual bool process() override;

private:
  struct PassTotal
  {
    int passed = 0;
    int total = 0;
  };

  bool enableAll = false;
  std::string subProcess;
  std::map<std::string, PassTotal> triggerResultsData;
  std::vector<std::string> triggerNames;
  std::vector<std::string> passedTriggers;
  const std::map<EnumTriggers, std::vector<std::string>> triggerEnumNameMap = {
    { EnumTriggers::singleMuonTriggers, {
        "HLT_L2Mu50_v2",
        "",
        "",
        ""} },
    { EnumTriggers::doubleMuonTriggers, {
        "",
        "",
        "",
        ""} },
    { EnumTriggers::singleElectronTriggers, {
        "",
        "",
        "",
        ""} },
    { EnumTriggers::doubleElectronTriggers, {
        "HLT_DoubleEle24_eta2p1_WPTight_Gsf_v5",
        "HLT_DoubleEle25_CaloIdL_MW_v2",
        "HLT_DoubleEle27_CaloIdL_MW_v2",
        "HLT_DoubleEle33_CaloIdL_MW_v15"} },
    { EnumTriggers::isoSingleMuonTriggers, {
        "",
        "",
        "",
        ""} },
    { EnumTriggers::isoDoubleMuonTriggers, {
        "",
        "",
        "",
        ""} },
    { EnumTriggers::isoSingleElectronTriggers, {
        "",
        "",
        "",
        ""} },
    { EnumTriggers::isoDoubleElectronTriggers, {
        "",
        "",
        "",
        ""} },
    { EnumTriggers::debug, {
        "HLT_IsoMu22_v2",
        "",
        "",
        ""} }
  };
};

#endif
