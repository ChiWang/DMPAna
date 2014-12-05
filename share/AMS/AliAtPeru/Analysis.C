/*
 * refer to Readme.md
 *
 * // unit for length: cm 
 * // strip 0,0 as reference.
 *
 */

#include "TFile.h"
#include "TTree.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TProfile.h"
#include "TH2F.h"
#include "TF1.h"
#include "TCanvas.h"

#include "include/Event.hh"
#include <iostream>
#include <vector>
#include <map>
//#include <fstream>


#define NLadder 5

using namespace std;

/*
  //  printf("%s\n", gSystem->GetMakeSharedLib());
  //gSystem->SetMakeSharedLib("cd $BuildDir ; clang++ -c $Opt -m64 -pipe -W -fsigned-char -fno-common -stdlib=libc++ -pthread $IncludePath $SourceFiles ; clang++ $ObjectFiles  -dynamiclib -single_module -Wl,-dead_strip_dylibs -O2 -m64 -mmacosx-version-min=10.9 -stdlib=libc++ $LinkedLibs -o $SharedLib");
  //  printf("%s\n", gSystem->GetMakeSharedLib());
  */

// forward declaration
namespace Tracking{
  void Initi();
};
// end of forward declaration


//-------------------------------------------------------------------
namespace Conf{

enum Location{
  PS = 0,
  SPS = 1,
};

namespace EnableCut{
  bool TotSN_Cut = true;
  bool Length_Cut = false;
  bool Cut_2 = false;
  bool Cut_3 = false;
  bool Cut_4 = false;
  bool Cut_5 = false;
  bool Cut_6 = false;
};

  void SetCutStatus(int st){
// *
// *  TODO:  how to set cut as gSytle->SetOptxx(011011011)?
// *
    int defaultCut=1000000;  // order: refer to namespace EnableCut, from up to down
    st += 0;
  }

  TString Path = "./Data/";
  TString treeName ="t4";
  TString branchName ="cluster_branch";
  short ExHall = SPS;// or PS
// *
// *  TODO:   which side near upstream is important
// *
  /*
   *  at PS, wich side (s or k) near upstream??
   *  at SPS, k side near upstream
   *
   */
  TString  File = "NO";     // file includes 5 ladders data
  long     entries = -1;
  long     evtID = -1;
  TTree    *tree = 0;
  Event    *AMS_Evt = 0;
  float     Distance[2][4] = {{119.5,   112,    45,     95},        // PS
                              {113.3,   761.1,  39.0,   96.5}};     // SPS.     unit cm. beam --->
  float     Pitch[2] = {0.011,0.0208};    // unit cm. first is s side, then k side
  float     kGap = 0.1392;      // 676*2 + 40 (um);  40: sensor distance,  676: active area to edge of sensor
  short     SensorNumber[NLadder] = {4,4,4,12,12};  // same for PS and SPS. from upstream to downstream

  // alignment parameter
// *
// *  TODO:   offset for PS and SPS
// *
  float     Offset_CoG[NLadder][2] = {
          {322.8,   815.7},     // ladder 0 as reference. CoG of verticle track
          {-4.502,  -9.778},    // others are offset
          {-54.05,  -82.7},
          {-52.17,  -80.39},
          {-46.15,  -40.33}};
  float     Offset[NLadder][2] = {
          {0.0,             0.},     // ladder 0 as reference. CoG of verticle track
          {-4.502*Pitch[0], -9.778*Pitch[1]},    // others are offset
          {(-54.05+4)*Pitch[0], -82.7 *Pitch[1]},   // TODO, offset
          {(-52.17+4)*Pitch[0], -80.39*Pitch[1]},
          {(-46.15+4)*Pitch[0], -40.33*Pitch[1]}};
  float  Position_Z[2][NLadder]={  // update it while load input
          {0,10,20,30,40},        // PS
          {0,10,20,30,40}};     // SPS.     unit cm. beam --->

  TF1 *gausFit = new TF1("GF","gaus",0,150);
  TF1 *linearFit = new TF1("LF","pol1",-10,1200);

  void LoadAlignmentParameter(TString aFile){
    cout<<"loading new alignment parameter. from file: "<<aFile<<endl;
    //ifstream input();
    // TODO: update Offset_CoG[][] and Offset[][];
  }

