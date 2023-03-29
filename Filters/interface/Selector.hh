#ifndef SELECTOR_HH
#define SELECTOR_HH

class InputModule;
class Event;

class Selector
{
    public:
        virtual ~Selector() {}
        virtual void selectParticles(const InputModule* input, Event& event) = 0;
};

#endif