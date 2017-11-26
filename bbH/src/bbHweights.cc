#include "HiggsPtReweighting/bbH/interface/bbHweights.h"

bbHweights::bbHweights(TString fileName) {

  TFile file(fileName);

  ptMin = 0;
  ptMax = 600;
  nBinsPtHist = 14;

  for (const auto& iM : massD) {
    TString massStr(int(iM));

    TH1D * hist = (TH1D*)file.Get("higgsPt_"+massStr);
    hists.push_back(hist);
    nBinsPtHist = hist->GetNbinsX();
    ptMin = hist->GetXaxis()->GetBinCenter(1);
    ptMax = hist->GetXaxis()->GetBinCenter(nBinsPtHist);

    TH2D * hist2D = (TH2D*)file.Get("2D_"+massStr);
    hists2D.push_back(hist2D);
    nBinsPtHistX = hist2D->GetNbinsX();
    ptMinX = hist2D->GetXaxis()->GetBinCenter(1);
    ptMaxX = hist2D->GetXaxis()->GetBinCenter(nBinsPtHistX);
    nBinsPtHistY = hist2D->GetNbinsY();
    ptMinY = hist2D->GetYaxis()->GetBinCenter(1);
    ptMaxY = hist2D->GetYaxis()->GetBinCenter(nBinsPtHistY);
  }
  int nBins = massD.size() + 1;
  binning = new TH1D("binning","",nBins,&massD[0]);

}

bbHweights::~bbHweights() {

}

double bbHweights::weight(double higgsPt,
			  double higgsMass) {

  double output = 1;

  if (higgsMass>massD.back()) higgsMass = massD.back() - 1;
  if (higgsMass<massD[0]) higgsMass = massD[0] + 1;

  int massBin = binning->FindBin(higgsMass) - 1;
  if (massBin>=(int(hists.size())-1)) massBin = int(hists.size()) - 2;
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

double bbHweights::weight2D(double higgsPt,
			    double higgsMass,
			    double bquarkPt) {

  double output = 1;

  if (higgsMass>massD.back()) higgsMass = massD.back() - 1;
  if (higgsMass<massD[0]) higgsMass = massD[0] + 1;

  int massBin = binning->FindBin(higgsMass) - 1;
  if (massBin>=(int(hists.size())-1)) massBin = int(hists.size()) - 2;
  TH2D * histLow = hists2D.at(massBin);
  TH2D * histHigh = hists2D.at(massBin+1);
  double massLow = binning->GetXaxis()->GetBinLowEdge(massBin+1);
  double massHigh = binning->GetXaxis()->GetBinLowEdge(massBin+2);
  double weightLow = 1;
  double weightHigh  = 1;

  double x = higgsPt;
  double y = bquarkPt;
  if (x>ptMaxX) x = ptMaxX - 0.1;
  if (x<ptMinX) x = ptMinX + 0.1;
  if (y>ptMaxY) y = ptMaxY - 0.1;
  if (y<ptMinY) y = ptMinY + 0.1;

  weightLow = histLow->Interpolate(x,y);
  weightHigh = histHigh->Interpolate(x,y);


  output = weightLow + (weightHigh-weightLow)*(higgsMass-massLow)/(massHigh-massLow);

  /*
  std::cout << "higgsMass = " << higgsMass << std::endl;
  std::cout << "higgsPt = " << higgsPt << std::endl;
  std::cout << "lower mass = " << massLow << "  higher mass = " << massHigh << std::endl;
  std::cout << "weight = " << output << std::endl;
  */

  return output;

}

