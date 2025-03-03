#include "CMSAnalysis/EventFiles/interface/NanoAODEventFile.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/GenSimSimpleImplementation.hh"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include <iostream>
#include <fstream>
#include <string>
#include "CMSAnalysis/Utility/interface/Utility.hh"

int NanoAODEventFile::positiveCounter = 0;
int NanoAODEventFile::negativeCounter = 0;

std::vector<bool> NanoAODEventFile::getTriggerResults(std::string subProcess) const
{
    // determines whether it passes the trigger"s criteria or not
    std::vector<bool> v_results = {};

    for(auto& trigger : triggers)
    {
        // std::cout << "Is this for loop working" << "\n";
        v_results.push_back(*(trigger.second));
    }

    return v_results;
}

std::vector<std::string> NanoAODEventFile::getTriggerNames(std::string subProcess) const
{
    std::vector<std::string> v_names = {};

    for(auto& trigger : triggers)
    {
        v_names.push_back(trigger.first);
    }

    return v_names;
}

bool NanoAODEventFile::checkTrigger(std::string triggerName, std::string subProcess) const
{
    // add cout statement to check, it in fact does pass through this code
    //std::cout << "Does this Trigger work?" << "\n";
    //std::cout << triggerName << "\n";
    auto trigger = triggers.find(triggerName);
    if (trigger == triggers.end()) 
    {
       // return false;
       if(!tree->GetBranch(triggerName.c_str()))
       {
        //std::cout << triggerName << " doesn't exist\n";
        return false;
       }
        auto currentEntry = treeReader.GetCurrentEntry();
        treeReader.Restart();
        TTreeReaderValue<Bool_t> intermediate(treeReader, triggerName.c_str());
        trigger = triggers.emplace(triggerName, intermediate).first;
        //std::cout << triggerName <<"\n";
        treeReader.SetTree(tree);
        treeReader.SetEntry(currentEntry-1);
        treeReader.Next();
        // call Restart() which calls next but goes back to first entry, then we go to SetEntry again then treeReader.Next again, do this after setTree
    }
    return *(trigger->second);
     
}

