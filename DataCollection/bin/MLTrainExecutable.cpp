#include "TApplication.h"
#include "MLTrain.C"  // Assume this includes the relevant ROOT headers and implementations

int main(int argc, char** argv) {
    TApplication theApp("App", &argc, argv);
    TString emptyString = "";
    returnState(emptyString);  // Assuming MLTrain is a function you can call
    theApp.Run();
    return 0;
}