#include "MyPhotonSD.hh"
#include "MyPhotonHit.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4OpticalPhoton.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"

MyPhotonSD::MyPhotonSD(const G4String& name)
 : G4VSensitiveDetector(name),
   fHitsCollection(nullptr),
   fHitsCollectionID(-1)
{
  collectionName.insert("PhotonCollection");
}

MyPhotonSD::~MyPhotonSD() {}

void MyPhotonSD::Initialize(G4HCofThisEvent* hce)
{
  // Tworzymy nową kolekcję hitów
  fHitsCollection = new G4THitsCollection<MyPhotonHit>(
      SensitiveDetectorName, collectionName[0]);

  // Rejestrujemy ją w HCE
  if (fHitsCollectionID < 0) {
    fHitsCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection);
  }
  hce->AddHitsCollection(fHitsCollectionID, fHitsCollection);
}

G4bool MyPhotonSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
  // Sprawdzamy, czy to foton optyczny
  auto track = step->GetTrack();
  if (track->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition()) {
    return false;
  }

  // Sprawdzamy, czy osiągnęły koniec włókna (fGeomBoundary)
  if (step->GetPostStepPoint()->GetStepStatus() == fGeomBoundary) {
    // Tworzymy nowy hit
    auto hit = new MyPhotonHit();
    hit->SetEnergy(track->GetKineticEnergy());
    hit->SetTime(track->GetGlobalTime());

    fHitsCollection->insert(hit);
  }

  return true;
}

void MyPhotonSD::EndOfEvent(G4HCofThisEvent*){}
