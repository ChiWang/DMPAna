/*
 *  $Id: DmpAnalysisCombine.h, 2014-11-04 21:51:46 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 04/11/2014
*/

#ifndef DmpAnalysisCombine_H
#define DmpAnalysisCombine_H

#include <map>

#include "TCut.h"
#include "TClonesArray.h"
#include "TVector3.h"
#ifndef __CINT__
#include "Cluster.hh"   // for AMS cluster
#include "DmpStkSiCluster.h" // for stk cluster
#include "DmpEvtBgoHits.h"
#include "DmpEvtHeader.h"
#include "AncillaryEvent.hh"
#endif

class TChain;
class DmpEvtBgoHits;
class DmpEvtHeader;
class AncillaryEvent;

class DmpAnalysisCombine{
/*
 *  DmpAnalysisCombine
 *
 */
public:
  static DmpAnalysisCombine* GetInstance(){
    static DmpAnalysisCombine instance;
    return &instance;
  }
  ~DmpAnalysisCombine();

public:     // binding functions
  void LoadInputFile(const std::string &inputFileName,const int &BeamEnergy,const std::string &treeName="CollectionTree");
  int BeamEnergy()const{return fBeamE;}

public:
  double StkEnergy()const;
  double StkEnergy(const short &ladderID)const;
  short StkClusterNumber(const short &ladderID = -1)const;
  short AmsClusterNumber(const short &layerID = -1)const;

public:
  int   T0;             // start second, temprary use
  void SetTimeCut(const std::string &startTime_Date,const std::string &stopTime_Date="2113-01-01 00:00:00");
  void SetTimeCut(const int &startTime_Second,const int &stopTime_Second=999999999);
  TChain *GetTree();
  bool  ActiveTree(bool fource=false);
  TCut  TimeWindowCut()const{return (fTimeCut0&&fTimeCut1);}
  void ShowThisEvent()const;

private:
  DmpAnalysisCombine();

private:
  TCut      fTimeCut0;              // time cut, start time
  TCut      fTimeCut1;              // time cut, stop time
  std::map<std::string, TChain*>    fChainSet; // TChains
  int       fBeamE;         // beam energy. MeV
  std::string  fTreeName;

public:
  DmpEvtHeader      *fEvtHeader;
  TClonesArray      *fEvtAMS;        //
  DmpEvtBgoHits     *fEvtPsd;
  TClonesArray      *fEvtStk;
  DmpEvtBgoHits     *fEvtBgo;
  AncillaryEvent    *fEvtAnc;       // ancillary event

};

//-------------------------------------------------------------------
extern DmpAnalysisCombine  *gAnaCombine;

#endif

