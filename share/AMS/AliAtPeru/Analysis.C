/*
 *   root -l
 *   gSystem->Load("$DMPSWWORK/lib/libDmpEvent.so");
 *   gSystem->CompileMacro("Analysis.C");
 *   LoadInputFile("fileName","SPS");
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
#include "TProfile.h"
#include "TH2F.h"
#include "TF1.h"
#include "TCanvas.h"

#include "include/Event.hh"
#include <iostream>
#include <vector>
//#include <fstream>


#define NLadder 5

using namespace std;

/*
  //  printf("%s\n", gSystem->GetMakeSharedLib());
  //gSystem->SetMakeSharedLib("cd $BuildDir ; clang++ -c $Opt -m64 -pipe -W -fsigned-char -fno-common -stdlib=libc++ -pthread $IncludePath $SourceFiles ; clang++ $ObjectFiles  -dynamiclib -single_module -Wl,-dead_strip_dylibs -O2 -m64 -mmacosx-version-min=10.9 -stdlib=libc++ $LinkedLibs -o $SharedLib");
  //  printf("%s\n", gSystem->GetMakeSharedLib());
  */


//-------------------------------------------------------------------
namespace Conf{

enum Location{
  PS = 0,
  SPS = 1,
};

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
          {-54.05*Pitch[0], -82.7 *Pitch[1]},
          {-52.17*Pitch[0], -80.39*Pitch[1]},
          {-46.15*Pitch[0], -40.33*Pitch[1]}};
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
void ClusterNumberInLadder(int r[NLadder][2]){
  int n_cls = Conf::AMS_Evt->Cls->GetEntriesFast();
  for(short ic=0;ic<n_cls;++ic){
    Cluster *aCluster = Conf::AMS_Evt->GetCluster(ic);
    ++r[LadderInOrder(aCluster->ladder)][aCluster->side];
  }
}

//-------------------------------------------------------------------
bool SingleGoodClusterInLadder0(){
  int clusBN[NLadder][2]={0,0,0,0,0,0,0,0,0,0};
  ClusterNumberInLadder(clusBN);
  if(clusBN[0][0] !=1 || clusBN[0][1] != 1){
    return false;
  }
  return true;
}

