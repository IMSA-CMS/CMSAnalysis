import FWCore.ParameterSet.VarParsing as VarParsing

def registerDefaultMCOptions(options):
    options.register ('minMass',
                      -1,
                      VarParsing.VarParsing.multiplicity.singleton,
                      VarParsing.VarParsing.varType.float,          
                      "min mass")
    options.register ('maxMass',
                      -1, 
                      VarParsing.VarParsing.multiplicity.singleton,
                      VarParsing.VarParsing.varType.float,          
                      "max mass")

    options.register ('mass',
                      1000, 
                      VarParsing.VarParsing.multiplicity.singleton,
                      VarParsing.VarParsing.varType.float,          
                      "mass")
    options.register ('comEnergy',
                        13, 
                      VarParsing.VarParsing.multiplicity.singleton,
                      VarParsing.VarParsing.varType.float,          
                      "Centre of Mass Energy (in TeV)")
    options.register ('seed',
                      17, 
                      VarParsing.VarParsing.multiplicity.singleton,
                      VarParsing.VarParsing.varType.int,          
                      "seed to pass to random number generator to ensure unique events")
    options.register ('pTMin',
                      0,
                      VarParsing.VarParsing.multiplicity.singleton,
                      VarParsing.VarParsing.varType.float,
                      "pTMin")

    options.register ('pTMax',
                      0,
                      VarParsing.VarParsing.multiplicity.singleton,
                      VarParsing.VarParsing.varType.float,
                      "pTMax")
    options.register ('Lambda',
                      1000,
                      VarParsing.VarParsing.multiplicity.singleton,
                      VarParsing.VarParsing.varType.int,
                      "Compositeness Scale")
    options.register ('helicityLL',
                      0,
                      VarParsing.VarParsing.multiplicity.singleton,
                      VarParsing.VarParsing.varType.int,
                      "LLISM")
    options.register ('helicityLR',
                      0,
                      VarParsing.VarParsing.multiplicity.singleton,
                      VarParsing.VarParsing.varType.int,
                      "LRISM")
    options.register ('helicityRR',
                      0,
                      VarParsing.VarParsing.multiplicity.singleton,
                      VarParsing.VarParsing.varType.int,
                      "RRISM")
    options.register ('ciGen',
                      0,
                      VarParsing.VarParsing.multiplicity.singleton,
                      VarParsing.VarParsing.varType.int,
                      "Process Type")
    options.register ('pdgId',
                      13,
                      VarParsing.VarParsing.multiplicity.singleton,
                      VarParsing.VarParsing.varType.int,
                      "Particle ID")

    options.register ('ISR',
                      "on",
                      VarParsing.VarParsing.multiplicity.singleton,
                      VarParsing.VarParsing.varType.string,
                      "Turn ISR off")
    options.register ('FSR',
                      "on",
                      VarParsing.VarParsing.multiplicity.singleton,
                      VarParsing.VarParsing.varType.string,
                      "Turn FSR off")
    options.register ('ULE',
                      "on",
                      VarParsing.VarParsing.multiplicity.singleton,
                      VarParsing.VarParsing.varType.string,
                      "UnderLying Event")
