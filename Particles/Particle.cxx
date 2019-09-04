/*
 * Particle.cxx
 *
 *  Created on: Sep 4, 2019
 *      Author: schmollweger
 */

#include "Particle.h"
#include <iostream>

Particle::Particle() :
		fPDGCode(0) {
	// TODO Auto-generated constructor stub

}

Particle::~Particle() {
	// TODO Auto-generated destructor stub
}

void Particle::BoostMom(TVector3 boost) {
	fMom.Boost(-boost.X(), -boost.Y(), -boost.Z());
	return;
}

TVector3 Particle::BoostMomToRestFrame() {
	float beta = fMom.Beta();
	TVector3 boost;
	boost.SetX(beta * cos(fMom.Phi()) * sin(fMom.Theta()));
	boost.SetY(beta * sin(fMom.Phi()) * sin(fMom.Theta()));
	boost.SetZ(beta * cos(fMom.Theta()));

	fMom.Boost(-boost.X(), -boost.Y(), -boost.Z());
	return boost;
}

