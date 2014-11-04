/*
 *  $Id: evtHeader_time.C, 2014-11-04 07:01:50 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 04/11/2014
*/

void TimeStatistic(){
  TChain *RdcTree = gAnaMgr->GetTree("Event/Rdc");
  char name[50];
  snprintf(name,50,"Time-%d to %d",gCore->GetStartTime(),gCore->GetStopTime());
  TCanvas *c1 = new TCanvas(name,name);
  c1->Divide(3,1);
  c1->cd(1);   RdcTree->Draw("EventHeader.fSecond",gAnaMgr->TimeWindowCut());
  c1->cd(2);   RdcTree->Draw("EventHeader.fMillisecond",gAnaMgr->TimeWindowCut());
  c1->cd(3);   RdcTree->Draw("EventHeader.fDeltaTime",gAnaMgr->TimeWindowCut());
}

