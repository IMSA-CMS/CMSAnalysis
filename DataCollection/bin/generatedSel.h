/////////////////////////////////////////////////////////////////////////
//   This class has been automatically generated 
//   (at Thu May  4 11:58:17 2023 by ROOT version 6.24/07)
//   from TTree mytree/TestTree
//   found on file: /eos/uscms/store/user/snowmass/Snowmass2021/DelphesNtuplizer/DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_14TeV-madgraphMLM-pythia8_200PU/DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_14TeV-madgraphMLM-pythia8_ntuple_9_1.root
/////////////////////////////////////////////////////////////////////////


#ifndef generatedSel_h
#define generatedSel_h

#define R__BRANCHPROXY_GENERATOR_VERSION 2

// ROOT headers needed by the proxy
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TPad.h>
#include <TH1.h>
#include <TSelector.h>
#include <TBranchProxy.h>
#include <TBranchProxyDirector.h>
#include <TBranchProxyTemplate.h>
#include <TFriendProxy.h>
using namespace ROOT::Internal;
using ROOT::Detail::TBranchProxy;

// forward declarations needed by this particular proxy


// Header needed by this particular proxy


class unbinnedFitTest_Interface {
   // This class defines the list of methods that are directly used by generatedSel,
   // and that can be overloaded in the user's script
public:
   void unbinnedFitTest_Begin(TTree*) {}
   void unbinnedFitTest_SlaveBegin(TTree*) {}
   Bool_t unbinnedFitTest_Notify() { return kTRUE; }
   Bool_t unbinnedFitTest_Process(Long64_t) { return kTRUE; }
   void unbinnedFitTest_SlaveTerminate() {}
   void unbinnedFitTest_Terminate() {}
};


class generatedSel : public TSelector, public unbinnedFitTest_Interface {
public :
   TTree          *fChain;         //!pointer to the analyzed TTree or TChain
   TH1            *htemp;          //!pointer to the histogram
   TBranchProxyDirector fDirector; //!Manages the proxys

   // Optional User methods
   TClass         *fClass;    // Pointer to this class's description

