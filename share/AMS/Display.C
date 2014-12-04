
namespace AMS{

//-------------------------------------------------------------------
void ShowThisEvent_0(long evtIndex){
  static bool hasPar=false;
  static double colPar[5][2]={0.};
  if(! hasPar){
    ifstream collimatePar("./AMS/AMSAlignment_1.txt",ios::in);
    if(collimatePar.good()){
      char tmp[100];
      collimatePar.getline(tmp,100);
    }else{
      std::cout<<"DEBUG: "<<__FILE__<<"("<<__LINE__<<") false"<<std::endl;
      return;
    }
    double temp;
    for(short i=0;i<5;++i){
      collimatePar>>temp>>colPar[i][0]>>temp>>colPar[i][1]>>temp;
      cout<<"m0 = "<<colPar[i][0]<<" m1 = "<<colPar[i][1]<<endl;
    }
    hasPar = true;
  }
  char name[50];
  TH2F *h_evt_side[2];
  snprintf(name,50,"AMS event %d s_side",evtIndex);
  h_evt_side[0] = new TH2F(name,name,14,-1,6,700,0,700);
  h_evt_side[0]->GetXaxis()->SetTitle("ladder ID");
  h_evt_side[0]->GetYaxis()->SetTitle("strip ID / pitch(110 um)");
  snprintf(name,50,"AMS event %d k_side",evtIndex);
  h_evt_side[1] = new TH2F(name,name,14,-1,6,500,600,1100);
  h_evt_side[1]->GetXaxis()->SetTitle("ladder ID");
  h_evt_side[1]->GetYaxis()->SetTitle("strip ID / pitch(208 um)");

  gAnaCombine->GetTree()->GetEntry(evtIndex);

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
    h_evt_side[side]->Fill(ladder,acluster->GetSeedAdd()-colPar[ladder][side],acluster->GetTotSig());
  }
  TCanvas *c1 = new TCanvas(name,name);
  c1->Divide(2,1);
  c1->cd(1);
  h_evt_side[0]->Draw("colz");
  c1->cd(2);
  h_evt_side[1]->Draw("colz");
}

//-------------------------------------------------------------------
void ShowThisEvent_1(long evtIndex){
  static bool hasPar=false;
  static double colPar[4][4]={0.};
  if(! hasPar){
    ifstream collimatePar("./AMS/AMSAlignment_2.txt",ios::in);
    if(collimatePar.good()){
      char tmp[100];
      collimatePar.getline(tmp,100);
      std::cout<<"DEBUG: "<<__FILE__<<"("<<__LINE__<<") opend "<<tmp<<std::endl;
    }else{
      std::cout<<"DEBUG: "<<__FILE__<<"("<<__LINE__<<") false"<<std::endl;
      return;
    }
    double temp;
    for(short i=0;i<4;++i){
      collimatePar>>temp>>colPar[i][0]>>colPar[i][1]>>colPar[2]>>colPar[3];
      cout<<"ladder "<<i+1<<"\ts side: p0 = "<<colPar[i][0]<<" p1 = "<<colPar[i][1]<<"\t k side: p0 = "<<colPar[i][2]<<" p1 = "<<colPar[i][3]<<endl;
    }
    hasPar = true;
  }
  char name[50];
  TH2F *h_evt_side[2] ;
  snprintf(name,50,"AMS event %d s_side",evtIndex);
  h_evt_side[0] = new TH2F(name,name,14,-1,6,700,0,700);
  snprintf(name,50,"AMS event %d k_side",evtIndex);
  h_evt_side[1] = new TH2F(name,name,14,-1,6,500,600,1100);

  gAnaCombine->GetTree()->GetEntry(evtIndex);

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
    h_evt_side[side]->Fill(ladder,acluster->GetSeedAdd(),acluster->GetTotSig());
  }
  TCanvas *c1 = new TCanvas(name,name);
  c1->Divide(2,1);
  c1->cd(1);
  h_evt_side[0]->Draw("colz");
  c1->cd(2);
  h_evt_side[1]->Draw("colz");
}

//-------------------------------------------------------------------
void ShowEvent_0(long id0,long id1){
  for(long i = id0;i<=id1;++i){
    ShowThisEvent_0(i);
  }
}

/*
void ShowEvent(){
  TChain *tree = gAnaCombine->GetTree();
  long nEvt = tree->GetEntries();
  for(long ievt=0,ievt<nEvt;++ievt){
    tree->GetEntry(ievt);
  }
}
*/

}