  void LoadInput(TString fName){
    gStyle->SetOptStat(11111111);
    gStyle->SetOptFit(111111111);
    static TFile *f=0;
    File = fName;
    if(f){
      f->Close();
      delete f;
      f=0;
    }
    f = new TFile(File,"READ");
    tree = (TTree*)(f->Get(treeName));
    entries = tree->GetEntries();
    if(AMS_Evt){
      delete AMS_Evt;
      AMS_Evt = 0;
    }
    AMS_Evt = new Event();
    tree->SetBranchAddress(branchName,&AMS_Evt);

    LoadAlignmentParameter("TODOME");

    // load z position
    if(Position_Z[PS][1] == 10){
      for(short i=0;i<NLadder;++i){
         Position_Z[PS][i] =0;
         Position_Z[SPS][i] =0;
         for(short iD=0;iD<i;++iD){
           Position_Z[PS][i] += Distance[PS][iD];
           Position_Z[SPS][i] += Distance[SPS][iD];
         }
      }
    }
    Tracking::Initi();
  }

};

//-------------------------------------------------------------------
void LoadEvent(){
  Conf::tree->GetEntry(Conf::evtID);
}

//-------------------------------------------------------------------
short LadderInOrder(int ladderID){
  short n=0;
  if(Conf::ExHall == Conf::PS){
    // beam (--->):  ladder 0, 1, 5, 4, 3
    if(ladderID <=1){
      n = ladderID;
    }else{
      n = 7-ladderID;
      if(n != 2 || n != 3 || n!=4){
        cout<<"ERROR: ladder "<<ladderID<<" not exist at "<<Conf::ExHall<<endl;
      }
    }
  }else if(Conf::ExHall == Conf::SPS){
    // beam (--->): ladder 12, 13, 0, 1, 2
    if(ladderID <=2){
      n = 2+ladderID;
    }else{
      n = ladderID-12;
      if(n != 0 && n !=1){
        cout<<"ERROR: ladder "<<ladderID<<" not exist at "<<Conf::ExHall<<endl;
      }
    }
  }
  return n;
}

//-------------------------------------------------------------------
bool GoodClusterCheck(Cluster *aC){
  if(Conf::EnableCut::TotSN_Cut){
    if(aC->GetTotSN()< 3.8 || aC->GetTotSN()>14.0){
      return false;
    }
  }
  if(Conf::EnableCut::Length_Cut){
// *
// *  TODO:  check cut value
// *
    if(aC->GetLength()< 1 || aC->GetTotSN()>10){
      return false;
    }
  }
  return true;
}

//-------------------------------------------------------------------
void ClusterNumberInLadder(vector<int> &r, bool choosingGoodCluster=true){
  r.resize(NLadder*2,0);
  int n_cls = Conf::AMS_Evt->Cls->GetEntriesFast();
  for(short ic=0;ic<n_cls;++ic){
    Cluster *aCluster = Conf::AMS_Evt->GetCluster(ic);
    if(choosingGoodCluster){
      if(! GoodClusterCheck(aCluster)){
        continue;
      }
    }
    ++r[LadderInOrder(aCluster->ladder)*2 + aCluster->side];
  }
}

//-------------------------------------------------------------------
bool N_ClustersInLadder_I(int N, short I, bool OnlyGoodCluster=true){
  // both sides has n clusters
  vector<int> clusBN;
  ClusterNumberInLadder(clusBN,OnlyGoodCluster);
  if(clusBN[I*2+0] !=N || clusBN[I*2+1] != N){
    return false;
  }
  return true;
}

//-------------------------------------------------------------------
bool ClusterNumberLessThan2_forAllS_Side(){
  vector<int> clusBN;
  ClusterNumberInLadder(clusBN);
  for(short id=1;id<NLadder;++id){
    if(clusBN[id*2+0]>1){
      return false;
    }
  }
  return true;
}

//-------------------------------------------------------------------
float GetPosition(Cluster *aCluster){
        /*
         *  if side = 1, just give the nearest postion to (0,0)
         *
         */
  short side = aCluster->side;
  float CoG = aCluster->GetCoG();
  float v = (side==1?CoG-640:CoG)*Conf::Pitch[side];
  return v+(Conf::Offset[LadderInOrder(aCluster->ladder)][side]);
}

