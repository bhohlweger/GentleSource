/*
 * Pair.cxx
 *
 *  Created on: Sep 4, 2019
 *      Author: schmollweger
 */

#include "Pair.h"
#include "TCanvas.h"
#include <iostream>
Pair::Pair()
    : fInitHist(false),
      fRadDist(nullptr),
      fPairCoordAngleDist(nullptr),
      fPairMomAngleDist(nullptr),
      fPairMomPhiAngleDist(nullptr),
      fAngleDistOne(nullptr),
      fAngleDistTwo(nullptr) {
}

Pair::~Pair() {
}
void Pair::SetupHist(double rad, const char* name) {
  if (fInitHist) {
    Warning("Pair::SetupHist", "Histogramms already setup");
    return;
  }
  fRadDist = new TH1F(TString::Format("fRadDist_%s", name),
                      TString::Format("fRadDist_%s", name), 400, -rad, rad);
  fRadDist->GetXaxis()->SetTitle("|r_{one - two}|");

  fPairCoordAngleDist = new TH1F(TString::Format("fPairAngleDist_%s", name),
                                 TString::Format("fPairAngleDist_%s", name),
                                 400, -rad, rad);
  fPairMomAngleDist = new TH1F(TString::Format("fPairMomAngleDist%s", name),
                               TString::Format("fPairMomAngleDist%s", name),
                               200, -TMath::Pi(), TMath::Pi());
  fPairMomPhiAngleDist = new TH1F(
      TString::Format("fPairMomPhiAngleDist%s", name),
      TString::Format("fPairMomPhiAngleDist%s", name), 200, -TMath::Pi(),
      TMath::Pi());

  fAngleDistOne = new TH2F(TString::Format("fAngleDistOne_%s", name),
                           TString::Format("fAngleDistOne_%s", name), 200, -0.1,
                           TMath::Pi(), 500, 0, 10);
  fAngleDistOne->GetXaxis()->SetTitle("Angle");
  fAngleDistOne->GetYaxis()->SetTitle("|P_{One}| (GeV/#it{c})");

  fAngleDistTwo = new TH2F(TString::Format("fAngleDistTwo_%s", name),
                           TString::Format("fAngleDistTwo_%s", name), 200, -0.1,
                           TMath::Pi(), 500, 0, 10);
  fAngleDistTwo->GetXaxis()->SetTitle("Angle");
  fAngleDistTwo->GetYaxis()->SetTitle("|P_{Two}| (GeV/#it{c})");

  fInitHist = true;
}
void Pair::FillDist(Particle *one, Particle *two) {
  if (!fInitHist) {
    Warning("Pair::FillDist", "No histogramms setup, call SetupHist first \n");
    return;
  }
  TLorentzVector radius = one->GetPos() - two->GetPos();
  fPairCoordAngleDist->Fill(one->GetPos().Angle(two->GetPos().Vect()));
  fRadDist->Fill(radius.P());
  fPairMomAngleDist->Fill(one->GetMom().Angle(two->GetMom().Vect()));
  fPairMomPhiAngleDist->Fill(one->GetMom().Phi() - two->GetMom().Phi());
  fAngleDistOne->Fill(one->GetMom().Angle(radius.Vect()), one->GetMom().P());
  fAngleDistTwo->Fill(two->GetMom().Angle(radius.Vect()), two->GetMom().P());
  return;
}

void Pair::DrawQAHist(const char* name) {
  if (!fInitHist) {
    Warning("Pair::DrawQAHist",
            "No histogramms setup, call SetupHist first \n");
    return;
  }
  auto cQAPair = new TCanvas(TString::Format("cQAPair_%s", name),
                             TString::Format("cQAPair_%s", name));
  cQAPair->Divide(2, 2);
  cQAPair->cd(1);
  fRadDist->Draw();
  cQAPair->cd(2);
  fPairCoordAngleDist->Draw();
  cQAPair->cd(3);
  fAngleDistOne->Draw("COLZ");
  cQAPair->cd(4);
  fAngleDistTwo->Draw("COLZ");
  auto cQAMom = new TCanvas(TString::Format("cQAMom_%s", name),
                            TString::Format("cQAMom_%s", name));
  cQAMom->Divide(2,1);
  cQAMom->cd(1);
  fPairMomAngleDist->Draw();
  cQAMom->cd(2);
  fPairMomPhiAngleDist->Draw();
}