//-------------------------------------------------------------------
bool ClusterNumberLessThan2_forAllS_Side(){
  int clusBN[NLadder][2]={0,0,0,0,0,0,0,0,0,0};
  ClusterNumberInLadder(clusBN);
  for(short id=1;id<NLadder;++id){
    if(clusBN[id][0]>1){
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
    TH1F *h_clsSeed[NLadder][2] = {0};  // cluster seed
    TH1F *h_clsNB[NLadder][2] = {0};  // cluster numbers
    TH2F *h_COG_SNR[NLadder][2] = {0};  // COG, center of geometry. GetCoG(). SNR: signal inoise ratio of cluster. GetTotSN()
    TH2F *h_COG_Length[NLadder][2] = {0};  // length of this cluster: GetLength()
    for(short i =0;i<NLadder; ++i){
      for(short j =0;j<2;++j){
        h_clsSeed[i][j] = new TH1F(Form("L%d_S%d--cluster seed",i,j),Form("L%d_S%d cluster seed",i,j),1024,0,1024);
        h_clsSeed[i][j]->SetLabelSize(0.12);
        h_clsNB[i][j] = new TH1F(Form("L%d_S%d--cluster number",i,j),Form("L%d_S%d cluster number",i,j),8,0,8);
        h_clsNB[i][j]->SetLabelSize(0.12);
        //h_clsNB[i][j]->GetYaxis()->SetTitleSize(1.5);
        h_COG_SNR[i][j] = new TH2F(Form("L%d_S%d--COG VS SNR",i,j),Form("L%d_S%d CoG VS SNR",i,j),1024*2,j*640,640+j*384,300,0,60);
        h_COG_SNR[i][j]->SetLabelSize(0.12);
        h_COG_SNR[i][j]->SetLabelSize(0.08,"Y");
        h_COG_Length[i][j]=new TH2F(Form("L%d_S%d--COG VS Length",i,j),Form("L%d_S%d CoG VS Length",i,j),1024*2,j*640,640+j*384,30,0,30);
        h_COG_Length[i][j]->SetLabelSize(0.12);
        h_COG_Length[i][j]->SetLabelSize(0.08,"Y");
      }
    }

    // event loop
    for(Conf::evtID =0;Conf::evtID<Conf::entries;++Conf::evtID){
      LoadEvent();
      int clusBN[NLadder][2]={0,0,0,0,0,0,0,0,0,0};
      ClusterNumberInLadder(clusBN);
      for(short id=0;id<NLadder;++id){
        for(short s=0;s<2;++s){
          h_clsNB[id][s]->Fill(clusBN[id][s]);
        }
      }
      int n_cls = Conf::AMS_Evt->Cls->GetEntriesFast();
      for(short ic=0;ic<n_cls;++ic){
        Cluster *aCluster = Conf::AMS_Evt->GetCluster(ic);
        short order = LadderInOrder(aCluster->ladder);
        short side = aCluster->side;
        float CoG = aCluster->GetCoG();
        h_COG_SNR[order][side]->Fill(CoG,aCluster->GetTotSN());
        h_COG_Length[order][side]->Fill(CoG,aCluster->GetLength());
        h_clsSeed[order][side]->Fill(aCluster->GetSeedAdd());
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
    for(short id=0;id<NLadder;++id){
        c0->cd(id+1);
        h_clsSeed[id][0]->Draw();
        h_clsSeed[id][1]->Draw("same");
      for(short s=0;s<2;++s){
        c1->cd(id*2+s+1);
        h_clsNB[id][s]->Draw();
        c2->cd(id*2+s+1);
        h_COG_SNR[id][s]->Draw("colz");
        //h_COG_SNR[id][s]->ProfileX()->Draw("same");
        c3->cd(id*2+s+1);
        h_COG_Length[id][s]->Draw("colz");
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
    }
    for(short i =1;i<NLadder; ++i){
      for(short j =0;j<2;++j){
        h_align_CoG[i][j] = new TH1F(Form("L%d_S%d--Alignment",i,j),Form("L%d_S%d Alignment",i,j),200*6,-200,200);
        h_align_CoG[i][j]->SetLabelSize(0.12);
      }
    }

    for(Conf::evtID =0;Conf::evtID<Conf::entries;++Conf::evtID){
      LoadEvent();
      // one track event
      if(! SingleGoodClusterInLadder0()){   // both sides
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
  void Get_dx(vector<float> &dx){ // s side strips give x position
   /*
    *   use all clusters to fit a track, weighted by total signal of this cluster
    *
    *   and return the dx of all clusters
    */

    static long couts = -1;     ++couts;
    dx.clear();
    TH2F *track_xz = new TH2F(Form("Event%08d track_xz",couts),Form("Event%08d track_xz",couts),(Conf::ExHall*600+400),-10,Conf::ExHall*650+400,10000,0,10);
    track_xz->SetMarkerSize(5);
    int n_cls = Conf::AMS_Evt->Cls->GetEntriesFast();
    // linear fitting parameter
    vector<float>   xPosi;
    vector<float>   zPosi;
    for(short ic=0;ic<n_cls;++ic){
      Cluster *aCluster = Conf::AMS_Evt->GetCluster(ic);
      short side = aCluster->side;
      if(side != 0){
        continue;
      }
      if(aCluster->GetTotSN()< 3.8 || aCluster->GetTotSN()>14.0){
        continue;
      }
      xPosi.push_back(GetPosition(aCluster));
      zPosi.push_back(Conf::Position_Z[Conf::ExHall][LadderInOrder(aCluster->ladder)]);
      track_xz->Fill(zPosi[ic],xPosi[ic],aCluster->GetTotSig());
    }
    cout<<"\nevent "<< couts<<endl;
    for(short ic =0;ic<n_cls;++ic){
      cout<<xPosi[ic]<<"/"<<zPosi[ic]<<"\t";
    }
    cout<<endl;
    track_xz->Fit(Conf::linearFit,"0QF");
    //if(couts%10 ==0){
      TCanvas *cc = new TCanvas(Form("AMS Track x-z event %08d",couts),Form("AMS Track x-z Event %08d",couts));
      cc->cd();
      track_xz->Draw();
      Conf::linearFit->DrawCopy("lsame");
    //}

    /*
    for(short ic=0;ic<n_cls;++ic){
      Cluster *aCluster = Conf::AMS_Evt->GetCluster(ic);
      short side = aCluster->side;
      if(side != 0){
        continue;
      }
      float posi = GetPosition(aCluster);
      ++r[LadderInOrder(aCluster->ladder)][aCluster->side];
    }
    */
  }

  void Plots(){
    // DX VS s-side strips
    for(Conf::evtID =0;Conf::evtID<Conf::entries;++Conf::evtID){
      LoadEvent();
      // one track event
      if(! ClusterNumberLessThan2_forAllS_Side()){
        continue;
      }
      if(Conf::evtID>10) return;
      vector<float> dx;
      Get_dx(dx);
    }
    
    // DX VS k-side strips
  }
};



