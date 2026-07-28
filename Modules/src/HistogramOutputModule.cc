#include "CMSAnalysis/Modules/interface/HistogramOutputModule.hh"
#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "CMSAnalysis/Modules/interface/Module.hh"
#include "CMSAnalysis/Utility/interface/HistogramPrototype.hh"
#include "TH1.h"
#include "TH2.h"
#include <boost/algorithm/string/join.hpp>
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

void HistogramOutputModule::writeObjects(TFile *outFile)
{
    for (const auto &entry : objects)
    {
        //std::cout << "Writing object: " << std::get<1>(entry.first) << " to path: " << std::get<0>(entry.first) << "\n";
        writeRootObj(outFile, "hists/" + std::get<0>(entry.first), entry.second);
    }
}

void HistogramOutputModule::addHistogram(std::shared_ptr<HistogramPrototype> hist)
{
    for (const auto &scaleFactor : scaleFactors)
    {
        hist->addScaleFactor(scaleFactor);
    }
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

void HistogramOutputModule::addObject(std::string path, std::string name, TObject *obj)
{
    if (objects.find(std::tuple(path, name)) == objects.end())
    {
        objects.insert({std::tuple(path, name), obj});
    }
    else
    {
        objects.at(std::tuple(path, name)) = obj;
    }
}

TObject *HistogramOutputModule::getObject(const std::string &path, const std::string &name)
{
    auto it = objects.find(std::tuple(path, name));
    if (it != objects.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

const TObject *HistogramOutputModule::getObject(const std::string &path, const std::string &name) const
{
    auto it = objects.find(std::tuple(path, name));
    if (it != objects.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

void HistogramOutputModule::makeHistogram(std::shared_ptr<HistogramPrototype> h)
{
    auto name = h->getName();
    addObject(getFilterPath(), name, h->makeHistogram(name, name));
    //std::cout << "Making histogram: " << name << " in path: " << getFilterPath() << "\n";
    for (const auto &scaleFactor : h->getScaleFactors())
    {
        addObject(scaleFactor->getName() + "_Up/" + getFilterPath(), name, h->makeHistogram(name, name));
        addObject(scaleFactor->getName() + "_Down/" + getFilterPath(), name, h->makeHistogram(name, name));
    }
}

void HistogramOutputModule::fillHistogram(const std::string &path, const std::string &name, std::vector<double> values,
                                          double weight)
{
    auto hist = getHistogram(path, name);
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
            // std::cout << "currentNum: " << currentNum << "\n";
            // std::cout << "Weight: " << weight << "\n";
            hist->Fill(currentNum, weight);
        }
    }
}

double HistogramOutputModule::eventWeightL(ScaleFactor::SystematicType type, std::shared_ptr<ScaleFactor> scaleFactorToChange) const
{
  //std::cout << "Weight event input: " << getInput() <<std::endl;
  double weight = 1.0;
  for (auto scaleFactor : scaleFactors)
  {
    if (scaleFactor == scaleFactorToChange)
    {
      weight *= scaleFactor->getScaleFactor(getInput(), type);
      //std::cout << "Up: " << scaleFactor->getScaleFactor(getInput(), ScaleFactor::SystematicType::Up) << std::endl;
      //std::cout << "Down: " << scaleFactor->getScaleFactor(getInput(), ScaleFactor::SystematicType::Down) << std::endl;

    }
    else
    {
      weight *= scaleFactor->getScaleFactor(getInput());
      //std::cout << "Scale Factor: " << scaleFactor->getScaleFactor(getInput()) << std::endl;
    }
    //std::cout << "weight: " << weight << std::endl;
  }
  return weight;
  
}

bool HistogramOutputModule::process()
{

    //eventWeight need to be roemved and replaced with the code from  histogramprototype
    // get all scale factors here and then use them later instead of eventweight
    // event module stores scale factors, we need to store them here
    // use map for up and down weights, call weights here, use just numbers later


    // std::cout << "HistOutMod running \n";
    double weight = eventWeightL();
    std::unordered_map<std::shared_ptr<ScaleFactor>, std::pair<double, double>> systematicWeights;

    for (auto scaleFactor : scaleFactors)
    {
        systematicWeights[scaleFactor] = std::make_pair(eventWeightL(ScaleFactor::SystematicType::Up, scaleFactor), eventWeightL(ScaleFactor::SystematicType::Down, scaleFactor));
    }

    


    for (const auto &hist : histograms)
    {
        bool draw = hist->shouldDraw(); // call the shouldDraw function so we can
                                        // call process on the FilterModules
        // 2/2/2023 investigating shouldDraw problem, this comment is just a placeholder

        if (!draw)
        {
            continue;
        }



        // Fill the histogram if shouldDraw(event) (draw) returns true
        // if (draw) {
        // for (double value : hist->value())
        // {
        //   std::cout << hist->getFilteredName() << " has " << value << "\n";
        // }
        // std::cout << "Module particle size: " <<
        // getInput()->getParticles(EventInput::RecoLevel::Reco).getNumParticles() << "\n"; std::cout <<
        // "HistOutputModule event input: " << getInput() <<std::endl;
        auto value = hist->value();
                // If the histogram doesn't exist, make it

        if(value.empty()){
            continue;
        }

        if (getObject(getFilterPath(), hist->getName()) == nullptr)
        {
            makeHistogram(hist);
        }
        fillHistogram(getFilterPath(), hist->getName(), value, weight);
        for (const auto &scaleFactor : hist->getScaleFactors())
        {
            fillHistogram(scaleFactor->getName() + "_Up/" + getFilterPath(), hist->getName(), value,
                          systematicWeights[scaleFactor].first); //*
            // std::cout << "UP eventWeight: " << hist->eventWeightL(ScaleFactor::SystematicType::Up, scaleFactor) <<
            // "\n";
            fillHistogram(scaleFactor->getName() + "_Down/" + getFilterPath(), hist->getName(), value,
                          systematicWeights[scaleFactor].second); //*
            // std::cout << "DOWN eventWeight: " << hist->eventWeight(ScaleFactor::SystematicType::Down,
            // scaleFactor) << "\n";
        }
    }
    return true;
}

void HistogramOutputModule::finalize()
{
    // std::cout << "Starting histogram finalization \n";
    // std::cout << "There are " << histograms.size() << " histograms in this event\n";
    for (const auto &hist : histograms)
    {
        auto tHist = getHistogram(getFilterPath(), hist->getName());
        if (tHist != nullptr && tHist->GetEntries() == 0)
        {
            objects.erase(objects.find(std::tuple(getFilterPath(), hist->getName())));
        }
    }
}

std::string HistogramOutputModule::getFilterPath()
{
    return boost::algorithm::join(getFilters(), "/");
}

  TH1* HistogramOutputModule::getHistogram(const std::string &path, const std::string &name)
  {

        return dynamic_cast<TH1 *>(getObject(path, name));
  }
