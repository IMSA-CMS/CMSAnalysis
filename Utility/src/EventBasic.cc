#include "CMSAnalysis/Utility/interface/EventBasic.hh"

// Constructor
EventBasic::EventBasic(MassType mass, SignType sign)
    : massType(mass), signType(sign) {}

EventBasic::EventBasic()
    : massType(LOW), signType(SAME) {}

void EventBasic::setSignatureType(MassType mass, SignType sign) {
    massType = mass;
    signType = sign;
}

// Method to get the mass type as a string
std::string EventBasic::getMassType() const {
    return (massType == LOW) ? "Low Mass" : "High Mass";
}

// Method to get the sign type as a string
std::string EventBasic::getSignType() const {
    return (signType == SAME) ? "Same Sign" : "Opposite Sign";
}

std::string EventBasic::getSignatureType() const {
    return "Particle Type: " + getMassType() + ", " + getSignType();
}

// Method to print the particle type
void EventBasic::printSignatureType() const {
    std::cout << "Particle Type: " << getMassType() << ", " << getSignType() << std::endl;
}

// Method to compare two EventBasic objects
bool EventBasic::isEqual(const EventBasic& other) const {
    return (massType == other.massType) && (signType == other.signType);
}