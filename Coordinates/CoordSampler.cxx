#include "CoordSampler.h"
#include "TRandom3.h"
#include "TCanvas.h"
#include <iostream>

CoordSampler::CoordSampler() :
		fQACoord(nullptr) {
	gRandom->SetSeed(0);
}

CoordSampler::~CoordSampler() {

}

void CoordSampler::DrawFromCircle(DawgParticle *Part, float Radius) {
	if (!fQACoord) {
		fQACoord = new TH3F("fQACoord", "fQACoord", 200, -1.5 * Radius,
				1.5 * Radius, 200, -1.5 * Radius, 1.5 * Radius, 200,
				-1.5 * Radius, 1.5 * Radius);
	}
	double Phi = 2 * M_PI * gRandom->Uniform(0, 1);
	double Theta = TMath::ACos(1 - 2 * gRandom->Uniform(0, 1));
	double x = Radius * TMath::Sin(Theta) * TMath::Cos(Phi);
	double y = Radius * TMath::Sin(Theta) * TMath::Sin(Phi);
	double z = Radius * TMath::Cos(Theta);

	Part->SetCoordinates(x,y,z);
	fQACoord->Fill(x,y,z);
}

void CoordSampler::DrawQAHisto() {
	auto cQACoord = new TCanvas("cQACoord","cQACoord");
	cQACoord->Divide(2,2);
	cQACoord->cd(1);
	fQACoord->SetMarkerColor(kBlue);
	fQACoord->SetMarkerSize(1.2);
	fQACoord->Draw("");
	cQACoord->cd(2);
	fQACoord->Project3D("xy")->Draw("COLZ");
	cQACoord->cd(3);
	fQACoord->Project3D("xz")->Draw("COLZ");
	cQACoord->cd(4);
	fQACoord->Project3D("yz")->Draw("COLZ");
}
