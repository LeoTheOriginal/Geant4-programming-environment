#include "MyEventAction.hh"
#include "G4GenericAnalysisManager.hh" 
#include "G4Event.hh"
#include "G4AnalysisManager.hh"

MyEventAction::MyEventAction() : G4UserEventAction() {}

MyEventAction::~MyEventAction() {}

void MyEventAction::BeginOfEventAction(const G4Event*) {}

void MyEventAction::EndOfEventAction(const G4Event* event) {}
