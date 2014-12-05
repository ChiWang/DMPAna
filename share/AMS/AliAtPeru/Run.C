

void Run(){
  FileStat_t x;
  TString libName = "libAMSAnalysis";
  if(gSystem->GetPathInfo(libName,x)){
    gSystem->Load("$DMPSWWORK/lib/libDmpEvent.so");
    gSystem->CompileMacro("./Analysis.C","k",libName);
  }else{
    gSystem->Load(libName);
  }
}

//-------------------------------------------------------------------
void testProton(long nEvt = 200){
  Conf::LoadInput("./Data/Root_Data/Combine_run_1416338929_ANC_476.root");
  Performance::Clusters();
  Tracking::Plots(nEvt);
  Alignment::SingleStrack_S_Side();
}

void testElectron(long nEvt=200){
  Conf::LoadInput("./Data/Root_Data/Combine_run_1416070809_ANC_366.root");
  Performance::Clusters();
  Tracking::Plots(nEvt);
  Alignment::SingleStrack_S_Side();
}

void testPhoton(long nEvt = 200){
  Conf::LoadInput("./Data/Root_Data/Combine_run_1416276173_ANC_451.root");
  Performance::Clusters();
  Tracking::Plots(nEvt);
  Alignment::SingleStrack_S_Side();
}

