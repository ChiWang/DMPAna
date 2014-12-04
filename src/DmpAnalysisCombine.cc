/*
 *  $Id: DmpAnalysisCombine.cc, 2014-11-04 01:58:08 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 04/11/2014
*/

#include <map>
#include "TChain.h"
#include "TMultiGraph.h"
#include "TGraph.h"
#include "TLegend.h"

#include "DmpAnalysisCombine.h"
#include "DmpTimeConvertor.h"
#include "DmpLog.h"
#include "DmpRootIOSvc.h"
#include "DmpCore.h"
#include "DmpEvtBgoHits.h"
#include "DmpBgoBase.h"
#include "TCanvas.h"
#include "TH2D.h"

DmpAnalysisCombine::DmpAnalysisCombine():T0(0),fBeamE(200000),fTreeName("CollectionTree"),fEvtHeader(0),fEvtAMS(0),fEvtPsd(0),fEvtStk(0),fEvtBgo(0),fEvtAnc(0){
  SetTimeCut(gCore->GetStartTime(),gCore->GetStopTime());
  std::cout<<"DEBUG: "<<__FILE__<<"("<<__LINE__<<")"<<std::endl;
  fEvtHeader = new DmpEvtHeader();
  fEvtAMS = new TClonesArray("Cluster",2000);
  fEvtPsd = new DmpEvtBgoHits();
  fEvtStk = new TClonesArray("DmpStkSiCluster",2000);
  fEvtBgo = new DmpEvtBgoHits();
  fEvtAnc = new AncillaryEvent();
}

//-------------------------------------------------------------------
DmpAnalysisCombine::~DmpAnalysisCombine(){
  for(std::map<std::string,TChain*>::iterator it=fChainSet.begin();it!=fChainSet.end();++it){
    delete it->second;
  }
  fChainSet.clear();
  delete fEvtHeader;
  delete fEvtAMS;
  delete fEvtPsd;
  delete fEvtStk;
  delete fEvtBgo;
  delete fEvtAnc;
}

//-------------------------------------------------------------------
void DmpAnalysisCombine::LoadInputFile(const std::string &f,const int &BeamEnergy,const std::string &name){
  fBeamE =  BeamEnergy;
  fTreeName = name;
  gRootIOSvc->InputFile(f);
  for(std::map<std::string, TChain*>::iterator it=fChainSet.begin();it!=fChainSet.end();++it){
    delete it->second;
  }
  fChainSet.clear();
  this->ActiveTree(true);
}

//-------------------------------------------------------------------
#include <boost/lexical_cast.hpp>
void DmpAnalysisCombine::SetTimeCut(const std::string &start,const std::string &stop){
  gCore->StartTime(start);
  std::string t0="EventHeader.fSecond > "+boost::lexical_cast<std::string>(DmpTimeConvertor::Date2Second(start));
  fTimeCut0 = t0.c_str();

  gCore->StopTime(stop);
  std::string t1="EventHeader.fSecond < "+boost::lexical_cast<std::string>(DmpTimeConvertor::Date2Second(stop));
  fTimeCut1 = t1.c_str();
}

//-------------------------------------------------------------------
void DmpAnalysisCombine::SetTimeCut(const int &start,const int &stop){
  gCore->StartTime(DmpTimeConvertor::Second2Date(start));
  T0 = start;
  std::string t0="EventHeader.fSecond > "+boost::lexical_cast<std::string>(start);
  fTimeCut0 = t0.c_str();

  gCore->StopTime(DmpTimeConvertor::Second2Date(stop));
  std::string t1="EventHeader.fSecond < "+boost::lexical_cast<std::string>(stop);
  fTimeCut1 = t1.c_str();
}

//-------------------------------------------------------------------
TChain* DmpAnalysisCombine::GetTree(){
  if(fChainSet.find(fTreeName) == fChainSet.end()){
    ActiveTree();
  }
  return fChainSet[fTreeName];
}

//-------------------------------------------------------------------
bool DmpAnalysisCombine::ActiveTree(bool fource){
  static bool actived=false;
  if(actived && (!fource)){
    return actived;
  }
  TChain *chain = new TChain(fTreeName.c_str());
  std::string files = gRootIOSvc->GetInputPath()+gRootIOSvc->GetInputFileName();
  chain->AddFile(files.c_str());
  fChainSet.insert(std::make_pair(fTreeName,chain));
  if("CollectionTree" == fTreeName){
std::cout<<"DEBUG: "<<__FILE__<<"("<<__LINE__<<")"<<std::endl;
    chain->SetBranchAddress("EventHeader",&fEvtHeader);
    chain->SetBranchAddress("AmsClusterCollection",&fEvtAMS);
    chain->SetBranchAddress("DmpEvtPsdHits",&fEvtPsd);
    chain->SetBranchAddress("StkClusterCollection",&fEvtStk);
    chain->SetBranchAddress("DmpEvtBgoHits",&fEvtBgo);
    chain->SetBranchAddress("AncillaryEvent",&fEvtAnc);
  }else if("Event/Cal" == fTreeName){
std::cout<<"DEBUG: "<<__FILE__<<"("<<__LINE__<<")"<<std::endl;
    chain->SetBranchAddress("EventHeader",&fEvtHeader);
    chain->SetBranchAddress("Hits",&fEvtBgo);
    chain->SetBranchAddress("PsdHits",&fEvtPsd);
  }
  actived = true;
  return actived;
}

