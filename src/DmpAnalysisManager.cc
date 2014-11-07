/*
 *  $Id: DmpAnalysisManager.cc, 2014-11-04 01:58:08 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 04/11/2014
*/

#include <map>
#include "TChain.h"

#include "DmpAnalysisManager.h"
#include "DmpTimeConvertor.h"
#include "DmpLog.h"
#include "DmpRootIOSvc.h"
#include "DmpCore.h"
#include "DmpEvtBgoHits.h"
#include "DmpBgoBase.h"
#include "TCanvas.h"
#include "TH2D.h"

DmpAnalysisManager::DmpAnalysisManager():T0(0){
std::cout<<"DEBUG: "<<__FILE__<<"("<<__LINE__<<")"<<gCore->GetStopTime()<<std::endl;
  SetTimeCut(gCore->GetStartTime(),gCore->GetStopTime());
  std::cout<<"DEBUG: "<<__FILE__<<"("<<__LINE__<<")"<<std::endl;
}

//-------------------------------------------------------------------
DmpAnalysisManager::~DmpAnalysisManager(){
  for(std::map<std::string,TChain*>::iterator it=fChainSet.begin();it!=fChainSet.end();++it){
    delete it->second;
  }
  fChainSet.clear();
}

//-------------------------------------------------------------------
void DmpAnalysisManager::LoadInputFile(const std::string &f){
  gRootIOSvc->InputFile(f);
  for(std::map<std::string, TChain*>::iterator it=fChainSet.begin();it!=fChainSet.end();++it){
    delete it->second;
  }
  fChainSet.clear();
}

//-------------------------------------------------------------------
#include <boost/lexical_cast.hpp>
void DmpAnalysisManager::SetTimeCut(const std::string &start,const std::string &stop){
  gCore->StartTime(start);
  std::string t0="EventHeader.fSecond > "+boost::lexical_cast<std::string>(DmpTimeConvertor::Date2Second(start));
  fTimeCut0 = t0.c_str();

  gCore->StopTime(stop);
  std::string t1="EventHeader.fSecond < "+boost::lexical_cast<std::string>(DmpTimeConvertor::Date2Second(stop));
  fTimeCut1 = t1.c_str();
}

//-------------------------------------------------------------------
void DmpAnalysisManager::SetTimeCut(const int &start,const int &stop){
  gCore->StartTime(DmpTimeConvertor::Second2Date(start));
  T0 = start;
  std::string t0="EventHeader.fSecond > "+boost::lexical_cast<std::string>(start);
  fTimeCut0 = t0.c_str();

  gCore->StopTime(DmpTimeConvertor::Second2Date(stop));
  std::string t1="EventHeader.fSecond < "+boost::lexical_cast<std::string>(stop);
  fTimeCut1 = t1.c_str();
}

//-------------------------------------------------------------------
TChain* DmpAnalysisManager::GetTree(const std::string &treeName){
  if(fChainSet.find(treeName) == fChainSet.end()){
    ActiveTree(treeName);
  }
  return fChainSet[treeName];
}

//-------------------------------------------------------------------
bool DmpAnalysisManager::ActiveTree(const std::string &treeName){
  TChain *chain = new TChain(treeName.c_str());
  std::string files = gRootIOSvc->GetInputPath()+gRootIOSvc->GetInputFileName();
  chain->AddFile(files.c_str());
  fChainSet.insert(std::make_pair(treeName,chain));
  return true;
}

//-------------------------------------------------------------------
void DmpAnalysisManager::ShowThisEvent(char *name,DmpEvtBgoHits *evt)const{
  std::string xn  = (std::string)name + "-XZ";
  TH2F *h_layer_bar_e_X = new TH2F(xn.c_str(),"layer_bar_e-XZ",22,0,22,14,0,14);
  h_layer_bar_e_X->GetXaxis()->SetTitle("bar");
  h_layer_bar_e_X->GetYaxis()->SetTitle("x layer");

  std::string yn  = (std::string)name + "-YZ";
  TH2F *h_layer_bar_e_Y = new TH2F(yn.c_str(),"layer_bar_e-YZ",22,0,22,14,0,14);
  h_layer_bar_e_Y->GetXaxis()->SetTitle("bar");
  h_layer_bar_e_Y->GetYaxis()->SetTitle("y layer");
  short nBar = evt->fEnergy.size();
  for(short i=0;i<nBar;++i){
    if((DmpBgoBase::GetLayerID(evt->fGlobalBarID[i]) % 2) == 0){
      h_layer_bar_e_Y->Fill(DmpBgoBase::GetBarID(evt->fGlobalBarID[i]),DmpBgoBase::GetLayerID(evt->fGlobalBarID[i]),evt->fEnergy[i]);
    }else{
      h_layer_bar_e_X->Fill(DmpBgoBase::GetBarID(evt->fGlobalBarID[i]),DmpBgoBase::GetLayerID(evt->fGlobalBarID[i]),evt->fEnergy[i]);
    }
  }

  TCanvas *c1 = new TCanvas(name,name);
  c1->Divide(2,1);
  c1->cd(1);
  h_layer_bar_e_X->Draw("colz");
  c1->cd(2);
  h_layer_bar_e_Y->Draw("colz");
}

//-------------------------------------------------------------------
DmpAnalysisManager *gAnaMgr = DmpAnalysisManager::GetInstance();

