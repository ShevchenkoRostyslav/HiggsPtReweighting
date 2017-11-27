#include "HiggsPtReweighting/bbH/interface/bbHweights.h"

bbHweights::bbHweights(TString fileName) : pathToFile_(fileName), done_(false) {

}

bbHweights::~bbHweights() {

}

void bbHweights::Read1DWeights_(){
	/*
	 * Read pT weights from 1D histograms
	 *
	 * file with pathToFile_ is used
	 */
	TFile file(pathToFile_);
	for (const auto& iM : massD) {
	    TString massStr(std::to_string(int(iM)));

	    TString hist_name = TString("higgsPt_") + massStr;
	    TH1D * hist = static_cast<TH1D*>(file.Get(hist_name));
	    hists.push_back(hist);
	    nBinsPtHistX = hist->GetNbinsX();
	    ptMinX = hist->GetXaxis()->GetBinCenter(1);
	    ptMaxX = hist->GetXaxis()->GetBinCenter(nBinsPtHistX);
	}
	int nBins = massD.size() - 1;
	binning = new TH1D("binning","",nBins,&massD[0]);

	done_ = true;
}

void bbHweights::Read2DWeights_(){
	/*
	 * Read pT weights from 2D histograms
	 *
	 * file with pathToFile_ is used
	 */
	TFile file(pathToFile_);
	for (const auto& iM : massD) {
	    TString massStr(std::to_string(int(iM)));

	    TString hist_name = TString("2D_") + massStr;
	    TH2D * hist2D = static_cast<TH2D*>(file.Get(hist_name));
	    hists2D.push_back(hist2D);
	    nBinsPtHistX = hist2D->GetNbinsX();
	    ptMinX = hist2D->GetXaxis()->GetBinCenter(1);
	    ptMaxX = hist2D->GetXaxis()->GetBinCenter(nBinsPtHistX);
	    nBinsPtHistY = hist2D->GetNbinsY();
	    ptMinY = hist2D->GetYaxis()->GetBinCenter(1);
	    ptMaxY = hist2D->GetYaxis()->GetBinCenter(nBinsPtHistY);
	}
	int nBins = massD.size() - 1;
	binning = new TH1D("binning","",nBins,&massD[0]);

	done_ = true;

}

double bbHweights::weight(const double& higgsPt,
			  double higgsMass) {

	if(!done_) Read1DWeights_();

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

  if (higgsPt>ptMaxX) {
    weightLow = histLow->GetBinContent(nBinsPtHistX);
    weightHigh = histHigh->GetBinContent(nBinsPtHistX);
  }
  else if (higgsPt<ptMinX) {
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

double bbHweights::weight2D(const double& higgsPt,
			    double higgsMass,
			    const double& bquarkPt) {

	if(!done_) Read2DWeights_();
  double output = 1;

  if (higgsMass>massD.back()) higgsMass = massD.back() - 1;
  if (higgsMass<massD[0]) higgsMass = massD[0] + 1;

  int massBin = binning->FindBin(higgsMass) - 1;
  if (massBin>=(int(hists2D.size())-1)) massBin = int(hists2D.size()) - 2;
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

//  std::cout << "\nhiggsMass = " << higgsMass << " mass bin = " << massBin << std::endl;
//  std::cout << "higgsPt = " << higgsPt << std::endl;
//  std::cout << "lower mass = " << massLow << "  higher mass = " << massHigh << std::endl;
//  std::cout << "weight low = " << weightLow << "  higher weight = "<< weightHigh << std::endl;
//  std::cout << "weight = " << output << std::endl;

  return output;

}

