#include "MySteppingAction.hh"

#include "G4OpticalPhoton.hh"
#include "G4SteppingManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Track.hh"
#include "MyAnalysis.hh"
#include "G4EventManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Cerenkov.hh"  // Dodajemy G4Cerenkov.hh
#include "G4StepPoint.hh"  // Dodajemy G4StepPoint.hh
#include "G4ProcessManager.hh"  // Dodajemy G4ProcessManager.hh

MySteppingAction::MySteppingAction(): totalEnergyDeposit(0.) {}

MySteppingAction::~MySteppingAction() {}

void MySteppingAction::UserSteppingAction(const G4Step* step) {

  // G4cout << "UserSteppingAction executed!" << G4endl;
  G4LogicalVolume* volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
  if (volume->GetName() == "Fiber") { // Replace "TargetVolume" with the actual name of your volume
    // Get the energy deposited in this step
    G4double edep = step->GetTotalEnergyDeposit();
    
    // Add the energy deposit to the total
    totalEnergyDeposit += edep;
  }
  // Sprawdź, czy cząstka jest fotonem optycznym
  G4Track* track = step->GetTrack();
  G4cout << "Cząstka: " << track->GetDefinition()->GetParticleName() << G4endl;
  // G4cout << "Cząstka: " << G4OpticalPhoton::OpticalPhotonDefinition() << G4endl;
  // if (track->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()) {
  //   G4cout << "Optical photon detected!" << G4endl;
  // }

  // G4cout << (track->GetDefinition()->GetParticleName() != "opticalphoton") << G4endl;
  if (track->GetDefinition()->GetParticleName() != "opticalphoton")
    return;



  // Sprawdź, czy foton dociera do końca włókna
  G4StepPoint* postStepPoint = step->GetPostStepPoint();
  G4double fiberLength = 50.0 * cm;  // Długość włókna
  
  // G4cout << (postStepPoint->GetPosition().z() < (fiberLength / 2)) << G4endl;
  // if (postStepPoint->GetPosition().z() < fiberLength / 2) return;

  // G4ProcessManager* pmanager = track->GetDefinition()->GetProcessManager();
  // const G4Cerenkov* cerenkovProcess =
  //     dynamic_cast<const G4Cerenkov*>(pmanager->GetProcess("Cerenkov"));
  // G4cout << (!cerenkovProcess || step->GetPostStepPoint()->GetProcessDefinedStep() != cerenkovProcess) << G4endl;
  // if (!cerenkovProcess || step->GetPostStepPoint()->GetProcessDefinedStep() != cerenkovProcess) return;

  // Pobierz wskaźnik do managera analizy
  auto analysisManager = G4AnalysisManager::Instance();

  // Pobierz numer zdarzenia
  G4int eventID =
      G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();

  // Pobierz energię i czas fotonu
  G4double energy = track->GetKineticEnergy();
  G4double time = track->GetGlobalTime();

  G4cout << "Photon detected! EventID: " << eventID
        << ", Energy: " << energy / MeV << " MeV, Time: " << time / ns
        << " ns" << G4endl;
  
  // Zapisz dane do ntupla
  analysisManager->FillNtupleIColumn(0, eventID);
  analysisManager->FillNtupleDColumn(1, energy / MeV);
  analysisManager->FillNtupleDColumn(2, time / ns);
  analysisManager->AddNtupleRow();
}