#ifndef MYPRIMARYGENERATORACTION_HH
#define MYPRIMARYGENERATORACTION_HH

#include "G4VUserPrimaryGeneratorAction.hh"

class G4ParticleGun;
class G4Event;

class MyPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
 public:
  MyPrimaryGeneratorAction();
  ~MyPrimaryGeneratorAction() override;

  void GeneratePrimaries(G4Event*) override;

 private:
  G4ParticleGun* particleGun;
};

#endif