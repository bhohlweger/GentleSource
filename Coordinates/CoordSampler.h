
#ifndef COORDSAMPLER_H_
#define COORDSAMPLER_H_
#include "DawgParticle.h"
#include "TH3F.h"

class CoordSampler {
 public:
  CoordSampler();
  ~CoordSampler();
  void DrawFromCircle(DawgParticle *Part, float Radius);
  void DrawQAHisto();
 private:
  TH3F* fQACoord;
};

#endif 
