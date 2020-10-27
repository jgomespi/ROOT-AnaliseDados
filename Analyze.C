#define Analyze_cxx
#include "Analyze.h"
#include <TH2.h>
#include <TStyle.h>

//**********Definition section**********
TH1* chi2Hist = NULL;
TH1* ebeamHist = NULL;
TH2* chi2ebeamHist = NULL;
TH1* thetaHist = NULL;
TH1* ptHist = NULL;

void Analyze::Begin(TTree * /*tree*/)
{
   TString option = GetOption();

   //**********Initialization section**********
   chi2Hist = new TH1D("chi2", "Histogram of Chi2", 100, 0, 2.);
   chi2Hist->GetXaxis()->SetTitle("chi2");
   chi2Hist->GetYaxis()->SetTitle("number of events");

   // Create a ebeamHist histogram, to Fill ebeam var, and set its axis titles.
   ebeamHist = new TH1D("ebeam", "Histogram of ebeam", 100, 149., 151.);
   ebeamHist->GetXaxis()->SetTitle("ebeam (GeV)");
   ebeamHist->GetYaxis()->SetTitle("number of events");

   // Create a scatterplot for ebeam and chi2 vars and set its axis titles.
   chi2ebeamHist = new TH2D("chi2ebeam", "ScatterPlot of chi2 and ebeam", 100, 0, 2., 100, 149., 151.);
   chi2ebeamHist->GetXaxis()->SetTitle("chi2");
   chi2ebeamHist->GetYaxis()->SetTitle("ebeam (GeV)");

   // Create a ptHist histogram to Fill pT var (Float_t type, also created here) and set its axis titles.
   ptHist = new TH1D("pt", "Histogram of pT", 100, 0, 35);
   ptHist->GetXaxis()->SetTitle("pT (GeV)");
   ptHist->GetYaxis()->SetTitle("number of events");

   // Create a thetaHist histogram to Fill theta var (Float_t type, also created here) and set its axis titles.
   thetaHist = new TH1D("theta", "Histogram of theta", 100, -3.15, 3.15);
   thetaHist->GetXaxis()->SetTitle("theta");
   thetaHist->GetYaxis()->SetTitle("number of events");


}

void Analyze::SlaveBegin(TTree * /*tree*/){}

Bool_t Analyze::Process(Long64_t entry)
{
   // Don’t delete this line! Without it the program will crash
   fReader.SetLocalEntry(entry);

   //**********Loop section**********
   GetEntry(entry);
   chi2Hist->Fill(*chi2);
   // Fill ebeamHist with ebeam var
   ebeamHist->Fill(*ebeam);
   // Fill chi2ebeamHist with ebeam and chi2 vars
   chi2ebeamHist->Fill(*chi2,*ebeam);

   // Calc pT:
   pT = TMath::Sqrt((*px)*(*px)+(*py)*(*py));
   ptHist->Fill(pT);

   // Calc theta:
   theta = TMath::ATan2((*py),(*px));
   thetaHist->Fill(theta);
   
   // j will count all events
   j++;
   // i will count events with pz < 145.0 GeV
   if (TMath::Abs(*pz)<145.) {
   	  // Here we print the value of pz (when pz<145 GeV) on screen
      std::cout << *pz << i << std::endl; 
      i++;
   }

   return kTRUE;
}

void Analyze::SlaveTerminate(){}

void Analyze::Terminate()
{
    //**********Wrap-up section**********
    // Draw chi2Hist with error bars
    chi2Hist->Draw("E1");

    //Fit a gaussian to ebeam distribuition and draw ebeamHist with error bars
    ebeamHist->Fit("gaus","V","E1",149.,151.);

    // Set the position of Stat Box
   	gStyle->SetStatX(0.9);
   	gStyle->SetStatY(0.9);

   	// Set the position of chi2ebeamHist Y axis title
   	chi2ebeamHist->GetYaxis()->SetTitleOffset(1.4);
   	// Draw chi2ebeamHist scatterplot with "COLZ" option, which include the palette colors
    chi2ebeamHist->Draw("COLZ"); 

    // Reset the stat box
    gStyle->Reset();
    // Set the position of ptHist Y axis title
    ptHist->GetYaxis()->SetTitleOffset(1.4);
    // Draw thetaHist and ptHist
    thetaHist->Draw();    
    ptHist->Draw();

    // Print on screen how many events have pz<145 GeV
    std::cout << "The number of events with pz<145.0 GeV is " << i << std::endl;

    // Recreate a file named "experiment-output.root" and write the histrograms
    TFile f("experiment-output.root","recreate");
    chi2Hist->Write();
    ebeamHist->Write();
    chi2ebeamHist->Write();
    ptHist->Write();
    thetaHist->Write();
    f.Write();
    f.Close();
}