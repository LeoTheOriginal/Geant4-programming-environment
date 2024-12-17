#ifndef MYDETECTORCONSTRUCTION_HH
#define MYDETECTORCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class MyDetectorConstruction : public G4VUserDetectorConstruction {
public:
  MyDetectorConstruction();
  virtual ~MyDetectorConstruction();

  virtual G4VPhysicalVolume* Construct();

private:
  void DefineMaterials();
  void SetupGeometry();

  // World volume
  G4LogicalVolume* fWorldLogical;
  G4VPhysicalVolume* fWorldPhysical;
};

#endif
