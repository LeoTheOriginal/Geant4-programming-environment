#include "MyPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"

MyPrimaryGeneratorAction::MyPrimaryGeneratorAction() {
    G4int n_particle = 1;
    fParticleGun = new G4ParticleGun(n_particle);

    // Wybierz cząstkę pierwotną
    G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle("gamma");
    
    // Ustaw energię i kierunek
    fParticleGun->SetParticleEnergy(100.*MeV); // Wystarczająca energia
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0, 0, 1));
    fParticleGun->SetParticlePosition(G4ThreeVector(0, 0, -10.*cm));
}


MyPrimaryGeneratorAction::~MyPrimaryGeneratorAction() {
  delete fParticleGun;
}

void MyPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
  fParticleGun->GeneratePrimaryVertex(anEvent);
}
