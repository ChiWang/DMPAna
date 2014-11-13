
namespace Reco{
void MaxEnergyBar(int energy_MeV,short LID,short barID,bool ClockTrig = true,char *treeName="Event/Cal"){
  gStyle->SetOptStat(111111);
  gStyle->SetOptFit(111111);
  TChain *HitsTree = gAnaMgr->GetTree(treeName);

  char name[50];
  if(ClockTrig){
    snprintf(name,50,"ClockTrig__layer_%d-bar_%d",LID,barID);
  }else{
    snprintf(name,50,"EventTrig__layer_%d-bar_%d",LID,barID);
  }
  TH1D *h_HittedBar = new TH1D(name,name,400,0,energy_MeV/5.0);
  h_HittedBar->GetXaxis()->SetTitle("Energy / MeV");
  h_HittedBar->GetYaxis()->SetTitle("Counts");

  DmpEvtBgoHits *bgoHits = new DmpEvtBgoHits();
  HitsTree->SetBranchAddress("Hits",&bgoHits);

  DmpEvtHeader *evtHeader = new DmpEvtHeader();
  HitsTree->SetBranchAddress("EventHeader",&evtHeader);

  long nentries = HitsTree->GetEntries();
  for(long ievt =0;ievt<nentries;++ievt){
    HitsTree->GetEntry(ievt);
    if(evtHeader->GeneratedPeriodTrigger() == ClockTrig){
    //if(evtHeader->GeneratedPeriodTrigger() && !evtHeader->InjectedExternalTrigger()){
     continue;
    }
    short nBar = bgoHits->fEnergy.size();
    for(short i=0;i<nBar;++i){
      short l=DmpBgoBase::GetLayerID(bgoHits->fGlobalBarID[i]);
      short b = DmpBgoBase::GetBarID(bgoHits->fGlobalBarID[i]);
      if(l==LID && b == barID){
        h_HittedBar->Fill(bgoHits->fEnergy[i]);
      }
    }
  }
  TCanvas *c2 = new TCanvas(name,name);
  c2->Divide(2,2);
  c2->cd(1);
  h_HittedBar->Draw("colz");
}

}

