/*
 *  $Id: TriggerMonitor.C, 2014-10-30 01:12:12 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 22/10/2014
 *
 *    Usage:
 *        $root -l TriggerMonitor.C
 *  root[0]LoadInputFile("aInputFile")
 *  root[1]TriggerStatus()
*/

namespace DataQuality{

void TriggerStatus(){
  TCanvas *c1 = new TCanvas("Trigger0~4","Trigger0~4");
  c1->Divide(5,3);
  c1->cd(1);    RdcTree->Draw("EventHeader.ChoosedTriggerType(0)");
  c1->cd(2);    RdcTree->Draw("EventHeader.ChoosedTriggerType(1)");
  c1->cd(3);    RdcTree->Draw("EventHeader.ChoosedTriggerType(2)");
  c1->cd(4);    RdcTree->Draw("EventHeader.ChoosedTriggerType(3)");
  c1->cd(5);    RdcTree->Draw("EventHeader.ChoosedTriggerType(4)");

  c1->cd(6);    RdcTree->Draw("EventHeader.GeneratedTrigger(0):EventHeader.ChoosedTriggerType(0)");
  c1->cd(7);    RdcTree->Draw("EventHeader.GeneratedTrigger(1):EventHeader.ChoosedTriggerType(1)");
  c1->cd(8);    RdcTree->Draw("EventHeader.GeneratedTrigger(2):EventHeader.ChoosedTriggerType(2)");
  c1->cd(9);    RdcTree->Draw("EventHeader.GeneratedTrigger(3):EventHeader.ChoosedTriggerType(3)");
  c1->cd(10);   RdcTree->Draw("EventHeader.GeneratedTrigger(4):EventHeader.ChoosedTriggerType(4)");

  c1->cd(11);   RdcTree->Draw("EventHeader.EnabledTrigger(0):EventHeader.ChoosedTriggerType(0)");
  c1->cd(12);   RdcTree->Draw("EventHeader.EnabledTrigger(1):EventHeader.ChoosedTriggerType(1)");
  c1->cd(13);   RdcTree->Draw("EventHeader.EnabledTrigger(2):EventHeader.ChoosedTriggerType(2)");
  c1->cd(14);   RdcTree->Draw("EventHeader.EnabledTrigger(3):EventHeader.ChoosedTriggerType(3)");
  c1->cd(15);   RdcTree->Draw("EventHeader.EnabledTrigger(4):EventHeader.ChoosedTriggerType(4)");

  TCanvas *c2 = new TCanvas("External_Period","External_Period");
  c2->Divide(2,2);
  c2->cd(1);    RdcTree->Draw("EventHeader.InjectedExternalTrigger()");
  c2->cd(2);    RdcTree->Draw("EventHeader.GeneratedPeriodTrigger()");
  
  c2->cd(3);    RdcTree->Draw("EventHeader.InjectedExternalTrigger():EventHeader.EnabledExternalTrigger()");
  c2->cd(4);    RdcTree->Draw("EventHeader.GeneratedPeriodTrigger():EventHeader.EnabledPeriodTrigger()");
}


}

