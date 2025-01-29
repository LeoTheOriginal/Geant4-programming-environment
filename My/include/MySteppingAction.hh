#ifndef MYSTEPPINGACTION_HH
#define MYSTEPPINGACTION_HH

#include "G4SystemOfUnits.hh"  // Add this line
#include "G4UserSteppingAction.hh"
#include "G4Types.hh"

class MySteppingAction : public G4UserSteppingAction {
 public:
  MySteppingAction();
  ~MySteppingAction() override;

  void UserSteppingAction(const G4Step* step) override;


  private:
  G4double totalEnergyDeposit; // Add this line
};

#endif