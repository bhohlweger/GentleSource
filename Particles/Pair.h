/*
 * Pair.h
 *
 *  Created on: Sep 4, 2019
 *      Author: schmollweger
 */

#ifndef GENTLESOURCE_PARTICLES_PAIR_H_
#define GENTLESOURCE_PARTICLES_PAIR_H_
#include "Particle.h"
#include "TLorentzVector.h"
#include "TH1F.h"
#include "TH2F.h"

class Pair {
public:
	Pair();
	virtual ~Pair();
	void SetupHist(double rad, const char* name);
	void FillDist(Particle *one, Particle *two);
	void DrawQAHist(const char* name);
private:
	bool fInitHist;
	TH1F* fRadDist;
	TH1F* fPairCoordAngleDist;
	TH1F* fPairMomAngleDist;
	TH1F* fPairMomPhiAngleDist;
	TH2F* fAngleDistOne;
	TH2F* fAngleDistTwo;
};

#endif /* GENTLESOURCE_PARTICLES_PAIR_H_ */
