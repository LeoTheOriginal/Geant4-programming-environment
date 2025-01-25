#ifndef MYEVENTACTION_HH
#define MYEVENTACTION_HH

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4GenericAnalysisManager.hh"


class G4Event;
class G4HCofThisEvent;

class MyEventAction : public G4UserEventAction {
public:
    MyEventAction();
    virtual ~MyEventAction();

    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);

private:
    G4VAnalysisManager* fAnalysisManager;
};

#endif
