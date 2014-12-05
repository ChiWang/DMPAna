

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
enum FileType{
  electron = 0,
  muon = 1,
  photon = 2,
};

//-------------------------------------------------------------------
void Load(short type, TString InFN="NO")
{
  TString f = "NO";
  switch (type){
    case electron:
      f = "./Data/Root_Data/Combine_run_1416070809_ANC_366.root";
      break;
    case muon:
      f = "./Data/Root_Data/Combine_run_1416338929_ANC_476.root";
      break;
    case photon:
      f = "./Data/Root_Data/Combine_run_1416276173_ANC_451.root";
      break;
  }
  Conf::LoadInput(f);
}

//-------------------------------------------------------------------
void test()
{
  Performance::Clusters();
  Alignment::SingleStrack_S_Side();
  //Tracking::Plots(nEvt);
}

