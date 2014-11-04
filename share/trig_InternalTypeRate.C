/*
 *  $Id: trig_InternalTypeRate.C, 2014-11-04 18:42:50 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 04/11/2014
*/

namespace Trigger{
void trig_InternalTypeRate(){
  gSystem->Load("$DMPSWWORK/lib/libDmpEventRaw.so");
  DmpEvtHeader *header = new DmpEvtHeader();
  TChain *RdcTree = gAnaMgr->GetTree("Event/Rdc");
  RdcTree->SetBranchAddress("EventHeader",&header);
  long entries = RdcTree->GetEntries();
  double Trig[5]={0};
  double ChoosedTriggerType[5]={0};
  RdcTree->GetEntry(entries);
  for(short i=0;i<5;++i){
    ChoosedTriggerType[i]=header->ChoosedTriggerType(i);
  }
  double maxEvt=0;
  for(long ievt=0;ievt<entries;++ievt){
    RdcTree->GetEntry(ievt);
    if(header->GetSecond() < gAnaMgr->T0){
      continue;
    }
    for(short i=0;i<5;++i){
      if(header->GeneratedTrigger(i)){
        ++Trig[i];
      }
      if(ChoosedTriggerType[i] != header->ChoosedTriggerType(i)){
        std::cout<<"trigger type not match: type "<<i<<" chosed "<<ChoosedTriggerType[i]<<"\t this event ("<<ievt<<") is "<<header->ChoosedTriggerType(i)<<std::endl;
      }
    }
    ++maxEvt;
  }
  std::cout<<"file name: "<<gRootIOSvc->GetInputFileName()<<"\ttime range: "<<gAnaMgr->TimeWindowCut().GetTitle()<<"\tevent number: "<<maxEvt<<std::endl;
  for(short i=0;i<5;++i){
    std::cout<<"trigger class_"<<i<<" (type: "<<ChoosedTriggerType[i]<<")\t\tgenerated number: "<<Trig[i]<<"\t rate: "<<(Trig[i]/maxEvt)<<std::endl;
  }
}
}

