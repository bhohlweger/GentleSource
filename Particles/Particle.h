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

class Particle {
public:
	Particle();
	virtual ~Particle();
	void BoostMom(TVector3 boost);
	void BoostCoord(TVector3 boost);
	TVector3 BoostMomToRestFrame();
	void SetPDGCode(int PDGCode) {
		fPDGCode = PDGCode;
	}
	void SetParticleMomentum(double px, double py, double pz) {
		fMom.SetXYZM(px, py, pz,
				TDatabasePDG::Instance()->GetParticle(fPDGCode)->Mass());
	}
private:
	TLorentzVector fPos;
	TLorentzVector fMom;
	int fPDGCode;
};

#endif /* GENTLESOURCE_PARTICLES_PARTICLE_H_ */
