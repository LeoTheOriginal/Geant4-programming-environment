#include "MyRunAction.hh"
#include "MyRun.hh"
#include "G4Run.hh"
#include "G4ios.hh"
#include "G4AnalysisManager.hh"

MyRunAction::MyRunAction() {}


MyRunAction::~MyRunAction() {}


void MyRunAction::BeginOfRunAction(const G4Run*) {
    G4cout << "### Run start ###" << G4endl;
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->OpenFile("output.root");

    analysisManager->CreateNtuple("Photons", "Optical photons");
    analysisManager->CreateNtupleDColumn("Energy"); // Kolumna 0
    analysisManager->CreateNtupleDColumn("Time");   // Kolumna 1
    analysisManager->FinishNtuple();
}

void MyRunAction::EndOfRunAction(const G4Run* aRun) {
  G4cout << "### Run end. Number of events: " 
         << aRun->GetNumberOfEvent() << " ###" << G4endl;

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
}

G4Run* MyRunAction::GenerateRun() {
  return new MyRun();
}
