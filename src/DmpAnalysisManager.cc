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
void DmpAnalysisManager::LoadInputFile(const std::string &f)const{
  gRootIOSvc->Set("Input/FileName",f);
}

//-------------------------------------------------------------------
#include <boost/lexical_cast.hpp>
void DmpAnalysisManager::SetTimeCut(const std::string &start,const std::string &stop){
  gCore->Set("StartTime",start);
  std::string t0="EventHeader.fSecond > "+boost::lexical_cast<std::string>(DmpTimeConvertor::Date2Second(start));
  fTimeCut0 = t0.c_str();

  gCore->Set("StopTime",stop);
  std::string t1="EventHeader.fSecond < "+boost::lexical_cast<std::string>(DmpTimeConvertor::Date2Second(stop));
  fTimeCut1 = t1.c_str();
}

//-------------------------------------------------------------------
void DmpAnalysisManager::SetTimeCut(const int &start,const int &stop){
  gCore->Set("StartTime",DmpTimeConvertor::Second2Date(start));
  std::string t0="EventHeader.fSecond > "+boost::lexical_cast<std::string>(start);
  fTimeCut0 = t0.c_str();

  gCore->Set("StopTime",DmpTimeConvertor::Second2Date(stop));
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
DmpAnalysisManager *gAnaMgr = DmpAnalysisManager::GetInstance();

