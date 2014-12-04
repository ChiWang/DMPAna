

#include <vector>

namespace AMS{
        /*
         *  use Alignment_2
         *
         */

//-------------------------------------------------------------------
void Alignment_0(int cluster_length_leftCut = 2,int cluster_length_rightCut = 7,float TotSignal_Cut = 5){
  gStyle->SetOptStat(11111111);
  gStyle->SetOptFit(111111111);
  //gAnaCombine->LinkBranches();

  TH1D *h_ladder_stripID[5][2];
  TH1D *h_Refer[5][2];
  char name[80];
  for(short lid =0;lid<5;++lid){
    for(short sid=0;sid<2;++sid){
      snprintf(name,80,"ladder_%d-side_%d",lid,sid);
      h_ladder_stripID[lid][sid] = new TH1D(name,name,500,0,1100);
      snprintf(name,80,"ladder_%d-side_%d VS ladder 0",lid,sid);
      h_Refer[lid][sid] = new TH1D(name,name,200,-200,200);
    }
    snprintf(name,80,"ladder_%d",lid);
    h_ladder_stripID[lid][0]->SetTitle(name);
    snprintf(name,80,"Seed Address ladder_%d",lid);
    h_Refer[lid][0]->SetTitle(name);
  }

  TChain *tree = gAnaCombine->GetTree();
  long nEvt = tree->GetEntries();
  for(long ievt=0;ievt<nEvt;++ievt){
    tree->GetEntry(ievt);
    short nCluster = gAnaCombine->fEvtAMS->GetEntriesFast();
    float Ref[2]={0.,0.}, RefSN[2]={0.,0.};
    for(int iclu=0;iclu<nCluster;++iclu){
      Cluster *acluster = (Cluster*)gAnaCombine->fEvtAMS->At(iclu);
      if(acluster->ladder !=0){
        continue;
      }
      short length = acluster->length;
      int side = acluster->side;
      if(acluster->GetTotSN()>RefSN[side] && cluster_length_leftCut<=length && length<=cluster_length_rightCut && acluster->GetTotSig()>TotSignal_Cut){
        Ref[side] = acluster->GetSeedAdd();
        RefSN[side] = acluster->GetTotSN();
      }
    }
    for(int iclu=0;iclu<nCluster;++iclu){
      Cluster *acluster = (Cluster*)gAnaCombine->fEvtAMS->At(iclu);
      short length = acluster->length;
      if(length < cluster_length_leftCut || length>cluster_length_rightCut || acluster->GetTotSig()<TotSignal_Cut || acluster->GetTotSig()<TotSignal_Cut){
        continue;
      }
      int ladder = acluster->ladder;
      if(ladder==5){
        ladder = 2;
      }else if(ladder == 4){
        ladder = 3;
      }else if(ladder == 3){
        ladder = 4;
      }
      int side = acluster->side;
      h_ladder_stripID[ladder][side]->Fill(acluster->GetSeedAdd());
      h_Refer[ladder][side]->Fill(acluster->GetSeedAdd()-Ref[side]);
    }
  }
  snprintf(name,80,"AMS Alignment_0_a: cluster length %d~%d, total signal cut %f",cluster_length_leftCut,cluster_length_rightCut,TotSignal_Cut);
  TCanvas *c1 = new TCanvas(name,name);
  c1->Divide(2,3);
  snprintf(name,80,"AMS Alignment_0_b: cluster length %d~%d, total signal cut %f",cluster_length_leftCut,cluster_length_rightCut,TotSignal_Cut);
  TCanvas *c2 = new TCanvas(name,name);
  c2->Divide(2,3);
  for(short lid=0;lid<5;++lid){
    c1->cd(lid+1);
    h_ladder_stripID[lid][0]->Draw();
    h_ladder_stripID[lid][1]->SetLineColor(6);
    h_ladder_stripID[lid][1]->Draw("same");
    c2->cd(lid+1);
    h_Refer[lid][0]->Draw();
    h_Refer[lid][1]->SetLineColor(6);
    h_Refer[lid][1]->Draw("same");
  }
}

//-------------------------------------------------------------------
void Alignment_1(int cluster_length_leftCut = 2,int cluster_length_rightCut = 7,float TotSignal_Cut = 5){
  gStyle->SetOptStat(11111111);
  gStyle->SetOptFit(111111111);
  //gAnaCombine->LinkBranches();
  ofstream outputlog("./AMS/AMSAlignment_1.txt",ios::out | ios::app);
  outputlog<<"\nfile: "<<gRootIOSvc->GetInputFileName()<<".\tCluster length: "<<cluster_length_leftCut<<"~"<<cluster_length_rightCut<<".\tTotal signale cut "<<TotSignal_Cut<<endl;

  TH1D *h_ladder_stripID[5][2];
  TH1D *h_Refer[5][2];
  char name[80];
  for(short lid =0;lid<5;++lid){
    for(short sid=0;sid<2;++sid){
      snprintf(name,80,"ladder_%d-side_%d",lid,sid);
      h_ladder_stripID[lid][sid] = new TH1D(name,name,500,0,1100);
      snprintf(name,80,"ladder_%d-side_%d VS ladder 0",lid,sid);
      h_Refer[lid][sid] = new TH1D(name,name,200,-200,200);
    }
    snprintf(name,80,"ladder_%d",lid);
    h_ladder_stripID[lid][0]->SetTitle(name);
    snprintf(name,80,"Seed Address ladder_%d",lid);
    h_Refer[lid][0]->SetTitle(name);
  }

  TChain *tree = gAnaCombine->GetTree();
  long nEvt = tree->GetEntries();
  std::vector<Cluster*> clusterIndex[5][2];
  for(long ievt=0;ievt<nEvt;++ievt){
    tree->GetEntry(ievt);
    short nCluster = gAnaCombine->fEvtAMS->GetEntriesFast();
    for(int iclu=0;iclu<nCluster;++iclu){
      Cluster *acluster = (Cluster*)gAnaCombine->fEvtAMS->At(iclu);
      if(acluster->GetTotSig()>TotSignal_Cut && cluster_length_leftCut<=acluster->length && acluster->length <= cluster_length_rightCut){
        int ladder = acluster->ladder;
        if(ladder==5){
          ladder = 2;
        }else if(ladder == 4){
          ladder = 3;
        }else if(ladder == 3){
          ladder = 4;
        }
        clusterIndex[ladder][acluster->side].push_back(acluster);
      }
    }
    if(clusterIndex[0][0].size() == 1 && clusterIndex[0][1].size() == 1){
      for(short il=0;il<5;++il){
        for(short is=0;is<2;++is){
          if(clusterIndex[il][is].size() == 1){
            h_ladder_stripID[il][is]->Fill(clusterIndex[il][is][0]->GetSeedAdd());
            h_Refer[il][is]->Fill(clusterIndex[il][is][0]->GetSeedAdd() - clusterIndex[0][is][0]->GetSeedAdd());
          }
          clusterIndex[il][is].clear();
        }
      }
    }
  }
  snprintf(name,80,"AMS Alignment_1_a: cluster length %d~%d, total signal cut %f",cluster_length_leftCut,cluster_length_rightCut,TotSignal_Cut);
  TCanvas *c1 = new TCanvas(name,name);
  c1->Divide(2,3);
  snprintf(name,80,"AMS Alignment_1_b: cluster length %d~%d, totalt signal cut %f",cluster_length_leftCut,cluster_length_rightCut,TotSignal_Cut);

  TF1 *gausFit = new TF1("GausFit","gaus",0,150);
  TCanvas *c2 = new TCanvas(name,name);
  c2->Divide(2,3);
  double mean=0.,sigma =0.;
  for(short lid=0;lid<5;++lid){
    c1->cd(lid+1);
    h_ladder_stripID[lid][0]->Draw();
    h_ladder_stripID[lid][1]->SetLineColor(6);
    h_ladder_stripID[lid][1]->Draw("same");

    c2->cd(lid+1);
    h_Refer[lid][0]->Draw();
    mean = h_Refer[lid][0]->GetMean(); sigma = h_Refer[lid][0]->GetRMS();
    gausFit->SetRange(mean-2.5*sigma,mean+2.5*sigma);
    h_Refer[lid][0]->Fit(gausFit,"R0Q");
    gausFit->DrawCopy("lsame");
    outputlog<<lid<<"  "<<gausFit->GetParameter(1)<<"  "<<gausFit->GetParameter(2)<<"    ";

    h_Refer[lid][1]->SetLineColor(6);
    h_Refer[lid][1]->Draw("same");
    mean = h_Refer[lid][1]->GetMean(); sigma = h_Refer[lid][1]->GetRMS();
    gausFit->SetRange(mean-2.5*sigma,mean+2.5*sigma);
    h_Refer[lid][1]->Fit(gausFit,"R0Q");
    gausFit->DrawCopy("lsame");
    outputlog<<gausFit->GetParameter(1)<<"  "<<gausFit->GetParameter(2)<<endl;
  }
}

//-------------------------------------------------------------------
void Alignment_2(int cluster_length_leftCut = 2,int cluster_length_rightCut = 7,float TotSignal_Cut = 5,bool mask=false){
  gStyle->SetOptStat(11111111);
  gStyle->SetOptFit(111111111);
  //gAnaCombine->LinkBranches();
  ofstream outputlog("./AMS/AMSAlignment_2.txt",ios::out | ios::app);
  outputlog<<"\nfile: "<<gRootIOSvc->GetInputFileName()<<".\tCluster length: "<<cluster_length_leftCut<<"~"<<cluster_length_rightCut<<".\tTotal signale cut "<<TotSignal_Cut<<endl;

  TH2D *h_ladder[4][2]; // refer to ladder 0
  char name[80];
  for(short lid =0;lid<4;++lid){
    for(short sid=0;sid<2;++sid){
      snprintf(name,80,"ladder_%d-side_%d",lid+1,sid);
      h_ladder[lid][sid] = new TH2D(name,name,400,0+sid*600,800+sid*200,400,0+sid*600,800+sid*200);
      snprintf(name,80,"ladder_0-side_%d / strip id",sid);
      h_ladder[lid][sid]->GetXaxis()->SetTitle(name);
      snprintf(name,80,"ladder_%d-side_%d / strip id",lid+1,sid);
      h_ladder[lid][sid]->GetYaxis()->SetTitle(name);
    }
  }

  TChain *tree = gAnaCombine->GetTree();
  long nEvt = tree->GetEntries();
  for(long ievt=0;ievt<nEvt;++ievt){
    tree->GetEntry(ievt);
    short nCluster = gAnaCombine->fEvtAMS->GetEntriesFast();
    float Ref[2]={0.,0.}, RefSN[2]={0.,0.};
    for(int iclu=0;iclu<nCluster;++iclu){
      Cluster *acluster = (Cluster*)gAnaCombine->fEvtAMS->At(iclu);
      if(acluster->ladder != 0){
        continue;
      }
      short length = acluster->length;
      int side = acluster->side;
      //if(cluster_length_leftCut<=length && length<=cluster_length_rightCut && acluster->GetTotSig()>TotSignal_Cut && acluster->GetTotSig()>TotSignal_Cut && acluster->GetTotSN()>RefSN[side]){
      if(acluster->GetTotSN()>RefSN[side]){
        Ref[side] = acluster->GetSeedAdd();
        RefSN[side] = acluster->GetTotSN();
      }
    }
    /*
    if(Ref[0] == 0 || Ref[1] == 0){
      std::cout<<"DEBUG: "<<__FILE__<<"("<<__LINE__<<") event "<<ievt<<" s = "<<Ref[0]<<" k = "<<Ref[1]<<std::endl;
    }
    */

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
      if(ladder != 0){
        short length = acluster->length;
        if((cluster_length_leftCut<=length && length<=cluster_length_rightCut && acluster->GetTotSig()>TotSignal_Cut && acluster->GetTotSig()>TotSignal_Cut)){
          int side = acluster->side;
          if(side == 0){
            h_ladder[ladder-1][side]->Fill(Ref[side],acluster->GetSeedAdd(),acluster->GetSeedSN());
          }else{
            h_ladder[ladder-1][side]->Fill(Ref[side],acluster->GetSeedAdd(),acluster->GetSeedSN());
          }
        }
      }
    }
  }

  TCanvas *c1[4];
  for(short ild = 0;ild<4;++ild){
    snprintf(name,80,"AMS ladder_%d Alignment_2: cluster length %d~%d, total signal cut %f",ild+1,cluster_length_leftCut,cluster_length_rightCut,TotSignal_Cut);
    c1[ild]= new TCanvas(name,name);
    c1[ild]->Divide(2,2);
    c1[ild]->cd(1);
    h_ladder[ild][0]->Draw("colz");
    c1[ild]->cd(2);
    h_ladder[ild][1]->Draw("colz");
    c1[ild]->cd(3);
    h_ladder[ild][0]->ProfileX()->Draw();
    c1[ild]->cd(4);
    h_ladder[ild][1]->ProfileX()->Draw();
  }

}

}

