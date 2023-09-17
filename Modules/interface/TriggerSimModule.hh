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
    isoSingleMuonTriggers, isoDoubleMuonTriggers, isoSingleElectronTriggers, isoDoubleElectronTriggers, photonTriggers, muonElectronTriggers, debug};

  TriggerSimModule(std::string _subProcess = "HLT"); // Defaults to HLT -- There are three main sub processes: HLT, SIM, and RECO. In data files, SIM is replaced with PAT

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
  virtual std::map<std::string, bool> checkEvent(std::shared_ptr<EventInput> input);
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
        "HLT_IsoMu24_v7",
        "HLT_IsoMu24_v6",
        "HLT_IsoMu24_v5",
        "HLT_IsoMu24_v13",
        ""} },
    { EnumTriggers::muonElectronTriggers, {
      // do not delete these, add in the right version numbers (from new data spreadsheet)
      // new trigger
        
        "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v4",
        "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v6",
        "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v6",
        "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v5",
        "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v8",
        "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v13",
        "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v15"
    } },
    { EnumTriggers::doubleMuonTriggers, {
        "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v7",
        "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v9",
        "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v8",
        "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v14"
        } },
    { EnumTriggers::singleElectronTriggers, {
        "HLT_Ele27_WPTight_Gsf_v10",
        "HLT_Ele27_WPTight_Gsf_v9",
        "HLT_Ele27_WPTight_Gsf_v16", 
        ""} },

    { EnumTriggers::doubleElectronTriggers, {
        "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v12",
        "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v11",
        "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v19",
        ""} },
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
    { EnumTriggers::photonTriggers, {
        "HLT_Photon75",
        "",
        "",
        ""} },
    { EnumTriggers::debug, {
        "DST_Physics_v2",
        "",
        "",
        ""} }
  };
};

#endif