//-------------------------------------------------------------------
namespace CombineSPS{
/*
 *  only save data of clusters in Event.hh
 *
 */

TString file_s0 = "./test0.root";
TString file_s1 = "./test1.root";

TTree *tree_s0=0;
TTree *tree_s1=0;

Event *event_s0 = 0;
Event *event_s1 = 0;

namespace Error{
enum Type{
  RunID_NotMatch = 0,
  Entries_NotMatch = 1,
  NotSPS = 2,
};
};

bool MyException(short type){
  switch (type) {
    case Error::RunID_NotMatch:
      cout<<"ERROR:  run ID not match: "<<file_s0<<"\t"<<file_s1<<endl;
      break;
    case Error::Entries_NotMatch:
      cout<<"ERROR:  entries not match: "<<file_s0<<" "<<tree_s0->GetEntries()<<"\t"<<file_s1<<" "<<tree_s1->GetEntries()<<endl;
      break;
    case Error::NotSPS:
      cout<<"ERROR:  Conf::ExHall is not SPS..."<<endl;
      break;
  }
  return false;
}

bool LoadInput(){
  if(Conf::ExHall != Conf::SPS){
    return MyException(Error::NotSPS);
  }
  static TFile *f_s0 = 0, *f_s1 = 0;
  TString tf0 = file_s0,tf1 = file_s1;
  if(tf0.Remove(0,tf0.Length()-13) != tf1.Remove(0,tf1.Length()-13)){
    return MyException(Error::RunID_NotMatch);
  }
  if(f_s0 != 0){
    f_s0->Close();
    delete f_s0;
    f_s0 = 0;
  }
  f_s0 = new TFile(Conf::Path+"/Root_Data_side0/"+file_s0,"READ");
  tree_s0 = (TTree*)(f_s0->Get(Conf::treeName));
  Conf::entries = tree_s0->GetEntries();
  if(f_s1 != 0){
    f_s1->Close();
    delete f_s1;
    f_s1 = 0;
  }
  f_s1 = new TFile(Conf::Path+"/Root_Data_side1/"+file_s1,"READ");
  tree_s1 = (TTree*)(f_s1->Get(Conf::treeName));
  if(tree_s1->GetEntries() != Conf::entries){
    return MyException(Error::Entries_NotMatch);
  }

  event_s0 = new Event();
  tree_s0->SetBranchAddress(Conf::branchName,&event_s0);
  event_s1 = new Event();
  tree_s1->SetBranchAddress(Conf::branchName,&event_s1);
  return true;
}

//-------------------------------------------------------------------
void CombineThem(TString f0="run_1416155587_ANC_387.root",TString f1="run_1416155589_ANC_387.root"){
  file_s0 = f0;
  file_s1 = f1;
  if(LoadInput() == false){
    return;
  }

  Event *event_01 = new Event();
  TString name = "Combine_"+file_s1;
  TFile *f_01 = new TFile(Conf::Path+"/Root_Data/"+name,"RECREATE");
  TTree *tree_01 = new TTree(Conf::treeName,"Cluster tree");
  tree_01->Branch(Conf::branchName,"Event",&event_01,32000,2);

  // event loop, combine side 0 and side 1
  for(Conf::evtID = 0;Conf::evtID<Conf::entries;++Conf::evtID){
    tree_s0->GetEntry(Conf::evtID);
    int nC = event_s0->Cls->GetEntriesFast();
    for(int ic=0;ic<nC;++ic){
      const Cluster* cl = event_s0->GetCluster(ic);
      event_01->AddCluster(cl);
    }
    tree_s1->GetEntry(Conf::evtID);
    int nC1 = event_s1->Cls->GetEntriesFast();
    for(int ic=0;ic<nC1;++ic){
      const Cluster* cl = event_s1->GetCluster(ic);
      event_01->AddCluster(cl);
    }
    tree_01->Fill();
    event_01->Clear();
  }

  tree_01->Write("",TObject::kOverwrite);
  f_01->Close();
  delete f_01;
  delete event_01;
  delete event_s0;
  delete event_s1;

}

};