NanoAODEventFile::NanoAODEventFile(TFile *ifile, std::shared_ptr<FileParams> iparams) : 
    EventFile(ifile, iparams)
    {
    //initializing variables from header file
    std::vector<std::shared_ptr<TreeVariableBase>> treeVariables = {
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("elec_eta", "Electron_eta"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("elec_phi", "Electron_phi"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("elec_mass", "Electron_mass"),
		std::make_shared<TreeVariable<TTreeReaderArray<Int_t>>>("elec_charge", "Electron_charge"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("elec_pt", "Electron_pt"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("elec_reliso", "Electron_miniPFRelIso_all"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("elec_dxy", "Electron_dxy"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("elec_dz", "Electron_dz"),
        //std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("elec_dEscaleDown", "Electron_dEscaleDown"),
		//std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("elec_dEscaleUp", "Electron_dEscaleUp"),
		//std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("elec_dEsigmaDown", "Electron_dEsigmaDown"),
        //std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("elec_dEsigmaUp", "Electron_dEsigmaUp"),
		std::make_shared<TreeVariable<TTreeReaderValue<UInt_t>>>("muon_size", "nMuon"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("muon_eta", "Muon_eta"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("muon_phi", "Muon_phi"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("muon_mass", "Muon_mass"),
		std::make_shared<TreeVariable<TTreeReaderArray<Int_t>>>("muon_charge", "Muon_charge"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("muon_pt", "Muon_pt"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("muon_reliso", "Muon_miniPFRelIso_all"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("muon_dxy", "Muon_dxy"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("muon_dz", "Muon_dz"),

		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("photon_eta", "Photon_eta"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("photon_phi", "Photon_phi"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("photon_pt", "Photon_pt"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("met_phi", "MET_phi"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("met_pt", "MET_pt"),
		
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("jet_eta", "Jet_eta"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("jet_phi", "Jet_phi"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("jet_mass", "Jet_mass"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("jet_pt", "Jet_pt"),


		std::make_shared<TreeVariable<TTreeReaderArray<Int_t>>>("elec_idpass", "Electron_cutBased"),

    	std::make_shared<TreeVariable<TTreeReaderArray<Bool_t>>>("mvaElecWP80", "Electron_mvaFall17V2Iso_WP80"),
		std::make_shared<TreeVariable<TTreeReaderArray<Bool_t>>>("mvaElecWP90", "Electron_mvaFall17V2Iso_WP90"),
		std::make_shared<TreeVariable<TTreeReaderArray<Bool_t>>>("mvaElecWPL", "Electron_mvaFall17V2Iso_WPL"),


        std::make_shared<TreeVariable<TTreeReaderArray<Bool_t>>>("elec_cutBasedHEEP", "Electron_cutBased_HEEP"),

        std::make_shared<TreeVariable<TTreeReaderArray<UChar_t>>>("muon_mvaid", "Muon_mvaId"),
		std::make_shared<TreeVariable<TTreeReaderArray<Bool_t>>>("muon_looseid", "Muon_looseId"),
		std::make_shared<TreeVariable<TTreeReaderArray<Bool_t>>>("muon_mediumid", "Muon_mediumId"),
		std::make_shared<TreeVariable<TTreeReaderArray<Bool_t>>>("muon_tightid", "Muon_tightId"),
		std::make_shared<TreeVariable<TTreeReaderValue<ULong64_t>>>("event_number", "event"),
        std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("jet_bTag", "Jet_btagCSVV2"),
        std::make_shared<TreeVariable<TTreeReaderValue<UInt_t>>>("jet_size", "nJet"),

        std::make_shared<TreeVariable<TTreeReaderValue<UInt_t>>>("gen_size", "nGenPart"),
		std::make_shared<TreeVariable<TTreeReaderArray<Int_t>>>("gen_pid", "GenPart_pdgId"),
		std::make_shared<TreeVariable<TTreeReaderArray<Int_t>>>("gen_status", "GenPart_status"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("gen_eta", "GenPart_eta"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("gen_phi", "GenPart_phi"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("gen_mass", "GenPart_mass"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("gen_pt", "GenPart_pt"),
		std::make_shared<TreeVariable<TTreeReaderArray<Int_t>>>("gen_m1", "GenPart_genPartIdxMother"),
        std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("pdf_weight", "LHEPdfWeight"),
        std::make_shared<TreeVariable<TTreeReaderValue<UInt_t>>>("num_pdfs", "nLHEPdfWeight"),
		std::make_shared<TreeVariable<TTreeReaderArray<Float_t>>>("gen_pileup", "Pileup_nTrueInt"),
        std::make_shared<TreeVariable<TTreeReaderArray<Int_t>>>("HEEP_bitmap", "Electron_vidNestedWPBitmapHEEP"),
        std::make_shared<TreeVariable<TTreeReaderArray<Int_t>>>("Electron_bitmap", "Electron_vidNestedWPBitmap"),
        std::make_shared<TreeVariable<TTreeReaderValue<ULong64_t>>>("event", "event"),
        std::make_shared<TreeVariable<TTreeReaderValue<UInt_t>>>("run", "run")
    };

    // std::make_shared<TreeVariable<TTreeReaderValue<UInt_t>>>("elec_size", "nElectron"),
    // std::make_shared<TreeVariable<TTreeReaderValue<UInt_t>>>("muon_size", "nMuon"),
    // std::make_shared<TreeVariable<TTreeReaderValue<UInt_t>>>("photon_size", "nPhoton"),
    // std::make_shared<TreeVariable<TTreeReaderValue<UInt_t>>>("jet_size", "nJet"),
    // std::make_shared<TreeVariable<TTreeReaderValue<UInt_t>>>("gen_size", "nGenPart"),
   
    tree = getFile()->Get<TTree>("Events");

    TBranch *branch_1 = tree->FindBranch("BeamSpot");
    TBranch *branch_2 = tree->FindBranch("GenProton");
    TBranch *branch_3 = tree->FindBranch("Rho");

    if(branch_1 && branch_2 && branch_3) //version 12
    {
        int versionNum=12;
        treeVariables.push_back(std::make_shared<TreeVariable<TTreeReaderValue<Int_t>>>("elec_size", "nElectron"));
        treeVariables.push_back(std::make_shared<TreeVariable<TTreeReaderValue<Int_t>>>("muon_size", "nMuon"));
        treeVariables.push_back(std::make_shared<TreeVariable<TTreeReaderValue<Int_t>>>("photon_size", "nPhoton"));
        std::cout<<"This is version 12\n";
    }
    else //version 9
    {
        int versionNum=9;
        treeVariables.push_back(std::make_shared<TreeVariable<TTreeReaderValue<UInt_t>>>("elec_size", "nElectron"));
        treeVariables.push_back(std::make_shared<TreeVariable<TTreeReaderValue<UInt_t>>>("muon_size", "nMuon"));
        treeVariables.push_back(std::make_shared<TreeVariable<TTreeReaderValue<UInt_t>>>("photon_size", "nPhoton"));
        std::cout<<"This is version 9\n";

    }

    for(auto& var : treeVariables)
    {
        // std::cout << "Checking " << var->getBranchName() << std::endl;
        if(tree->GetBranch(var->getBranchName().c_str()))
        {
            // std::cout << "Adding " << var->getBranchName() << std::endl;
            variables.emplace(var->getName(), var->makeReader(treeReader));
        }
        else
        {
            std::cout << "Branch " << var->getBranchName() << " not found" << std::endl;
        }
    }
    treeReader.SetTree(tree);
    setEventCount(1);
    treeReader.Next(); 
}

void NanoAODEventFile::nextEvent()
{ 
    treeReader.Next(); 
    setEventCount(getEventCount() + 1);
    
    if(variables.find("gen_size") != variables.end() && getVariable<UInt_t>("gen_size") > 0)
    {
        genSimParticles.clear(); 
        genSimParticles.reserve(getVariable<UInt_t>("gen_size") );

        //construct daughters from mothers
        int mothersIndex;
        std::vector<std::vector<Int_t>> daughtersVectors((int)getVariable<UInt_t>("gen_size"), std::vector<int>(0)); //Vector of Vectors, the first index represents the mother, the second vector has the aughter indexes        
        for (unsigned j = 0; j < getVariable<UInt_t>("gen_size"); j++)
        {
            mothersIndex = getArrayElement<Int_t>("gen_m1", j); //Starts at particle 0, then gets its mother
            if (mothersIndex < (int)getVariable<UInt_t>("gen_size") && mothersIndex > -1)
            {
                daughtersVectors[mothersIndex].push_back(j); //at daughtersVectors[mothersIndex], adds the current particle, making the current particle the daughter
                //daughersVectors[i] is a vector of the daughters of particle i
            }
        }

        for (unsigned i = 0; i < getVariable<UInt_t>("gen_size") ; i++) //Loops through every particle
        {
            std::vector<const GenSimParticle*> daughterCollectionVector{};
            for (auto index : daughtersVectors[i]) //for every particle, go throught its daughters and add add its index in genSimp ro daughter collection vector
            {
                //std::cout<< "index: " << index << "\n";
                //std::cout<< "size: " <<daughtersVectors[i].size() << "\n";

                daughterCollectionVector.push_back(&genSimParticles[index]); //probably something wrong here
             
               //gensim is cleared every time, so its always empty, so the daughter thing is pointing at garbage?, probably not

                //std::cout<< "gensimindex: " << &genSimParticles[index] << "\n";
                //std::cout<< "daughter collection index" << daughterCollectionVector[0] << "\n";                 //index and vector addresses are the same
            }
            GenSimParticle *mother = nullptr;
            if(getArrayElement<Int_t>("gen_m1", i) != -1){
                mother = &genSimParticles[getArrayElement<Int_t>("gen_m1", i)];
                //std::cout<<"Mother: "<<mother;
                
            }
            genSimParticles.push_back(GenSimParticle(reco::Candidate::LorentzVector(math::PtEtaPhiMLorentzVector(
                getArrayElement<Float_t>("gen_pt", i),
                getArrayElement<Float_t>("gen_eta", i), 
                getArrayElement<Float_t>("gen_phi", i), 
                getArrayElement<Float_t>("gen_mass", i))), 
                getArrayElement<Int_t>("gen_pid", i),
                mother,
                daughterCollectionVector, 
                getArrayElement<Int_t>("gen_status", i)));
            
            
            if(daughterCollectionVector.size() > 0) {
                //std::cout<< "GenSimParticle " << &genSimParticles[daughtersVectors[i][0]] << "\n";
                //std::cout<< "Daughter zero: " << daughterCollectionVector[0] << "\n";
                if(&genSimParticles[daughtersVectors[i][0]] != daughterCollectionVector[0]) {
                    //std::cout << "ERROR!" << "\n";
                }

                //std::cout<< "Daughter: " << daughterCollectionVector[0]->getName(); //segfaulted
            }

        }
    }

    //std::cout << "Run: " << getVariable<UInt_t>("run") << std::endl;
}

ParticleCollection<GenSimParticle> NanoAODEventFile::getGenSimParticles() const
{
    return genSimParticles;
}

ParticleCollection<Particle> NanoAODEventFile::getRecoParticles() const
{
    ParticleCollection<Particle> recoParticles;
    for (UInt_t i = 0; i < getVariable<UInt_t>("elec_size"); i++)
    {
        int charge = getArrayElement<Int_t>("elec_charge", i);
        //std::cout<<"Electron: " << charge << "\n";
        if (charge == 1)
        {
            positiveCounter++;
            //std::cout<<"\nPositive counter: " << positiveCounter << "\n";
        }
        else if (charge == -1)
        {
            negativeCounter++;
            //std::cout<<"\nNegative counter: " << negativeCounter << "\n";
        }

    // Particle::SelectionFit fit;
    //    if (getArrayElement<Bool_t>("mvaElecWP80", i)) 
    //     {
    //         fit = Particle::SelectionFit::Tight;
    //     } else if (getArrayElement<Bool_t>("mvaElecWP90", i)) 
    //     {
    //         fit = Particle::SelectionFit::Medium;
    //     } else if (getArrayElement<Bool_t>("mvaElecWPL", i)) 
    //     {
    //         fit = Particle::SelectionFit::Loose;
    //     } else {
    //         continue;
    //     }


        // std::make_shared<TreeVariable<TTreeReaderArray<Bool_t>>>("mvaElecWP80", "MVA Iso ID V2 WP80 Electron_mvaFall17V2Iso_WP80"),
		// std::make_shared<TreeVariable<TTreeReaderArray<Bool_t>>>("mvaElecWP90", "Electron_mvaFall17V2Iso_WP90"),
		// std::make_shared<TreeVariable<TTreeReaderArray<Bool_t>>>("mvaElecWPL", "Electron_mvaFall17V2Iso_WPL"),

        Particle::SelectionFit fit;
        if (getArrayElement<Int_t>("elec_idpass", i) == 4) 
        {
            fit = Particle::SelectionFit::Tight;
        } else if (getArrayElement<Int_t>("elec_idpass", i) == 3) 
        {
            fit = Particle::SelectionFit::Medium;
        } else if (getArrayElement<Int_t>("elec_idpass", i) == 2) 
        {
            fit = Particle::SelectionFit::Loose;
        } else {
            continue;
        }


        // std::cout << "Loading electron from NanoAOD\n";
        // Lorentz four-vector
        auto particle = Particle(
        reco::Candidate::LorentzVector(math::PtEtaPhiMLorentzVector(getArrayElement<Float_t>("elec_pt", i),
        getArrayElement<Float_t>("elec_eta", i), getArrayElement<Float_t>("elec_phi", i), getArrayElement<Float_t>("elec_mass", i))),
        getArrayElement<Float_t>("elec_dxy", i),
        getArrayElement<Float_t>("elec_dz", i),
        charge, ParticleType::electron(), fit);
        // std::cout << "NanoAOD: " << getArrayElement<Bool_t>("elec_cutBasedHEEP", i) << '\n';
//        particle.addInfo("CutBasedHEEP", getArrayElement<Bool_t>("elec_cutBasedHEEP", i));
        particle.addInfo("Isolation", getArrayElement<Float_t>("elec_reliso", i));
        particle.addInfo("HEEP_map", getArrayElement<Int_t>("HEEP_bitmap", i)); 
        particle.addInfo("Electron_map", getArrayElement<Int_t>("Electron_bitmap", i)); 
        particle.addInfo("dxy", getArrayElement<Float_t>("elec_dxy", i));
        particle.addInfo("dz", getArrayElement<Float_t>("elec_dz", i));
        //particle.addInfo("eScaleDown", getArrayElement<Float_t>("elec_dEscaleDown", i));
        //particle.addInfo("eScaleUp", getArrayElement<Float_t>("elec_dEscaleUp", i));
        //particle.addInfo("eSigmaDown", getArrayElement<Float_t>("elec_dEsigmaDown", i));
        //particle.addInfo("eSigmaUp", getArrayElement<Float_t>("elec_dEsigmaUp", i));



   
        recoParticles.addParticle(particle);
        // std::cout << "Particle: " << particle.getInfo("CutBasedHEEP") << '\n';
    }

    for (UInt_t i = 0; i < getVariable<UInt_t>("muon_size"); i++)
    {
        int charge = getArrayElement<Int_t>("muon_charge", i);
        //std::cout<<charge;
        Particle::SelectionFit fit;

        if (getArrayElement<UChar_t>("muon_mvaid", i) >= 3) 
        {
            fit = Particle::SelectionFit::Tight;
        } else if (getArrayElement<UChar_t>("muon_mvaid", i) == 2) 
        {
            fit = Particle::SelectionFit::Medium;
        } else if (getArrayElement<UChar_t>("muon_mvaid", i) == 1) 
        {
            fit = Particle::SelectionFit::Loose;
        } else {
            continue;
        }



        //  if (fit == Particle::SelectionFit::Loose) continue;

        // std::cout << "Loading muon from NanoAOD\n";
        // Lorentz four-vector
        auto particle = Particle(
        reco::Candidate::LorentzVector(math::PtEtaPhiMLorentzVector(getArrayElement<Float_t>("muon_pt", i),
        getArrayElement<Float_t>("muon_eta", i), getArrayElement<Float_t>("muon_phi", i), getArrayElement<Float_t>("muon_mass", i))),
        getArrayElement<Float_t>("muon_dxy", i), getArrayElement<Float_t>("muon_dz", i), charge, ParticleType::muon(), fit);
        
        particle.addInfo("Isolation", getArrayElement<Float_t>("muon_reliso", i)); 
        particle.addInfo("dxy", getArrayElement<Float_t>("muon_dxy", i));
        particle.addInfo("dz", getArrayElement<Float_t>("muon_dz", i));
        recoParticles.addParticle(particle);
    }
    for (UInt_t i = 0; i < getVariable<UInt_t>("photon_size"); i++)
    {
        Particle::SelectionFit fit = Particle::SelectionFit::None;

        // std::cout << "Loading photon from NanoAOD\n";
        auto particle = Particle(
        reco::Candidate::LorentzVector(math::PtEtaPhiMLorentzVector(getArrayElement<Float_t>("photon_pt", i),
        getArrayElement<Float_t>("photon_eta", i), getArrayElement<Float_t>("photon_phi", i), 0)),
            0, 0, 0, ParticleType::photon(), fit);
        recoParticles.addParticle(particle);
    }
// //    for (auto& particle : recoParticles)
// //    {
//         particle.addInfo("numPDFs", getVariable<UInt_t>("num_pdfs"));
//         for (UInt_t i = 0; i < getVariable<UInt_t>("num_pdfs"); i++)
//         {
//             auto weight = getArrayElement<Float_t>("pdf_weight", i);
//             particle.addInfo("pweight" + std::to_string(i), weight);
//         }
//        }    
    return recoParticles;
}



ParticleCollection<Particle> NanoAODEventFile::getRecoJets() const
{
    ParticleCollection<Particle> recoParticles;
    for(UInt_t i = 0; i < getVariable<UInt_t>("jet_size"); i++) 
    {
        Particle particle(reco::Candidate::LorentzVector(getArrayElement<Float_t>("jet_pt", i), getArrayElement<Float_t>("jet_eta", i), getArrayElement<Float_t>("jet_phi", i), getArrayElement<Float_t>("jet_mass", i)), 
        //getArrayElement<Float_t>("jet_dxy", i), getArrayElement<Float_t>("jet_dz", i), 0, 
        0, 0, 0,
        ParticleType::jet());
        particle.addInfo("bJet", getArrayElement<Float_t>("jet_bTag", i));
        recoParticles.addParticle(particle);
             
    }
    return recoParticles;
}

double NanoAODEventFile::getMET() const
{
    // std::cout << "met_pt TEST: " << static_cast<double>(getArrayElement<Float_t>("met_pt", 0)) << std::endl;
    return static_cast<double>(getArrayElement<Float_t>("met_pt", 0));
}

unsigned long long NanoAODEventFile::getEventIDNum() const
{
    return getVariable<ULong64_t>("event");
}
long NanoAODEventFile::getRunNum() const
{
    return getVariable<UInt_t>("run");
}

int NanoAODEventFile::getNumPileUpInteractions() const
{
    return 1;
}

bool NanoAODEventFile::isDone() const
{
    return getEventCount() > tree->GetEntries();
}

std::vector<double> NanoAODEventFile::getPDFWeights() const
{
    std::vector<double> pdfWeights;
    for (UInt_t i = 0; i < getVariable<UInt_t>("num_pdfs"); ++i)
    {
        pdfWeights.push_back((double) getArrayElement<Float_t>("pdf_weight", i));
    }

    return pdfWeights;
}