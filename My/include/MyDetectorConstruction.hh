#ifndef MYDETECTORCONSTRUCTION_HH
#define MYDETECTORCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

class MyDetectorConstruction : public G4VUserDetectorConstruction {
 public:
  MyDetectorConstruction();
  ~MyDetectorConstruction() override;

  G4VPhysicalVolume* Construct() override;

 private:
  void DefineMaterials();
  void SetupGeometry();

  G4LogicalVolume* fpWorldLogical;
  G4VPhysicalVolume* fpWorldPhysical;
};

#endif