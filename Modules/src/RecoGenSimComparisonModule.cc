
#include "CMSAnalysis/Modules/interface/RecoGenSimComparisonModule.hh"
RecoGenSimComparisonModule::RecoGenSimComparisonModule(std::string compType, bool writeOutput):
comparisonType(compType),
eventOutput(writeOutput)
{
}
bool RecoGenSimComparisonModule::process()
{
    auto recoParticles = getInput()->getParticles(EventInput::RecoLevel::Reco, ParticleType::none());
    auto genParticles = getInput()->getParticles(EventInput::RecoLevel::GenSim, ParticleType::none());
    
    printMatchInfo(recoParticles, genParticles);
    return true;
}

void RecoGenSimComparisonModule::finalize()
{
    std::cout << comparisonType << "\n\n";
    if (comparisonType == "SameSignInvariantMass")
    {
        std::cout << "\nPer reconstructed event counters:\n";
        std::cout << "Total events: " << eventCounter << "\n";
        std::cout << "Total electron events: " << elecEventCounter << "\n";
        std::cout << "Total muon events: " << muonEventCounter << "\n";
        std::cout << "Accurate event ratio: " << accurateEventCounter/(double)eventCounter << "\n";
        std::cout << "\nEssential to huge invariant mass:\n\n";
        std::cout << "Wrong charge electron events ratio: " << elecWrongChargeCounter/(double)eventCounter << "\n";
        std::cout << "Mismeasured pT electron events ratio: " << elecMismeasuredPtCounter/(double)eventCounter << "\n";
        std::cout << "Fake photon electron events ratio: " << elecFakePhotonCounter/(double)eventCounter << "\n";
        std::cout << "No match electron events ratio: " << elecNoMatchCounter/(double)eventCounter << "\n\n";
        std::cout << "Accurate electron event ratio: " << elecAccurateCounter/(double)eventCounter << "\n";
        std::cout << "\nWrong charge muon events ratio: " << muonWrongChargeCounter/(double)eventCounter << "\n";
        std::cout << "Mismeasured pT muon events ratio: " << muonMismeasuredPtCounter/(double)eventCounter << "\n";
        std::cout << "Fake photon muon events ratio: " << muonFakePhotonCounter/(double)eventCounter << "\n";
        std::cout << "No match muon events ratio: " << muonNoMatchCounter/(double)eventCounter << "\n";
        std::cout << "Accurate event ratio: " << muonAccurateCounter/(double)eventCounter << "\n";
    }
    if (comparisonType == "perParticle")
    {
        std::cout << "\nPer reconstructed event counters:\n";
        std::cout << "Total events: " << eventCounter << "\n";
        std::cout << "Total electrons: " << electronCounter << "\n";
        std::cout << "Total muons: " << muonCounter << "\n";
        std::cout << "\nEssential to huge invariant mass:\n\n";
        std::cout << "Wrong charge electrons ratio: " << elecWrongChargeCounter/(double)electronCounter << "\n";
        std::cout << "Mismeasured pT electrons ratio: " << elecMismeasuredPtCounter/(double)electronCounter << "\n";
        std::cout << "Fake photon electrons ratio: " << elecFakePhotonCounter/(double)electronCounter << "\n";
        std::cout << "No match electrons ratio: " << elecNoMatchCounter/(double)electronCounter << "\n\n";
        std::cout << "Accurate electron ratio: " << elecAccurateCounter/(double)electronCounter << "\n";
        std::cout << "\nWrong charge muons ratio: " << muonWrongChargeCounter/(double)muonCounter << "\n";
        std::cout << "Mismeasured pT muons ratio: " << muonMismeasuredPtCounter/(double)muonCounter << "\n";
        std::cout << "Fake photon muons ratio: " << muonFakePhotonCounter/(double)muonCounter << "\n";
        std::cout << "No match muons ratio: " << muonNoMatchCounter/(double)muonCounter << "\n";
        std::cout << "Accurate muon ratio: " << muonAccurateCounter/(double)muonCounter << "\n";
    }
    if (comparisonType == "fakePhoton")
    {
        std::cout << "\nPer reconstructed event counters:\n";
        std::cout << "Total events: " << eventCounter << "\n";

        std::cout << "Electron pT, less than 50: " << electronCounter_lt50 << "\n"; 
        std::cout << "Electron pT, 50-100: " << electronCounter_50to100 << "\n"; 
        std::cout << "Electron pT, 100-200: " << electronCounter_100to200 << "\n"; 
        std::cout << "Electron pT, 200-300: " << electronCounter_200to300 << "\n";
        std::cout << "Electron pT, more than 300: " << electronCounter_gt300 << "\n"; 

        std::cout << "Fake Photon pT, less than 50: " << fakePhotonCounter_lt50 << "\n";
        std::cout << "Fake Photon pT, 50-100: " << fakePhotonCounter_50to100 << "\n"; 
        std::cout << "Fake Photon pT, 100-200: " << fakePhotonCounter_100to200 << "\n"; 
        std::cout << "Fake Photon pT, 200-300: "<< fakePhotonCounter_200to300 << "\n";
        std::cout << "Fake Photon pT, more than 300: " << fakePhotonCounter_gt300 << "\n";

    }
    if (comparisonType == "mother") 
    {
        std::cout << "Total events: " << eventCounter << "\n";
        std::cout << "\nTotal 4-lepton events: " << numOfDesiredEvents << "\n";
        std::cout << "Total electron events: " << electronCounter << "\n";
        std::cout << "Total muon events: " << muonCounter << "\n";

        
        std::cout << "\nRatios are out of total 4-lepton events\n";
        std::cout << "Events with ISR " << isrCounter/(double)numOfDesiredEvents << "\n";
        std::cout << "Events with FSR " << fsrCounter/(double)numOfDesiredEvents << "\n";
        std::cout << "No match events: " << noMatchCounter/(double)numOfDesiredEvents << "\n\n";
        std::cout << "\nElectron events with ISR " << elecIsrCounter/(double)numOfDesiredEvents << "\n";
        std::cout << "Electron events with FSR " << elecFsrCounter/(double)numOfDesiredEvents << "\n";
        std::cout << "No match electron events: " << elecNoMatchCounter/(double)numOfDesiredEvents << "\n\n";
        std::cout << "\nMuon events with ISR " << muonIsrCounter/(double)numOfDesiredEvents << "\n";
        std::cout << "Muon events with FSR " << muonFsrCounter/(double)numOfDesiredEvents << "\n";
        std::cout << "No match muon events: " << muonNoMatchCounter/(double)numOfDesiredEvents << "\n\n";
        std::cout << "Fraction with Mu24 " << muon24Count/(double)eventCounter << "\n\n";
    }
}
void RecoGenSimComparisonModule::printMatchInfo(const ParticleCollection<Particle>& recoParts, 
        const ParticleCollection<Particle>& genParts, 
        std::ostream& output)
{
    eventCounter++;
    
    if (comparisonType == "sameSignInvariantMass" && recoParts.calculateSameSignInvariantMass(true) > 500)
    {
        printEventHeader(output);
        sameSignDeltaRComparison(recoParts, genParts, output);
    }
    if (comparisonType == "perParticle")
    {
        printEventHeader(output);
        perParticleComparison(recoParts, genParts, output);
    }
    if (comparisonType == "fakePhoton")
    {
        printEventHeader(output);
        fakePhotonComparison(recoParts, genParts, output);
    }
    if (comparisonType == "mother")
    {
        mothersComparison(recoParts, genParts, output);
    }
}
void RecoGenSimComparisonModule::printEventHeader(std::ostream& output) {
    if (eventOutput)
    {
        output << "■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■" << "\n";
        //event info
        output << "EVENT #" << eventCounter << "\n";
        
        output << "■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■" << "\n";
        //column labels
        output << std::left << std::setw(8) << "element" << std::setw(11) << "| type"
            << std::setw(15) << "| charge"
            << std::setw(15) << "| pT"
            << std::setw(15) << "| Eta"
            << std::setw(15) << "| Phi"
            << std::setw(10) << "| E"
            << std::setw(10) << "| mass"
            << std::setw(10) << "| mother"
            << std::setw(10) << "| grandma" << std::endl;
    }
}
void RecoGenSimComparisonModule::printRecoPart(const Particle& recoPart, const int recoEventElement, std::ostream& output) {
    if (eventOutput) {
        output << "++++++++++++++++ Reco Element ++++++++++++++++\n";
        output << std::setw(8) << recoEventElement << "| " << std::setw(9) << recoPart.getName() << "| ";
        // // Particle properties
        output << std::setw(13) << recoPart.getCharge() << "| " 
            << std::setw(13) << recoPart.getPt() << "| " 
            << std::setw(13) << recoPart.getEta() << "| " 
            << std::setw(13) << recoPart.getPhi() << "| ";
    
        try
        {
            output << std::setw(13) << recoPart.getEnergy() << "| " 
            << std::setw(13) << recoPart.getMass() << "\n";
        }
        catch(const std::exception& e)
        {
            output << std::setw(13) << "N/A" << "| " 
            << std::setw(13) << recoPart.getMass() << std::endl;
        }
        output << std::left << "-------------- GenSim Matches --------------\n";
    }
}
void RecoGenSimComparisonModule::perParticleComparison(const ParticleCollection<Particle>& recoParts, 
        const ParticleCollection<Particle>& genParts, 
        std::ostream& output)
{
    int recoEventElement = 1;
    int genEventElement = 1;
    bool wrongCharge = false;
    bool fakePhoton = false;
    bool mismeasuredPt = false;
    bool accurate = false;
    bool noMatch = true;
    auto recoType = ParticleType::photon();
    if (getFilter() != "") 
    {
        output << getFilter() << "\n\n";
    }
    for(auto &recoPart : recoParts)
    {
        // Standard info
        printRecoPart(recoPart, recoEventElement, output);
        recoType = recoPart.getType();
        recoEventElement++;
        genEventElement = 1;
        //loop through all gensim particles
        for(auto &genPart : genParts)
        {
            GenSimParticle genSimPart(genPart);
            if (!genSimPart.isFinalState())
            {
                continue;
            }
            //match using deltaR (distance in phi-eta plane)
            double deltaR = recoPart.getDeltaR(genPart);
            //double deltaR = std::sqrt( std::pow(recoPart.getPhi() - genPart.getPhi(), 2) + std::pow(recoPart.getEta() - genPart.getEta(), 2) );
            if (deltaR < 0.1 && getFilter() != "")
            {
                if (eventOutput)
                {
                    
                    output << std::setw(8) << genEventElement << "| " << std::setw(9) << genPart.getName() << "| ";
                    // Particle properties
                    output << std::setw(13) << genPart.getCharge() << "| " 
                        << std::setw(13) << genPart.getPt() << "| " 
                        << std::setw(13) << genPart.getEta() << "| " 
                        << std::setw(13) << genPart.getPhi() << "| ";
                    try{
                    output << std::setw(10) << genPart.getEnergy() << "| " 
                        << std::setw(10) << genPart.getMass() << std::endl;
                    }
                    catch(const std::exception& e){
                    output << std::setw(10) << "N/A" << "| " 
                        << std::setw(10) << genPart.getMass() << std::endl;
                    }
                }
                // matchCounter++;
                noMatch = false;
                
                if (recoPart.getType()==genPart.getType())
                {
                    fakePhoton = false;
                    if((recoPart.getPt() - genPart.getPt())/genPart.getPt() < 0.1 && genPart.getCharge() == recoPart.getCharge())
                    {
                        // accurateMatchCounter++;
                        accurate = true;
                        //break;
                    }
                    
                    if(!((recoPart.getPt() - genPart.getPt())/genPart.getPt() < 0.1))
                    {
                        mismeasuredPt = true;
                    }
                    if (genPart.getCharge() == - recoPart.getCharge())
                    {
                        wrongCharge = true;
                    }
                }
                if (genPart.getType() == ParticleType::photon() && !(mismeasuredPt || wrongCharge))
                {
                    fakePhoton = true;
                }
            }
            genEventElement++;
        }
        if (recoType==ParticleType::electron()) 
        {
            electronCounter++;
            elecWrongChargeCounter += wrongCharge;
            elecFakePhotonCounter += fakePhoton;
            elecNoMatchCounter += noMatch;
            elecMismeasuredPtCounter += mismeasuredPt;
            elecAccurateCounter += accurate;
        }
        else if (recoType==ParticleType::muon()) 
        {
            muonCounter++;
            muonWrongChargeCounter += wrongCharge;
            muonFakePhotonCounter += fakePhoton;
            muonNoMatchCounter += noMatch;
            muonMismeasuredPtCounter += mismeasuredPt;
            muonAccurateCounter += accurate;
        }
    }
}
void RecoGenSimComparisonModule::sameSignDeltaRComparison(const ParticleCollection<Particle>& recoParts,  const ParticleCollection<Particle>& genParts, std::ostream& output) 
{
    int recoEventElement = 1;
    int genEventElement = 1;
    bool wrongCharge = false;
    bool fakePhoton = false;
    bool mismeasuredPt = false;
    bool accurate = false;
    bool noMatch = true;
    bool accurateEvent = true;
    ParticleType recoPartType = ParticleType::photon(); // eefault particle type set to photon
    ParticleType wrongChargeType = ParticleType::photon(); // to store type for particles with wrong charge
    ParticleType fakePhotonType = ParticleType::photon(); // to store type for fake photon detection
    ParticleType mismeasuredPtType = ParticleType::photon(); 
    ParticleType noMatchType = ParticleType::photon(); 
    ParticleCollection<Particle> notWrongCharge;
    ParticleCollection<Particle> notFakePhoton;
    ParticleCollection<Particle> notMismeasuredPt;
    ParticleCollection<Particle> notNoMatch;
    
    // loop through all reconstructed particles (recoParts)
    for(auto &recoPart : recoParts) {
        // Output reconstructed particle info
        output << "++++++++++++++++ Reco Element ++++++++++++++++\n";
        output << std::setw(8) << recoEventElement << "| " << std::setw(9) << recoPart.getName() << "| ";
        output << std::setw(13) << recoPart.getCharge() << "| " 
               << std::setw(13) << recoPart.getPt() << "| " 
               << std::setw(13) << recoPart.getEta() << "| " 
               << std::setw(13) << recoPart.getPhi() << "| ";
        try {
            output << std::setw(13) << recoPart.getEnergy() << "| " 
                   << std::setw(13) << recoPart.getMass() << "\n";
        }
        catch(const std::exception& e) {
            output << std::setw(13) << "N/A" << "| " 
                   << std::setw(13) << recoPart.getMass() << std::endl;
        }
        recoEventElement++; // increment reco event element number
        wrongCharge = false; // reset flags for each reco particle
        fakePhoton = false;
        mismeasuredPt = false;
        accurate = false;
        noMatch = true;

        output << std::left << "-------------- GenSim Matches --------------\n";
        // Loop through all generated particles (genParts)
        for(auto &genPart : genParts) {
            // Calculate deltaR (distance in phi-eta plane) between reco and gen particle
            //double deltaR = std::sqrt( std::pow(recoPart.getPhi() - genPart.getPhi(), 2) + std::pow(recoPart.getEta() - genPart.getEta(), 2) );
            double deltaR = recoPart.getDeltaR(genPart);
            if (deltaR < 0.1) { // Match if deltaR is smaller than threshold
                // Output gen particle info if matched with reco particle
                output << std::setw(8) << genEventElement << "| " << std::setw(9) << genPart.getName() << "| ";
                output << std::setw(13) << genPart.getCharge() << "| " 
                       << std::setw(13) << genPart.getPt() << "| " 
                       << std::setw(13) << genPart.getEta() << "| " 
                       << std::setw(13) << genPart.getPhi() << "| ";
                try {
                    output << std::setw(10) << genPart.getEnergy() << "| " 
                           << std::setw(10) << genPart.getMass() << std::endl;
                }
                catch(const std::exception& e) {
                    output << std::setw(10) << "N/A" << "| " 
                           << std::setw(10) << genPart.getMass() << std::endl;
                }
                noMatch = false; // Match found

                // Check if the reco and gen particles match in type and further conditions
                if (recoPart.getType() == genPart.getType()) {
                    fakePhoton = false; // Reset fake photon flag
                    // Check if the pt difference is within a 10% threshold and charge is the same
                    if (std::abs(recoPart.getPt() - genPart.getPt()) / genPart.getPt() < 0.1 && genPart.getCharge() == recoPart.getCharge()) {
                        accurate = true; // Mark as accurate if pt and charge match
                        break; // Exit loop since we found an accurate match
                    }

                    // Mark mismeasured Pt if pt difference is outside threshold
                    if (!(std::abs(recoPart.getPt() - genPart.getPt()) / genPart.getPt() < 0.1)) {
                        mismeasuredPt = true;
                    }
                    // Mark wrong charge if charges do not match
                    if (genPart.getCharge() != recoPart.getCharge()) {
                        wrongCharge = true;
                    }
                }
                
                // handle fake photon detection: if a photon doesn't match properly, it is considered fake
                if (genPart.getType() == ParticleType::photon() && !(mismeasuredPt || wrongCharge)) {
                    fakePhoton = true;
                }
            }
            genEventElement++; // Increment gen event element number
        }

        // store reco particle in corresponding collections based on classification
        if (accurate) {
            notWrongCharge.addParticle(recoPart);
            notFakePhoton.addParticle(recoPart);
            notMismeasuredPt.addParticle(recoPart);
            notNoMatch.addParticle(recoPart);
        }
        if (!wrongCharge) {
            notWrongCharge.addParticle(recoPart); 
        } else {
            if (wrongChargeType == ParticleType::photon()) {
                wrongChargeType = recoPart.getType(); 
            }
        }
        if (!mismeasuredPt) {
            notMismeasuredPt.addParticle(recoPart); // add to collection if no mismeasured Pt
        } else {
            if (mismeasuredPtType == ParticleType::photon()) {
                mismeasuredPtType = recoPart.getType(); // store particle type for mismeasured Pt
            }
        }
        if (!fakePhoton) {
            notFakePhoton.addParticle(recoPart); // add to collection if not fake photon
        } else {
            if (fakePhotonType == ParticleType::photon()) {
                fakePhotonType = recoPart.getType(); // store particle type for fake photon
            }
        }
        if (!noMatch) {
            notNoMatch.addParticle(recoPart); 
        } else {
            if (noMatchType == ParticleType::photon()) {
                noMatchType = recoPart.getType(); 
            }
        }
    }

    // Event classification and counting based on deltaR-based comparisons
    if (notWrongCharge.calculateSameSignInvariantMass(true) < 0.1) {
        wrongChargeCounter++; // Increment wrong charge counter if deltaR condition is met
        recoPartType = wrongChargeType; // Set reco particle type to wrong charge type
        output << "wrong charge essential " << notWrongCharge.calculateSameSignInvariantMass(true) << std::endl;
    }
    if (notFakePhoton.calculateSameSignInvariantMass(true) < 0.1) {
        fakePhotonCounter++; // Increment fake photon counter if deltaR condition is met
        recoPartType = fakePhotonType; // Set reco particle type to fake photon type
        output << "| fake photon essential " << notFakePhoton.calculateSameSignInvariantMass(true) << std::endl;
    }
    if (notMismeasuredPt.calculateSameSignInvariantMass(true) < 0.1) {
        mismeasuredPtCounter++; // increment mismeasured Pt counter if deltaR condition is met
        recoPartType = mismeasuredPtType; // set reco particle type to mismeasured Pt type
        output << "| mismeasured pt essential " << notMismeasuredPt.calculateSameSignInvariantMass(true) << std::endl;
    }
    if (notNoMatch.calculateSameSignInvariantMass(true) < 0.1) {
        noMatchCounter++; 
        recoPartType = noMatchType; 
        output << "| no match essential " << notNoMatch.calculateSameSignInvariantMass(true) << std::endl;
    }

    // accurately matched events classification and counting
    if (accurateEvent) {
        accurateEventCounter++; // increment accurate event counter
        recoPartType = notNoMatch.getParticles()[0].getType(); // set reco particle type to first particle's type in no match collection
        if (notNoMatch.getParticles()[0].getType() == ParticleType::electron()) {
            elecAccurateCounter++; // increment electron accurate counter
            output << "| accurate event electron " << notNoMatch.calculateSameSignInvariantMass(true) << std::endl;
        }
        if (notNoMatch.getParticles()[0].getType() == ParticleType::muon()) {
            muonAccurateCounter++; // increment muon accurate counter
            output << "| accurate event muon " << notNoMatch.calculateSameSignInvariantMass(true) << std::endl;
        }
    }

    // Event counters by particle type (electron or muon)
    if (recoPartType == ParticleType::electron()) {
        elecEventCounter++; // increment electron event counter
    }
    if (recoPartType == ParticleType::muon()) {
        muonEventCounter++; // increment muon event counter
    }
}


     void RecoGenSimComparisonModule::fakePhotonComparison(const ParticleCollection<Particle>& recoParts, const ParticleCollection<Particle>& genParts, std::ostream& output)
{
    int recoEventElement = 1;

    ParticleType recoType;
    int wrongCharge;
    int fakePhoton;
    int noMatch;
    int mismeasuredPt;
    int accurate;
    

    for (auto& recoPart : recoParts)
    {
        if (eventOutput)
        {
            output << "++++++++++++++++ Reco Element ++++++++++++++++\n";
            output << std::setw(8) << recoEventElement << "| " << std::setw(9) << recoPart.getName() << "| ";
            output << std::setw(13) << recoPart.getCharge() << "| "
                    <<std::setw(13) << recoPart.getPt() << "| "
                    <<std::setw(13) << recoPart.getEta() << "| "
                   << std::setw(13) << recoPart.getPhi() << "| ";

            try
            {
                output << std::setw(13) << recoPart.getEnergy() << "| "
                       << std::setw(13) << recoPart.getMass() << "\n";
            }
            catch (const std::exception& e)
            {
                output << std::setw(13) << "N/A" << "| "
                       << std::setw(13) << recoPart.getMass() << std::endl;
            }
            output << std::left << "-------------- GenSim Matches --------------\n";
        }

        recoEventElement++; // increment event counter
        eventCounter++;     // increment total event counter

        bool matchesElectron = false; // fake photon matches electron
        bool matchesPhoton = false;   // real photon match

        if (recoPart.getType() == ParticleType::photon()) 
        {
            for (auto& genPart : genParts)
            {
                if (recoPart.getDeltaR(genPart) <= 0.1)
                {
                    if (genPart.getType() == ParticleType::electron())
                    {
                        matchesElectron = true;
                    }
                    else if (genPart.getType() == ParticleType::photon())
                    {
                        matchesPhoton = true;
                    }
                }
            }
            if (recoType==ParticleType::electron()) 
            {
                electronCounter++;
                elecWrongChargeCounter += wrongCharge;
                elecFakePhotonCounter += fakePhoton;
                elecNoMatchCounter += noMatch;
                elecMismeasuredPtCounter += mismeasuredPt;
                elecAccurateCounter += accurate;
            } 
            if(noMatch){
                noMatchCounter++;
            }
             
        }
        // determines if the photon is fake
        bool isFakePhoton = matchesElectron && !matchesPhoton;

        // determines the pT range for the reconstructed particle
        double pT = recoPart.getPt();

        if (recoPart.getType() == ParticleType::electron())
        {
            if (pT < 50) {
                electronCounter_lt50++;
            } else if (pT < 100) {
                electronCounter_50to100++;
            } else if (pT < 200) {
                electronCounter_100to200++;
            } else if (pT < 300) {
                electronCounter_200to300++;
            } else {
                electronCounter_gt300++;
            }
        }


        if (isFakePhoton) {
            if (pT < 50) {
                fakePhotonCounter_lt50++;
            } else if (pT < 100) {
               fakePhotonCounter_50to100++;
            } else if (pT < 200) {
                fakePhotonCounter_100to200++;
            } else if (pT < 300) {
                fakePhotonCounter_200to300++;
            } else {
                fakePhotonCounter_gt300++;
            }
    }

    
}




}
void RecoGenSimComparisonModule::mothersComparison(const ParticleCollection<Particle>& recoParts, const ParticleCollection<Particle>& genParts, std::ostream& output)
{
    std::ofstream unmatchedFile("unmatchedFile2.txt");
    unmatchedFile << "event #\tevent element\tname\tcharge\tpt\tEta\tPhi\tEnergy\tMass\n";
    int recoEventElement = 1;
    int genEventElement = 1;
    int leptonCount = 0;
    int elecCount = 0;
    int muonCount = 0;
    bool moun24Found = false;
    int fromQuark = 0;
    int fromLep = 0;
    bool elecEvent = false;
    bool muonEvent = false;
    bool isr = false;
    bool fsr = false;
    for(auto &recoPart : recoParts)
    {
        if (recoPart.getType() == ParticleType::electron() && recoPart.getPt() > 5)
        {
            elecCount++;
            leptonCount++;
        }
        if (recoPart.getType() == ParticleType::muon() && recoPart.getPt() > 5)
        {
            muonCount++;
            leptonCount++;
            moun24Found = moun24Found || recoPart.getPt() > 24;
        }
    }
    if (moun24Found) 
    {
        ++muon24Count; 
    }
    
    if (leptonCount < 4)
    {
        return;
    }
    printEventHeader(output);
    numOfDesiredEvents++;
    int noMatchCount = 0;
    bool noMatch = true;
    for(auto &recoPart : recoParts)
    {
        noMatch = true;
        printRecoPart(recoPart, recoEventElement, output);
        for (auto &genPart : genParts)
        {
            genEventElement = 1;
            double deltaR = std::sqrt( std::pow(recoPart.getPhi() - genPart.getPhi(), 2) + std::pow(recoPart.getEta() - genPart.getEta(), 2) );
            
            if (deltaR < 0.1) //here deltaR is used instead of invariant mass 
            {
                noMatch = false;
                if (eventOutput) 
                {
                    output << std::setw(8) << genEventElement << "| " << std::setw(9) << genPart.getName() << "| ";
                    // Particle properties
                    output << std::setw(13) << genPart.getCharge() << "| " 
                        << std::setw(13) << genPart.getPt() << "| " 
                        << std::setw(13) << genPart.getEta() << "| " 
                        << std::setw(13) << genPart.getPhi() << "| ";
                    try{
                    output << std::setw(13) << genPart.getEnergy() << "| " 
                        << std::setw(13) << genPart.getMass() << std::endl;
                    }
                    catch(const std::exception& e){
                    output << std::setw(13) << "N/A" << "| " 
                        << std::setw(13) << genPart.getMass() << std::endl;
                    }
                }

                //
                std::cout << !GenSimParticle(genPart).hasUniqueMother();
                if (!GenSimParticle(genPart).hasUniqueMother())
                {
                    if (eventOutput) output << std::endl;
                    continue;
                }
                if (eventOutput) 
                {
                    output << std::setw(10) << GenSimParticle(genPart).uniqueMother().getType().getpdgId();
                }
                if (!GenSimParticle(genPart).uniqueMother().hasUniqueMother())
                {
                    output << std::endl;
                    continue;
                }
                if (eventOutput) 
                {
                    output << std::setw(10) << GenSimParticle(genPart).uniqueMother().uniqueMother().getType().getpdgId() << std::endl;
                }
                if (GenSimParticle(genPart).uniqueMother().uniqueMother().getType() == ParticleType::electron() || GenSimParticle(genPart).uniqueMother().uniqueMother().getType() == ParticleType::muon())
                {
                    fromLep++;
                }
                else if (GenSimParticle(genPart).uniqueMother().uniqueMother().getType().getpdgId() < 7 && GenSimParticle(genPart).uniqueMother().uniqueMother().getType().getpdgId()>0)
                {
                    fromQuark++;
                }

                // I do not know what this is
                // if (recoPart.getType()==genPart.getType())
                // {
                //     if((recoPart.getPt() - genPart.getPt())/genPart.getPt() < 0.25 && genPart.getCharge() == recoPart.getCharge())
                //     {
                        
                //     }
                // }
            }
            genEventElement++;
        }
        if (noMatch)
        {
            output << eventCounter << "\t" << recoEventElement << "\t" << recoPart.getName() << "\t";
            // Particle properties
            output << recoPart.getCharge() << "\t" 
            << recoPart.getPt() << "\t"
            << recoPart.getEta() << "\t" 
            << recoPart.getPhi() << "\t";
            try
            {
                output << "\t" << recoPart.getEnergy() << "\t" << recoPart.getMass() << std::endl;
            }
            catch(const std::exception& e)
            {
                output << "\t" << "N/A" << "\t" << recoPart.getMass() << std::endl;
            }
            noMatchCount += noMatch;
        }
        recoEventElement++;
    }
    if (elecCount > 2) 
    {
        electronCounter++;
        elecEvent = true;
    }
    if (muonCount > 2) 
    {
        muonCounter++;
        muonEvent = true;
    }
    if (elecCount == 2 && muonCount == 2) 
    {
        electronCounter++;
        elecEvent = true;
    }
    if (fromQuark > 2) 
    {
        isr = true;
    }
    if (fromLep >= 2)
    {
        fsr = true;
    }
    bool noMatchCaused = muonCount + elecCount - noMatchCount < 4;
    noMatchCounter += noMatchCaused;
    elecNoMatchCounter += noMatchCaused && elecEvent;
    muonNoMatchCounter += noMatchCaused && muonEvent;

    isrCounter += isr;
    fsrCounter += fsr;
    neitherCounter += !isr && !fsr;
    elecIsrCounter += isr && elecEvent;
    elecFsrCounter += fsr && elecEvent;
    muonIsrCounter += isr && muonEvent;
    muonFsrCounter += fsr && muonEvent;
    unmatchedFile.close();
}
//new Code
// function to calculate deltaR
double calculateDeltaR(double eta1, double phi1, double eta2, double phi2) {
    // account for the periodicity of phi
    double dPhi = std::fabs(phi1 - phi2);
    if (dPhi > M_PI) {
        dPhi = 2 * M_PI - dPhi;
    }
    return std::sqrt(std::pow(eta1 - eta2, 2) + std::pow(dPhi, 2));
}

