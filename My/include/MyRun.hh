#ifndef MYRUN_HH
#define MYRUN_HH

#include "G4Run.hh"

class MyRun : public G4Run {
public:
  MyRun();
  virtual ~MyRun();

  void RecordEvent(const G4Event* event) override;

private:
  G4int fPhotonCollectionId;
};

#endif
