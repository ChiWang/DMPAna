/*
 *  $Id: DmpAnalysisManager.h, 2014-11-04 21:51:46 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 04/11/2014
*/

#ifndef DmpAnalysisManager_H
#define DmpAnalysisManager_H

#include "TCut.h"
#include <map>

class TChain;

class DmpAnalysisManager{
/*
 *  DmpAnalysisManager
 *
 */
public:
  static DmpAnalysisManager* GetInstance(){
    static DmpAnalysisManager instance;
    return &instance;
  }
  ~DmpAnalysisManager();

public:     // binding functions
  void LoadInputFile(const std::string &f)const;
  void SetTimeCut(const std::string &startTime_Date,const std::string &stopTime_Date="2113-01-01 00:00:00");
  void SetTimeCut(const int &startTime_Second,const int &stopTime_Second=999999999);
  TChain *GetTree(const std::string &treeName);
  bool  ActiveTree(const std::string &treeName);
  TCut  TimeWindowCut()const{return (fTimeCut0&&fTimeCut1);}
  int   T0;             // start second, temprary use

private:
  DmpAnalysisManager();

private:
  TCut      fTimeCut0;              // time cut, start time
  TCut      fTimeCut1;              // time cut, stop time
  std::map<std::string, TChain*>    fChainSet; // TChains

};

//-------------------------------------------------------------------
extern DmpAnalysisManager  *gAnaMgr;

#endif

