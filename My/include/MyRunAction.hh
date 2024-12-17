#ifndef MYRUNACTION_HH
#define MYRUNACTION_HH

#include "G4UserRunAction.hh"

class MyRunAction : public G4UserRunAction {
public:
  MyRunAction();
  virtual ~MyRunAction();

  virtual void BeginOfRunAction(const G4Run*);
  virtual void EndOfRunAction(const G4Run*);

  virtual G4Run* GenerateRun();
};

#endif
