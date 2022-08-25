import FWCore.ParameterSet.VarParsing as VarParsing
#led = Large Extra Dimenions 
def registerLedMcCmndLineOptions(options):
    options.register ('t',
                      "1",
                      VarParsing.VarParsing.multiplicity.singleton,
                      VarParsing.VarParsing.varType.string,
                      "Form Factor Value")
    options.register ('n',
                      "2",
                      VarParsing.VarParsing.multiplicity.singleton,
                      VarParsing.VarParsing.varType.string,
                      "Number of extra dimenions")
    options.register ('md',
                      "2000",
                      VarParsing.VarParsing.multiplicity.singleton,
                      VarParsing.VarParsing.varType.string,
                      "Fundamental scale of gravity in D = 4 + n dimensions")
    options.register ('lambdaT',
                      "3500",
                      VarParsing.VarParsing.multiplicity.singleton,
                      VarParsing.VarParsing.varType.string,
                      "Ultraviolet cutoff parameter for the virtual graviton exchange processes")
    options.register ('cutOffMode',
                      "0",
                      VarParsing.VarParsing.multiplicity.singleton,
                      VarParsing.VarParsing.varType.string,
                      "method for low energy scale validation")
