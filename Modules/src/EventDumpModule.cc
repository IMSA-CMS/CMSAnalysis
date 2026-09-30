#include "CMSAnalysis/Modules/interface/EventDumpModule.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Utility/interface/ParticleType.hh"
#include <algorithm>
#include <boost/algorithm/string/predicate.hpp>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>

EventDumpModule::EventDumpModule(bool iGenSim, bool iReco, int inumOfEvents, std::shared_ptr<MatchingModule> mod)
    : genSim(iGenSim), reco(iReco), numOfEvents(inumOfEvents), counter(0), matchingMod(std::move(mod))
{
    // clears out the text files of current data
    std::ofstream my_file;
    my_file.open("GenSimEventDump.txt", std::ofstream::out | std::ofstream::trunc);
    my_file.close();
    my_file.open("RecoEventDump.txt", std::ofstream::out | std::ofstream::trunc);
    my_file.close();
}

// update this to remove event parameter
bool EventDumpModule::process()
{
    if (counter >= numOfEvents && numOfEvents != -1)
    {
        return true;
    }

    // 3 underscores to prevent eee__LowMass from matching
    if (boost::contains(getFilter(), "ee___"))
    {
        const auto recoLeps = getInput()->getLeptons(EventInput::RecoLevel::Reco);

        const auto m = recoLeps.calculateSameSignInvariantMass();
        if (m < 150 || 600 < m)
        {
            return true;
        }

        auto matches = matchingMod->getMatchingBestPairs();
        std::cout << "----\n";
        for (const auto &pair : matches.getPairs())
        {
            std::cout << pair.getGenParticle().getCharge() << "," << pair.getRecoParticle().getCharge() << "\n";
            const auto gen = pair.getGenParticle();
            const auto reco = pair.getRecoParticle();
            if (gen.getType() == ParticleType::photon() && reco.getType().isLepton())
            {
                // photon fake
                photonFakeCounter += 1;
                continue;
            }
            if (gen.getCharge() != reco.getCharge())
            {
                // charge misid
                chargeMisIdCounter += 1;
                continue;
            }

            // if ((gen.getType().isLepton() || reco.getType().isLepton()) && (gen.getCharge() != reco.getCharge()))
            // {
            //     std::cout << "ignoring event\n";
            //     return true;
            // }
        }

        for (const auto &lep : getInput()->getLeptons(EventInput::RecoLevel::Reco))
        {
            bool fake = true;
            for (const auto& pair : matches.getPairs())
            {
                if (lep == pair.getRecoParticle())
                {
                    // Ok
                    fake = false;
                    break;
                }
            }
            if (fake)
            {
                unmatchedLepCounter += 1;
            }
        }

        ParticleCollection genSimLeps;
        for (const auto &lep : getInput()->getLeptons(EventInput::RecoLevel::GenSim))
        {
            if (GenSimParticle(lep).isFinalState())
            {
                genSimLeps.addParticle(lep);
            }
        }

        std::ofstream my_file;
        if (genSim)
        {
            auto genParticles = getInput()->getParticles(EventInput::RecoLevel::GenSim, ParticleType::none());
            my_file.open("GenSimEventDump.txt", std::ios::app);
            printGenSimParticleCollection(genParticles, my_file);
            // std::cout << "An event was printed\n";
            my_file.close();
        }
        if (reco)
        {
            auto recoParticles = getInput()->getLeptons(EventInput::RecoLevel::Reco);
            my_file.open("RecoEventDump.txt", std::ios::app);
            printRecoParticleCollection(recoParticles, my_file);
            // std::cout << "An event was printed\n";
            my_file.close();
        }

        counter++;
        return true;
    }

    return true;
}

void EventDumpModule::finalize()
{
    std::cout << "Photon fakes: " << photonFakeCounter << "\n";
    std::cout << "Charge misid: " << chargeMisIdCounter << "\n";
    std::cout << "Unmatched leptons: " << unmatchedLepCounter << "\n";
}

void EventDumpModule::printGenSimParticleCollection(const ParticleCollection<GenSimParticle> &genParts,
                                                    std::ofstream &my_file) const
{
    int eventElement = 1;
    int motherColumnWidth = 20;
    int daughterColumnWidth = 20;
    const auto &particleGroup = genParts.getParticles();
    my_file << "--------------------------------------------------------" << "\n";
    my_file << "EVENT #" << (counter + 1) << "\n";
    my_file << "--------------------------------------------------------" << "\n";

    my_file << std::left << std::setw(11) << "element" << std::setw(15) << "| name" << std::setw(11) << "| pdgId"
            << std::setw(10) << "| status" << std::setw(motherColumnWidth) << "| mothers"
            << std::setw(daughterColumnWidth) << "| daughters" << std::setw(15) << "| pT" << std::setw(15) << "| Eta"
            << std::setw(15) << "| Phi" << std::setw(15) << "| E" << std::setw(5) << "| mass\n";
    // Prints out all of the particles
    for (const auto &part : particleGroup)
    {
        std::string partName = part.getType().getName();
        my_file << std::setw(11) << eventElement << "| " << std::setw(13) << partName
                << std::pair<GenSimParticle, std::vector<GenSimParticle>>{part, particleGroup} << "\n";
        eventElement++;
    }
}

