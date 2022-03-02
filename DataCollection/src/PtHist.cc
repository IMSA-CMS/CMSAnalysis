#include "CMSAnalysis/DataCollection/interface/PtHist.hh"

#include "CMSAnalysis/DataCollection/interface/GenSimRecoPrototype.hh"

std::vector<double> PtHist::protectedValue(InputModule::RecoLevel level) const
{
    auto genParticles = getInput()->getLeptons(level);
    auto genSimPt = genParticles.getLeadingTransverseMomentum();
    return {genSimPt};
}