//-------------------------------------------------------------------
namespace Performance{ // without any cuts
  void Clusters(){
    vector<TH1F*>   h_clsSeed(NLadder*2);  // cluster seed
    vector<TH1F*>   h_clsNB(NLadder*2);  // cluster numbers
    vector<TH2F*>   h_COG_SNR(NLadder*2);  // COG, center of geometry. GetCoG(). SNR: signal inoise ratio of cluster. GetTotSN()
    vector<TH2F*>   h_COG_Length(NLadder*2);  // length of this cluster: GetLength()
    for(short i =0;i<NLadder; ++i){
      for(short j =0;j<2;++j){
        h_clsSeed[i*2+j] = new TH1F(Form("L%d_S%d--cluster seed",i,j),Form("L%d_S%d cluster seed",i,j),1024,0,1024);
        h_clsSeed[i*2+j]->SetLabelSize(0.12);
        h_clsSeed[i*2+j]->SetLabelSize(0.08,"Y");
        h_clsSeed[i*2+j]->SetLineColor(j+3);
        h_clsNB[i*2+j] = new TH1F(Form("L%d_S%d--cluster number",i,j),Form("L%d_S%d cluster number",i,j),8,0,8);
        h_clsNB[i*2+j]->SetLabelSize(0.12);
        h_clsNB[i*2+j]->SetLabelSize(0.08,"Y");
        //h_clsNB[i][j]->GetYaxis()->SetTitleSize(1.5);
        h_COG_SNR[i*2+j] = new TH2F(Form("L%d_S%d--COG VS SNR",i,j),Form("L%d_S%d CoG VS SNR",i,j),1024*2,j*640,640+j*384,200,0,40);
        h_COG_SNR[i*2+j]->SetLabelSize(0.12);
        h_COG_SNR[i*2+j]->SetLabelSize(0.08,"Y");
        h_COG_Length[i*2+j]=new TH2F(Form("L%d_S%d--COG VS Length",i,j),Form("L%d_S%d CoG VS Length",i,j),1024*2,j*640,640+j*384,30,0,30);
        h_COG_Length[i*2+j]->SetLabelSize(0.12);
        h_COG_Length[i*2+j]->SetLabelSize(0.08,"Y");
      }
    }

    // event loop
    for(Conf::evtID =0;Conf::evtID<Conf::entries;++Conf::evtID){
      LoadEvent();
      vector<int> clusBN;
      ClusterNumberInLadder(clusBN);
      for(short id=0;id<NLadder;++id){
        for(short s=0;s<2;++s){
          h_clsNB[id*2+s]->Fill(clusBN[id*2+s]);
        }
      }
      int n_cls = Conf::AMS_Evt->Cls->GetEntriesFast();
      for(short ic=0;ic<n_cls;++ic){
        Cluster *aCluster = Conf::AMS_Evt->GetCluster(ic);
        short order = LadderInOrder(aCluster->ladder);
        short side = aCluster->side;
        float CoG = aCluster->GetCoG();
        h_COG_SNR[order*2+side]->Fill(CoG,aCluster->GetTotSN());
        h_COG_Length[order*2+side]->Fill(CoG,aCluster->GetLength());
        h_clsSeed[order*2+side]->Fill(aCluster->GetSeedAdd());
      }
    }

    // draw
    TCanvas *c0 = new TCanvas(Conf::File+"  Cluster seed",Conf::File+"  Cluster seed");
    c0->Divide(1,5,0,0);
    TCanvas *c1 = new TCanvas(Conf::File+"  Cluster number",Conf::File+"  Clusters number");
    c1->Divide(2,5,0,0);
    TCanvas *c2 = new TCanvas(Conf::File+"  Cluster CoG_SNR",Conf::File+"  Cluster CoG_SNR");
    c2->Divide(2,5,0,0);
    TCanvas *c3 = new TCanvas(Conf::File+"  Cluster CoG_Length",Conf::File+"  Cluster CoG_Length");
    c3->Divide(2,5,0,0);
    TCanvas *c4 = new TCanvas(Conf::File+"  Cluster SNR",Conf::File+"  Cluster SNR");
    c4->Divide(2,5,0,0);
    TCanvas *c5 = new TCanvas(Conf::File+"  Cluster Length",Conf::File+"  Cluster Length");
    c5->Divide(2,5,0,0);
    for(short id=0;id<NLadder;++id){
        c0->cd(id+1);
        h_clsSeed[id*2+0]->Draw();
        h_clsSeed[id*2+1]->Draw("same");
      for(short s=0;s<2;++s){
        c1->cd(id*2+s+1);
        h_clsNB[id*2+s]->Draw();
        c2->cd(id*2+s+1);
        h_COG_SNR[id*2+s]->Draw("colz");
        //h_COG_SNR[id][s]->ProfileX()->Draw("same");
        c3->cd(id*2+s+1);
        h_COG_Length[id*2+s]->Draw("colz");
        c4->cd(id*2+s+1);
        TH1D *h_SNR = h_COG_SNR[id*2+s]->ProjectionY();
        h_SNR->SetTitle(Form("L%d_S%d Cluster SNR",id,s));
        h_SNR->SetLabelSize(0.12);
        h_SNR->SetLabelSize(0.08,"Y");
        h_SNR->Draw();
        c5->cd(id*2+s+1);
        TH1D *h_Length = h_COG_Length[id*2+s]->ProjectionY();
        h_Length->SetTitle(Form("L%d_S%d Cluster Length",id,s));
        h_Length->SetLabelSize(0.12);
        h_Length->SetLabelSize(0.08,"Y");
        h_Length->Draw();
      }
    }
  }

};

