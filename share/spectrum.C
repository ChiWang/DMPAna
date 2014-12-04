

namespace Reco{

void spectrum(double Cut_Psd_energy_back = 50){
  gStyle->SetOptStat(111111);
  gStyle->SetOptFit(111111);
  int energy_MeV = gAnaCombine->BeamEnergy();
  TChain *HitsTree = gAnaCombine->GetTree();
  DmpEvtHeader *evtHeader = gAnaCombine->fEvtHeader;
  DmpEvtBgoHits *bgoHits = gAnaCombine->fEvtBgo;
  DmpEvtBgoHits *psdHits = gAnaCombine->fEvtPsd;

  TH1D *h_spectrum = new TH1D("Bgo energy","Bgo energy",400,0,energy_MeV*1.3);
  TH1D *h_spectrumS0 = new TH1D("Engergy side 0","side 0",400,0,energy_MeV*1.3);
  TH1D *h_spectrumS1 = new TH1D("Engergy side 1","side 1",400,0,energy_MeV*1.3);
  h_spectrum->GetXaxis()->SetTitle("Energy / MeV");
  h_spectrum->GetYaxis()->SetTitle("Counts");

  TH1D *h_psd_spectrum = new TH1D("Psd energy","Psd energy",400,0,120);
  TH1D *h_psd_spectrumS0 = new TH1D("Psd engergy side 0","side 0",400,0,120);
  TH1D *h_psd_spectrumS1 = new TH1D("Psd engergy side 1","side 1",400,0,120);
  h_psd_spectrum->GetXaxis()->SetTitle("Energy / MeV");
  h_psd_spectrum->GetYaxis()->SetTitle("Counts");

  TH1D *h_stk_spectrum = new TH1D("Stk energy","Stk energy",400,0,energy_MeV*20);
  h_stk_spectrum->GetXaxis()->SetTitle("Energy / MeV");
  h_stk_spectrum->GetYaxis()->SetTitle("Counts");

  TH1D  *h_eTotal = new TH1D("total energy","Psd + Bgo energy",400,0,energy_MeV*1.3);
  TH1D  *h_eTotal_withBack = new TH1D("energy_psdback","reduce psdBack",400,0,energy_MeV*1.3);
  h_eTotal->GetXaxis()->SetTitle("Energy / MeV");
  h_eTotal->GetYaxis()->SetTitle("Counts");

  TH2D *h_bgoToPsd = new TH2D("Psd_Bgo","Psd_Bgo",400,0,energy_MeV*1.3,400,0,120);
  h_bgoToPsd->GetXaxis()->SetTitle("Bgo energy / MeV");
  h_bgoToPsd->GetYaxis()->SetTitle("Psd energy / MeV");

  long entries = HitsTree->GetEntries();
  for(long ievt=0;ievt<entries;++ievt){
    HitsTree->GetEntry(ievt);
    double totE_bgo = bgoHits->TotalEnergy();
    double totE_bgos0 = bgoHits->TotalEnergyS0();
    double totE_bgos1 = bgoHits->TotalEnergyS1();
    h_spectrum->Fill(totE_bgo);
    h_spectrumS0->Fill(totE_bgos0);
    h_spectrumS1->Fill(totE_bgos1);

    double totE_psd =   psdHits->TotalEnergy();
    double totE_psds0 = psdHits->TotalEnergyS0();
    double totE_psds1 = psdHits->TotalEnergyS1();
    h_psd_spectrum->Fill(totE_psd);
    h_psd_spectrumS0->Fill(totE_psds0);
    h_psd_spectrumS1->Fill(totE_psds1);

    h_eTotal->Fill(totE_bgo + totE_psd);
    h_bgoToPsd->Fill(totE_bgo,totE_psd);
    if(totE_psd >= Cut_Psd_energy_back){
      h_eTotal_withBack->Fill(totE_bgo+totE_psd);
    }

    double totE_stk = gAnaCombine->StkEnergy();
    h_stk_spectrum->Fill(totE_stk);
  }

  std::string name = "energy of "+gRootIOSvc->GetInputStem();
  TCanvas *c1 = new TCanvas(name.c_str(),name.c_str());
  c1->Divide(3,3);
  c1->cd(1);
  h_spectrum->SetLineColor(2);
  h_spectrum->SetLineWidth(2);
  h_spectrum->Draw();
  h_spectrumS0->SetLineColor(4);
  h_spectrumS0->SetLineWidth(2);
  h_spectrumS0->Draw("same");
  h_spectrumS1->SetLineColor(3);
  h_spectrumS1->SetLineWidth(2);
  h_spectrumS1->Draw("same");
  TLegend *legBgo = new TLegend(0.3, 0.7, 0.45, 0.9);
  legBgo->AddEntry(h_spectrum, h_spectrum->GetTitle(), "l");
  legBgo->AddEntry(h_spectrumS0, h_spectrumS0->GetTitle(), "l");
  legBgo->AddEntry(h_spectrumS1, h_spectrumS1->GetTitle(), "l");
  legBgo->Draw("same");

  c1->cd(2);
  gPad->SetLogy();
  h_psd_spectrum->SetLineColor(2);
  h_psd_spectrum->SetLineWidth(2);
  h_psd_spectrum->Draw();
  h_psd_spectrumS0->SetLineColor(4);
  h_psd_spectrumS0->SetLineWidth(2);
  h_psd_spectrumS0->Draw("same");
  h_psd_spectrumS1->SetLineColor(3);
  h_psd_spectrumS1->SetLineWidth(2);
  h_psd_spectrumS1->Draw("same");
  TLegend *legPsd = new TLegend(0.3, 0.7, 0.45, 0.9);
  legPsd->AddEntry(h_psd_spectrum,   h_psd_spectrum->GetTitle(), "l");
  legPsd->AddEntry(h_psd_spectrumS0, h_psd_spectrumS0->GetTitle(), "l");
  legPsd->AddEntry(h_psd_spectrumS1, h_psd_spectrumS1->GetTitle(), "l");
  legPsd->Draw("same");

  c1->cd(3);
  h_bgoToPsd->Draw("colz");

  c1->cd(4);
  h_eTotal->Draw();
  h_eTotal_withBack->SetFillColor(3);
  h_eTotal_withBack->Draw("same");

  c1->cd(5);
  h_stk_spectrum->Draw();

}

}

