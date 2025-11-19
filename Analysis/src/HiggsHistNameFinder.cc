#include "CMSAnalysis/Analysis/interface/HiggsHistNameFinder.hh"
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <vector>

std::string HiggsHistNameFinder::getHistName(HistVariable histVariable) const
{
    const auto reco = "/Reco " + channel.substr(0, 4);
    std::string gensim;
    if (channel.size() > 4)
    {
        gensim = "/GenSim " + channel.substr(5, 4);
    }

    std::string syst;
    switch (histVariable.getSystematicType())
    {
    case ScaleFactor::SystematicType::Nominal:
        break;
    case ScaleFactor::SystematicType::Up:
        syst = histVariable.getSystematicName() + "_Up/";
        break;
    case ScaleFactor::SystematicType::Down:
        syst = histVariable.getSystematicName() + "_Down/";
        break;
    }

    std::string zSomething = zSelection ? "ZPeak" : "ZVeto";

    std::string path = "hists/" + syst + zSomething + reco + gensim + "/" + histVariable.getName();

    return path;

    // std::string path = channel + "__hists/" + channel;
    // if (isData)
    // {
    //     path += "_Pass";
    // }
    // path += "_" + histVariable.getName();
    // // Add systematic type

    // if (histVariable.getSystematicType() == ScaleFactor::SystematicType::Up)
    // {
    //     return path + "_" + histVariable.getSystematicName() + "_Up";
    // }
    // else if (histVariable.getSystematicType() == ScaleFactor::SystematicType::Down)
    // {
    //     return path + "_" + histVariable.getSystematicName() + "_Down";
    // }
    // else
    // {
    //     return path;
    // }
}