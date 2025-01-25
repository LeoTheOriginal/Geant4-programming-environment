#include "MyRun.hh"
#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4THitsCollection.hh"
#include "MyPhotonHit.hh"
#include "G4AnalysisManager.hh"

MyRun::MyRun()
 : G4Run(),
   fPhotonCollectionId(-1)
{
  auto sdManager = G4SDManager::GetSDMpointer();
  fPhotonCollectionId = sdManager->GetCollectionID("Photon/PhotonCollection");
}

MyRun::~MyRun() {}

void MyRun::RecordEvent(const G4Event* event)
{
  G4Run::RecordEvent(event);

  // Pobieramy HCE
  auto hce = event->GetHCofThisEvent();
  if (!hce) return; // brak kolekcji

  // Wyszukujemy kolekcję fotonów
  if (fPhotonCollectionId < 0) return; // nie znaleziono ID
  auto photonHits = dynamic_cast<G4THitsCollection<MyPhotonHit>*>(
      hce->GetHC(fPhotonCollectionId));
  if (!photonHits) return;

  auto analysisManager = G4AnalysisManager::Instance();

  // Iterujemy po wszystkich hitach (czyli fotonach)
  G4int nHits = photonHits->entries();
  for (G4int i=0; i<nHits; i++) {
    auto hit = (*photonHits)[i];
    G4double energy = hit->GetEnergy();
    G4double time   = hit->GetTime();

    analysisManager->FillNtupleIColumn(0, event->GetEventID());
    analysisManager->FillNtupleDColumn(1, energy);
    analysisManager->FillNtupleDColumn(2, time);
    analysisManager->AddNtupleRow();
  }
}