//-------------------------------------------------------------------
namespace Alignment{
// *
// *  TODO: ios has not be declared
// *
  TString outFilename = Conf::Path+"Root_Data/"+Conf::File+"_align.txt";
  //ofstream output(outFilename,ios::out | iso::app);
  //output<<"ladder id\tmean\terror";

  void SingleStrack_S_Side(bool reload=true){    // for each ladder
    gStyle->SetOptStat(1111);
    gStyle->SetOptFit(100111);

    TH1F *h_align_CoG[NLadder][2] = {0};  // cluster numbers
    for(short j =0;j<2;++j){
      h_align_CoG[0][j] = new TH1F(Form("L0_S%d--CoG",j),Form("L0_S%d CoG",j),1024*2,j*640,640+j*384);
      h_align_CoG[0][j]->SetLabelSize(0.12);
      h_align_CoG[0][j]->SetLabelSize(0.08,"Y");
    }
    for(short i =1;i<NLadder; ++i){
      for(short j =0;j<2;++j){
        h_align_CoG[i][j] = new TH1F(Form("L%d_S%d--Alignment",i,j),Form("L%d_S%d Alignment",i,j),200*6,-200,200);
        h_align_CoG[i][j]->SetLabelSize(0.12);
        h_align_CoG[i][j]->SetLabelSize(0.08,"Y");
      }
    }

    for(Conf::evtID =0;Conf::evtID<Conf::entries;++Conf::evtID){
      LoadEvent();
      // one track event
      if(! N_ClustersInLadder_I(1,0)){   // both sides
        continue;
      }
      if(! ClusterNumberLessThan2_forAllS_Side()){
        continue;
      }
      // update reference
      float CoG_Ref_ladder0[2]={0.0,0.};    // ladder 0, side 0, 1
      int n_cls = Conf::AMS_Evt->Cls->GetEntriesFast();
      for(short ic=0;ic<n_cls;++ic){
        Cluster *aCluster = Conf::AMS_Evt->GetCluster(ic);
        short order = LadderInOrder(aCluster->ladder);
        if(order != 0){
          continue;
        }
        short side = aCluster->side;
        CoG_Ref_ladder0[side] = aCluster->GetCoG();;
        h_align_CoG[0][side]->Fill(CoG_Ref_ladder0[side]);
      }
      // alignment
      for(short ic=0;ic<n_cls;++ic){
        Cluster *aCluster = Conf::AMS_Evt->GetCluster(ic);
        short order = LadderInOrder(aCluster->ladder);
        if(order == 0){
          continue;
        }
        short side = aCluster->side;
        h_align_CoG[order][side]->Fill(aCluster->GetCoG()-CoG_Ref_ladder0[side]);
      }
    }

    TCanvas *c1 = new TCanvas(Conf::File+"  Alignment",Conf::File+"  Alignment");
    c1->Divide(2,5,0.002,0.002);
    for(short id=0;id<NLadder;++id){
      for(short s=0;s<2;++s){
        c1->cd(id*2+s+1);
        h_align_CoG[id][s]->Draw();
        float mean = h_align_CoG[id][s]->GetMean(), rms = h_align_CoG[id][s]->GetRMS();
        Conf::gausFit->SetRange(mean-2*rms,mean+2*rms);
        h_align_CoG[id][s]->Fit(Conf::gausFit,"R0Q");
        Conf::gausFit->DrawCopy("lsame");
        //output<<
      }
    }
    if(reload) Conf::LoadAlignmentParameter(outFilename); 
  }
}

