/*
 *  $Id: trig_System.C, 2014-11-04 07:43:43 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 22/10/2014
 *
 *    Usage:
 *        $root -l TriggerMonitor.C
 *  root[0]LoadInputFile("aInputFile")
 *  root[1]TriggerStatus()
*/

namespace Trigger{
void Status(char *treeName="Event/Cal"){
  //gSystem->Load("$DMPSWWORK/lib/libDmpEventRaw.so");
  TChain *RdcTree = gAnaMgr->GetTree(treeName);
  char name[50];
  snprintf(name,50,"Trigger0~4: Time-%d to %d",gCore->GetStartTime(),gCore->GetStopTime());
  TCanvas *c1 = new TCanvas(name,name);
  c1->Divide(5,3);
  c1->cd(1);    RdcTree->Draw("EventHeader.ChoosedTriggerType(0)",gAnaMgr->TimeWindowCut());
  c1->cd(2);    RdcTree->Draw("EventHeader.ChoosedTriggerType(1)",gAnaMgr->TimeWindowCut());
  c1->cd(3);    RdcTree->Draw("EventHeader.ChoosedTriggerType(2)",gAnaMgr->TimeWindowCut());
  c1->cd(4);    RdcTree->Draw("EventHeader.ChoosedTriggerType(3)",gAnaMgr->TimeWindowCut());
  c1->cd(5);    RdcTree->Draw("EventHeader.ChoosedTriggerType(4)",gAnaMgr->TimeWindowCut());

  c1->cd(6);    RdcTree->Draw("EventHeader.GeneratedTrigger(0):EventHeader.ChoosedTriggerType(0)",gAnaMgr->TimeWindowCut(),"colz");
  c1->cd(7);    RdcTree->Draw("EventHeader.GeneratedTrigger(1):EventHeader.ChoosedTriggerType(1)",gAnaMgr->TimeWindowCut(),"colz");
  c1->cd(8);    RdcTree->Draw("EventHeader.GeneratedTrigger(2):EventHeader.ChoosedTriggerType(2)",gAnaMgr->TimeWindowCut(),"colz");
  c1->cd(9);    RdcTree->Draw("EventHeader.GeneratedTrigger(3):EventHeader.ChoosedTriggerType(3)",gAnaMgr->TimeWindowCut(),"colz");
  c1->cd(10);   RdcTree->Draw("EventHeader.GeneratedTrigger(4):EventHeader.ChoosedTriggerType(4)",gAnaMgr->TimeWindowCut(),"colz");

  c1->cd(11);   RdcTree->Draw("EventHeader.EnabledTrigger(0):EventHeader.ChoosedTriggerType(0)",gAnaMgr->TimeWindowCut(),"colz");
  c1->cd(12);   RdcTree->Draw("EventHeader.EnabledTrigger(1):EventHeader.ChoosedTriggerType(1)",gAnaMgr->TimeWindowCut(),"colz");
  c1->cd(13);   RdcTree->Draw("EventHeader.EnabledTrigger(2):EventHeader.ChoosedTriggerType(2)",gAnaMgr->TimeWindowCut(),"colz");
  c1->cd(14);   RdcTree->Draw("EventHeader.EnabledTrigger(3):EventHeader.ChoosedTriggerType(3)",gAnaMgr->TimeWindowCut(),"colz");
  c1->cd(15);   RdcTree->Draw("EventHeader.EnabledTrigger(4):EventHeader.ChoosedTriggerType(4)",gAnaMgr->TimeWindowCut(),"colz");

  TCanvas *c2 = new TCanvas("External_Period","External_Period");
  c2->Divide(2,2);
  c2->cd(1);    RdcTree->Draw("EventHeader.InjectedExternalTrigger()",gAnaMgr->TimeWindowCut());
  c2->cd(2);    RdcTree->Draw("EventHeader.GeneratedPeriodTrigger()", gAnaMgr->TimeWindowCut());
  
  c2->cd(3);    RdcTree->Draw("EventHeader.InjectedExternalTrigger():EventHeader.EnabledExternalTrigger()",gAnaMgr->TimeWindowCut(),"colz");
  c2->cd(4);    RdcTree->Draw("EventHeader.GeneratedPeriodTrigger():EventHeader.EnabledPeriodTrigger()",   gAnaMgr->TimeWindowCut(),"colz");
}
}

namespace Trigger{
// *
// *  TODO: 
// *
void Check_External2Period_Is_1_To_(TChain *RdcTree,short bili){
  gSystem->Load("$DMPSWWORK/lib/libDmpEventRaw.so");
  TChain *RdcTree = gAnaMgr->GetTree("Event/Rdc");
  long entries = RdcTree->GetEntries();
  DmpEvtHeader *header = new DmpEvtHeader();
  RdcTree->SetBranchAddress("Rdc",&header);
  long firstExternalTriggerIndex = -1;
  for(long i=0;i<entries;++i){
    RdcTree->GetEntry(i);
    if(header->GetSecond()<=){
      continue;
    }
    if((not header->GeneratedPeriodTrigger()) && header->InjectedExternalTrigger()){
      if(firstExternalTriggerIndex == -1){
        firstExternalTriggerIndex = i;
      }
      if(0 != ((i-firstExternalTriggerIndex)%bili)){
        cout<<"bili bu dui, i = "<<i<<endl;
      }
    }
  }
}
}

