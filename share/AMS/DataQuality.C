
#include <vector>

namespace AMS{

void DataQuality(){
  gStyle->SetOptStat(11111111);
  gStyle->SetOptFit(111111111);
  //gAnaCombine->LinkBranches();
  TH1F *h_SeedSN[5][2];
  TH1F *h_ClusterSN[5][2];
  TH1F *h_SeedAdd[5][2];
  TH1F *h_Length[5][2];
  TH1F *h_ClusteNumber[5][2];
  TH1F *h_ClusterDistance[5][2];
  TH1F *h_ClusterTotS[5][2];
  for(short lid =0;lid<5;++lid){
    for(short sid=0;sid<2;++sid){
      char name[80];
      snprintf(name,80,"ladder_%d-side_%d Seed signal 2 noise",lid,sid);
      h_SeedSN[lid][sid] = new TH1F(name,name,100,0,100);

      snprintf(name,80,"ladder_%d-side_%d Cluster signal 2 noise",lid,sid);
      h_ClusterSN[lid][sid] = new TH1F(name,name,120,0,120);

      snprintf(name,80,"ladder_%d-side_%d Seed address",lid,sid);
      h_SeedAdd[lid][sid] = new TH1F(name,name,500,0,1100);

      snprintf(name,80,"ladder_%d-side_%d Cluster length",lid,sid);
      h_Length[lid][sid] = new TH1F(name,name,60,0,60);

      snprintf(name,80,"ladder_%d-side_%d Cluster number",lid,sid);
      h_ClusteNumber[lid][sid] = new TH1F(name,name,10,0,10);

      snprintf(name,80,"ladder_%d-side_%d Cluster distance",lid,sid);
      h_ClusterDistance[lid][sid] = new TH1F(name,name,300,0,600);

      snprintf(name,80,"ladder_%d-side_%d Cluster total signal",lid,sid);
      h_ClusterTotS[lid][sid] = new TH1F(name,name,300,0,600);
    }
  }
  h_SeedSN[0][0]->SetTitle("Seed SNRatio");
  h_ClusterSN[0][0]->SetTitle("Cluster SNRatio");
  h_SeedAdd[0][0]->SetTitle("Seed Address");
  h_Length[0][0]->SetTitle("Cluster Length");
  h_ClusteNumber[0][0]->SetTitle("Cluster Number");
  h_ClusterTotS[0][0]->SetTitle("Cluster total signal");

  TChain *tree = gAnaCombine->GetTree();
  long nEvt = tree->GetEntries();
  std::vector<Cluster*> clusterIndex[5][2];
  for(long ievt=0;ievt<nEvt;++ievt){
    tree->GetEntry(ievt);
    short nCluster = gAnaCombine->fEvtAMS->GetEntriesFast();
    for(int iclu=0;iclu<nCluster;++iclu){
      Cluster *acluster = (Cluster*)gAnaCombine->fEvtAMS->At(iclu);
      int ladder = acluster->ladder;
      if(ladder==5){
        ladder = 2;
      }else if(ladder == 4){
        ladder = 3;
      }else if(ladder == 3){
        ladder = 4;
      }
      int side = acluster->side;
      h_SeedSN[ladder][side]->Fill(acluster->GetSeedSN());
      h_ClusterSN[ladder][side]->Fill(acluster->GetTotSN());
      h_SeedAdd[ladder][side]->Fill(acluster->GetSeedAdd());
      h_Length[ladder][side]->Fill(acluster->GetLength());
      h_ClusterTotS[ladder][side]->Fill(acluster->GetTotSig());
      clusterIndex[ladder][side].push_back(acluster);
    }
    for(short il=0;il<5;++il){
      for(short is=0;is<2;++is){
        h_ClusteNumber[il][is]->Fill(clusterIndex[il][is].size());
        for(size_t icc=0;icc<clusterIndex[il][is].size();++icc){
          if(icc != 0){
            h_ClusterDistance[il][is]->Fill(clusterIndex[il][is][icc]->GetSeedAdd() - clusterIndex[il][is][0]->GetSeedAdd());
          }
        }
        clusterIndex[il][is].clear();
      }
    }
  }

  std::string xx = "AMS data check: "+gRootIOSvc->GetInputFileName();
  TCanvas *c1 = new TCanvas(xx.c_str(),xx.c_str());
  c1->Divide(2,3);
  c1->cd(1);
  gPad->SetLogy();
  TLegend *leg_SeedSN = new TLegend(0.3, 0.7, 0.45, 0.9);
  for(short lid=0;lid<5;++lid){
    for(short sid=0;sid<2;++sid){
      h_SeedSN[lid][sid]->SetLineColor(lid+sid*2+1);
      leg_SeedSN->AddEntry(h_SeedSN[lid][sid],h_SeedSN[lid][sid]->GetName(),"l");
      if(lid == 0 && sid ==0){
        h_SeedSN[lid][sid]->Draw();
      }else{
        h_SeedSN[lid][sid]->Draw("same");
      }
    }
  }
  leg_SeedSN->Draw("same");

  c1->cd(2);
  gPad->SetLogy();
  TLegend *leg_ClusterSN = new TLegend(0.3, 0.7, 0.45, 0.9);
  for(short lid=0;lid<5;++lid){
    for(short sid=0;sid<2;++sid){
      h_ClusterSN[lid][sid]->SetLineColor(lid+sid*2+1);
      leg_ClusterSN->AddEntry(h_ClusterSN[lid][sid],h_ClusterSN[lid][sid]->GetName(),"l");
      if(lid == 0 && sid ==0){
        h_ClusterSN[lid][sid]->Draw();
      }else{
        h_ClusterSN[lid][sid]->Draw("same");
      }
    }
  }
  leg_ClusterSN->Draw("same");

  c1->cd(3);
  gPad->SetLogy();
  TLegend *leg_SeedAdd = new TLegend(0.3, 0.7, 0.45, 0.9);
  for(short lid=0;lid<5;++lid){
    for(short sid=0;sid<2;++sid){
      h_SeedAdd[lid][sid]->SetLineColor(lid+sid*2+1);
      leg_SeedAdd->AddEntry(h_SeedAdd[lid][sid],h_SeedAdd[lid][sid]->GetName(),"l");
      if(lid == 0 && sid ==0){
        h_SeedAdd[lid][sid]->Draw();
      }else{
        h_SeedAdd[lid][sid]->Draw("same");
      }
    }
  }
  leg_SeedAdd->Draw("same");

  c1->cd(4);
  gPad->SetLogy();
  TLegend *leg_Length = new TLegend(0.3, 0.7, 0.45, 0.9);
  for(short lid=0;lid<5;++lid){
    for(short sid=0;sid<2;++sid){
      h_Length[lid][sid]->SetLineColor(lid+sid*2+1);
      leg_Length->AddEntry(h_Length[lid][sid],h_Length[lid][sid]->GetName(),"l");
      if(lid == 0 && sid ==0){
        h_Length[lid][sid]->Draw();
      }else{
        h_Length[lid][sid]->Draw("same");
      }
    }
  }
  leg_Length->Draw("same");

  c1->cd(5);
  TLegend *leg_ClusterNumber = new TLegend(0.3, 0.7, 0.45, 0.9);
  for(short lid=0;lid<5;++lid){
    for(short sid=0;sid<2;++sid){
      h_ClusteNumber[lid][sid]->SetLineColor(lid+sid*2+1);
      leg_ClusterNumber->AddEntry(h_ClusteNumber[lid][sid],h_ClusteNumber[lid][sid]->GetName(),"l");
      if(lid == 0 && sid ==0){
        h_ClusteNumber[lid][sid]->Draw();
      }else{
        h_ClusteNumber[lid][sid]->Draw("same");
      }
    }
  }
  leg_ClusterNumber->Draw("same");

  c1->cd(6);
  TLegend *leg_ClusterTotS = new TLegend(0.3, 0.7, 0.45, 0.9);
  for(short lid=0;lid<5;++lid){
    for(short sid=0;sid<2;++sid){
      h_ClusterTotS[lid][sid]->SetLineColor(lid+sid*2+1);
      leg_ClusterTotS->AddEntry(h_ClusterTotS[lid][sid],h_ClusterTotS[lid][sid]->GetName(),"l");
      if(lid == 0 && sid ==0){
        h_ClusterTotS[lid][sid]->Draw();
      }else{
        h_ClusterTotS[lid][sid]->Draw("same");
      }
    }
  }
  leg_ClusterTotS->Draw("same");

  //
  std::string xx = "AMS data check: "+gRootIOSvc->GetInputFileName()+" cluster distance";
  TCanvas *c2 = new TCanvas(xx.c_str(),xx.c_str());
  c2->Divide(3,2);
  for(short lid=0;lid<5;++lid){
      c2->cd(lid+1);
      h_ClusterDistance[lid][0]->Draw();
      h_ClusterDistance[lid][1]->SetLineColor(6);
      h_ClusterDistance[lid][1]->Draw("same");
  }
}

}

