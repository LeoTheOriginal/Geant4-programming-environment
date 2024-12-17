#ifndef MYPRIMARYGENERATORACTION_HH
#define MYPRIMARYGENERATORACTION_HH

#include "G4VUserPrimaryGeneratorAction.hh"

class G4ParticleGun;

class MyPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
  MyPrimaryGeneratorAction();
  virtual ~MyPrimaryGeneratorAction();
  virtual void GeneratePrimaries(G4Event*);

private:
  G4ParticleGun* fParticleGun;
};

#endif
