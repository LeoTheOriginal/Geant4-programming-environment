#include "MyDetectorConstruction.hh"
#include "MyPhysicsList.hh"
#include "MyPrimaryGeneratorAction.hh"
#include "MyRunAction.hh"
#include "MyRun.hh"
#include "MyEventAction.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

int main(int argc, char** argv)
{
  // Run manager
  G4RunManager* runManager = new G4RunManager;

  // Inicjalizacja detektora i fizyki
  runManager->SetUserInitialization(new MyDetectorConstruction);
  runManager->SetUserInitialization(new MyPhysicsList);

  // Ustawienia akcji
  runManager->SetUserAction(new MyPrimaryGeneratorAction);
  runManager->SetUserAction(new MyRunAction);
  runManager->SetUserAction(new MyEventAction);

  // Inicjalizacja Geant4 kernel
  runManager->Initialize();

  // Wizualizacja
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

  // UI manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if (argc == 1) {
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  } else {
    G4String macro = argv[1];
    UImanager->ApplyCommand("/control/execute " + macro);
  }

  delete visManager;
  delete runManager;

  return 0;
}
