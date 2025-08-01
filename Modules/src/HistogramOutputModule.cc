#include "CMSAnalysis/Modules/interface/HistogramOutputModule.hh"
#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "CMSAnalysis/Modules/interface/Module.hh"
#include "CMSAnalysis/Utility/interface/HistogramPrototype.hh"
#include "TH1.h"
#include "TH2.h"
#include <stdexcept>
#include <tuple>

void HistogramOutputModule::addScaleFactor(std::shared_ptr<ScaleFactor> scaleFactor)
{
    scaleFactors.push_back(scaleFactor);
    for (const auto &hist : histograms)
    {
        hist->addScaleFactor(scaleFactor);
    }
}

void HistogramOutputModule::finalizeFilterString(TFile *outFile)
{
    for (auto &entry : baseObjects)
    {
        writeRootObj(outFile, std::get<0>(entry.second), std::get<1>(entry.second));
    }
}

void HistogramOutputModule::addHistogram(std::shared_ptr<HistogramPrototype> hist)
{
    hist->setInput(getInput());
    histograms.push_back(hist);
}

void HistogramOutputModule::setInput(const EventInput *iInput)
{
    if (!getInput())
    {
        Module::setInput(iInput);
        for (auto hist : histograms)
        {
            hist->setInput(iInput);
        }
    }
}

void HistogramOutputModule::addObject(std::string name, std::string path, TObject *obj)
{
    //std::cout << "adding object " << name << " " << obj->ClassName() << "\n";
    if (baseObjects.find(name) == baseObjects.end())
    {
        baseObjects.insert({name, std::tuple(path, obj)});
    }
    else
    {
        baseObjects.at(name) = std::tuple(path, obj);
    }

    //   std::cout << "Histogram added: " << name << '\n';
    //std::cout << "Histogram added: " << name << '\n';
}

TObject *HistogramOutputModule::getObject(const std::string &name)
{
    auto it = baseObjects.find(getObjectName(name));
    if (it != baseObjects.end())
    {
        return std::get<1>(baseObjects.at(getObjectName(name)));
    }
    else
    {
        return nullptr;
    }
}

const TObject *HistogramOutputModule::getObject(const std::string &name) const
{
    auto it = baseObjects.find(getObjectName(name));
    if (it != baseObjects.end())
    {
        return std::get<1>((baseObjects.find(getObjectName(name)))->second);
    }
    else
    {
        return nullptr;
    }
}

void HistogramOutputModule::makeHistogram(
    const std::string &name, const std::string &title, int nbins, double min, double max)
{
    throw std::runtime_error("Don't use this function!");

    // auto newHist = new TH1F(name.c_str(), title.c_str(), nbins, min, max);
    // addObject(name, newHist);
}

void HistogramOutputModule::makeHistogram(std::shared_ptr<HistogramPrototype> h, std::string name)
{
    //std::cout << "second makeHistogram: " << name  + " // " + getObjectName(name) << "\n";
    addObject(getObjectName(name), getFilter(), h->makeHistogram(getObjectName(name), getObjectName(name)));
    for (const auto &scaleFactor : h->getScaleFactors())
    {
        std::string newName = name + "_" + scaleFactor->getName() + "_Up";
        addObject(getObjectName(newName),
                  scaleFactor->getName() + "_Up/" + getFilter(),
                  h->makeHistogram(getObjectName(newName), getObjectName(newName)));
        std::string newName2 = name + "_" + scaleFactor->getName() + "_Down";
        addObject(getObjectName(newName2),
                  scaleFactor->getName() + "_Down/" + getFilter(),
                  h->makeHistogram(getObjectName(newName2), getObjectName(newName2)));
    }
}

