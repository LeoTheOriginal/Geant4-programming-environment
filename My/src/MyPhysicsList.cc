#include "MyPhysicsList.hh"
#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4HadronPhysicsFTFP_BERT.hh"
#include "G4IonPhysics.hh"
#include "G4OpticalPhysics.hh"
#include "G4StoppingPhysics.hh"

MyPhysicsList::MyPhysicsList() : G4VModularPhysicsList() {
  SetVerboseLevel(1);

  // Default physics
  RegisterPhysics(new G4DecayPhysics());

  // EM physics
  RegisterPhysics(new G4EmStandardPhysics_option4());

  // Hadronic physics
  RegisterPhysics(new G4HadronPhysicsFTFP_BERT());
  RegisterPhysics(new G4HadronElasticPhysicsHP());
  RegisterPhysics(new G4StoppingPhysics());
  RegisterPhysics(new G4IonPhysics());

  // Optical physics
  G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
  auto opticalParams = G4OpticalParameters::Instance();
  opticalParams->SetCerenkovMaxPhotonsPerStep(100);
  opticalParams->SetCerenkovTrackSecondariesFirst(true);
  RegisterPhysics(opticalPhysics);
}

MyPhysicsList::~MyPhysicsList() {}

void MyPhysicsList::SetCuts() {
  G4VUserPhysicsList::SetCuts();
}