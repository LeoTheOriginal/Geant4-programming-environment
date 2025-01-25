#include "MyPhysicsList.hh"
#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4VProcess.hh"
#include "G4Decay.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "G4BaryonConstructor.hh"
#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

// EM processes
#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"
#include "G4eMultipleScattering.hh"

#include "G4Cerenkov.hh"

MyPhysicsList::MyPhysicsList() : G4VUserPhysicsList() {
  defaultCutValue = 1.0*mm;
  SetVerboseLevel(1);
}

MyPhysicsList::~MyPhysicsList() {}

void MyPhysicsList::ConstructParticle() {
  // Cząstki
  G4BosonConstructor bosonConstruct; bosonConstruct.ConstructParticle();
  G4LeptonConstructor leptonConstruct; leptonConstruct.ConstructParticle();
  G4BaryonConstructor baryonConstruct; baryonConstruct.ConstructParticle();
  G4MesonConstructor mesonConstruct; mesonConstruct.ConstructParticle();
  G4IonConstructor ionConstruct; ionConstruct.ConstructParticle();
  G4ShortLivedConstructor shortLived; shortLived.ConstructParticle();
}

void MyPhysicsList::ConstructProcess() {
  AddTransportation();
  ConstructEM();
  ConstructGeneral();
}

void MyPhysicsList::ConstructEM() {
  auto theParticleIterator=GetParticleIterator();
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();

    if (particleName == "gamma") {
      pmanager->AddDiscreteProcess(new G4GammaConversion());
      pmanager->AddDiscreteProcess(new G4ComptonScattering());
      pmanager->AddDiscreteProcess(new G4PhotoElectricEffect());

    } else if (particleName == "e-") {
      pmanager->AddProcess(new G4Cerenkov(), -1,1,1);
      pmanager->AddProcess(new G4eIonisation,         -1,2,2);
      pmanager->AddProcess(new G4eBremsstrahlung,     -1,3,3);

    } else if (particleName == "e+") {
      pmanager->AddProcess(new G4Cerenkov(), -1,1,1);
      pmanager->AddProcess(new G4eIonisation,         -1,2,2);
      pmanager->AddProcess(new G4eBremsstrahlung,     -1,3,3);
      pmanager->AddProcess(new G4eplusAnnihilation,    0,-1,4);
      
    } else if (particleName == "opticalphoton") {
        auto cerenkov = new G4Cerenkov("Cerenkov");
        cerenkov->SetMaxNumPhotonsPerStep(300); // Maks. liczba fotonów na krok
        cerenkov->SetTrackSecondariesFirst(true);
        pmanager->AddProcess(cerenkov);
    }

  }
}

void MyPhysicsList::ConstructGeneral() {
  G4Decay* decayProcess = new G4Decay();
  auto theParticleIterator=GetParticleIterator();
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    if (decayProcess->IsApplicable(*particle)) {
      pmanager->AddProcess(decayProcess);
      pmanager->SetProcessOrdering(decayProcess, idxPostStep);
      pmanager->SetProcessOrdering(decayProcess, idxAtRest);
    }
  }
}

void MyPhysicsList::SetCuts() {
  SetCutValue(defaultCutValue, "gamma");
  SetCutValue(defaultCutValue, "e-");
  SetCutValue(defaultCutValue, "e+");
  if (verboseLevel>0) DumpCutValuesTable();
}
