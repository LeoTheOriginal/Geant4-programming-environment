#ifndef MYSTEPPINGACTION_HH
#define MYSTEPPINGACTION_HH

#include "G4SystemOfUnits.hh"
#include "G4UserSteppingAction.hh"
#include "G4Types.hh"
#include <vector>

class MySteppingAction : public G4UserSteppingAction {
 public:
  MySteppingAction();
  ~MySteppingAction() override;

  void UserSteppingAction(const G4Step* step) override;


  private:
  G4double totalEnergyDeposit;
  std::vector<double> xPos, yPos, zPos; 
};

#endif