void HistogramOutputModule::fillHistogram(const std::string &name, std::vector<double> values, double weight)
{
    auto hist = getHistogram(name);
    if (!hist)
        throw std::runtime_error("Argument to getHistogram was not of TH1 type!  Name: " + name + "&! \n");
    // " and Root type: " + getObject(name)->ClassName());

    if (auto hist2D = dynamic_cast<TH2 *>(hist)) // If the hist is 2D hist
    {
        if (values.size() % 2 == 1) // If the size of values is odd
        {
            throw std::runtime_error("Number of values in values vector is not even!");
        }

        for (int x = 0; x < static_cast<int>(values.size()); x += 2)
        {
            // Since x and y values are just inputed into the values vector as {x, y,
            // x, y, x, y, ...} The for loop loops through every other element in
            // values (i.e. every x value) The y value is then the element immediately
            // after the x element
            hist2D->Fill(values[x], values[x + 1], weight);
        }
    }

    else // If the hist is 1D hist
    {
        for (double currentNum : values)
        {
            //std::cout << "currentNum: " << currentNum << "\n";
            //std::cout << "Weight: " << weight << "\n";
            hist->Fill(currentNum, weight);
        }
    }
}

std::string HistogramOutputModule::getObjectName(const std::string &str) const
{
    std::string newName = getFilter() + str;
    //std::cout << "get object name: " << newName << "\n";
    return newName;
}

bool HistogramOutputModule::process()
{
    //std::cout << "HistOutMod running \n";
    for (auto hist : histograms)
    {
        bool draw = hist->shouldDraw(); // call the shouldDraw function so we can
                                        // call process on the FilterModules
        // 2/2/2023 investigating shouldDraw problem, this comment is just a placeholder

        if (draw)
        {
            // If the histogram without mass bin doesn't exist, make it
            if (baseObjects.find(getObjectName(hist->getFilteredName())) == baseObjects.end())
            {
                makeHistogram(hist, hist->getFilteredName());
            }

            // Fill the histogram if the filter string isn't empty
            /*
      if (hist->getName() !=
          hist->getFilteredName()) // If the filter string is empty, then the name
                                  // and the filtered name should be the same
                                  //|| hist->getFilteredName() != getObjectName(hist->getFilteredName())
      {
        makeHistogram(hist, hist->getFilteredName());
      }
      */

            // Fill the histogram if shouldDraw(event) (draw) returns true
            //if (draw) {
            // for (double value : hist->value())
            // {
            //   std::cout << hist->getFilteredName() << " has " << value << "\n";
            // }
            //std::cout << "Module particle size: " << getInput()->getParticles(EventInput::RecoLevel::Reco).getNumParticles() << "\n";
            //std::cout << "HistOutputModule event input: " << getInput() <<std::endl;
            fillHistogram(hist->getFilteredName(), hist->value(), hist->eventWeight());
            for (auto scaleFactor : hist->getScaleFactors())
            {
                fillHistogram(hist->getFilteredName() + "_" + scaleFactor->getName() + "_Up",
                              hist->value(),
                              hist->eventWeight(ScaleFactor::SystematicType::Up, scaleFactor));
                //std::cout << "UP eventWeight: " << hist->eventWeight(ScaleFactor::SystematicType::Up, scaleFactor) << "\n";
                fillHistogram(hist->getFilteredName() + "_" + scaleFactor->getName() + "_Down",
                              hist->value(),
                              hist->eventWeight(ScaleFactor::SystematicType::Down, scaleFactor));
                //std::cout << "DOWN eventWeight: " << hist->eventWeight(ScaleFactor::SystematicType::Down, scaleFactor) << "\n";
            }
        }
    }
    return true;
}

void HistogramOutputModule::finalize()
{
    //std::cout << "Starting histogram finalization \n";
    // std::cout << "There are " << histograms.size() << " histograms in this event\n";
    for (auto hist : histograms)
    {
        //std::cout << "Histogram filtered name = " << hist->getFilteredName() << "\n";
        auto Thist = getHistogram(hist->getFilteredName());

        // for (int i = 1; i < Thist->GetEntries(); ++i)
        // {
        //   std::cout << "Bin " << Thist->GetBinWidth(i) * i << "has " << Thist->GetBinContent(i) << " entries\n";
        // }

        if (Thist && Thist->GetEntries() == 0)
        {
            auto it = baseObjects.find(getObjectName(hist->getFilteredName()));
            if (it != baseObjects.end())
            {
                // std::cout << "Deleting " << hist->getFilteredName() << " histogram\n";
                baseObjects.erase(it);
                //should probably remove hist from histograms tho not sure how to do that - George 2/23/23
            }
        }
    }
}
