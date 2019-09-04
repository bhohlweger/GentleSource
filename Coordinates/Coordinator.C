

#include "TRandom3.h" 
#include "TH2F.h" 
#include "TH1F.h" 
#include "TCanvas.h" 
#include "TMath.h"


void Coordinator() { 
  gRandom->SetSeed(0); 
  double coordX1; 
  double coordX2; 
  double coordY1;
  double coordY2;
  double ranAngle; 
  double radius = 1; //in fm
  int nIter = 10000; 
  TH2F* QACoordinates1 = new TH2F("QACoordinates1", "QACoordinates1", 200,-radius*2, radius*2, 200, -radius*2, radius*2); 
  TH2F* QACoordinates2 = new TH2F("QACoordinates2", "QACoordinates2", 200,-radius*2, radius*2, 200, -radius*2, radius*2); 
  TH1F* QARadiusSampled = new TH1F("QARadiusSampled","QARadiusSampled", 200, 0, 3);
  TH1F* QAAngleSampled = new TH1F("QAAngleSampled","QAAngleSampled", 200, -10, 10);
  
  for (int i = 0; i < nIter; ++i) { 
    ranAngle = TMath::ACos(gRandom->Uniform(-1,1));
    //ranAngle = gRandom->Uniform(-1,1)*2*TMath::Pi(); 
    QAAngleSampled->Fill(ranAngle); 

    coordX1 = radius*TMath::Cos(ranAngle); 
    coordY1 = radius*TMath::Sin(ranAngle); 
    if (gRandom->Uniform() < 0.5) { 
      coordY1 = -coordY1;
    }
    
    /*
    //ranAngle = TMath::ACos(gRandom->Uniform(-1,1));
    ranAngle = gRandom->Uniform(-1,1)*2*TMath::Pi();  
    QAAngleSampled->Fill(ranAngle); 

    coordX2 = radius*TMath::Cos(ranAngle); 
    coordY2 = radius*TMath::Sin(ranAngle); 
    if (gRandom->Uniform() < 0.5) { 
      coordY2 = -coordY2; 
    }
    QACoordinates1->Fill(coordX1,coordY1); 
    QACoordinates2->Fill(coordX2,coordY2); 
    double radius = TMath::Sqrt((coordX1-coordX2)*(coordX1-coordX2)+(coordY1-coordY2)*(coordY1-coordY2));
    QARadiusSampled->Fill(radius); 
    */

  } 
  auto cQACoordinates = new TCanvas("cQACoordinates", "cQACoordinates");
  cQACoordinates->Divide(2,3); 
  cQACoordinates->cd(1); 
  QACoordinates1->Draw("COLZ");
  cQACoordinates->cd(2);  
  QACoordinates2->Draw("COLZ");
  cQACoordinates->cd(3); 
  QACoordinates1->ProjectionX()->Draw("");
  cQACoordinates->cd(5); 
  QACoordinates1->ProjectionY()->Draw("");
  cQACoordinates->cd(4); 
  QACoordinates2->ProjectionX()->Draw("");
  cQACoordinates->cd(6); 
  QACoordinates2->ProjectionY()->Draw("");
  
  cQACoordinates->SaveAs("QACoordinates.pdf"); 
  
  auto cQARadius = new TCanvas("cQARadius","cQARadius"); 
  cQARadius->cd(); 
  QARadiusSampled->Draw();
  
  auto cQAAngle = new TCanvas("cQAAngle","cQAAngle"); 
  cQAAngle->cd(); 
  QAAngleSampled->Draw();
  
} 

int main(int argc, char* argv[]) {
  Coordinator(); 
  return 0; 
}
