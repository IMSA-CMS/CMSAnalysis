#ifndef GENSIMLEPTON_HH
#define GENSIMLEPTON_HH
#include <vector>
#include "DataFormats/Candidate/interface/Candidate.h"

class LeptonJet;


class GenSimLepton : public GenSimParticle, public Lepton
{
    public:
        using GenSimParticle::GenSimParticle;
        using Lepton::Lepton;

};
#endif