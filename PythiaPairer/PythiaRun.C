
// main91.cc is a part of the PYTHIA event generator.
// Copyright (C) 2019 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL v2 or later, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// This is a simple test program.
// It studies the charged multiplicity distribution at the LHC.
// Modified by Rene Brun, Axel Naumann and Bernhard Meirose
// to use ROOT for histogramming.

// Stdlib header file for input and output.
#include <iostream>

// Header file to access Pythia 8 program elements.
#include "Pythia8/Pythia.h"

// ROOT, for histogramming.
#include "TH1.h"
#include "TH2.h"
#include "TVector3.h"
#include "TROOT.h"
#include "TString.h"
#include "TNtuple.h"
// ROOT, for interactive graphics.
#include "TVirtualPad.h"
#include "TApplication.h"

// ROOT, for saving file.
#include "TFile.h"

#include "DawgParticle.h"
#include "CoordSampler.h"
#include "Pair.h"

using namespace Pythia8;


void motherFinder(Pythia *pythia, int id, TVector3* Proton, int lastid, int nChangeCounter) { 
  //calculate the angle between the idProton and id 
  TVector3 pSub(pythia->event[id].px(),pythia->event[id].py(),pythia->event[id].pz()); 
  if (Proton->Angle(pSub) > TMath::Pi()/2.) { 
    TVector3 pSubLast(pythia->event[lastid].px(),pythia->event[lastid].py(),pythia->event[lastid].pz()); 
    if(pSub.Angle(pSubLast) > TMath::Pi()/2.) { 
      nChangeCounter++; 
    }
    TH1F* changeStatus = (TH1F*)gROOT->FindObject("ChangeStatus"); 
    changeStatus->Fill(pythia->event[id].status()); 
  }
  int mother1 = pythia->event[id].mother1();
  int mother2 = pythia->event[id].mother2();
  if (mother1 == mother2) { 
    if (mother1 == 0 && (id == 1 || id == 2)) { // beam particle 1 or 2
      //calculate the expected number of angle changes between idProton and id
      int nExpected; 
      if (Proton->Angle(pSub) > TMath::Pi()/2.) { 
	nExpected = 1; 
      } else { 
	nExpected = 0; 
      }
      //fill histogram nChangeCounter vs. nChangeExpected
      TH2F* changeVsExpected  = (TH2F*)gROOT->FindObject("changeVsExpected"); 
      changeVsExpected->Fill(nChangeCounter,nExpected); 
      //make sure we exit this submethod
      return; 
    } else if (mother1 > 2) { //particle is a carbon copy
      motherFinder(pythia, mother1, Proton, id, nChangeCounter); 
    } else {
      std::cout << "This is not supposed to happen!\n"; 
      std::cout << "id: " << id << " mother1: " << mother1 << " mother2: " << mother2 << std::endl;
    }
  } else { //mother1 != mother2 
    motherFinder(pythia, mother1, Proton, id, nChangeCounter);  
    if(mother2 > 0) motherFinder(pythia, mother2, Proton, id, nChangeCounter);  //in case particle comes not out of a shower
  } 
  
  return; 
} 


