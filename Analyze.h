#ifndef Analyze_h
#define Analyze_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>

// Headers needed by this particular selector


class Analyze : public TSelector {
public :
   TTreeReader     fReader;  //!the tree reader
   TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain

   // Readers to access the data (delete the ones you do not need).
   TTreeReaderValue<Int_t> event = {fReader, "event"};
   TTreeReaderValue<Float_t> ebeam = {fReader, "ebeam"};
   TTreeReaderValue<Float_t> px = {fReader, "px"};
   TTreeReaderValue<Float_t> py = {fReader, "py"};
   TTreeReaderValue<Float_t> pz = {fReader, "pz"};
   TTreeReaderValue<Float_t> zv = {fReader, "zv"};
   TTreeReaderValue<Float_t> chi2 = {fReader, "chi2"};


   Analyze(TTree * /*tree*/ =0) { }
   virtual ~Analyze() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();
   // Declaring pT and theta vars.
   Float_t pT;
   Float_t theta;
   // Declaring i and j to count the number of eventos above some cut defined on .C file
   Int_t i=0;
   Int_t j=0;
   ClassDef(Analyze,0);

};

#endif

#ifdef Analyze_cxx
void Analyze::Init(TTree *tree)
{
   fReader.SetTree(tree);
}

Bool_t Analyze::Notify()
{
   return kTRUE;
}


#endif // #ifdef Analyze_cxx
