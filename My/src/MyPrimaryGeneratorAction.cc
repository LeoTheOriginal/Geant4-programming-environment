#include "MyPrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"

MyPrimaryGeneratorAction::MyPrimaryGeneratorAction() {
  G4int n_particle = 1;
  particleGun = new G4ParticleGun(n_particle);

  // Domyślne ustawienia działa - cząstka gamma o energii 100 MeV
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle("gamma");

  particleGun->SetParticleDefinition(particle);
  particleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
  particleGun->SetParticleEnergy(100. * MeV);
  particleGun->SetParticlePosition(G4ThreeVector(0., 0., -15. * cm));
}

MyPrimaryGeneratorAction::~MyPrimaryGeneratorAction() { delete particleGun; }

void MyPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
  particleGun->GeneratePrimaryVertex(anEvent);
}