/*
namespace DataQuality{
TChain *EvtTree = 0;
void _GerateTreePtr(){
  EvtTree = new TChain("Event/Rdc");
  EvtTree->AddFile((Configure::inputFiles).c_str());
}

void TimeStatistic(){
  if(EvtTree == 0){
    _GerateTreePtr();
  }
  std::string name = "Time-"+(std::string)Configure::startSecond.GetTitle()+"_"+(std::string)Configure::stopSecond.GetTitle();
  std::vector<TCanvas*>     canvasSet;
  short i = canvasSet.size();
  canvasSet.push_back(new TCanvas(name.c_str(),name.c_str()));
  canvasSet[i]->Divide(3,1);
  canvasSet[i]->cd(1);   EvtTree->Draw("EventHeader.fSecond",Configure::startSecond && Configure::stopSecond);
  canvasSet[i]->cd(2);   EvtTree->Draw("EventHeader.fMillisecond",Configure::startSecond && Configure::stopSecond);
  canvasSet[i]->cd(3);   EvtTree->Draw("EventHeader.fDeltaTime",Configure::startSecond && Configure::stopSecond && "EventHeader.fDeltaTime != 30000");
}

void TriggerStatistic(){
  if(EvtTree == 0){
    _GerateTreePtr();
  }
  TCanvas *c1 = new TCanvas("Trigger0~4","Trigger0~4");
  c1->Divide(5,3);
  c1->cd(1);    EvtTree->Draw("EventHeader.ChoosedTriggerType(0)",Configure::startSecond && Configure::stopSecond);
  c1->cd(2);    EvtTree->Draw("EventHeader.ChoosedTriggerType(1)",Configure::startSecond && Configure::stopSecond);
  c1->cd(3);    EvtTree->Draw("EventHeader.ChoosedTriggerType(2)",Configure::startSecond && Configure::stopSecond);
  c1->cd(4);    EvtTree->Draw("EventHeader.ChoosedTriggerType(3)",Configure::startSecond && Configure::stopSecond);
  c1->cd(5);    EvtTree->Draw("EventHeader.ChoosedTriggerType(4)",Configure::startSecond && Configure::stopSecond);

  c1->cd(6);    EvtTree->Draw("EventHeader.GeneratedTrigger(0):EventHeader.ChoosedTriggerType(0)",Configure::startSecond && Configure::stopSecond);
  c1->cd(7);    EvtTree->Draw("EventHeader.GeneratedTrigger(1):EventHeader.ChoosedTriggerType(1)",Configure::startSecond && Configure::stopSecond);
  c1->cd(8);    EvtTree->Draw("EventHeader.GeneratedTrigger(2):EventHeader.ChoosedTriggerType(2)",Configure::startSecond && Configure::stopSecond);
  c1->cd(9);    EvtTree->Draw("EventHeader.GeneratedTrigger(3):EventHeader.ChoosedTriggerType(3)",Configure::startSecond && Configure::stopSecond);
  c1->cd(10);   EvtTree->Draw("EventHeader.GeneratedTrigger(4):EventHeader.ChoosedTriggerType(4)",Configure::startSecond && Configure::stopSecond);

  c1->cd(11);   EvtTree->Draw("EventHeader.EnabledTrigger(0):EventHeader.ChoosedTriggerType(0)",Configure::startSecond && Configure::stopSecond);
  c1->cd(12);   EvtTree->Draw("EventHeader.EnabledTrigger(1):EventHeader.ChoosedTriggerType(1)",Configure::startSecond && Configure::stopSecond);
  c1->cd(13);   EvtTree->Draw("EventHeader.EnabledTrigger(2):EventHeader.ChoosedTriggerType(2)",Configure::startSecond && Configure::stopSecond);
  c1->cd(14);   EvtTree->Draw("EventHeader.EnabledTrigger(3):EventHeader.ChoosedTriggerType(3)",Configure::startSecond && Configure::stopSecond);
  c1->cd(15);   EvtTree->Draw("EventHeader.EnabledTrigger(4):EventHeader.ChoosedTriggerType(4)",Configure::startSecond && Configure::stopSecond);

  TCanvas *c2 = new TCanvas("External_Period","External_Period");
  c2->Divide(2,2);
  c2->cd(1);    EvtTree->Draw("EventHeader.InjectedExternalTrigger()",Configure::startSecond && Configure::stopSecond);
  c2->cd(2);    EvtTree->Draw("EventHeader.GeneratedPeriodTrigger()", Configure::startSecond && Configure::stopSecond);
  
  c2->cd(3);    EvtTree->Draw("EventHeader.InjectedExternalTrigger():EventHeader.EnabledExternalTrigger()",Configure::startSecond && Configure::stopSecond);
  c2->cd(4);    EvtTree->Draw("EventHeader.GeneratedPeriodTrigger():EventHeader.EnabledPeriodTrigger()",   Configure::startSecond && Configure::stopSecond);
}

void Check_External2Period_Is_1_To_(short bili){
  if(EvtTree == 0){
    _GerateTreePtr();
  }
  long entries = EvtTree->GetEntries();
  DmpEvtHeader *header = new DmpEvtHeader();
  Configure::EvtTree->SetBranchAddress("Rdc",&header);
  long firstExternalTriggerIndex = -1;
  for(long i=0;i<entries;++i){
    EvtTree->GetEntry(i);
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
*/

