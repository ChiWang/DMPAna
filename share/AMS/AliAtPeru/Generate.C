

void Generate(){
  gSystem->Load("$DMPSWWORK/lib/libDmpEvent.so");
  gSystem->CompileMacro("./Analysis.C");
}

void test(){
  Conf::LoadInput("./Data/Root_Data/Combine_run_1416338929_ANC_476.root");
  Performance::Clusters();
  Tracking::Plots(200);
  Alignment::SingleStrack_S_Side();
}


