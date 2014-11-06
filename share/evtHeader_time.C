/*
 *  $Id: evtHeader_time.C, 2014-11-04 07:01:50 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 04/11/2014
*/

void TimeStatistic(std::string treeName="Event/Rdc"){
  TChain *RdcTree = gAnaMgr->GetTree(treeName.c_str());
  char name[50];
  snprintf(name,50,"from time: %d",gAnaMgr->T0);
  TCanvas *c1 = new TCanvas(name,name);
  c1->Divide(3,1);
  gStyle->SetOptStat(111111);
  c1->cd(1);   RdcTree->Draw("EventHeader.fSecond",gAnaMgr->TimeWindowCut());
  //TH1F *h1 = htemp->Clone("h1"); h1->SetNameTitle("h1","h1");
  c1->cd(2);   RdcTree->Draw("EventHeader.fMillisecond",gAnaMgr->TimeWindowCut());
  c1->cd(3);   RdcTree->Draw("EventHeader.fDeltaTime",gAnaMgr->TimeWindowCut());
}

