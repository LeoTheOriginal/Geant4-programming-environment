#include "MyDetectorConstruction.hh"
#include "G4Box.hh"
#include "G4Colour.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "G4OpticalSurface.hh"

MyDetectorConstruction::MyDetectorConstruction()
    : fpWorldLogical(nullptr), fpWorldPhysical(nullptr) {}

MyDetectorConstruction::~MyDetectorConstruction() {}

G4VPhysicalVolume* MyDetectorConstruction::Construct() {
  DefineMaterials();
  SetupGeometry();
  return fpWorldPhysical;
}

void MyDetectorConstruction::DefineMaterials() {
  G4NistManager* nistManager = G4NistManager::Instance();

  // Define materials
  G4Material* air = nistManager->FindOrBuildMaterial("G4_AIR");
  G4Material* copper = nistManager->FindOrBuildMaterial("G4_Cu");
  G4Material* quartz = nistManager->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
  G4Material* pmma = nistManager->FindOrBuildMaterial("G4_PLEXIGLASS");
  G4Material* tungsten = nistManager->FindOrBuildMaterial("G4_W");

  // Optical properties for Quartz (SiO2)
  G4MaterialPropertiesTable* mptQuartz = new G4MaterialPropertiesTable();
  G4double photonEnergy[] = {2.0*eV, 3.0*eV}; // Example energy range
  G4double rindexQuartz[] = {1.54, 1.54}; // Refractive index
  G4double absorptionQuartz[] = {50.*cm, 50.*cm}; // Absorption length
  mptQuartz->AddProperty("RINDEX", photonEnergy, rindexQuartz, 2);
  mptQuartz->AddProperty("ABSLENGTH", photonEnergy, absorptionQuartz, 2);
  quartz->SetMaterialPropertiesTable(mptQuartz);

  // Optical properties for PMMA (Cladding)
  G4MaterialPropertiesTable* mptPMMA = new G4MaterialPropertiesTable();
  G4double rindexPMMA[] = {1.49, 1.49};
  G4double absorptionPMMA[] = {50.*cm, 50.*cm};
  mptPMMA->AddProperty("RINDEX", photonEnergy, rindexPMMA, 2);
  mptPMMA->AddProperty("ABSLENGTH", photonEnergy, absorptionPMMA, 2);
  pmma->SetMaterialPropertiesTable(mptPMMA);

  // --- Optical properties for Tungsten ---
  G4double rindexTungsten[] = {3.3, 3.3};  // Refractive index
  G4double absorptionTungsten[] = {10.*cm, 10.*cm};  // Absorption length

  G4MaterialPropertiesTable* mptTungsten = new G4MaterialPropertiesTable();
  mptTungsten->AddProperty("RINDEX", photonEnergy, rindexTungsten, 2);
  mptTungsten->AddProperty("ABSLENGTH", photonEnergy, absorptionTungsten, 2);
  tungsten->SetMaterialPropertiesTable(mptTungsten);

  // --- Surface properties ---
  G4OpticalSurface* opticalSurface = new G4OpticalSurface("FiberSurface");
  opticalSurface->SetType(dielectric_metal);
  opticalSurface->SetModel(unified);
  opticalSurface->SetFinish(polished);

  G4MaterialPropertiesTable* surfaceMPT = new G4MaterialPropertiesTable();
  G4double reflectivity[] = {0.9, 0.9};  // Odbicie
  surfaceMPT->AddProperty("REFLECTIVITY", photonEnergy, reflectivity, 2);
  opticalSurface->SetMaterialPropertiesTable(surfaceMPT);

  // Print material information
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

void MyDetectorConstruction::SetupGeometry() {
  // World
  G4double worldSizeXY = 1.0 * m;
  G4double worldSizeZ = 3.0 * m;
  G4Box* worldSolid =
      new G4Box("World", worldSizeXY / 2, worldSizeXY / 2, worldSizeZ / 2);
  fpWorldLogical = new G4LogicalVolume(worldSolid, G4Material::GetMaterial("G4_AIR"),
                                       "World");
  fpWorldPhysical = new G4PVPlacement(
      nullptr, G4ThreeVector(), fpWorldLogical, "World", nullptr, false, 0);

  // Absorber (Tungsten)
  G4double absorberSizeXY = 1.0 * cm;
  G4double absorberSizeZ = 50.0 * cm;
  G4Box* absorberSolid = new G4Box("Absorber", absorberSizeXY / 2,
                                   absorberSizeXY / 2, absorberSizeZ / 2);
  G4LogicalVolume* absorberLogical =
      new G4LogicalVolume(absorberSolid, G4Material::GetMaterial("G4_W"),
                          "Absorber");
  new G4PVPlacement(nullptr, G4ThreeVector(), absorberLogical, "Absorber",
                    fpWorldLogical, false, 0);

  // Pre-absorber (Copper)
  G4double preAbsorberThickness = 10.0 * cm;
  G4Box* preAbsorberSolid =
      new G4Box("PreAbsorber", absorberSizeXY / 2, absorberSizeXY / 2,
                preAbsorberThickness / 2);
  G4LogicalVolume* preAbsorberLogical =
      new G4LogicalVolume(preAbsorberSolid, G4Material::GetMaterial("G4_Cu"),
                          "PreAbsorber");
  new G4PVPlacement(nullptr, G4ThreeVector(0., 0., -absorberSizeZ / 2 -
                                                    preAbsorberThickness / 2),
                    preAbsorberLogical, "PreAbsorber", fpWorldLogical, false, 0);

  // Fiber (Quartz)
  G4double fiberRadius = 0.625 * mm;
  G4double fiberLength = absorberSizeZ;
  G4Tubs* fiberSolid =
      new G4Tubs("Fiber", 0., fiberRadius, fiberLength / 2, 0., 360. * deg);
  G4LogicalVolume* fiberLogical =
      new G4LogicalVolume(fiberSolid, G4Material::GetMaterial("G4_SILICON_DIOXIDE"),
                          "Fiber");

  // PMMA Cladding
  G4double claddingThickness = 0.1 * mm;
  G4Tubs* claddingSolid = new G4Tubs(
      "Cladding", fiberRadius, fiberRadius + claddingThickness, fiberLength / 2,
      0., 360. * deg);
  G4LogicalVolume* claddingLogical =
      new G4LogicalVolume(claddingSolid, G4Material::GetMaterial("G4_PLEXIGLASS"),
                          "Cladding");
  new G4PVPlacement(nullptr, G4ThreeVector(), claddingLogical, "Cladding",
                    fiberLogical, false, 0);

  // Placement of fibers in a 3x3 grid
  G4double fiberPitch = 2.632 * mm;
  for (G4int i = -1; i <= 1; i++) {
    for (G4int j = -1; j <= 1; j++) {
      G4double xPosFiber = i * fiberPitch;
      G4double yPosFiber = j * fiberPitch;
      G4cout << "Fiber position X: " << xPosFiber << " mm, Y: " << yPosFiber
             << " mm" << G4endl;
      new G4PVPlacement(nullptr, G4ThreeVector(xPosFiber, yPosFiber, 0.),
                        fiberLogical, "Fiber", absorberLogical, false,
                        (i + 1) * 3 + (j + 1));
    }
  }

  // Visualization attributes
  fpWorldLogical->SetVisAttributes(G4VisAttributes::GetInvisible());

  G4VisAttributes* absorberVisAtt = new G4VisAttributes(G4Colour::Gray());
  absorberVisAtt->SetVisibility(true);
  absorberLogical->SetVisAttributes(absorberVisAtt);

  G4VisAttributes* preAbsorberVisAtt = new G4VisAttributes(G4Colour::Brown());
  preAbsorberVisAtt->SetVisibility(true);
  preAbsorberLogical->SetVisAttributes(preAbsorberVisAtt);

  G4VisAttributes* fiberVisAtt = new G4VisAttributes(G4Colour::Blue());
  fiberVisAtt->SetVisibility(true);
  fiberLogical->SetVisAttributes(fiberVisAtt);

  G4VisAttributes* claddingVisAtt = new G4VisAttributes(G4Colour::White());
  claddingVisAtt->SetVisibility(true);
  claddingLogical->SetVisAttributes(claddingVisAtt);
}