int main(int argc, char* argv[]) {

  // Create the ROOT application environment.
  //TApplication theApp("hist", &argc, argv);
  int nOutput = atoi(argv[1]);
  
  // Create Pythia instance and set it up to generate hard QCD processes
  // above pTHat = 20 GeV for pp collisions at 14 TeV.
  Pythia pythia;
  pythia.readString("SoftQCD:all = on"); 
  //pythia.readString("HardQCD:all = on");
  //pythia.readString("PhaseSpace:pTHatMin = 20.");
  pythia.readString("Tune:pp = 14"); 
  pythia.readString("Tune:ee = 7");  //monarsch tune. 
  pythia.readString("Random:seed = 0"); 
  pythia.readString("Random:setSeed = on"); 
  pythia.readString("Beams:eCM = 13000.");
  pythia.readString("Stat:showPartonLevel = true");
 
  pythia.init();
  //const int pdgParticle = 3122;
  //const int  = 2212; 
  DawgParticle Proton("Proton");
  Proton.SetPDGCode(2212);
  DawgParticle Lambda("Lambda");
  Lambda.SetPDGCode(3122);
  float cutOffDeg = atof(argv[2]);
  double cutOff = cutOffDeg * TMath::Pi() / 180.;
  std::cout << "cutOff: " << cutOff << std::endl;
  int samplerCounter = 0;
  
  // Create file on which histogram(s) can be saved.
  TFile* outFile = new TFile(TString::Format("Output_%u.root",nOutput).Data(), "RECREATE");
  // Book histogram.
  TH1F *multv0a = new TH1F("multv0a","charged multiplicity", 200, -0.5, 199.5);
  TH1F *multMid = new TH1F("multMid","charged multiplicity", 200, -0.5, 199.5);
  TH1F *multMidHM = new TH1F("multMidHM","charged multiplicity", 200, -0.5, 199.5);
  // TH1F *multProton = new TH1F("multProton","charged multiplicity", 200, -0.5, 199.5);
  TH2F *multv0aVsMid = new TH2F("multv0avsMid","multv0avsMid", 200, -0.5, 199.5,200, -0.5, 199.5);
  // TH1F* outhist1 = new TH1F("outmom1","outmom1",10000,-4999.5,5000.5); 
  // TH1F* outhist2 = new TH1F("outmom2","outmom2",10000,-4999.5,5000.5); 
  // TNtuple* PDGmoms = new TNtuple("PDGmoms","PDGmoms", "momOne:momTwo"); 
  //Histogramm for the change counter vs. expected
  TH2F *ChangedvsExpected = new TH2F("changeVsExpected","changeVsExpected", 11, -0.5, 10.5, 11, -0.5, 10.5); 
  ChangedvsExpected->SetTitle(";nChanged;nExpected"); 
  //Histogram for the status when the change happens 
  TH1F *ChangeStatus = new TH1F("ChangeStatus","ChangeStatus", 201, -100.5, 100.5); 
  // Begin event loop. Generate event; skip if generation aborted.
  
  Pair unboostedProton;
  Pair unboostedProtonLambda;
  Pair unboostedLambda;
  unboostedProton.SetupHist(10, "unboostedProton");
  unboostedProtonLambda.SetupHist(10, "unboostedProtonLambda");
  unboostedLambda.SetupHist(10, "unboostedLambda");
  Pair boostedProton;
  Pair boostedProtonLambda;
  Pair boostedLambda;
  boostedProton.SetupHist(10, "boostedProton");
  boostedProtonLambda.SetupHist(10, "boostedProtonLambda");
  boostedLambda.SetupHist(10, "boostedLambda");

  CoordSampler* sampler = new CoordSampler();
  std::vector<DawgParticle> theUnboostedProton;
  std::vector<DawgParticle> theUnboostedLambda;
  
  for (int iEvent = 1; iEvent < 2.5e5; ++iEvent) {
    //std::cout << "======================= \n"; 
    //std::cout << "======== EVENT ======== \n"; 
    //std::cout << "======================= \n"; 
    if (!pythia.next()) continue;
    
    // Find number of all final charged particles.
    int nChargedv0a = 0;
    int nChargedMid = 0;
    int nCharged  = 0;
    int nProton = 0;
    for (int i = 0; i < pythia.event.size(); ++i) { 
      if (pythia.event[i].isFinal() && pythia.event[i].isCharged()) { 
	if (pythia.event[i].eta() > -1.0 && pythia.event[i].eta() < 1.0){
	  ++nChargedMid;
	} 
	if (pythia.event[i].eta() > 2.8 && pythia.event[i].eta() < 5.1){
	  ++nChargedv0a;
	} 
      }
    }
    //if (nChargedv0a > 70) { 
      
    //pythia.event.list(false,false);
    //std::cout << "event code: " << pythia.info.code() << std::endl; 
    for (int i = 0; i < pythia.event.size(); ++i) { 
      if (pythia.event[i].isFinal() && pythia.event[i].isCharged()) { 
	if (pythia.event[i].eta() > -1.0 && pythia.event[i].eta() < 1.0){
	  ++nCharged;
	} 
      }
      int status = ::abs(pythia.event[i].status()); 
      std::cout << status << std::endl; 
      if( status > 80 && status < 90) {
	if (pythia.event[i].eta() > -1.0 && pythia.event[i].eta() < 1.0) { 
	  if ((::abs(pythia.event[i].id()) == 2212) || (::abs(pythia.event[i].id()) == 3122) ){//Proton 2212 Lambda 3122 
	    TVector3 pPart(pythia.event[i].px(),pythia.event[i].py(),pythia.event[i].pz()); 

	    motherFinder(&pythia, i, &pPart, i, 0); 

	    if (::abs(pythia.event[i].id()) == 2212) { 
	      Proton.SetMomentum(pPart.Px(),pPart.Py(),pPart.Pz());
	      sampler->DrawFromCircle(&Proton, 1);
	      samplerCounter++;
	      while (!Proton.RadialExpanding(cutOff)) {
		sampler->DrawFromCircle(&Proton, 1);
		samplerCounter++;
	      }
	      theUnboostedProton.push_back(Proton);
	    } else if (::abs(pythia.event[i].id()) == 3122) {
	      Lambda.SetMomentum(pPart.Px(),pPart.Py(),pPart.Pz());
	      sampler->DrawFromCircle(&Lambda, 1);
	      samplerCounter++;
	      while (!Lambda.RadialExpanding(cutOff)) {
		sampler->DrawFromCircle(&Lambda, 1);
		samplerCounter++;
	      }
	      theUnboostedLambda.push_back(Lambda);
	    }
	  }
	} 
      }
    
      multMidHM->Fill( nCharged );
    }
    
    if (theUnboostedProton.size() > 1) { 
      for (auto itOne = theUnboostedProton.begin(); itOne < theUnboostedProton.end(); itOne++) {
        for (auto itTwo = itOne + 1; itTwo < theUnboostedProton.end(); itTwo++) {
          DawgParticle One = *itOne;
          DawgParticle Two = *itTwo;
          unboostedProton.FillDist(&One, &Two);
          Two.Boost(One.BoostMomToRestFrame());
          boostedProton.FillDist(&One, &Two);
        }
      } 
    }
    if (theUnboostedLambda.size() > 1) { 
      for (auto itOne = theUnboostedLambda.begin(); itOne < theUnboostedLambda.end(); itOne++) {
        for (auto itTwo = itOne + 1; itTwo < theUnboostedLambda.end(); itTwo++) {
          DawgParticle One = *itOne;
          DawgParticle Two = *itTwo;
          unboostedLambda.FillDist(&One, &Two);
          Two.Boost(One.BoostMomToRestFrame());
          boostedLambda.FillDist(&One, &Two);
        }
      } 
    }
    if (theUnboostedLambda.size() > 1 && theUnboostedProton.size() > 1) { 
      for (auto itOne = theUnboostedLambda.begin(); itOne < theUnboostedLambda.end(); itOne++) {
        for (auto itTwo = theUnboostedProton.begin(); itTwo < theUnboostedProton.end(); itTwo++) {
          DawgParticle One = *itOne;
          DawgParticle Two = *itTwo;
          unboostedProtonLambda.FillDist(&One, &Two);
          Two.Boost(One.BoostMomToRestFrame());
          boostedProtonLambda.FillDist(&One, &Two);
        }
      } 
    }
    theUnboostedProton.clear();
    theUnboostedLambda.clear();
  }
  // Statistics on event generation.
  //std::cout << "Stat on Event Generation \n" ;
  //pythia.stat();
  // Save histogram on file and close file.
  multv0a->Write();
  multMid->Write();
  multv0aVsMid->Write(); 
  //multProton->Write(); 
  multMidHM->Write();
  ChangedvsExpected->Write();
  ChangeStatus->Write(); 
  // PDGmoms->Write();
  // outhist1->Write();
  //outhist2->Write();
  
  Proton.StoreQAHisto(outFile);
  Lambda.StoreQAHisto(outFile);
  sampler->StoreQAHisto(outFile);

  unboostedProton.StoreQAHist(outFile);
  boostedProton.StoreQAHist(outFile);

  unboostedLambda.StoreQAHist(outFile);
  boostedLambda.StoreQAHist(outFile);
  
  unboostedProtonLambda.StoreQAHist(outFile);
  boostedProtonLambda.StoreQAHist(outFile);

  delete outFile;
  // Done.
  return 0;
}


