#include "HiggsPtReweighting/bbH/interface/bbHweights.h"

bbHweights::bbHweights(TString fileName) {

  file = new TFile(fileName);

  ptMin = 0;
  ptMax = 600;
  nBinsPtHist = 14;

  for (int iM=0; iM<10; ++iM) {
    char massC[4];
    if (massD[iM]<999) 
      sprintf(massC,"%3i",int(massD[iM]+0.2));
    else
      sprintf(massC,"%4i",int(massD[iM]+0.2));
    TString massStr(massC); 
    masses.insert(std::pair<double,TString>(massD[iM],massStr));
    TH1D * hist = (TH1D*)file->Get("higgsPt_"+massStr);
    hists.push_back(hist);
    ptMin = hist->GetXaxis()->GetBinCenter(1);
    nBinsPtHist = hist->GetNbinsX();
    ptMax = hist->GetXaxis()->GetBinCenter(nBinsPtHist);
  }
  int nBins = 9;
  binning = new TH1D("binning","",nBins,massD);

}

bbHweights::~bbHweights() {

}

double bbHweights::weight(double higgsPt,
			  double higgsMass) {

  double output = 1;

  if (higgsMass>massD[9]) higgsMass = massD[9] - 1;
  if (higgsMass<massD[0]) higgsMass = massD[0] + 1;

  int massBin = binning->FindBin(higgsMass) - 1;
  TH1D * histLow = hists.at(massBin);
  TH1D * histHigh = hists.at(massBin+1);
  double massLow = binning->GetXaxis()->GetBinLowEdge(massBin+1);
  double massHigh = binning->GetXaxis()->GetBinLowEdge(massBin+2);
  double weightLow = 1;
  double weightHigh  = 1;

  if (higgsPt>ptMax) { 
    weightLow = histLow->GetBinContent(nBinsPtHist);
    weightHigh = histHigh->GetBinContent(nBinsPtHist);
  }
  else if (higgsPt<ptMin) {
    weightLow = histLow->GetBinContent(1);
    weightHigh = histHigh->GetBinContent(1);
  }
  else {
    weightLow = histLow->GetBinContent(histLow->FindBin(higgsPt));
    weightHigh = histHigh->GetBinContent(histHigh->FindBin(higgsPt));
  }

  output = weightLow + (weightHigh-weightLow)*(higgsMass-massLow)/(massHigh-massLow);

  /*
  std::cout << "higgsMass = " << higgsMass << std::endl;
  std::cout << "higgsPt = " << higgsPt << std::endl;
  std::cout << "lower mass = " << massLow << "  higher mass = " << massHigh << std::endl;
  std::cout << "weight = " << output << std::endl;
  */

  return output;

}

