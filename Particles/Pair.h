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

class Pair {
public:
	Pair();
	virtual ~Pair();
private:
	Particle fPartOne;
	Particle fPartTwo;
	TLorentzVector fRelMom;
	TLorentzVector fRadius;
};

#endif /* GENTLESOURCE_PARTICLES_PAIR_H_ */
