#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"

// Nasze klasy:
#include "MyDetectorConstruction.hh"
#include "MyPhysicsList.hh"
#include "MyPrimaryGeneratorAction.hh"
#include "MyRunAction.hh"
#include "MySteppingAction.hh"
#include "G4SystemOfUnits.hh"

// (Jeśli używasz w tym projekcie EventAction, SteppingAction – dołączyć je tu)

int main(int argc, char** argv)
{
  // 1) Stworzenie run managera
  G4RunManager* runManager = new G4RunManager;

  // 2) Ustawiamy klasy inicjalizacyjne (detektor, fizyka, generator)
  runManager->SetUserInitialization(new MyDetectorConstruction());
  runManager->SetUserInitialization(new MyPhysicsList());

  // (Jeśli używasz niestandardowego ActionInitialization,
  //  można w nim zarejestrować MyPrimaryGeneratorAction, MyRunAction itp. 
  //  ale najprościej tak:)
  runManager->Initialize();

  // 3) Ustawiamy akcje użytkownika „na piechotę” (lub można w ActionInitialization)
  runManager->SetUserAction(new MyPrimaryGeneratorAction());
  runManager->SetUserAction(new MyRunAction());
  runManager->SetUserAction(new MySteppingAction());
  // ewentualnie:
  //   runManager->SetUserAction(new MyEventAction());
  //   runManager->SetUserAction(new MySteppingAction());
  
  // 4) UI i wizualizacja:
  G4UImanager* UI = G4UImanager::GetUIpointer();

  // Sprawdzamy, czy uruchamiamy się w trybie wsadowym (batch) – z plikiem makr,
  // czy w trybie interaktywnym (bez parametru lub z parametrem).
  if (argc == 1)
  {
    // 4a) Tryb interaktywny
    //    - uruchamiamy UIExecutive (np. interfejs terminalowy G4)
    //    - odpalimy makro z wizualizacją

    // Inicjalizujemy wizualizację
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();

    // Uruchamiamy interfejs:
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
    
    // Załaduj nasze makro do wizualizacji (init_vis.mac)
    //  można je też odpalić ręcznie z konsoli, ale często robi się tak:
    UI->ApplyCommand("/control/execute init_vis.mac");

    // Start sesji
    ui->SessionStart();

    delete ui;
    delete visManager;
  }
  else
  {
    // 4b) Tryb wsadowy (makro podane jako argument, np. ./My run.mac)
    G4String macroFileName = argv[1];
    G4String command = "/control/execute " + macroFileName;
    UI->ApplyCommand(command);
  }

  // 5) Koniec pracy
  delete runManager;
  return 0;
}
