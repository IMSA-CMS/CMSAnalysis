#ifndef EVENTLOADER_HH
#define EVENTLOADER_HH

class TFile;

class EventLoader
{
    public:
        EventLoader(int outputEvery) : numOfEvents(0), eventCount(0), outputEvery(outputEvery) {}
        void changeFile(TFile* jfile) {newFile(jfile);}
        virtual void nextEvent() const = 0;
        virtual bool isDone() const = 0;
        virtual ParticleCollection getLeptons() const = 0;
    protected:
        TFile* getFile() {return file;}
        virtual void newFile(TFile* ifile) const = 0;
    private:
        TFile* file;
        int numOfEvents;
        int outputEvery;
        int eventCount;
};




#endif