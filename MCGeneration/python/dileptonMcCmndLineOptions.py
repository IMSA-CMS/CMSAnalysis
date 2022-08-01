import FWCore.ParameterSet.VarParsing as VarParsing

def registerDefaultMCOptions(options):
    options.register ('filename',
                      "defaultName",
                      VarParsing.VarParsing.multiplicity.singleton,
                      VarParsing.VarParsing.varType.string,
                      "output file name")
    options.register ('inFile',
                      "defaultName",
                      VarParsing.VarParsing.multiplicity.singleton,
                      VarParsing.VarParsing.varType.string,
                      "input file name")
