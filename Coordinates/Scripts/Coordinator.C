#include "TRandom3.h" 
#include "TH2F.h" 
#include "TH1F.h" 
#include "TCanvas.h" 
#include "TMath.h"
#include "TApplication.h"

void Coordinator() {
	gRandom->SetSeed(0);
	double coordX1;
	double coordX2;
	double coordY1;
	double coordY2;
	double ranAngle;
	double radius = 1; //in fm
	int nIter = 10000;
	TH2F* QACoordinates1 = new TH2F("QACoordinates1", "QACoordinates1", 200,
			-radius * 2, radius * 2, 200, -radius * 2, radius * 2);
	TH1F* QARadiusSampled = new TH1F("QARadiusSampled", "QARadiusSampled", 200,
			0, 3);
	TH1F* QARadLengthSampled = new TH1F("QARadLengthSampled",
			"QARadLengthSampled", 200, 0, 10);
	TH1F* QAAngleSampled = new TH1F("QAAngleSampled", "QAAngleSampled", 200,
			-10, 10);

	for (int i = 0; i < nIter; ++i) {
		ranAngle = gRandom->Uniform(0, 2 * TMath::Pi());
//    ranAngle = TMath::ASin(gRandom->Uniform(0,1));
		QAAngleSampled->Fill(ranAngle);

		coordX1 = radius * TMath::Cos(ranAngle);
		coordY1 = radius * TMath::Sin(ranAngle);
//    if (gRandom->Uniform() < 0.5) {
//      coordY1 = -coordY1;
//    }
		QACoordinates1->Fill(coordX1, coordY1);
		double b = ranAngle * radius;
		QARadLengthSampled->Fill(b);
	}

	auto cQACoordinates = new TCanvas("cQACoordinates", "cQACoordinates");
	cQACoordinates->Divide(2, 2);
	cQACoordinates->cd(1);
	QACoordinates1->Draw("COLZ");
	cQACoordinates->cd(2);
	QARadLengthSampled->Draw();
	cQACoordinates->cd(3);
	QACoordinates1->ProjectionX()->Draw();
	cQACoordinates->cd(4);
	QACoordinates1->ProjectionY()->Draw();

	auto cQARadius = new TCanvas("cQARadius", "cQARadius");
	cQARadius->cd();
	QARadiusSampled->Draw();

	auto cQAAngle = new TCanvas("cQAAngle", "cQAAngle");
	cQAAngle->cd();
	QAAngleSampled->Draw();

}

int main(int argc, char* argv[]) {
	TApplication app("app", &argc, argv);
	Coordinator();
	app.Run();
	return 0;
}
