
namespace Reco{
  void TotalE(std::string file,double MaxEnergy_MeV){
    TChain *eventTree = new TChain("Event/Cal");
    eventTree->AddFile(file.c_str());
    DmpEvtBgoHits *evts = new DmpEvtBgoHits();
    eventTree->SETBranchAddress("Hits",&evts);
    long nEvt = eventTree->GETEntries();
    TH1D *ES0 = new TH1D("E_side0","E_side0",MaxEnergy_MeV/10,0,MaxEnergy_MeV);
    TH1D *ES1 = new TH1D("E_side1","E_side1",MaxEnergy_MeV/10,0,MaxEnergy_MeV);
    TH1D *ET = new TH1D("E_T","E_T",MaxEnergy_MeV/10,0,MaxEnergy_MeV);
    for(long iEvt=0;i<nEvt;++iEvt){
      eventTree->GETEntry(iEvt);
      short nBar = evts->fEnergy.size();
      double sumS0=0,sumS1=0,sumT=0;
      for(size_t i=0;i<nBar;++i){
        sumS0 += evts->fES0[i];
        sumS1 += evts->fES1[i];
        sumT += evts->fEnergy[i];
      }
      ES0->Fill(sumS0);
      ES1->Fill(sumS1);
      ET->Fill(sumT);
    }
    TCanvas *c1 = new TChain("Total Energy","Total Energy");
    ES0->Draw();
    ES1->Draw("same");
    ET->Draw("same");
  
  }
}
