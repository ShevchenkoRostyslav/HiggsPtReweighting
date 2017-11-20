#ifndef bbHweights_h
#define bbHweights_h

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "TString.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"

using namespace std;

class bbHweights {

 public: 

 bbHweights(TString fileName);
 ~bbHweights();

 double weight(double higgsPt, 
	       double higgsMass);

 private:

 TFile * file;

 double ptMin;
 double ptMax;
 int nBinsPtHist;

 const double massD[10] = {250,350,400,500,600,700,900,1000,1200,1400};
 std::vector<TH1D*> hists;
 std::map<double, TString> masses;

 TH1D * binning;
   


};

#endif