   // Proxy for each of the branches, leaves and friends of the tree
   TIntProxy          evt_size;
   TIntProxy          vtx_size;
   TIntProxy          trueInteractions;
   TIntProxy          npuVertices;
   TArrayFloatProxy   vtx_pt2;
   TArrayFloatProxy   vtx_x;
   TArrayFloatProxy   vtx_y;
   TArrayFloatProxy   vtx_z;
   TFloatProxy        genweight;
   TIntProxy          lheweight_size;
   TArrayIntProxy     lheweight_val;
   TIntProxy          genpart_size;
   TArrayIntProxy     genpart_pid;
   TArrayIntProxy     genpart_status;
   TArrayFloatProxy   genpart_pt;
   TArrayFloatProxy   genpart_eta;
   TArrayFloatProxy   genpart_phi;
   TArrayFloatProxy   genpart_mass;
   TArrayIntProxy     genpart_m1;
   TArrayIntProxy     genpart_m2;
   TArrayIntProxy     genpart_d1;
   TArrayIntProxy     genpart_d2;
   TIntProxy          genjet_size;
   TArrayFloatProxy   genjet_pt;
   TArrayFloatProxy   genjet_eta;
   TArrayFloatProxy   genjet_phi;
   TArrayFloatProxy   genjet_mass;
   TIntProxy          genmet_size;
   TArrayFloatProxy   genmet_pt;
   TArrayFloatProxy   genmet_phi;
   TIntProxy          gamma_size;
   TArrayFloatProxy   gamma_pt;
   TArrayFloatProxy   gamma_eta;
   TArrayFloatProxy   gamma_phi;
   TArrayFloatProxy   gamma_mass;
   TArrayFloatProxy   gamma_idvar;
   TArrayFloatProxy   gamma_reliso;
   TArrayUIntProxy    gamma_idpass;
   TArrayUIntProxy    gamma_isopass;
   TIntProxy          elec_size;
   TArrayFloatProxy   elec_pt;
   TArrayFloatProxy   elec_eta;
   TArrayFloatProxy   elec_phi;
   TArrayFloatProxy   elec_mass;
   TArrayIntProxy     elec_charge;
   TArrayFloatProxy   elec_idvar;
   TArrayFloatProxy   elec_reliso;
   TArrayUIntProxy    elec_idpass;
   TArrayUIntProxy    elec_isopass;
   TIntProxy          muon_size;
   TArrayFloatProxy   muon_pt;
   TArrayFloatProxy   muon_eta;
   TArrayFloatProxy   muon_phi;
   TArrayFloatProxy   muon_mass;
   TArrayIntProxy     muon_charge;
   TArrayFloatProxy   muon_idvar;
   TArrayFloatProxy   muon_reliso;
   TArrayUIntProxy    muon_idpass;
   TArrayUIntProxy    muon_isopass;
   TIntProxy          tau_size;
   TArrayFloatProxy   tau_pt;
   TArrayFloatProxy   tau_eta;
   TArrayFloatProxy   tau_phi;
   TArrayFloatProxy   tau_mass;
   TArrayIntProxy     tau_charge;
   TArrayFloatProxy   tau_decaymode;
   TArrayFloatProxy   tau_neutraliso;
   TArrayFloatProxy   tau_chargediso;
   TArrayFloatProxy   tau_combinediso;
   TArrayUIntProxy    tau_isopass;
   TIntProxy          jetpuppi_size;
   TArrayFloatProxy   jetpuppi_pt;
   TArrayFloatProxy   jetpuppi_eta;
   TArrayFloatProxy   jetpuppi_phi;
   TArrayFloatProxy   jetpuppi_mass;
   TArrayUIntProxy    jetpuppi_idpass;
   TArrayFloatProxy   jetpuppi_DeepJET;
   TArrayIntProxy     jetpuppi_btag;
   TIntProxy          jetchs_size;
   TArrayFloatProxy   jetchs_pt;
   TArrayFloatProxy   jetchs_eta;
   TArrayFloatProxy   jetchs_phi;
   TArrayFloatProxy   jetchs_mass;
   TArrayUIntProxy    jetchs_idpass;
   TArrayFloatProxy   jetchs_DeepJET;
   TArrayIntProxy     jetchs_btag;
   TIntProxy          fatjet_size;
   TArrayFloatProxy   fatjet_pt;
   TArrayFloatProxy   fatjet_eta;
   TArrayFloatProxy   fatjet_phi;
   TArrayFloatProxy   fatjet_mass;
   TArrayFloatProxy   fatjet_tau1;
   TArrayFloatProxy   fatjet_tau2;
   TArrayFloatProxy   fatjet_tau3;
   TArrayFloatProxy   fatjet_tau4;
   TArrayFloatProxy   fatjet_msoftdrop;
   TIntProxy          metpuppi_size;
   TArrayFloatProxy   metpuppi_pt;
   TArrayFloatProxy   metpuppi_phi;
   TIntProxy          metpf_size;
   TArrayFloatProxy   metpf_pt;
   TArrayFloatProxy   metpf_phi;


