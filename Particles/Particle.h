/*
 * Particle.h
 *
 *  Created on: Sep 4, 2019
 *      Author: schmollweger
 */

#ifndef GENTLESOURCE_PARTICLES_PARTICLE_H_
#define GENTLESOURCE_PARTICLES_PARTICLE_H_

#include "TDatabasePDG.h"
#include "TLorentzVector.h"
#include "TH1F.h"
#include <iostream>

class Particle {
public:
	Particle(const char* name);
	virtual ~Particle();
	void Boost(TVector3 boost);
	TVector3 BoostMomToRestFrame();
	void SetPDGCode(int PDGCode) {
		fPDGCode = PDGCode;
	}
	void SetMomentum(double px, double py, double pz) {
		fMom.SetXYZM(px, py, pz,
				TDatabasePDG::Instance()->GetParticle(fPDGCode)->Mass());
	}
	void SetCoordinates(double x, double y, double z) {
		fPos.SetXYZM(x,y,z,0); //sets time to assume simultaneous emmission
	}
	TLorentzVector GetPos() { return fPos;};
	TLorentzVector GetMom() { return fMom;};
	bool RadialExpanding(double cutOff);
	void SetupHist();
  void DrawQAHisto();
private:
	const char* fName;
	TLorentzVector fPos;
	TLorentzVector fMom;
	bool fInitHist;
	TH1F* fAngleSampled;
	TH1F* fAngleRadial;
	int fPDGCode;
};

#endif /* GENTLESOURCE_PARTICLES_PARTICLE_H_ */
