#ifndef MYPHOTONHIT_HH
#define MYPHOTONHIT_HH

#include "G4VHit.hh"
#include "G4ThreeVector.hh"

class MyPhotonHit : public G4VHit {
public:
  MyPhotonHit();
  virtual ~MyPhotonHit();
  
  // Set / Get energii fotonu
  inline void SetEnergy(G4double energy) { fEnergy = energy; }
  inline G4double GetEnergy() const      { return fEnergy; }
  
  // Set / Get czasu dotarcia
  inline void SetTime(G4double time) { fTime = time; }
  inline G4double GetTime() const    { return fTime; }
  
private:
  G4double fEnergy;
  G4double fTime;
};

#endif
