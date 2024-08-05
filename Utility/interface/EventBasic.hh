#ifndef EVENTBASIC_HH
#define EVENTBASIC_HH

#include <iostream>
#include <string>

class EventBasic {
public:
    // Enum for mass type
    enum MassType {
        LOW,
        HIGH
    };

    // Enum for sign type
    enum SignType {
        SAME,
        OPPOSITE
    };

    // Constructor
    EventBasic(MassType mass, SignType sign);
    EventBasic();

    void setSignatureType(MassType mass, SignType sign);

    // Method to get the mass type as a string
    std::string getMassType() const;

    // Method to get the sign type as a string
    std::string getSignType() const;

    std::string getSignatureType() const;

    // Method to print the particle type
    void printSignatureType() const;

    // Method to compare two EventBasic objects
    bool isEqual(const EventBasic& other) const;

private:
    MassType massType;
    SignType signType;
};

#endif // EVENTBASIC_HH
