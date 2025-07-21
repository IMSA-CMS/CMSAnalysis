#include <TFile.h>
#include <TKey.h>
#include <TDirectory.h>
#include <TObjString.h>
#include <TObject.h>
#include <TH1.h>
#include <TTree.h>

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <cctype>

// Check if a TObject is versioned by seeing if multiple cycles exist
bool isVersionedStringKey(TFile* file, const std::string& baseName) {
    int count = 0;
    TIter next(file->GetListOfKeys());
    TKey* key;
    while ((key = (TKey*)next())) {
        if (std::string(key->GetName()) == baseName) {
            ++count;
        }
    }
    return count > 1;
}

// Recursively process objects and folders
void processDirectory(TDirectory* inDir, TDirectory* outDir) {
    std::map<std::string, std::vector<double>> valueMap;

    TIter nextKey(inDir->GetListOfKeys());
    TKey* key;

    // First pass: collect versioned TObjString values
    while ((key = (TKey*)nextKey())) {
        std::string keyName = key->GetName();
        TObject* obj = key->ReadObj();
        if (!obj) continue;

        // Recurse into folders
        if (obj->InheritsFrom("TDirectory")) {
            TDirectory* subIn = (TDirectory*)obj;
            outDir->cd();
            TDirectory* subOut = outDir->mkdir(subIn->GetName());
            subOut->cd();
            processDirectory(subIn, subOut);
            delete obj;
            continue;
        }

        // Collect versioned TObjString values
        TObjString* strObj = dynamic_cast<TObjString*>(obj);
        if (strObj) {
            std::string valStr = strObj->GetString().Data();
            try {
                double val = std::stod(valStr);
                valueMap[keyName].push_back(val);
            } catch (...) {
                // Not a number, just copy it later
            }
            delete obj;
            continue;
        }

        delete obj;
    }

    // Second pass: write everything (except versioned TObjStrings)
    TIter copyKeys(inDir->GetListOfKeys());
    while ((key = (TKey*)copyKeys())) {
        std::string keyName = key->GetName();
        TObject* obj = key->ReadObj();
        if (!obj) continue;

        // Skip TObjStrings we already summed
        if (valueMap.count(keyName) > 0) {
            delete obj;
            continue;
        }

        outDir->cd();
        TObject* clone = obj->Clone(keyName.c_str());
        if (clone) {
            if (clone->InheritsFrom("TH1")) {
                static_cast<TH1*>(clone)->SetDirectory(outDir);
            } else if (clone->InheritsFrom("TTree")) {
                static_cast<TTree*>(clone)->SetDirectory(outDir);
            }
            clone->Write();
        }

        delete obj;
    }

    // Write summed strings
    for (const auto& pair : valueMap) {
        double sum = 0;
        for (double v : pair.second) sum += v;
        std::ostringstream oss;
        oss << sum;

        TObjString result(oss.str().c_str());
        outDir->cd();
        result.Write(pair.first.c_str());
    }
}

// Main entry point
void cleanfile(const char* inputFileName = "input.root", const char* outputFileName = "output.root") {
    TFile* inFile = TFile::Open(inputFileName, "READ");
    if (!inFile || inFile->IsZombie()) {
        std::cerr << "Error: Cannot open input file " << inputFileName << std::endl;
        return;
    }

    TFile* outFile = TFile::Open(outputFileName, "RECREATE");
    if (!outFile || outFile->IsZombie()) {
        std::cerr << "Error: Cannot create output file " << outputFileName << std::endl;
        inFile->Close();
        return;
    }

    processDirectory(inFile, outFile);

    outFile->Close();
    inFile->Close();
    std::cout << "Done. Output written to " << outputFileName << std::endl;
}
