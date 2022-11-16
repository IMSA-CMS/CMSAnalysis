################################################################################
# VARIABLES: Definition of the relevant variables from the configuration script.
################################################################################

# Set the shell.
SHELL=/usr/bin/env bash

# Include the configuration.
-include Makefile.inc

# Handle GZIP support.
GZIP_INC=
GZIP_FLAGS=
ifeq ($(GZIP_USE),true)
  GZIP_INC+= -DGZIPSUPPORT -I$(GZIP_INCLUDE)
  GZIP_FLAGS+= -L$(GZIP_LIB) -Wl,-rpath,$(GZIP_LIB) -lz
endif

# Check distribution (use local version first, then installed version).
ifneq ("$(wildcard ../lib/libpythia8.*)","")
  PREFIX_LIB=../lib
  PREFIX_INCLUDE=../include
endif
CXX_COMMON:=-I$(PREFIX_INCLUDE) $(CXX_COMMON)
CXX_COMMON+= -L$(PREFIX_LIB) -Wl,-rpath,$(PREFIX_LIB) -lpythia8 -ldl 

################################################################################
# RULES: Definition of the rules used to build the PYTHIA examples.
################################################################################

# Rules without physical targets (secondary expansion for specific rules).
.SECONDEXPANSION:
.PHONY: all clean

# HEPMC2.
DarkRadiation Width: $$@.cc\
	$(PREFIX_LIB)/libpythia8.a
ifeq ($(HEPMC2_USE),true)
	HEPMC2_LIB=/uscms/home/etuecke/nobackup/darkPhotonsBranchingRatios/hepMC2/cleanDIR/hepmc/install/lib
	$(CXX) $< -o $@ -I$(HEPMC2_INCLUDE) $(CXX_COMMON)\
	 -L/uscms/home/etuecke/nobackup/darkPhotonsBranchingRatios/hepMC2/cleanDIR/hepmc/install/lib -Wl,-rpath,$(HEPMC2_LIB) -lHepMC\
	 $(GZIP_INC) $(GZIP_FLAGS)
else
	@echo "Error: $@ requires HEPMC2"
endif
