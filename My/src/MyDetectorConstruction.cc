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
  // Wolfram
  nist->FindOrBuildMaterial("G4_W");
  // Kwarc (SiO2)
  G4Material* quartz = nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
  G4MaterialPropertiesTable* quartzProperties = new G4MaterialPropertiesTable();

  const G4int nEntries = 2;
  G4double photonEnergy[nEntries] = {1.5*eV, 6.2*eV}; // Zakres energii światła
  G4double refractiveIndex[nEntries] = {1.46, 1.46};  // Współczynnik załamania

  quartzProperties->AddProperty("RINDEX", photonEnergy, refractiveIndex, nEntries);
  quartz->SetMaterialPropertiesTable(quartzProperties);

  G4Material* pmma = nist->FindOrBuildMaterial("G4_PLEXIGLASS");

  G4MaterialPropertiesTable* pmmaProperties = new G4MaterialPropertiesTable();
  G4double pmmaRefractiveIndex[nEntries] = {1.49, 1.49};
  pmmaProperties->AddProperty("RINDEX", photonEnergy, pmmaRefractiveIndex, nEntries);
  pmma->SetMaterialPropertiesTable(pmmaProperties);

}

void MyDetectorConstruction::SetupGeometry() {
  G4NistManager* nist = G4NistManager::Instance();
  G4Material* air   = nist->FindOrBuildMaterial("G4_AIR");
  G4Material* tungsten = nist->FindOrBuildMaterial("G4_W");
  G4Material* quartz= nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
  

  // Wymiary świata
  G4double worldX = 1.0*m;
  G4double worldY = 1.0*m;
  G4double worldZ = 1.0*m;

  G4Box* worldSolid = new G4Box("World_Solid", worldX/2., worldY/2., worldZ/2.);
  fWorldLogical = new G4LogicalVolume(worldSolid, air, "World_Logical");
  fWorldPhysical = new G4PVPlacement(0, G4ThreeVector(), fWorldLogical, 
                                     "World_Physical", 0, false, 0);

  // Blok wolframowy: 1x1x50 cm^3
  G4double WX = 1.0*cm; 
  G4double WY = 1.0*cm;
  G4double WZ = 50.*cm; 
  G4Box* copperBlockSolid = new G4Box("CopperBlock_Solid", WX/2., WY/2., WZ/2.);
  G4LogicalVolume* copperBlockLogical = 
  new G4LogicalVolume(copperBlockSolid, tungsten, "CopperBlock_Logical");

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

  G4SDManager* sdManager = G4SDManager::GetSDMpointer();
  MyPhotonSD* photonSD = new MyPhotonSD("Photon");
  sdManager->AddNewDetector(photonSD);
  fiberLogical->SetSensitiveDetector(photonSD);

}
