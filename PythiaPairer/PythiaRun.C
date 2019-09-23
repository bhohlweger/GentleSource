// Header file to access Pythia 8 program elements.
#include "Pythia8/Pythia.h"

using namespace Pythia8;

int main(int argc, char* argv[]) {
  Pythia pythia;
  pythia.readString("SoftQCD:all = on"); 
  return 0;
}
