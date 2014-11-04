
namespace Reco{
  using namespace Configure;
  namespace Psd{
  TChain *eventTree=0;
  void PsdTotal(){
    DmpEvtHeader  *evtHeader = new DmpEvtHeader();
    DmpEvtBgoHits *psdHits = new DmpEvtBgoHits();
    eventTree->SetBranchAddress("PsdHits",&psdHits);
    long nEvt = eventTree->GetEntries();
    long startEvt = 0;
    for(long i = 0;i<nEvt;++i){
      if(evtHeader->GetSecond()<start_scond){
        continue;
      }else{
        startEvt = i;
        break;
      }
    }
    TH1D *ES0 = new TH1D("E_side0","E_side0",1000,0,2000);
    TH1D *ES1 = new TH1D("E_side1","E_side1",1000,0,2000);
    TH1D *ET = new TH1D("E_T","E_T",1000,0,2000);
    for(long iEvt=startEvt;iEvt<nEvt;++iEvt){
      eventTree->GetEntry(iEvt);
      short nBar = psdHits->fEnergy.size();
      double sumS0=0,sumS1=0,sumT=0;
      for(size_t i=0;i<nBar;++i){
        sumS0 += psdHits->fES0[i];
        sumS1 += psdHits->fES1[i];
        sumT += psdHits->fEnergy[i];
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
  void PsdProfile_Event(){
    DmpEvtBgoHits *psdHits = new DmpEvtBgoHits();
    eventTree->SetBranchAddress("PsdHits",&psdHits);
    TH1D *profile_X = new TH1D("profile_X","profile_X",41,0,41);
    static nEvt =0;
    eventTree->GetEntry(nEvt);
    short nstripHitted = psdHits->fEnergy.size();
    for(){
    short stripID = 0;
    }
    ++nEvt;
  }
}
}