   generatedSel(TTree *tree=0) : 
      fChain(0),
      htemp(0),
      fDirector(tree,-1),
      fClass                (TClass::GetClass("generatedSel")),
      evt_size          (&fDirector,"evt_size"),
      vtx_size          (&fDirector,"vtx_size"),
      trueInteractions  (&fDirector,"trueInteractions"),
      npuVertices       (&fDirector,"npuVertices"),
      vtx_pt2           (&fDirector,"vtx_pt2"),
      vtx_x             (&fDirector,"vtx_x"),
      vtx_y             (&fDirector,"vtx_y"),
      vtx_z             (&fDirector,"vtx_z"),
      genweight         (&fDirector,"genweight"),
      lheweight_size    (&fDirector,"lheweight_size"),
      lheweight_val     (&fDirector,"lheweight_val"),
      genpart_size      (&fDirector,"genpart_size"),
      genpart_pid       (&fDirector,"genpart_pid"),
      genpart_status    (&fDirector,"genpart_status"),
      genpart_pt        (&fDirector,"genpart_pt"),
      genpart_eta       (&fDirector,"genpart_eta"),
      genpart_phi       (&fDirector,"genpart_phi"),
      genpart_mass      (&fDirector,"genpart_mass"),
      genpart_m1        (&fDirector,"genpart_m1"),
      genpart_m2        (&fDirector,"genpart_m2"),
      genpart_d1        (&fDirector,"genpart_d1"),
      genpart_d2        (&fDirector,"genpart_d2"),
      genjet_size       (&fDirector,"genjet_size"),
      genjet_pt         (&fDirector,"genjet_pt"),
      genjet_eta        (&fDirector,"genjet_eta"),
      genjet_phi        (&fDirector,"genjet_phi"),
      genjet_mass       (&fDirector,"genjet_mass"),
      genmet_size       (&fDirector,"genmet_size"),
      genmet_pt         (&fDirector,"genmet_pt"),
      genmet_phi        (&fDirector,"genmet_phi"),
      gamma_size        (&fDirector,"gamma_size"),
      gamma_pt          (&fDirector,"gamma_pt"),
      gamma_eta         (&fDirector,"gamma_eta"),
      gamma_phi         (&fDirector,"gamma_phi"),
      gamma_mass        (&fDirector,"gamma_mass"),
      gamma_idvar       (&fDirector,"gamma_idvar"),
      gamma_reliso      (&fDirector,"gamma_reliso"),
      gamma_idpass      (&fDirector,"gamma_idpass"),
      gamma_isopass     (&fDirector,"gamma_isopass"),
      elec_size         (&fDirector,"elec_size"),
      elec_pt           (&fDirector,"elec_pt"),
      elec_eta          (&fDirector,"elec_eta"),
      elec_phi          (&fDirector,"elec_phi"),
      elec_mass         (&fDirector,"elec_mass"),
      elec_charge       (&fDirector,"elec_charge"),
      elec_idvar        (&fDirector,"elec_idvar"),
      elec_reliso       (&fDirector,"elec_reliso"),
      elec_idpass       (&fDirector,"elec_idpass"),
      elec_isopass      (&fDirector,"elec_isopass"),
      muon_size         (&fDirector,"muon_size"),
      muon_pt           (&fDirector,"muon_pt"),
      muon_eta          (&fDirector,"muon_eta"),
      muon_phi          (&fDirector,"muon_phi"),
      muon_mass         (&fDirector,"muon_mass"),
      muon_charge       (&fDirector,"muon_charge"),
      muon_idvar        (&fDirector,"muon_idvar"),
      muon_reliso       (&fDirector,"muon_reliso"),
      muon_idpass       (&fDirector,"muon_idpass"),
      muon_isopass      (&fDirector,"muon_isopass"),
      tau_size          (&fDirector,"tau_size"),
      tau_pt            (&fDirector,"tau_pt"),
      tau_eta           (&fDirector,"tau_eta"),
      tau_phi           (&fDirector,"tau_phi"),
      tau_mass          (&fDirector,"tau_mass"),
      tau_charge        (&fDirector,"tau_charge"),
      tau_decaymode     (&fDirector,"tau_decaymode"),
      tau_neutraliso    (&fDirector,"tau_neutraliso"),
      tau_chargediso    (&fDirector,"tau_chargediso"),
      tau_combinediso   (&fDirector,"tau_combinediso"),
      tau_isopass       (&fDirector,"tau_isopass"),
      jetpuppi_size     (&fDirector,"jetpuppi_size"),
      jetpuppi_pt       (&fDirector,"jetpuppi_pt"),
      jetpuppi_eta      (&fDirector,"jetpuppi_eta"),
      jetpuppi_phi      (&fDirector,"jetpuppi_phi"),
      jetpuppi_mass     (&fDirector,"jetpuppi_mass"),
      jetpuppi_idpass   (&fDirector,"jetpuppi_idpass"),
      jetpuppi_DeepJET  (&fDirector,"jetpuppi_DeepJET"),
      jetpuppi_btag     (&fDirector,"jetpuppi_btag"),
      jetchs_size       (&fDirector,"jetchs_size"),
      jetchs_pt         (&fDirector,"jetchs_pt"),
      jetchs_eta        (&fDirector,"jetchs_eta"),
      jetchs_phi        (&fDirector,"jetchs_phi"),
      jetchs_mass       (&fDirector,"jetchs_mass"),
      jetchs_idpass     (&fDirector,"jetchs_idpass"),
      jetchs_DeepJET    (&fDirector,"jetchs_DeepJET"),
      jetchs_btag       (&fDirector,"jetchs_btag"),
      fatjet_size       (&fDirector,"fatjet_size"),
      fatjet_pt         (&fDirector,"fatjet_pt"),
      fatjet_eta        (&fDirector,"fatjet_eta"),
      fatjet_phi        (&fDirector,"fatjet_phi"),
      fatjet_mass       (&fDirector,"fatjet_mass"),
      fatjet_tau1       (&fDirector,"fatjet_tau1"),
      fatjet_tau2       (&fDirector,"fatjet_tau2"),
      fatjet_tau3       (&fDirector,"fatjet_tau3"),
      fatjet_tau4       (&fDirector,"fatjet_tau4"),
      fatjet_msoftdrop  (&fDirector,"fatjet_msoftdrop"),
      metpuppi_size     (&fDirector,"metpuppi_size"),
      metpuppi_pt       (&fDirector,"metpuppi_pt"),
      metpuppi_phi      (&fDirector,"metpuppi_phi"),
      metpf_size        (&fDirector,"metpf_size"),
      metpf_pt          (&fDirector,"metpf_pt"),
      metpf_phi         (&fDirector,"metpf_phi")
      { }
   ~generatedSel();
   Int_t   Version() const {return 1;}
   void    Begin(::TTree *tree);
   void    SlaveBegin(::TTree *tree);
   void    Init(::TTree *tree);
   Bool_t  Notify();
   Bool_t  Process(Long64_t entry);
   void    SlaveTerminate();
   void    Terminate();

