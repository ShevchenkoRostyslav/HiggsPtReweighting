# HiggsPtReweighting

# installation

git clone https://github.com/raspereza/HiggsPtReweighting.git HiggsPtReweighting

cd HiggsPtReweighting

scramv1 b

# Usage

#include "HiggsPtReweighting/bbH/interface/bbHweights.h"

// instantiating class and loading Higgs pt weights

std::string cmsswBase = (getenv ("CMSSW_BASE"))

TString fullPath = TString(cmsswBase) + "/src/HiggsPtReweighting/bbH/data/bbHweights.root";

bbHweights * higgsPtWeight = new bbHweights(fullPath); 


// getting weight

// inputs : double higgsPt, double higgsMass

double weight = higgsPtWeight->weight(higgsPt,higgsMass);