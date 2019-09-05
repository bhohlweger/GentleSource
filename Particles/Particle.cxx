/*
 * Particle.cxx
 *
 *  Created on: Sep 4, 2019
 *      Author: schmollweger
 */

#include "Particle.h"
#include "TCanvas.h"
#include <iostream>

Particle::Particle(const char* name)
    : fName(name),
      fPDGCode(0),
      fInitHist(false),
      fAngleSampled(nullptr),
      fAngleRadial(nullptr) {
}

Particle::~Particle() {
}

void Particle::Boost(TVector3 boost) {
  fMom.Boost(-boost.X(), -boost.Y(), -boost.Z());
  fPos.Boost(-boost.X(), -boost.Y(), -boost.Z());
  return;
}

TVector3 Particle::BoostMomToRestFrame() {
  float beta = fMom.Beta();
  TVector3 boost;
  boost.SetX(beta * cos(fMom.Phi()) * sin(fMom.Theta()));
  boost.SetY(beta * sin(fMom.Phi()) * sin(fMom.Theta()));
  boost.SetZ(beta * cos(fMom.Theta()));

  fMom.Boost(-boost.X(), -boost.Y(), -boost.Z());
  fPos.Boost(-boost.X(), -boost.Y(), -boost.Z());
  return boost;
}

bool Particle::RadialExpanding(double cutOff) {
  if (!fInitHist) {
    SetupHist();
  }
  bool out = true;
  float angle = fMom.Angle(fPos.Vect());
  fAngleSampled->Fill(angle);
  if (TMath::Abs(angle) > cutOff) {
    out = false;
  }
  if (out) {
    fAngleRadial->Fill(angle);
  }
  return out;
}

void Particle::SetupHist() {
  fInitHist = true;
  fAngleSampled = new TH1F(TString::Format("fAngleSampled_%s", fName),
                           TString::Format("fAngleSampled_%s", fName), 200,
                           -0.1, TMath::Pi());
  fAngleRadial = new TH1F(TString::Format("fAngleRadial%s", fName),
                          TString::Format("fAngleRadial%s", fName), 200, -0.1,
                          TMath::Pi());
}

void Particle::DrawQAHisto() {
  if (!fInitHist) {
    Warning("Particle::DrawQAHisto",
            "No Histograms Initialized, not Plotting Angular Distribution \n ");
    return;
  }
  auto cQAAngel = new TCanvas(TString::Format("cQAAngel_%s", fName),
                              TString::Format("cQAAngel_%s", fName));
  cQAAngel->Divide(2, 1);
  cQAAngel->cd(1);
  fAngleSampled->Draw("");
  cQAAngel->cd(2);
  fAngleRadial->Draw("");
  return;
}

