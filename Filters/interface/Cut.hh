#ifndef CUT_HH
#define CUT_HH
#include <vector>

class Particle;
class Lepton;

// Interface for Cut objects, which apply cuts in EventModule. Contains counters which help it to determine
// its overall dependent and independent efficiencies (dependent = in sequence with other cuts, indep = as if only this single cut was applied)
// Can be added to an EventModule as a shared_ptr with eventModule.addCut()
class Cut
{
    public:
        virtual ~Cut() {}

        int getPassCount() {return passCount;}
        int getCheckCount() {return checkCount;}
        
        double getDependentEfficiency() {return double(passCount)/checkCount;}
        double getIndependentEfficiency() {return double(independentPassCount + passCount) / (independentCheckCount + checkCount);}

        // Check whether event passes the cut. Do not override, instead override checkEventInternal().
        // Simply calls checkEventInternal(), increments passCount if the event passes, and increments checkCount if true
        bool checkEvent(const Event& event, bool dependent = true);
            
    protected: 
        // The function to be overridden by children. Called immediately by the public checkEvent.
        virtual bool checkEventInternal(const Event& event) const = 0;
    private:
        int passCount = 0;
        int checkCount = 0;

        // events which didn't pass a different cut than this object in EventModule, but would have passed this cut. 
        int independentPassCount = 0;
        // independent number of checks (indepCheckCount + checkCount = total number of times checkEvent() has been called)
        int independentCheckCount = 0;
};

#endif