#ifndef SELECTOR_HH
#define SELECTOR_HH

class EventInput;
class Event;

class Selector
{
    public:
        virtual ~Selector() {}
        virtual void selectParticles(const EventInput* input, Event& event) const = 0;
};

#endif