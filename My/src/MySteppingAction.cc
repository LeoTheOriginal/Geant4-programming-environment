#include "MySteppingAction.hh"

#include "G4OpticalPhoton.hh"
#include "G4SteppingManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Track.hh"
#include "MyAnalysis.hh"
#include "G4EventManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Cerenkov.hh"  
#include "G4StepPoint.hh"  
#include "G4ProcessManager.hh" 

MySteppingAction::MySteppingAction(): totalEnergyDeposit(0.) {}

MySteppingAction::~MySteppingAction() {}

void MySteppingAction::UserSteppingAction(const G4Step* step) {

  // G4cout << "UserSteppingAction executed!" << G4endl;
  G4LogicalVolume* volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
  if (volume->GetName() == "Fiber") {
    // Get the energy deposited in this step
    G4double edep = step->GetTotalEnergyDeposit();
    
    // Add the energy deposit to the total
    totalEnergyDeposit += edep;
  }
  
  // Check if the particle is an optical photon
  G4Track* track = step->GetTrack();

  // G4cout << (track->GetDefinition()->GetParticleName() != "opticalphoton") << G4endl;
  if (track->GetDefinition()->GetParticleName() != "opticalphoton")
    return;
  
  // Check if the photon reaches the end of the fiber
  G4StepPoint* postStepPoint = step->GetPostStepPoint();
  G4double fiberLength = 50.0 * cm;
  
  // G4cout << (postStepPoint->GetPosition().z() < (fiberLength / 2)) << G4endl;
  // if (postStepPoint->GetPosition().z() < fiberLength / 2) return;

  // G4ProcessManager* pmanager = track->GetDefinition()->GetProcessManager();
  // const G4Cerenkov* cerenkovProcess =
  //     dynamic_cast<const G4Cerenkov*>(pmanager->GetProcess("Cerenkov"));
  // G4cout << (!cerenkovProcess || step->GetPostStepPoint()->GetProcessDefinedStep() != cerenkovProcess) << G4endl;
  // if (!cerenkovProcess || step->GetPostStepPoint()->GetProcessDefinedStep() != cerenkovProcess) return;

  // Get a pointer to the analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // Get the event number
  G4int eventID =
      G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();

  // Get the photon's energy and time
  G4double energy = track->GetKineticEnergy();
  G4double time = track->GetGlobalTime();

  // G4cout << "Photon detected! EventID: " << eventID
  //       << ", Energy: " << energy / MeV << " MeV, Time: " << time / ns
  //       << " ns" << G4endl;1x1x50cm^3
  

  G4ThreeVector position = step->GetPostStepPoint()->GetPosition();
  xPos.push_back(position.x());
  yPos.push_back(position.y());
  zPos.push_back(position.z());

  // Save the data to the ntuple
  analysisManager->FillNtupleIColumn(0, eventID);
  analysisManager->FillNtupleDColumn(1, energy / MeV);
  analysisManager->FillNtupleDColumn(2, time / ns);
  analysisManager->FillNtupleDColumn(3, position.x() / cm);
  analysisManager->FillNtupleDColumn(4, position.y() / cm);
  analysisManager->FillNtupleDColumn(5, position.z() / cm);
  analysisManager->AddNtupleRow();

}