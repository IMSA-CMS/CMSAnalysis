#ifndef INPUTMODULE_HH
#define INPUTMODULE_HH

#include <memory>

#include "Module.hh"
class TFile;

class InputModule : public Module 
{
    public:
        InputModule(const std::shared_ptr<EventLoader> iEventLoader);
        // void setFile(TFile* tfile) {file = tfile;}
        virtual ParticleCollection getLeptons() const = 0;
    protected:
        // const edm::EventBase& getEvent();
        TFile* getFile(); //{return file;}
    private:
        const std::shared_ptr<EventLoader> eventLoader;
};



#endif