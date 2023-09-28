#ifndef NANOAODEVENTLOADER_HH
#define NANOAODEVENTLOADER_HH

#include "EventFile.hh"
#include "TFile.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include <vector>


class TTree;

class NanoAODEventFile : public EventFile
{
    public:
        NanoAODEventFile(TFile* ifile);
        virtual void nextEvent() override;
        virtual bool isDone() const override;
        // simply dumps gensim particles from event into ParticleCollection
        virtual ParticleCollection<GenSimParticle> getGenSimParticles() const override;
        virtual ParticleCollection<Particle> getRecoParticles() const override;
        virtual ParticleCollection<Particle> getRecoJets() const override;
        virtual int getNumOfEvents() const override {return tree->GetEntries();}
        //virtual GenEventInfoProduct getGenInfo() const override;
        virtual double getMET() const override;
        virtual long getEventIDNum() const override; 
        virtual int getNumPileUpInteractions() const override;
        virtual std::vector<bool> getTriggerResults(std::string subProcess) const override;
        virtual std::vector<std::string> getTriggerNames(std::string subProcess) const override;
        virtual bool checkTrigger(std::string triggerName, std::string subProcess) const override; //checks if event passes trigger criteria
    private:

        TTree* tree;
        TTreeReader treeReader;

        class TreeVariableBase
        {
            public:
                virtual ~TreeVariableBase() {}
                virtual std::shared_ptr<ROOT::Internal::TTreeReaderValueBase> makeReader(TTreeReader& reader) const = 0;
                TreeVariableBase(std::string iname) : name(iname) {}
                std::string getName() const { return name; }
                virtual std::string getBranchName() const = 0;
            private:
                std::string name;
        };

        template<typename T>
        class TreeVariable : public TreeVariableBase
        {
            public:
                TreeVariable(std::string iname, std::string ibranchName) : TreeVariableBase(iname), branchName(ibranchName) {}
                virtual std::shared_ptr<ROOT::Internal::TTreeReaderValueBase> makeReader(TTreeReader& reader) const override
                {
                    return std::static_pointer_cast<ROOT::Internal::TTreeReaderValueBase>(std::make_shared<T>(reader, branchName.c_str()));
                }
                std::string getBranchName() const override { return branchName; }
            private:
                std::string branchName;
        };

        mutable std::unordered_map<std::string, std::shared_ptr<ROOT::Internal::TTreeReaderValueBase>> variables;
        mutable std::unordered_map<std::string, TTreeReaderValue<Bool_t>> triggers;

        std::vector<GenSimParticle> genSimParticles;

        //private helper functions to ease data access via TTreeReader
        template<typename T>
        T getArrayElement(std::string name, int index) const
        {
            auto it = variables.find(name);
            if (it == variables.end())
            {
                throw std::runtime_error("Variable " + name + " not found");
            }
            auto array = std::dynamic_pointer_cast<TTreeReaderArray<T>>(it->second);
            if (!array)
            {
                throw std::runtime_error("Variable " + name + " is not an array");
            }
            if (index >= static_cast<int>(array->GetSize()))
            {
                throw std::runtime_error("Index " + std::to_string(index) + " out of range for variable " + name);
            }
            return (*array)[index];
        }

        template<typename T>
        unsigned getArraySize(std::string name) const
        {
            auto it = variables.find(name);
            if (it == variables.end())
            {
                throw std::runtime_error("Variable " + name + " not found");
            }
            auto array = std::dynamic_pointer_cast<TTreeReaderArray<T>>(it->second);
            if (!array)
            {
                throw std::runtime_error("Variable " + name + " is not an array");
            }
            return static_cast<unsigned>(array->GetSize());
        }

        template<typename T>
        T getVariable(std::string name) const
        {
            auto it = variables.find(name);
            if (it == variables.end())
            {
                throw std::runtime_error("Variable " + name + " not found");
            }
            auto value = std::dynamic_pointer_cast<TTreeReaderValue<T>>(it->second);
            if (!value)
            {
                throw std::runtime_error("Variable " + name + " is not a value");
            }
            return **value;
        }
};


#endif
