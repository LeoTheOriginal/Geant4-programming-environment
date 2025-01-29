#ifndef MYRUNACTION_HH
#define MYRUNACTION_HH

#include "G4UserRunAction.hh"

class G4Run;

class MyRunAction : public G4UserRunAction {
 public:
  MyRunAction();
  ~MyRunAction() override;

  void BeginOfRunAction(const G4Run*) override;
  void EndOfRunAction(const G4Run*) override;
};

#endif