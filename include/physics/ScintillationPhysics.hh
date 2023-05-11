/**
 * @file ScintillationPhysics.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#pragma once

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"
#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4PhysicsListHelper.hh"
#include "G4SystemOfUnits.hh"

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4RayleighScattering.hh"
#include "G4KleinNishinaModel.hh"

#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#include "G4MuMultipleScattering.hh"
#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"

#include "G4hMultipleScattering.hh"
#include "G4hIonisation.hh"
#include "G4hBremsstrahlung.hh"
#include "G4hPairProduction.hh"

#include "G4ionIonisation.hh"
#include "G4IonParametrisedLossModel.hh"
#include "G4NuclearStopping.hh"

#include "G4MscStepLimitType.hh"

#include "G4LossTableManager.hh"
#include "G4UAtomicDeexcitation.hh"
#include "G4SystemOfUnits.hh"

#include "G4EmModelActivator.hh"
#include "G4EmParameters.hh"

#include "G4FastSimulationManagerProcess.hh"

#include "ScintillationProcess.hh"
#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpWLS.hh"
#include "G4OpBoundaryProcess.hh"

namespace marex
{
    class ScintillationPhysics : public G4VPhysicsConstructor
    {
    public:
        ScintillationPhysics();
        ~ScintillationPhysics();

        void ConstructProcess();
        void ConstructScintillationProcess();

        void ConstructParticle() {}

    private:

    };
}