void EventDumpModule::printRecoParticleCollection(const ParticleCollection<Particle> &recoParts,
                                                  std::ofstream &my_file) const
{
    int eventElement = 1;
    const auto &particleGroup = recoParts.getParticles();
    my_file << "--------------------------------------------------------" << "\n";
    my_file << "EVENT #" << (counter + 1) << "\n";
    my_file << "all lepton invariant mass: " << recoParts.calculateAllLeptonInvariantMass()
            << " | same sign invariant mass: " << recoParts.calculateSameSignInvariantMass(true) << "\n";
    my_file << "--------------------------------------------------------" << "\n";

    my_file << std::left << std::setw(8) << "event" << std::setw(8) << "element" << std::setw(11) << "| name"
            << std::setw(11) << "| type" << std::setw(15) << "| charge" << std::setw(15) << "| pT" << std::setw(15)
            << "| Eta" << std::setw(15) << "| Phi" << std::setw(15) << "| E" << std::setw(5) << "| mass\n";

    // Prints out all of the particles
    for (const auto &part : particleGroup)
    {
        // std::cout<<"Part: " << part.getType().getName() << "\n";
        const auto &partName = part.getType().getName();
        my_file << std::setw(8) << (counter + 1) << std::setw(8) << eventElement << "| " << std::setw(9) << partName
                << part << "\n";
        eventElement++;
    }
}

int EventDumpModule::getLatestIndexOfDaughters(const GenSimParticle &part,
                                               const std::vector<GenSimParticle> &genParts) const
{
    int indexOf = -1;
    for (const auto &daughter : part.getDaughters())
    {
        auto pos = std::ranges::find(genParts, daughter);
        if (pos != genParts.end())
        {
            indexOf = std::max((int)(pos - genParts.begin()), indexOf);
        }
    }

    return indexOf;
}

std::string EventDumpModule::formatDaughterParticles(const GenSimParticle &part,
                                                     const std::vector<GenSimParticle> &genParts)
{
    std::vector<int> daughterIndexes(part.numberOfDaughters());

    // Store all indexes
    for (int i = 0; i < part.numberOfDaughters(); i++)
    {
        const auto pos = std::ranges::find(genParts, part.daughter(i));
        if (pos != genParts.end())
        {
            daughterIndexes.at(i) = (int)(pos - genParts.begin());
        }
        else
        {
            daughterIndexes.at(i) = -1;
        }
    }
    // Create formatted string based on indexes
    std::string daughters;
    for (int i = 0; i < part.numberOfDaughters(); i++)
    {
        const int start = i;

        while ((i < (part.numberOfDaughters() - 1)) && (daughterIndexes.at(i) + 1 == daughterIndexes.at(i + 1)))
        {
            i++;
        }

        int res1 = -1;
        auto pos1 = std::ranges::find(genParts, part.daughter(start));
        if (pos1 != genParts.end())
        {
            res1 = (int)(pos1 - genParts.begin());
        }

        daughters = daughters + (1 + res1);

        if (start != i)
        {
            int res2 = -1;
            auto pos2 = std::ranges::find(genParts, part.daughter(i));
            if (pos2 != genParts.end())
            {
                res2 = (int)(pos2 - genParts.begin());
            }

            daughters = daughters + "-" + (1 + res2) + " ";
        }
        else
        {
            daughters += daughters + " ";
        }
    }

    return daughters;
}

std::string EventDumpModule::formatMotherParticle(const GenSimParticle &part,
                                                  const std::vector<GenSimParticle> &genParts)
{
    auto pos = std::ranges::find(genParts, part.mother());
    if (pos == genParts.end())
    {
        return "";
    }
    size_t index = pos - genParts.begin() + 1;
    // std::cout<<"The index is: " << index << "\n";
    return std::to_string(index);
}

std::ostream &operator<<(std::ostream &str, const std::pair<GenSimParticle, std::vector<GenSimParticle>> genParticles)
{
    GenSimParticle part = genParticles.first;
    // std::cout<<"The particles name is: " << part.getType().getName() << "\n";
    std::vector<GenSimParticle> genParts = genParticles.second;

    str << "| " << std::setw(9) << part.pdgId() << "| " << std::setw(8) << part.status() << "| ";

    // print mothers

    // std::cout<<"The mother is: " << part.hasMother().getName() << "\n";

    if (part.hasMother())
    {
        // str << std::setw(18) << part.mother().getType().getName() << "| "; //this was original
        // str << std::setw(18) << EventDumpModule :: formatMotherParticle(part, genParts) << "| ";
        str << std::setw(8) << EventDumpModule ::formatMotherParticle(part, genParts);
        str << std::setw(10) << part.mother().getType().getName() << "| ";
    }
    else
    {
        str << std::setw(18) << "" << "| ";
    }

    // str << std::setw(motherColumnWidth - 2) << formatMotherParticles(part, genParts) << "| ";
    //  Print daughters
    // formatDaughterParticles(part, particleGroup) was replaced by function not working string
    str << std::setw(18) << EventDumpModule::formatDaughterParticles(part, genParts) << "| ";
    // Particle properties
    str << std::setw(13) << part.getPt() << "| " << std::setw(13) << part.getEta() << "| " << std::setw(13)
        << part.getPhi() << "| ";
    str << std::setw(13) << part.getEnergy() << "| " << std::setw(13) << part.getMass();
    return str;
}

std::ostream &operator<<(std::ostream &str, const GenSimParticle part)
{
    str << "| " << std::setw(9) << part.pdgId() << "| " << std::setw(8) << part.status() << "| ";
    // print mothers
    str << std::setw(18) << "" << "| ";
    // str << std::setw(motherColumnWidth - 2) << formatMotherParticles(part, genParts) << "| ";
    // Print daughters
    // formatDaughterParticles(part, particleGroup) was replaced by function not working string
    str << std::setw(18) << "" << "| ";
    // Particle properties
    str << std::setw(13) << part.getPt() << "| " << std::setw(13) << part.getEta() << "| " << std::setw(13)
        << part.getPhi() << "| ";
    str << std::setw(13) << part.getEnergy() << "| " << std::setw(13) << part.getMass();
    return str;
}
