#ifndef MYPHYSICSLIST_HH
#define MYPHYSICSLIST_HH

#include "G4VUserPhysicsList.hh"

class MyPhysicsList: public G4VUserPhysicsList {
public:
  MyPhysicsList();
  virtual ~MyPhysicsList();

protected:
  virtual void ConstructParticle();
  virtual void ConstructProcess();
  virtual void SetCuts();

private:
  void ConstructEM();
  void ConstructGeneral();
};

#endif
