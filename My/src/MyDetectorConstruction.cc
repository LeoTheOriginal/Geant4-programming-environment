#include "MyDetectorConstruction.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4Material.hh"
#include "G4PVParameterised.hh"
#include "G4RunManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4SDManager.hh"
#include "G4PSSphereSurfaceCurrent.hh"
#include "G4Sphere.hh"

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
//////
G4String symbol;             
G4double a, z, density;     
G4int ncomponents, natoms;
G4double fractionmass;	
// Define simple materials
// Define beryllium, silicon and iron 

new G4Material("Titanium",  z=22., a=47.90*g/mole,    density=4.540*g/cm3);
new G4Material("Beryllium", z=4.,  a=9.012182*g/mole, density=1.8480*g/cm3);
new G4Material("Silicon",   z=14., a=28.0855*g/mole,  density=2.330*g/cm3);
new G4Material("Iron",      z=26., a=55.845*g/mole,   density=7.87*g/cm3);

new G4Material("Copper", z=29, a=63.546*g/mole, density=8.920*g/cm3);


// Define elements
G4Element* N = new G4Element("Nitrogen", symbol="N", z=7., a=14.01*g/mole);
G4Element* O = new G4Element("Oxygen",   symbol="O", z=8., a=16.00*g/mole);
G4Element* Si = new G4Element("Silicon", symbol="Si", z=14., a=28.09*g/mole);
G4Element* C = new G4Element ("Carbon", symbol="C", z=6, a=12.01*g/mole);
G4Element* H = new G4Element ("Hydrogen", symbol="H", z=1, a=1.01*g/mole);

//define quartz
G4Material* SiO2 = new G4Material("Quartz", density=2.65*g/cm3, ncomponents=2);
SiO2->AddElement(Si, natoms=1);
SiO2->AddElement(O, natoms=2);

// Define air
G4Material* air = new G4Material("Air", density= 1.290*mg/cm3, ncomponents=2);
air->AddElement(N, fractionmass=0.7);
air->AddElement(O, fractionmass=0.3);

// Define vacuum
G4Material* vacuum = new G4Material("Vacuum", density= 1.e-5*g/cm3, 
				  ncomponents=1, kStateGas, CLHEP::STP_Temperature, 
				  2.e-2*bar);

vacuum->AddMaterial(air, fractionmass=1.);

//Define PMMA (C502H8)
// NIST reference 
G4Material* PMMA = new G4Material("PMMA", 1.19*g/cm3, 3);
PMMA -> AddElement(C, 5);
PMMA -> AddElement(O, 2);
PMMA -> AddElement(H, 8);

// Dump material information
G4cout << *(G4Material::GetMaterialTable()) << G4endl;
///////
G4NistManager* nist = G4NistManager::Instance();
// Powietrze do świata
//nist->FindOrBuildMaterial("G4_AIR");
// Miedź
//nist->FindOrBuildMaterial("G4_Cu");
// Kwarc (SiO2)
//nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
//nist->FindOrBuildMaterial("SiO2");

}

void MyDetectorConstruction::SetupGeometry() {
  G4NistManager* nist = G4NistManager::Instance();
  G4Material* air   = nist->FindOrBuildMaterial("Air");
  G4Material* copper= nist->FindOrBuildMaterial("Copper");
  G4Material* quartz= nist->FindOrBuildMaterial("Quartz");
  G4Material* PMMA= nist->FindOrBuildMaterial("PMMA");


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
         
 // absorber 
	G4double absX = 1.0*cm; 
	G4double absY = 1.0*cm;
	G4double absZ = 10.*cm; 
	
  G4Box* copperAbsorberSolid = new G4Box("copperAbsorber_Solid", absX/2., absY/2., absZ/2.);
  G4LogicalVolume* copperAbsorberLogical = 
  new G4LogicalVolume(copperAbsorberSolid, copper, "copperAbsorber_Logical");

  new G4PVPlacement(0, G4ThreeVector(0,0,25*cm+absZ/2), copperAbsorberLogical, "copperAbsorber_Physical",
                    fWorldLogical, false, 0);
	

  // Włókna kwarcowe:
  // Promień włókna: 0.625 mm = 0.0625 cm
  // Długość włókna: 50 cm (tak jak blok)
  G4double fiberRadius = 0.625*mm;
  G4double fiberLength = 50.*cm;
  G4Tubs* fiberSolid = new G4Tubs("Fiber_Solid", 0., fiberRadius, fiberLength/2., 0.*deg, 360.*deg);

  G4LogicalVolume* fiberLogical = 
    new G4LogicalVolume(fiberSolid, quartz, "Fiber_Logical");
    
    
  //Powłoki PMMA
  G4double PMMA_thickness=0.1*mm;

  
  G4Tubs* PMMA_Solid = new G4Tubs("PMMA_tube", // name
	fiberRadius, // inner radius
	fiberRadius+PMMA_thickness, // outer radius
	fiberLength/2., // half length in Z
	0.*deg, // the starting phi angle
	360.*deg); // the angle of the segment
	
  G4LogicalVolume* PMMA_Logical = 
    new G4LogicalVolume(PMMA_Solid, PMMA, "PMMA_Logical");


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
      new G4PVPlacement(0, pos, fiberLogical, "Fiber_Physical", copperBlockLogical, false, (i+1)*3+(j+1));
                        
	  new G4PVPlacement(0, pos, PMMA_Logical, "PMMA_Physical", copperBlockLogical, false, (i+1)*3+(j+1));
                        
    }
  }
  

  // Atrybuty wizualizacji
  fWorldLogical->SetVisAttributes(G4VisAttributes::GetInvisible());

  G4VisAttributes* copperVis = new G4VisAttributes(G4Colour(0.7,0.4,0.4));
  copperVis->SetForceWireframe(true);
  copperBlockLogical->SetVisAttributes(copperVis);
 
  G4VisAttributes* copperAbsorberVis = new G4VisAttributes(G4Colour(0.8,0.5,0.5));
  copperAbsorberVis->SetForceSolid(true);
  copperAbsorberLogical->SetVisAttributes(copperAbsorberVis);

  G4VisAttributes* fiberVis = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  fiberVis->SetForceSolid(true);
  fiberLogical->SetVisAttributes(fiberVis);
  
  G4VisAttributes* PMMA_Vis = new G4VisAttributes(G4Colour(0.0,1.,0.7));
  PMMA_Vis->SetForceSolid(true);
  PMMA_Logical->SetVisAttributes(PMMA_Vis);
  

}
