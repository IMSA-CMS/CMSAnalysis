#include "CMSAnalysis/DataCollection/interface/EventLoaderInputModule.hh"
#include "CMSAnalysis/DataCollection/interface/InputModule.hh"
#include "CMSAnalysis/DataCollection/interface/ParticleCollection.hh"
#include "CMSAnalysis/DataCollection/interface/EventLoader.hh"
#include "CMSAnalysis/DataCollection/interface/Selector.hh"

InputModule::InputModule(const EventLoader *iEventLoader) : eventLoader(iEventLoader)
{
}