   ClassDef(generatedSel,0);


//inject the user's code
#include "unbinnedFitTest.C"
};

#endif


#ifdef __MAKECINT__
#pragma link C++ class generatedSel;
#endif


inline generatedSel::~generatedSel() {
   // destructor. Clean up helpers.

}

inline void generatedSel::Init(TTree *tree)
{
//   Set branch addresses
   if (tree == 0) return;
   fChain = tree;
   fDirector.SetTree(fChain);
   if (htemp == 0) {
      htemp = fDirector.CreateHistogram(GetOption());
      htemp->SetTitle("unbinnedFitTest.C");
      fObject = htemp;
   }
}

Bool_t generatedSel::Notify()
{
   // Called when loading a new file.
   // Get branch pointers.
   fDirector.SetTree(fChain);
   unbinnedFitTest_Notify();
   
   return kTRUE;
}
   

inline void generatedSel::Begin(TTree *tree)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
   unbinnedFitTest_Begin(tree);

}

inline void generatedSel::SlaveBegin(TTree *tree)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   Init(tree);

   unbinnedFitTest_SlaveBegin(tree);

}

inline Bool_t generatedSel::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either TTree::GetEntry() or TBranch::GetEntry()
   // to read either all or the required parts of the data. When processing
   // keyed objects with PROOF, the object is already loaded and is available
   // via the fObject pointer.
   //
   // This function should contain the "body" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.

   // WARNING when a selector is used with a TChain, you must use
   //  the pointer to the current TTree to call GetEntry(entry).
   //  The entry is always the local entry number in the current tree.
   //  Assuming that fChain is the pointer to the TChain being processed,
   //  use fChain->GetTree()->GetEntry(entry).


   fDirector.SetReadEntry(entry);
   htemp->Fill(unbinnedFitTest());
   unbinnedFitTest_Process(entry);
   return kTRUE;

}

inline void generatedSel::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.
   unbinnedFitTest_SlaveTerminate();
}

inline void generatedSel::Terminate()
{
   // Function called at the end of the event loop.
   htemp = (TH1*)fObject;
   Int_t drawflag = (htemp && htemp->GetEntries()>0);
   
   if (gPad && !drawflag && !fOption.Contains("goff") && !fOption.Contains("same")) {
      gPad->Clear();
   } else {
      if (fOption.Contains("goff")) drawflag = false;
      if (drawflag) htemp->Draw(fOption);
   }
   unbinnedFitTest_Terminate();
}
