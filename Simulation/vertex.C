/*
 *  $Id: vertex.C, 2014-10-30 01:04:47 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 22/10/2014
 *
*/

namespace Simulation{

  void VertexOfPhoton(std::string files){
    TChain *eventTree = new TChain("Event/MCTruth");
    eventTree->AddFile(files.c_str());
    TCanvas *c1 = new TCanvas("VertexOfPhoton","VertexOfPhoton");
    c1->Divide(2,2);
    c1->cd(1);
    eventTree->Draw("TrackVertex.fPosition.fZ","(TrackVertex.fPDGCode==22)&&(TrackVertex.fParentID == 1)&&(TrackVertex.fPosition.fZ < -5000)");
    c1->cd(2);
    eventTree->Draw("TrackVertex.fDirection.Theta()","(TrackVertex.fPDGCode==22)&&(TrackVertex.fParentID == 1)&&(TrackVertex.fPosition.fZ < -5000)");
    c1->cd(3);
    eventTree->Draw("TrackVertex.fEnergy:TrackVertex.fPosition.fZ","(TrackVertex.fPDGCode==22)&&(TrackVertex.fParentID == 1)&&(TrackVertex.fPosition.fZ < -5000)","colz");
    c1->cd(4);
    eventTree->Draw("TrackVertex.fDirection.Theta():TrackVertex.fEnergy","(TrackVertex.fPDGCode==22)&&(TrackVertex.fParentID == 1)&&(TrackVertex.fPosition.fZ < -5000)","colz");
  }

}


