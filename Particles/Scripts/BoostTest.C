#include "Particle.h"
#include "CoordSampler.h"
#include "Pair.h"

#include "TApplication.h"
#include "TRandom3.h"

#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
  double cutOffDeg = atof(argv[1]);
  double cutOff = cutOffDeg*TMath::Pi()/180.;
  std::cout << "cutOff: " << cutOff << std::endl;
  TApplication NeverStopNeverStopping("app", &argc, argv);
  Particle Part("Part");
  Particle AnotherPart("AnotherPart");
  Pair unboosted;
  unboosted.SetupHist(10, "unboosted");
  Pair boosted;
  boosted.SetupHist(10, "boosted");

  CoordSampler* sampler = new CoordSampler();
  Part.SetPDGCode(2212);
  AnotherPart.SetPDGCode(2212);
  int samplerCounter = 0;
  for (int i = 0; i < 1e7; ++i) {
    Part.SetMomentum(gRandom->Uniform(0, 3), gRandom->Uniform(0, 3),
                     gRandom->Uniform(0, 3));
    AnotherPart.SetMomentum(gRandom->Uniform(0, 3), gRandom->Uniform(0, 3),
                            gRandom->Uniform(0, 3));

    sampler->DrawFromCircle(&Part, 1);
    samplerCounter++;
    while (!Part.RadialExpanding(cutOff)) {
      sampler->DrawFromCircle(&Part, 1);
      samplerCounter++;
    }
    sampler->DrawFromCircle(&AnotherPart, 1);
    samplerCounter++;
    while (!AnotherPart.RadialExpanding(cutOff)) {
      sampler->DrawFromCircle(&AnotherPart, 1);
      samplerCounter++;
    }
    unboosted.FillDist(&Part, &AnotherPart);
    AnotherPart.Boost(Part.BoostMomToRestFrame());
    boosted.FillDist(&Part, &AnotherPart);
  }
  Part.DrawQAHisto();
  AnotherPart.DrawQAHisto();
  std::cout << "samplerCounter: " << samplerCounter << std::endl;
  sampler->DrawQAHisto();
  unboosted.DrawQAHist("unboosted");
  boosted.DrawQAHist("boosted");

  NeverStopNeverStopping.Run();
  return 0;
}
