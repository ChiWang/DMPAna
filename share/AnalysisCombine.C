/*
 *  $Id: AnalysisCombine.C, 2014-11-15 16:42:04 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 04/11/2014
*/

void AnalysisCombine(){
  gSystem->Load("$DMPSWWORK/lib/libDmpEvent.so");
  gSystem->Load("$DMPSWWORK/lib/libDmpAnalysisCombine.so");
}

#include "AMS/DataQuality.C"
#include "AMS/Alignment.C"
#include "AMS/Display.C"
#include "spectrum.C"
//#include "trig_System.C"
//#include "trig_InternalTypeRate.C"
//#include "spectrum_e.C"
//#include "MaxEnergyBar.C"
//#include "ProfileCheck.C"

