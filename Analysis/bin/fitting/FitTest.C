#include "CMSAnalysis/Analysis/interface/FitFunctionCollection.hh"
//to run to root FitTest.C
//see if it can read in the functions without breaking 
//the file your reading into is H++SignalParameterFunctions.txt
//mainly to check through file and makes sure things are read in --> will show in terminal
void FitTest()
{
	std::cout << "Starting FitTest()...\n";

    // loads functions from the text file
    FitFunctionCollection functions;
    try {
        functions = FitFunctionCollection::loadFunctions("H++SignalParameterFunctions.txt");
        std::cout << "Successfully read H++SignalParameterFunctions.txt\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to load functions: " << e.what() << std::endl;
        return; //if it does not load function, displays error message
    }

    // accesses the internal map of functions
    auto& funcMap = functions.getFunctions(); //

    if (funcMap.empty()) {
        std::cerr << "No functions were loaded. Check the file format or path.\n";
        return;
    }

    std::cout << "Loaded " << funcMap.size() << " functions:\n";

    // loops over each function and print details including parameter values and errors
    for (const auto& pair : funcMap)
    {
        const std::string& key = pair.first;  // function key, e.g., "\sigma_2"
        const FitFunction& func = pair.second;

        TF1* tf1 = func.getFunction();
        if (!tf1) {
            std::cerr << "Function '" << key << "' is null.\n";
            continue;
        }

        std::cout << "-----------------------------------------\n";
        std::cout << "Function name: " << key << "\n";
        std::cout << "TF1 formula: " << tf1->GetExpFormula("P") << "\n";
        std::cout << "Npar: " << tf1->GetNpar() << "\n";

        // prints parameter names, values, and errors
        std::cout << "Parameters and errors:\n";
        for (int i = 0; i < tf1->GetNpar(); ++i) {
            std::cout << "   " << tf1->GetParName(i)
                      << " = " << tf1->GetParameter(i)
                      << " ± " << tf1->GetParError(i) << "\n";
        }
    }

    std::cout << "FitTest() finished successfully.\n";
}





