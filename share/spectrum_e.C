
#include <map>

namespace Reco{
vector<double> spectrum_e(int energy_MeV,int nBars=50,bool showPass=false){
  ofstream outputlog("spectrum_e_result.txt",ios::out | ios::app);
  outputlog<<"file: "<<gRootIOSvc->GetInputFileName()<<endl;
  vector<double> results(2);
  //={0,0,0,0.};
  gStyle->SetOptStat(111111);
  gStyle->SetOptFit(111111);
  gSystem->Load("$DMPSWWORK/lib/libDmpEventRaw.so");
  TChain *HitsTree = gAnaMgr->GetTree("Event/Cal");

  TH1F *h_nBars = new TH1F("Fired bar number","Fired bar number",nBars,0,nBars);
  h_nBars->GetXaxis()->SetTitle("nBars");
  h_nBars->GetYaxis()->SetTitle("Counts");

  TH1D *h_spectrum = new TH1D("spectrum_e","spectrum_e",400,0,energy_MeV*1.8);
  h_spectrum->GetXaxis()->SetTitle("Energy / MeV");
  h_spectrum->GetYaxis()->SetTitle("Counts");

  TH2D *h_e_nBars = new TH2D("nBars-energy","nBars-energy",400,0,energy_MeV*1.8,nBars,0,nBars);
  h_e_nBars->GetXaxis()->SetTitle("Energy / MeV");
  h_e_nBars->GetYaxis()->SetTitle("nBars");

  TH2F *h_layer_bar_e_X = new TH2F("layer_bar_e-XZ","layer_bar_e-XZ",22,0,22,14,0,14);
  h_layer_bar_e_X->GetXaxis()->SetTitle("bar");
  h_layer_bar_e_X->GetYaxis()->SetTitle("x layer");

  TH2F *h_layer_bar_e_Y = new TH2F("layer_bar_e-YZ","layer_bar_e-YZ",22,0,22,14,0,14);
  h_layer_bar_e_Y->GetXaxis()->SetTitle("bar");
  h_layer_bar_e_Y->GetYaxis()->SetTitle("y layer");

  TH1D *h_mips_e = new TH1D("pass_through_spectrum","pass_through_spectrum-L11&L12&L13",400,0,energy_MeV*1.8);
  h_mips_e->GetXaxis()->SetTitle("Energy / MeV");
  h_mips_e->GetYaxis()->SetTitle("Count");

  //DmpEvtPsdHits *psdHits = new DmpEvtPsdHits();
  //HitsTree->SetBranchAddress("PsdHits",&psdHits);

  DmpEvtBgoHits *bgoHits = new DmpEvtBgoHits();
  HitsTree->SetBranchAddress("Hits",&bgoHits);

  DmpEvtHeader *evtHeader = new DmpEvtHeader();
  HitsTree->SetBranchAddress("EventHeader",&evtHeader);
  long entries = HitsTree->GetEntries();
  for(long ievt=0;ievt<entries;++ievt){
    HitsTree->GetEntry(ievt);
    /*
    if(evtHeader->GetDeltaTime() < TimeCut){
            continue;
    }
    if(evtHeader->GeneratedPeriodTrigger() && !evtHeader->InjectedExternalTrigger()){
     continue;
    }
    */
    double totE =0;
    short nBar = bgoHits->fEnergy.size();
    short nBarInLayer[14]={0};
    for(short i=0;i<nBar;++i){
            /*
            if(bgoHits->fEnergy[i]<0){
    std::cout<<"DEBUG: "<<__FILE__<<"xxxx xx ("<<__LINE__<<")"<<bgoHits->fEnergy[i]<<std::endl;
    continue;
            }
            */
       totE += bgoHits->fEnergy[i];
       if((DmpBgoBase::GetLayerID(bgoHits->fGlobalBarID[i]) % 2) == 0){
               if(totE > 10000){
         h_layer_bar_e_Y->Fill(DmpBgoBase::GetBarID(bgoHits->fGlobalBarID[i]),DmpBgoBase::GetLayerID(bgoHits->fGlobalBarID[i]),bgoHits->fEnergy[i]);
               }
       }else{
               if(totE > 10000){
         h_layer_bar_e_X->Fill(DmpBgoBase::GetBarID(bgoHits->fGlobalBarID[i]),DmpBgoBase::GetLayerID(bgoHits->fGlobalBarID[i]),bgoHits->fEnergy[i]);
               }
       }
       if(bgoHits->fEnergy[i] > 6 && bgoHits->fEnergy[i]<30){
         ++nBarInLayer[DmpBgoBase::GetLayerID(bgoHits->fGlobalBarID[i])];
       }
    }
    h_nBars->Fill(nBar);
    h_spectrum->Fill(totE);
    h_e_nBars->Fill(totE,nBar);
    /*
    for(short i=0;i<14;++i){
      if(nBarInLayer[i] != 1){
        nBarInLayer[0] = -1;
        break;
      }
    }
    */
    if(nBarInLayer[11] != 0 && nBarInLayer[12] != 0 && nBarInLayer[13] != 0){
      h_mips_e->Fill(totE);
      if(showPass){
        char mipsTime[50];
        snprintf(mipsTime,50,"Time: second: %d, millisecond %d",evtHeader->GetSecond(),evtHeader->GetMillisecond());
        gAnaMgr->ShowThisEvent(mipsTime,bgoHits);
      }
    }
    //std::cout<<"DEBUG: "<<__FILE__<<"("<<__LINE__<<") i = "<<ievt<<"\t e = "<<totE<<std::endl;
  }
  char name[60];
  snprintf(name,60,"spectrum_nBars-E_%d",energy_MeV);
  TCanvas *c1= new TCanvas(name,name);
  c1->Divide(3,2);
  c1->cd(1);
  //h_nBars->Draw();
  double mean=0,sigma =0;
  mean = h_nBars->GetMean(), sigma = h_nBars->GetRMS();
  TF1 *gausFit = new TF1("GausFit","gaus",0,150);
  for(short i = 0;i<3;++i){
    gausFit->SetRange(mean-2.5*sigma,mean+2.5*sigma);
    h_nBars->Fit(gausFit,"RQ");
    mean = gausFit->GetParameter(1);
    sigma = gausFit->GetParameter(2);
  }
  h_nBars->Draw();
  outputlog<<"nBars\t\tmean = "<<mean<<"\tsigma = "<<sigma<<std::endl;
  c1->cd(2);
  gPad->SetLogy();
  //h_spectrum->Draw();   // static error and fit
  results[0] = h_spectrum->GetMean(), results[1] = h_spectrum->GetRMS();
  for(short i = 0;i<3;++i){
    gausFit->SetRange(results[0]-2.5*results[1],results[0]+2.5*results[1]);
    h_spectrum->Fit(gausFit,"RQ");
    results[0] = gausFit->GetParameter(1);
    results[1] = gausFit->GetParameter(2);
  }
  h_spectrum->Draw();   // static error and fit
  h_mips_e->SetFillColor(6);
  h_mips_e->Draw("same");
  outputlog<<"energy\t\tmean = "<<results[0]<<"\tsigma = "<<results[1]<<std::endl;
  c1->cd(3);
  h_e_nBars->Draw("colz");
  c1->cd(4);
  h_layer_bar_e_X->Draw("colz");
  c1->cd(5);
  h_layer_bar_e_Y->Draw("colz");
  c1->cd(6);
  TH1D *h_pure = new TH1D("pure_spectrum","pure",400,0,energy_MeV*1.8);
  h_pure->Add(h_spectrum,h_mips_e,1,-1);
  results[0] = h_pure->GetMean(), results[1] = h_pure->GetRMS();
  for(short i = 0;i<3;++i){
    gausFit->SetRange(results[0]-2.5*results[1],results[0]+2.5*results[1]);
    h_pure->Fit(gausFit,"RQ");
    results[0] = gausFit->GetParameter(1);
    results[1] = gausFit->GetParameter(2);
  }
  h_pure->Draw();
  outputlog<<"energy\t\tmean = "<<results[0]<<"\tsigma = "<<results[1]<<std::endl;
  c1->Print("reslut_spectrum_e.ps");
  c1->Print("reslut_spectrum_e.png");
  return results;
}

void total_e_layer(int energy_MeV,char *treeName="Event/Cal",double TimeCut = 50,double lastLayerECut = 30.){
  gStyle->SetOptStat(111111);
  gStyle->SetOptFit(111111);
  TChain *HitsTree = gAnaMgr->GetTree(treeName);

  TH2D *h_e_layer = new TH2D("layer-energy","layer-energy",28,0,14,400,0,energy_MeV/2.0);
  h_e_layer->GetXaxis()->SetTitle("layer ID");
  h_e_layer->GetYaxis()->SetTitle("Energy / MeV");

  TH1D *h_Mips1 = new TH1D("Mips","Mips",400,0,energy_MeV*1.8);
  //h_Mips1->SetTitle("last 2 layers < 30MeV");
  //h_Mips1->GetXaxis()->SetTitle("Energy / MeV");

  TH1D *h_Middle = new TH1D("Middle","Middle",400,0,energy_MeV*1.8);
  //h_Middle->SetTitle("Middle range");
  //h_Middle->GetXaxis()->SetTitle("Energy / MeV");

  TH1D *h_eTotal = new TH1D("total E","tota E",400,0,energy_MeV*1.8);
  h_eTotal->GetXaxis()->SetTitle("Energy / MeV");

  DmpEvtBgoHits *bgoHits = new DmpEvtBgoHits();
  HitsTree->SetBranchAddress("Hits",&bgoHits);

  DmpEvtHeader *evtHeader = new DmpEvtHeader();
  HitsTree->SetBranchAddress("EventHeader",&evtHeader);

  long nentries = HitsTree->GetEntries();
  for(long ievt =0;ievt<nentries;++ievt){
    HitsTree->GetEntry(ievt);
    if(evtHeader->GetDeltaTime() < TimeCut){
            continue;
    }
    /*
    if(evtHeader->GeneratedPeriodTrigger() && !evtHeader->InjectedExternalTrigger()){
     continue;
    }
    */
    double eInLayer[14]={0.};
    double eTotal = 0.;
    short nBar = bgoHits->fEnergy.size();
    for(short i=0;i<nBar;++i){
      short l=DmpBgoBase::GetLayerID(bgoHits->fGlobalBarID[i]);
      eInLayer[l] += bgoHits->fEnergy[i];
    }
    for(short i=0;i<14;++i){
      h_e_layer->Fill(i,eInLayer[i]);
      eTotal += eInLayer[i];
    }
    h_eTotal->Fill(eTotal);
    if(eInLayer[12] < lastLayerECut && eInLayer>6 && eInLayer[13] <lastLayerECut && eInLayer[12] >6){
      h_Mips1->Fill(eTotal);
    }
    if(eInLayer[0]<50 && eInLayer[1]<50 && eTotal>1000){
      h_Middle->Fill(eTotal); 
    }
  }
  char name[50];
  snprintf(name,50,"DeltaTimeCut = %f",TimeCut);
  TCanvas *c2 = new TCanvas(name,name);
  c2->Divide(2,1);
  c2->cd(1);
  h_e_layer->Draw("colz");
  c2->cd(2);
  //h_eTotal->SetLineColor(3);
  h_eTotal->Draw();
  h_Mips1->SetLineColor(3);
  h_Mips1->Draw("same");
  h_Middle->SetLineColor(6);
  h_Middle->Draw("same");
}

}

