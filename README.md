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

// inputs : double higgsPt, double higgsMass, double bquarkPt

// for higgsMass take nominal mass of the Higgs (corresponding to a given processed sample)

// for higgsPt take Higgs boson (pdgId = 36 in PY8 samples) from PYTHIA particle list with status flag 62

// for bquarkPt take the leading (higher pT) b/b~ quark with status flag 71
 
double weight = higgsPtWeight->weight2D(higgsPt,higgsMass,bquarkPt);