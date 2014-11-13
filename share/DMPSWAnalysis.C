/*
 *  $Id: DMPSWAnalysis.C, 2014-11-04 06:56:46 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 04/11/2014
*/

void DMPSWAnalysis(){
  gSystem->Load("$DMPSWWORK/lib/libDmpAnalysis.so");
  gSystem->Load("$DMPSWWORK/lib/libDmpEvtSim.so");
  gSystem->Load("$DMPSWWORK/lib/libDmpEventRaw.so");
}

#include "evtHeader_time.C"
#include "trig_System.C"
#include "trig_InternalTypeRate.C"
#include "spectrum_e.C"
#include "MaxEnergyBar.C"

