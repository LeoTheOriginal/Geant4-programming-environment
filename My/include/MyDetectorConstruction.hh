#ifndef MYDETECTORCONSTRUCTION_HH
#define MYDETECTORCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "MyPhotonSD.hh"
#include "G4SDManager.hh"


class MyDetectorConstruction : public G4VUserDetectorConstruction {
public:
  MyDetectorConstruction();
  virtual ~MyDetectorConstruction();

  virtual G4VPhysicalVolume* Construct();

private:
  void DefineMaterials();
  void SetupGeometry();

  G4LogicalVolume* fWorldLogical;
  G4VPhysicalVolume* fWorldPhysical;
};

#endif
