/**
 * @file ScintillationPhysics.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#include "ScintillationPhysics.hh"

namespace marex
{
    ScintillationPhysics::ScintillationPhysics()
    : G4VPhysicsConstructor("ScintillationPhysics")
    {
    }

    ScintillationPhysics::~ScintillationPhysics()
    {
    }

    void ScintillationPhysics::ConstructProcess()
    {
        G4PhysicsListHelper* PhysicsListHelper = G4PhysicsListHelper::GetPhysicsListHelper();
    
        // Add standard EM Processes
        G4ParticleTable::G4PTblDicIterator* ParticleIterator = theParticleTable->GetIterator();
        ParticleIterator->reset();
        while ((*ParticleIterator)()) 
        {
            G4ParticleDefinition* particle = ParticleIterator->value();
            G4String particleName = particle->GetParticleName();
            
            if (particleName == "gamma") 
            {
                ////PhysicsListHelper->RegisterProcess(new G4RayleighScattering, particle);
                PhysicsListHelper->RegisterProcess(new G4PhotoElectricEffect, particle);
                G4ComptonScattering* cs = new G4ComptonScattering;
                cs->SetEmModel(new G4KleinNishinaModel());
                PhysicsListHelper->RegisterProcess(cs, particle);
                PhysicsListHelper->RegisterProcess(new G4GammaConversion, particle);
                
            } 
            else if (particleName == "e-") 
            {
                PhysicsListHelper->RegisterProcess(new G4eMultipleScattering(), particle);
                G4eIonisation* eIoni = new G4eIonisation();
                eIoni->SetStepFunction(0.1, 100 * um);
                PhysicsListHelper->RegisterProcess(eIoni, particle);
                PhysicsListHelper->RegisterProcess(new G4eBremsstrahlung(), particle);
                
            } 
            else if (particleName == "e+") 
            {
                PhysicsListHelper->RegisterProcess(new G4eMultipleScattering(), particle);
                G4eIonisation* eIoni = new G4eIonisation();
                eIoni->SetStepFunction(0.1, 100 * um);
                PhysicsListHelper->RegisterProcess(eIoni, particle);
                PhysicsListHelper->RegisterProcess(new G4eBremsstrahlung(), particle);
                PhysicsListHelper->RegisterProcess(new G4eplusAnnihilation(), particle);
                
            } 
            else if (particleName == "mu+" || particleName == "mu-") 
            {
                PhysicsListHelper->RegisterProcess(new G4MuMultipleScattering(), particle);
                G4MuIonisation* muIoni = new G4MuIonisation();
                muIoni->SetStepFunction(0.1, 50 * um);
                PhysicsListHelper->RegisterProcess(muIoni, particle);
                PhysicsListHelper->RegisterProcess(new G4MuBremsstrahlung(), particle);
                PhysicsListHelper->RegisterProcess(new G4MuPairProduction(), particle);
                
            } 
            else if (
                particleName == "proton" || 
                particleName == "pi-" ||
                particleName == "pi+"
            )
            {
                PhysicsListHelper->RegisterProcess(new G4hMultipleScattering(), particle);
                G4hIonisation* hIoni = new G4hIonisation();
                hIoni->SetStepFunction(0.1, 20 * um);
                PhysicsListHelper->RegisterProcess(hIoni, particle);
                PhysicsListHelper->RegisterProcess(new G4hBremsstrahlung(), particle);
                PhysicsListHelper->RegisterProcess(new G4hPairProduction(), particle);
                
            } 
            else if (particleName == "alpha" || particleName == "He3") 
            {
                PhysicsListHelper->RegisterProcess(new G4hMultipleScattering(), particle);
                G4ionIonisation* ionIoni = new G4ionIonisation();
                ionIoni->SetStepFunction(0.1, 20 * um);
                PhysicsListHelper->RegisterProcess(ionIoni, particle);
                PhysicsListHelper->RegisterProcess(new G4NuclearStopping(), particle);
                
            } 
            else if (particleName == "GenericIon") 
            {
                PhysicsListHelper->RegisterProcess(new G4hMultipleScattering(), particle);
                G4ionIonisation* ionIoni = new G4ionIonisation();
                ionIoni->SetEmModel(new G4IonParametrisedLossModel());
                ionIoni->SetStepFunction(0.1, 1 * um);
                PhysicsListHelper->RegisterProcess(ionIoni, particle);
                PhysicsListHelper->RegisterProcess(new G4NuclearStopping(), particle);
                
            } 
            else if (
                (!particle->IsShortLived()) &&
                (particle->GetPDGCharge() != 0.0) &&
                (particle->GetParticleName() != "chargedgeantino")
            ) 
            {
                PhysicsListHelper->RegisterProcess(new G4hMultipleScattering(), particle);
                PhysicsListHelper->RegisterProcess(new G4hIonisation(), particle);
            }
        }
    
        G4EmParameters* EmParameters = G4EmParameters::Instance();
        EmParameters->SetMinEnergy(10 * eV);      // default 100 eV
        EmParameters->SetMaxEnergy(10 * TeV);     // default 100 TeV
        EmParameters->SetNumberOfBinsPerDecade(12 * 10);    // default=12*7
        EmParameters->SetMscStepLimitType(fUseSafety);  // default
        
        G4VAtomDeexcitation* de = new G4UAtomicDeexcitation();
        de->SetFluo(true);
        de->SetAuger(false);
        de->SetPIXE(true);
        G4LossTableManager::Instance()->SetAtomDeexcitation(de);
        
        G4EmModelActivator mact(GetPhysicsName());
        ConstructScintillationProcess();
    }

    void ScintillationPhysics::ConstructScintillationProcess()
    {
        auto ParticleIterator = GetParticleIterator();
        ParticleIterator->reset();

        while((*ParticleIterator)())
        {
            G4ParticleDefinition* particle = ParticleIterator->value();
            G4ProcessManager* ProcessManager = particle->GetProcessManager();
            G4String particleName = particle->GetParticleName();
            if (ProcessManager) 
            {
                ScintillationProcess* scintillationProcess = new ScintillationProcess();
                scintillationProcess->SetDetailedSecondaries(true);
                scintillationProcess->SetStackElectrons(true);

                if (scintillationProcess->IsApplicable(*particle)) 
                {
                    ProcessManager->AddProcess(
                        scintillationProcess, 
                        ordDefault + 1, 
                        ordInActive, 
                        ordDefault + 1
                    );
                }

                G4OpBoundaryProcess* fBoundaryProcess = new G4OpBoundaryProcess();
                G4OpAbsorption* fAbsorptionProcess = new G4OpAbsorption();
                G4OpRayleigh* fRayleighScatteringProcess = new G4OpRayleigh();
                G4OpWLS* fTheWLSProcess = new G4OpWLS();

                if (particleName == "opticalphoton") 
                {
                    ProcessManager->AddDiscreteProcess(fAbsorptionProcess);
                    ProcessManager->AddDiscreteProcess(fRayleighScatteringProcess);
                    ProcessManager->AddDiscreteProcess(fTheWLSProcess);
                    ProcessManager->AddDiscreteProcess(fBoundaryProcess);
                }
            }
        }
    }
}