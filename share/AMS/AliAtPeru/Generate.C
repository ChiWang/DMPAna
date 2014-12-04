

void Generate(){
  gSystem->Load("$DMPSWWORK/lib/libDmpEvent.so");
  gSystem->CompileMacro("./Analysis.C");
}

