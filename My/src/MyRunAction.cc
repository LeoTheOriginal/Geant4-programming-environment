#include "MyRunAction.hh"

#include "G4Run.hh"
#include "G4SystemOfUnits.hh"
#include "MyAnalysis.hh"

MyRunAction::MyRunAction() {
  G4cout << "MyRunAction initialized!" << G4endl;
}

MyRunAction::~MyRunAction() {}

void MyRunAction::BeginOfRunAction(const G4Run*) {
  // Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // Create ntuple
  analysisManager->CreateNtuple("Photons", "Photon Data");
  analysisManager->CreateNtupleIColumn("EventID");
  analysisManager->CreateNtupleDColumn("Energy");
  analysisManager->CreateNtupleDColumn("Time");
  analysisManager->FinishNtuple();

  // Open an output file
  analysisManager->OpenFile("MyOutput.root");
}

void MyRunAction::EndOfRunAction(const G4Run*) {
  // Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();
  if (!analysisManager) {
    G4cerr << "ERROR: AnalysisManager is null in EndOfRunAction!" << G4endl;
    return;
  }

  // Save histograms & ntuple and close file
  analysisManager->Write();
  analysisManager->CloseFile();
  G4cout << "Dane zostały zapisane do pliku MyOutput.root" << G4endl;
}