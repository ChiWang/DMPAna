/*
 *  $Id: trig_InternalTypeRate.C, 2014-11-06 19:35:48 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 04/11/2014
*/

namespace Trigger{
void trig_InternalTypeRate(vector<double> input){
        //input[0] = FiredBar_mean, input[1] = FiredBar_sigma
        //input[2] = Energy_mean, input[3] = Energy_sigma
  if(input.size() != 4){
    cout<<"ERROR: input.size() != 4"<<endl;
    return;
  }
  float n_sigma=3;
  vector<double> results(7);
  /*
   * 0:  trig-0
   * 1:  trig-1
   * 2:  trig-2
   * 3:  trig-3
   * 4:  trig-4
   * 5:  passed events
   * 6:  total events
   */
  gSystem->Load("$DMPSWWORK/lib/libDmpEventRaw.so");
  TChain *HitsTree = gAnaMgr->GetTree("Event/Cal");
  DmpEvtHeader *header = new DmpEvtHeader();
  HitsTree->SetBranchAddress("EventHeader",&header);
  DmpEvtBgoHits *bgoHits = new DmpEvtBgoHits();
  HitsTree->SetBranchAddress("Hits",&bgoHits);
  results[6] = HitsTree->GetEntries();

  double TrigType[5]={0};
  HitsTree->GetEntry(results[6]);
  for(short i=0;i<5;++i){
    TrigType[i]=header->ChoosedTriggerType(i);
  }
  for(long ievt=0;ievt<results[6];++ievt){
    HitsTree->GetEntry(ievt);
    short nBar = bgoHits->fEnergy.size();
    if(nBar>input[0]+n_sigma*input[1] || nBar<input[0]-n_sigma*input[1]){
      continue;
    }
    double totE  =0;
    for(short ib=0;ib<nBar;++ib){
      totE += bgoHits->fEnergy[ib];
    }
    if(totE > input[2]+n_sigma*input[3] || totE < input[2]-n_sigma*input[3]){
      //std::cout<<"mean-3*sigma < total < mean+3*sigma: time: "<<header->GetSecond()<<std::endl;
      continue;
    }
    for(short i=0;i<5;++i){
      if(TrigType[i] != header->ChoosedTriggerType(i)){
        std::cout<<"trigger type not match: type "<<i<<" chosed "<<TrigType[i]<<"\t this event ("<<ievt<<") is "<<header->ChoosedTriggerType(i)<<std::endl;
        //throw;
      }
      if(header->GeneratedTrigger(i)){
        ++results[i];
      }
    }
    ++results[5];
  }
  std::cout<<"file name: "<<gRootIOSvc->GetInputFileName()<<"\ttime range: "<<gAnaMgr->TimeWindowCut().GetTitle()<<"\tpassed event number: "<<results[5]<<std::endl;
  ofstream rate("trig_rate.txt",ios::out|ios::app);
  for(short i=0;i<5;++i){
    std::cout<<"trigger class_"<<i<<" (type: "<<TrigType[i]<<")\t\tgenerated number: "<<results[i]<<"\t rate: "<<(results[i]/results[5])<<std::endl;
    rate<<results[i]<<"  ";
  }
  rate<<results[5]<<"  "<<results[6]<<endl;
}
}