//-------------------------------------------------------------------
void DmpAnalysisCombine::ShowThisEvent()const{
  char name[50];
  snprintf(name,50,"Time: second_%d millisecond_%d",fEvtHeader->GetSecond(),fEvtHeader->GetMillisecond());
  std::string xn  = (std::string)name + "-XZ";
  TH2F *h_layer_bar_e_X = new TH2F(xn.c_str(),"layer_bar_e-XZ",22,0,22,14,0,14);
  h_layer_bar_e_X->GetXaxis()->SetTitle("bar");
  h_layer_bar_e_X->GetYaxis()->SetTitle("x layer");

  std::string yn  = (std::string)name + "-YZ";
  TH2F *h_layer_bar_e_Y = new TH2F(yn.c_str(),"layer_bar_e-YZ",22,0,22,14,0,14);
  h_layer_bar_e_Y->GetXaxis()->SetTitle("bar");
  h_layer_bar_e_Y->GetYaxis()->SetTitle("y layer");
  short nBar = fEvtBgo->FiredBarNumber();
  double e_T[14]={0.},e_S0[14]={0.},e_S1[14]={0.},layerID[14]={0.};
  for(short i=0;i<14;++i){
    e_T[i] = fEvtBgo->TotalEnergy(i);
    e_S0[i]= fEvtBgo->TotalEnergyS0(i);
    e_S1[i]= fEvtBgo->TotalEnergyS1(i);
    layerID[i] = i;
  }
  for(short i=0;i<nBar;++i){
    short l=DmpBgoBase::GetLayerID(fEvtBgo->fGlobalBarID[i]);
    short b=DmpBgoBase::GetBarID(fEvtBgo->fGlobalBarID[i]);
    double e = fEvtBgo->fEnergy[i];
    if((l % 2) == 0){
      h_layer_bar_e_Y->Fill(b,l,e);
    }else{
      h_layer_bar_e_X->Fill(b,l,e);
    }
  }

  TCanvas *c1 = new TCanvas(name,name);
  c1->Divide(2,2);
  c1->cd(1);
  gPad->SetLogz();
  h_layer_bar_e_X->Draw("colz");
  c1->cd(2);
  gPad->SetLogz();
  h_layer_bar_e_Y->Draw("colz");

  c1->cd(3);
  TMultiGraph *grm = new TMultiGraph();
  TGraph *gTE = new TGraph(14,layerID,e_T);
  gTE->SetMarkerStyle(30);
  gTE->SetMarkerColor(2);
  grm->Add(gTE);

  TGraph *g_S0 = new TGraph(14,layerID,e_S0);
  g_S0->SetMarkerStyle(32);
  g_S0->SetMarkerColor(4);
  grm->Add(g_S0);

  TGraph *g_S1 = new TGraph(14,layerID,e_S1);
  g_S1->SetMarkerStyle(31);
  g_S1->SetMarkerColor(3);
  grm->Add(g_S1);
  grm->Draw("ap");

  TLegend *leg = new TLegend(0.3, 0.7, 0.45, 0.9);
  leg->AddEntry(gTE, "combined energy", "p");
  leg->AddEntry(g_S0, "energy side_0", "p");
  leg->AddEntry(g_S1, "energy side_1", "p");
  leg->Draw("same");

}

//-------------------------------------------------------------------
short DmpAnalysisCombine::StkClusterNumber(const short &ladderid)const{
  if(fTreeName != "CollectionTree"){
    return -999;
  }
  short nTot = fEvtStk->GetEntriesFast();
  if(ladderid == -1){
    return nTot;
  }
  short n=0;
  for(short i=0;i<nTot;++i){
    DmpStkSiCluster *stkCluster = (DmpStkSiCluster*)fEvtStk->At(i);
    if(stkCluster->getLadder() == ladderid){
      ++n;
    }
  }
  return n;
}

//-------------------------------------------------------------------
short DmpAnalysisCombine::AmsClusterNumber(const short &ladderid)const{
  if(fTreeName != "CollectionTree"){
    return -999;
  }
  short nTot = fEvtAMS->GetEntriesFast();
  if(ladderid == -1){
    return nTot;
  }
  short n=0;
  for(short i=0;i<nTot;++i){
    Cluster *aCluster = (Cluster*)fEvtAMS->At(i);
    if(aCluster->ladder == ladderid){
      ++n;
    }
  }
  return n;
}

//-------------------------------------------------------------------
double DmpAnalysisCombine::StkEnergy()const{
  if(fTreeName != "CollectionTree"){
    return -999;
  }
  short nTot = fEvtStk->GetEntriesFast();
  double e=0.;
  for(short i=0;i<nTot;++i){
    e += ((DmpStkSiCluster*)fEvtStk->At(i))->getEnergy();
  }
  return e;
}

//-------------------------------------------------------------------
double DmpAnalysisCombine::StkEnergy(const short &ladderid)const{
  if(fTreeName != "CollectionTree"){
    return -999;
  }
  short nTot = fEvtStk->GetEntriesFast();
  double e=0.;
  for(short i=0;i<nTot;++i){
    DmpStkSiCluster *stkCluster = (DmpStkSiCluster*)fEvtStk->At(i);
    if(stkCluster->getLadder() == ladderid){
      e += stkCluster->getEnergy();
    }
  }
  return e;
}

//-------------------------------------------------------------------
DmpAnalysisCombine *gAnaCombine = DmpAnalysisCombine::GetInstance();

