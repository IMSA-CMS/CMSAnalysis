#include "TApplication.h"
#include "SuperimposeRatio.C"  // Assume this includes the relevant ROOT headers and implementations

int main(int argc, char** argv) {
    TApplication theApp("App", &argc, argv);
    SuperimposeRatio();
    theApp.Run();
    return 0;
}