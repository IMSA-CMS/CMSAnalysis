#include "CMSAnalysis/Analysis/interface/FitFunctionCollection.hh"

void FitTest()
{
    std::cout << "Starting FitTest()...\n";

    // Load functions
    FitFunctionCollection functions;
    try {
        functions = FitFunctionCollection::loadFunctions("H++SignalFunctions.txt");
        std::cout << "Successfully read H++SignalFunctions.txt\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to load functions: " << e.what() << std::endl;
        return;
    }

    auto& funcMap = functions.getFunctions();
    if (funcMap.empty()) {
        std::cerr << "No functions were loaded.\n";
        return;
    }

    std::cout << "Loaded " << funcMap.size() << " functions:\n";

    // Loop over functions
    for (const auto& pair : funcMap)
    {
        const std::string& key = pair.first;
        const FitFunction& func = pair.second;

        TF1* tf1 = func.getFunction();
        if (!tf1) {
            std::cerr << "Function '" << key << "' is null.\n";
            continue;
        }

        std::cout << "Function name: " << key << "\n";
        std::cout << "TF1 formula: " << tf1->GetExpFormula("P") << "\n";
        std::cout << "Npar: " << tf1->GetNpar() << "\n";

        // Example systematic (+5%/-5%)
        if (tf1->GetNpar() >= 3)
        {
            std::vector<double> upParams, downParams;
            for (int i = 0; i < tf1->GetNpar(); ++i) {
                double val = tf1->GetParameter(i);
                upParams.push_back(val * 1.05);
                downParams.push_back(val * 0.95);
            }

            FitFunction& mutableFunc = const_cast<FitFunction&>(func);
            mutableFunc.addSystematic("scale", upParams, downParams);

            TF1* sysUp = mutableFunc.getSystematic("scale", true);
            TF1* sysDown = mutableFunc.getSystematic("scale", false);

            if (sysUp && sysDown) {
                std::cout << "  Added systematic 'scale':\n";
                std::cout << "    Up variation name: " << sysUp->GetName() << "\n";
                std::cout << "    Down variation name: " << sysDown->GetName() << "\n";
            }

            // List all systematics
            auto systNames = mutableFunc.listSystematics();
            std::cout << "  Current systematics stored: ";
            for (auto& name : systNames) std::cout << name << " ";
            std::cout << "\n";
        }

        // Print parameters
        std::cout << "Parameters and errors:\n";
        for (int i = 0; i < tf1->GetNpar(); ++i) {
            std::cout << "   " << tf1->GetParName(i)
                      << " = " << tf1->GetParameter(i)
                      << " ± " << tf1->GetParError(i) << "\n";
        }

        // Print all systematics again (optional)
        std::vector<std::string> sysList = func.listSystematics();
        if (!sysList.empty()) {
            std::cout << "  Added systematics:\n";
            for (const auto& sysName : sysList) {
                TF1* up = func.getSystematic(sysName, true);
                TF1* down = func.getSystematic(sysName, false);
                std::cout << "    " << sysName << ":\n"
                          << "      Up variation name: " << up->GetName() << "\n"
                          << "      Down variation name: " << down->GetName() << "\n";
            }
        }

        std::cout << "---------------------------------------------\n";
    } // end of for-loop

    // Print summary AFTER looping through all functions
    std::cout << "Total functions successfully processed: " << funcMap.size() << "\n";
    std::cout << "FitTest() finished successfully.\n";
}






