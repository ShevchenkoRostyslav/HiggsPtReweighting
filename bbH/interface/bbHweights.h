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

 double weight2D(double higgsPt, 
		 double higgsMass,
		 double bquarkPt);

 private:

 double ptMin;
 double ptMax;
 int nBinsPtHist;

 double ptMinY;
 double ptMaxY;
 int nBinsPtHistY;

 double ptMinX;
 double ptMaxX;
 int nBinsPtHistX;

 const double massD[10] = {250,350,400,500,600,700,900,1000,1200,1400};
 std::vector<TH1D*> hists;
 std::vector<TH2D*> hists2D;

 TH1D * binning;
   


};

#endif
