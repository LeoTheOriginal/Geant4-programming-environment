#include "MyRunAction.hh"
#include "MyRun.hh"
#include "G4Run.hh"
#include "G4ios.hh"

MyRunAction::MyRunAction(){}
MyRunAction::~MyRunAction(){}

void MyRunAction::BeginOfRunAction(const G4Run*) {
  G4cout << "### Run start ###" << G4endl;
}

void MyRunAction::EndOfRunAction(const G4Run* aRun) {
  G4cout << "### Run end. Number of events: " 
         << aRun->GetNumberOfEvent() << " ###" << G4endl;
}

G4Run* MyRunAction::GenerateRun() {
  return new MyRun();
}