//-------------------------------------------------------------------
namespace Tracking{
        /*
  void Get_dx(vector<float> &dx){ // s side strips give x position
    // *   use all clusters to fit a track, weighted by total signal of this cluster
    // *
    // *   and return the dx of all clusters
    // *

    gStyle->SetOptStat(00000000);
    gStyle->SetOptFit(000000000);

    dx.clear();
    int n_cls = Conf::AMS_Evt->Cls->GetEntriesFast();
    // linear fitting parameter
    vector<float>   xPosi;
    vector<float>   zPosi;
    vector<float>   totSig;
    //int n_cls = Conf::AMS_Evt->Cls->GetEntriesFast();
    for(short ic=0;ic<n_cls;++ic){
      Cluster *aCluster = Conf::AMS_Evt->GetCluster(ic);
      if(aCluster->side != 0){
        continue;
      }
      if(aCluster->GetTotSN()< 3.8 || aCluster->GetTotSN()>14.0){
        continue;
      }
      xPosi.push_back(GetPosition(aCluster));
      zPosi.push_back(Conf::Position_Z[Conf::ExHall][LadderInOrder(aCluster->ladder)]);
      totSig.push_back(aCluster->GetTotSig());
    }
    if(zPosi.size() < 3){
      return;
    }
    static TCanvas *cc = new TCanvas("AMS Track","AMS Track");
    static long couts=-1; ++couts;
    TH2F *track[2] = {
            new TH2F(Form("Event%08d track_xz",Conf::evtID),Form("Event%08d track_xz",Conf::evtID),(Conf::ExHall*65+40),-10,Conf::ExHall*650+400,500,0,10), new TH2F(Form("Event%08d track_yz",Conf::evtID),Form("Event%08d track_yz",Conf::evtID),(Conf::ExHall*65+40),-10,Conf::ExHall*650+400,500,0,10)};
    if(couts == 0){
      cc->Divide(2,1);
      track[0]->SetTitle("Track X-Z");  track[0]->SetXTitle("Z / cm"); track[0]->SetYTitle("X / cm");
      track[1]->SetTitle("Track Y-Z");  track[1]->SetXTitle("Z / cm"); track[1]->SetYTitle("Y / cm");
    }
    //cout<<"\nevent "<< Conf::evtID<<endl;
    for(short ic =0;ic<zPosi.size();++ic){
      //cout<<xPosi[ic]<<"/"<<zPosi[ic]<<"\t";
      track[0]->Fill(zPosi[ic],xPosi[ic],totSig[ic]);
    }
    //cout<<endl;
      cc->cd(1);
      track[0]->Fit(Conf::linearFit,"0Q");//QF
    if(couts%100 ==0){
      track[0]->SetMarkerSize(8);
      track[0]->Draw("same");
      Conf::linearFit->DrawCopy("same");
    }

  }
  */

