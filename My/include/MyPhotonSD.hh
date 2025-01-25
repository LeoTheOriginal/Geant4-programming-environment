#ifndef MYPHOTONSD_HH
#define MYPHOTONSD_HH

#include "G4VSensitiveDetector.hh"
#include "MyPhotonHit.hh"
#include "G4THitsCollection.hh"

class G4Step;
class G4TouchableHistory;

class MyPhotonSD : public G4VSensitiveDetector {
public:
  MyPhotonSD(const G4String& name);
  virtual ~MyPhotonSD();

  // Metody wirtualne
  void Initialize(G4HCofThisEvent* hce) override;
  G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;
  void EndOfEvent(G4HCofThisEvent* hce) override;

private:
  G4THitsCollection<MyPhotonHit>* fHitsCollection;
  G4int fHitsCollectionID;
};

#endif
