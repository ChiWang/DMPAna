/*
 *  $Id: DMPSWAnalysis.C, 2014-10-30 00:30:49 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 28/10/2014
 *
 *  Usage:
 *      $ root -l DMPSWAnalysis.C
 *      root[0]DMPSWAnalysis::
*/

//-------------------------------------------------------------------
void DMPSWAnalysis(){
  gSystem->Load("$DMPSWSYS/lib/libDmpBase.so");
  gSystem->Load("$DMPSWSYS/lib/libDmpEvent.so");
  gSystem->Load("$DMPSWWORK/lib/libDmpEvtSim.so");
  gSystem->Load("$DMPSWWORK/lib/libDmpEventRaw.so");
  gSystem
}

namespace DMPSWAna{

namespace Configure{
  TCut startSecond("EventHeader.fSecond > 0");
  TCut stopSecond("EventHeader.fSecond < 99999999");
  std::string inputFiles;
  //std::map<std::string,TChain*>  TreeSet;
}

void LoadInput(const std::string &files){
  Configure::inputFiles = files;
}

void SetTimeCut(const std::string start,const std::string stop="2024-01-01 12:00:00"){
  cout<<"Setting start time cut:  "<<start<<endl;
  int startT = DmpTimeConvertor::Date2Second(start);
  char tmp[50];
  snprintf(tmp,50,"EventHeader.fSecond > %d",startT);
  Configure::startSecond = tmp;
  if("2024-01-01 12:00:00" != stop){
    std::cout<<"Setting stop time cut:  "<<stop<<std::endl;
    int stopT = DmpTimeConvertor::Date2Second(stop);
    snprintf(tmp,50,"EventHeader.fSecond < %d",stopT);
    Configure::stopSecond = tmp;
  }
}

#include "Simulation/Simulation.h"
#include "DataQuality/DataQuality.h"
#include "Reco/Reco.h"

}