  TCanvas *c_track = 0;
  long trackID = 0;

//-------------------------------------------------------------------
  void Initi(){
    c_track = new TCanvas("AMS Track","AMS Track");
    c_track->Divide(2,1);
  }

//-------------------------------------------------------------------
  void FitTrack_XZ(vector<Cluster*> &xClus,double &p0, double &p1, vector<float> &xPosi,vector<float> &zPosi){
    gStyle->SetOptStat(00000000);
    gStyle->SetOptFit(000000000);
  //void FitTrack_XZ(vector<Cluster*> &xClus,double &p0, double &p1, vector<float> &xPosi,vector<float> &zPosi, vector<float> &totSig = vector<float>(0)){
       /*
        * input is all x Clusters(s side),
        *
        *  output are:
        *   linear fit parameter
        *   and, clusters' position and tot Sig,
        *
        */
    vector<float> totSig;
    p0 = -999; p1 = -999;
    xPosi.clear();
    zPosi.clear();
    totSig.clear();
    TH2F *track_xz = new TH2F(Form("Event%08d track_xz",Conf::evtID),Form("Event%08d track_xz",Conf::evtID),(Conf::ExHall*65+40),-10,Conf::ExHall*650+400,500,0,10);
    if(trackID == 0){
      track_xz->SetTitle("Track X-Z");
      track_xz->SetXTitle("Z / cm");
      track_xz->SetYTitle("X / cm");
    }
    track_xz->SetMarkerSize(4);
    for(short i=0;i<xClus.size();++i){
      xPosi.push_back(GetPosition(xClus[i]));
      zPosi.push_back(Conf::Position_Z[Conf::ExHall][LadderInOrder(xClus[i]->ladder)]);
      totSig.push_back(xClus[i]->GetTotSig());
      track_xz->Fill(zPosi[i],xPosi[i],totSig[i]);
    }
    track_xz->Fit(Conf::linearFit,"0Q");//QF
    if(trackID%100 ==0){
      c_track->cd(1);
      track_xz->Draw("same");
      Conf::linearFit->DrawCopy("same");
    }
    p0 = Conf::linearFit->GetParameter(0);
    p1 = Conf::linearFit->GetParameter(1);
  }

//-------------------------------------------------------------------
  void FitTrack_YZ(vector<Cluster*> &yClus,double &p0, double &p1, vector<float> &yPosi,vector<float> &zPosi){
    gStyle->SetOptStat(00000000);
    gStyle->SetOptFit(000000000);
  //void FitTrack_YZ(vector<Cluster*> &yClus,double &p0, double &p1, vector<float> &yPosi,vector<float> &zPosi, vector<float> &totSig = vector<float>(0)){
       /*
        * input is all y Clusters(k side),
        *
        *  output are:
        *   linear fit parameter
        *   and, clusters' position and tot Sig,
        *
        */
    vector<float> totSig;
    p0 = -999; p1 = -999;
    yPosi.clear();
    zPosi.clear();
    totSig.clear();
    TH2F *track_yz = new TH2F(Form("Event%08d track_yz",Conf::evtID),Form("Event%08d track_yz",Conf::evtID),(Conf::ExHall*65+40),-10,Conf::ExHall*650+400,500,0,50);
    if(trackID == 0){
      track_yz->SetTitle("Track Y-Z");
      track_yz->SetXTitle("Z / cm");
      track_yz->SetYTitle("Y / cm");
    }
    track_yz->SetMarkerSize(4);
    for(short i=0;i<yClus.size();++i){
      yPosi.push_back(GetPosition(yClus[i])); // back GetPos
      zPosi.push_back(Conf::Position_Z[Conf::ExHall][LadderInOrder(yClus[i]->ladder)]);
      totSig.push_back(yClus[i]->GetTotSig());
      track_yz->Fill(zPosi[i],yPosi[i],totSig[i]);
    }
    track_yz->Fit(Conf::linearFit,"0Q");//QF
    if(trackID%100 ==0){
      c_track->cd(2);
      track_yz->Draw("same");
      Conf::linearFit->DrawCopy("same");
    }
    p0 = Conf::linearFit->GetParameter(0);
    p1 = Conf::linearFit->GetParameter(1);
  }

//-------------------------------------------------------------------
  void Plots(long maxevt=999999999){
    // DX VS s-side strips
    for(Conf::evtID =0;(Conf::evtID<Conf::entries && Conf::evtID<maxevt);++Conf::evtID){
      LoadEvent();
      // one track event
      if(! N_ClustersInLadder_I(1,0)){
        continue;
      }
      if(! ClusterNumberLessThan2_forAllS_Side()){
        continue;
      }

      vector<Cluster*>  goodClusters[2];    // s-side , k-side
      int n_cls = Conf::AMS_Evt->Cls->GetEntriesFast();
      for(short ic=0;ic<n_cls;++ic){
        Cluster *aCluster = Conf::AMS_Evt->GetCluster(ic);
        if( ! GoodClusterCheck(aCluster)){
          continue;
        }
        goodClusters[aCluster->side].push_back(aCluster);
      }
      if(goodClusters[0].size() <2 || goodClusters[1].size()<2){
        continue;
      }

      vector<float>  xz_Pos[2];    // (0,1): (x,z)
      double xz_p0,  xz_p1;
      FitTrack_XZ(goodClusters[0],xz_p0,xz_p1,xz_Pos[0],xz_Pos[1]);

      vector<float>  yz_Pos[2];    // (0,1): (y,z)
      double yz_p0,  yz_p1;
      FitTrack_YZ(goodClusters[1],yz_p0,yz_p1,yz_Pos[0],yz_Pos[1]);
      ++trackID;

    }
    
    // DX VS k-side strips
  }
};



