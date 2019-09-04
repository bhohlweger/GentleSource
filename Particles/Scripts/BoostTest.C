#include "TApplication.h"
#include "Particle.h"
#include "TRandom3.h"
int main(int argc, char* argv[]) {
	Particle Part;
	Particle AnotherPart;
	Part.SetPDGCode(2212);
	AnotherPart.SetPDGCode(0);
	for (int i = 0; i< 10; ++i) {
		Part.SetParticleMomentum(gRandom->Uniform(0, 3), gRandom->Uniform(0, 3),
				gRandom->Uniform(0, 3));
//		AnotherPart.SetParticleMomentum(gRandom->Uniform(0, 3), gRandom->Uniform(0, 3),
//				gRandom->Uniform(0, 3));
		AnotherPart.SetParticleMomentum(0,0,0);
		AnotherPart.BoostMom(Part.BoostMomToRestFrame());
	}
	return 0;
}
