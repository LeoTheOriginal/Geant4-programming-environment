#include "MyDetectorConstruction.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

MyDetectorConstruction::MyDetectorConstruction()
: fWorldLogical(nullptr), fWorldPhysical(nullptr)
{}

MyDetectorConstruction::~MyDetectorConstruction(){}

G4VPhysicalVolume* MyDetectorConstruction::Construct() {
  DefineMaterials();
  SetupGeometry();
  return fWorldPhysical;
}

void MyDetectorConstruction::DefineMaterials() {
  G4NistManager* nist = G4NistManager::Instance();
  // Powietrze do świata
  nist->FindOrBuildMaterial("G4_AIR");
  // Miedź
  nist->FindOrBuildMaterial("G4_Cu");
  // Kwarc (SiO2)
  nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
}

void MyDetectorConstruction::SetupGeometry() {
  G4NistManager* nist = G4NistManager::Instance();
  G4Material* air   = nist->FindOrBuildMaterial("G4_AIR");
  G4Material* copper= nist->FindOrBuildMaterial("G4_Cu");
  G4Material* quartz= nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE");

  // Wymiary świata
  G4double worldX = 1.0*m;
  G4double worldY = 1.0*m;
  G4double worldZ = 1.0*m;

  G4Box* worldSolid = new G4Box("World_Solid", worldX/2., worldY/2., worldZ/2.);
  fWorldLogical = new G4LogicalVolume(worldSolid, air, "World_Logical");
  fWorldPhysical = new G4PVPlacement(0, G4ThreeVector(), fWorldLogical, 
                                     "World_Physical", 0, false, 0);

  // Blok miedziany: 1x1x50 cm^3
  G4double CuX = 1.0*cm; 
  G4double CuY = 1.0*cm;
  G4double CuZ = 50.*cm; 
  G4Box* copperBlockSolid = new G4Box("CopperBlock_Solid", CuX/2., CuY/2., CuZ/2.);
  G4LogicalVolume* copperBlockLogical = 
    new G4LogicalVolume(copperBlockSolid, copper, "CopperBlock_Logical");

  new G4PVPlacement(0, G4ThreeVector(), copperBlockLogical, "CopperBlock_Physical",
                    fWorldLogical, false, 0);

  // Włókna kwarcowe:
  // Promień włókna: 0.625 mm = 0.0625 cm
  // Długość włókna: 50 cm (tak jak blok)
  G4double fiberRadius = 0.625*mm;
  G4double fiberLength = 50.*cm;
  G4Tubs* fiberSolid = new G4Tubs("Fiber_Solid", 0., fiberRadius, fiberLength/2., 0.*deg, 360.*deg);

  G4LogicalVolume* fiberLogical = 
    new G4LogicalVolume(fiberSolid, quartz, "Fiber_Logical");

  // Układ 3x3 włókien:
  // Odległość osi włókien: 2.632 mm
  G4double spacing = 2.632*mm;

  // Włókna rozmieszczone symetrycznie wokół (0,0):
  // Indeksy i,j ∈ {-1,0,1}, xPos = i*spacing, yPos = j*spacing
  for (int i=-1; i<=1; i++) {
    for (int j=-1; j<=1; j++) {
      G4double xPos = i*spacing;
      G4double yPos = j*spacing;
      G4ThreeVector pos(xPos, yPos, 0.);
      new G4PVPlacement(0, pos, fiberLogical, "Fiber_Physical",
                        copperBlockLogical, false, (i+1)*3+(j+1));
    }
  }

  // Atrybuty wizualizacji
  fWorldLogical->SetVisAttributes(G4VisAttributes::GetInvisible());

  G4VisAttributes* copperVis = new G4VisAttributes(G4Colour(0.7,0.4,0.4));
  copperVis->SetForceWireframe(true);
  copperBlockLogical->SetVisAttributes(copperVis);

  G4VisAttributes* fiberVis = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  fiberVis->SetForceSolid(true);
  fiberLogical->SetVisAttributes(fiberVis);
}
