/**
 * @file ScintillationProcess.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @details Code adapted from Jason Brodsky's G4Integration code
 * in the NEST repository.
 * @date 2022-09-14
 */
#pragma once

#include <functional>

#include "G4Electron.hh"
#include "G4EmProcessSubType.hh"  
#include "G4ParticleTable.hh"
#include "G4ParticleTypes.hh"     
#include "G4PhysicalConstants.hh"
#include "G4ProductionCuts.hh"
#include "G4RandomDirection.hh"
#include "G4SystemOfUnits.hh"
#include "G4UserLimits.hh"
#include "G4Version.hh"
#include "StackingAction.hh"
#include "G4Track.hh"
#include "G4DynamicParticle.hh"
#include "G4Material.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4OpticalPhoton.hh"
#include "G4ParticleMomentum.hh"
#include "G4PhysicsOrderedFreeVector.hh"
#include "G4Poisson.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4VRestDiscreteProcess.hh"
#include "Randomize.hh"
#include "globals.hh"
#include "templates.hh"
#include "G4MaterialCutsCouple.hh"
#include "G4ProductionCuts.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"

#include "ThermalElectron.hh"
#include "EventManager.hh"

namespace marex
{
    class NoTimeParticleChange : public G4ParticleChange 
    {
    public:
        NoTimeParticleChange() : G4ParticleChange() { debugFlag = false; }
    };

    
    class ScintillationProcess : public G4VRestDiscreteProcess 
    {
    public:
        ScintillationProcess();
        ~ScintillationProcess();

    public:
        // Returns true -> 'is applicable', for any particle type except for an
        // 'opticalphoton' and for short-lived particles
        G4bool IsApplicable(const G4ParticleDefinition& aParticleType);
        
        // Returns infinity; i. e. the process does not limit the step, but
        // sets the 'StronglyForced' condition for the DoIt to be invoked at
        // every step.
        G4double GetMeanFreePath(const G4Track& aTrack, G4double, G4ForceCondition*);

        // Returns infinity; i. e. the process does not limit the time, but
        // sets the 'StronglyForced' condition for the DoIt to be invoked at
        // every step.
        G4double GetMeanLifeTime(const G4Track& aTrack, G4ForceCondition*);

        // For in-flight particles losing energy (or those stopped)
        G4VParticleChange* PostStepDoIt(const G4Track& aTrack, const G4Step& aStep);
        G4VParticleChange* AtRestDoIt(const G4Track& aTrack, const G4Step& aStep);

        // Called to set the scintillation quantum yield factor, useful for
        // shutting off scintillation entirely, or for producing a universal
        // re-scaling to (for example) represent detector effects. Internally is
        // used for Lindhard yield factor for NR. Default should be user-set
        // to be 1 (for ER) in your simulation -- see NEST readme
        void SetPhotonYieldFactor(const G4double yieldfactor);
        void SetElectronYieldFactor(const G4double yieldfactor);
        
        // Returns the quantum (photon/electron) yield factor. See above.
        G4double GetPhotonYieldFactor() const;
        G4double GetElectronYieldFactor() const;
        
        void MakeSinglePhoton(NESTInterfaceResult&);
        void MakeSingleElectron(NESTInterfaceResult&);

        void MakePhotons(NESTInterfaceResult&);
        void MakeElectrons(NESTInterfaceResult&);

        void SetDetailedSecondaries(bool detailed)  { mDetailedSecondaries = detailed; }
        void SetStackElectrons(bool stack_e)        { mStackElectrons = stack_e; }
        void SetStackPhotons(bool stack_ph)         { mStackPhotons = stack_ph; }
        void SetGammaBreak(double _gamma_break)     { this->mGammaBreak = _gamma_break; }
        double GetGammaBreak() const { return mGammaBreak; }
        

    protected:
        std::shared_ptr<NESTInterface> mNESTInterface = {nullptr};
        NoTimeParticleChange mParticleChange;

        G4double mPhotonYieldFactor = 1;  
        G4double mElectronYieldFactor = 1;
        G4bool mDetailedSecondaries = true;
        G4bool mStackElectrons = true;
        G4bool mStackPhotons = true;

        // Gammas will not pass on their lineage (if they have one, e.g.
        // bremsstrahlung) if they are this far from their origin.
        G4double mGammaBreak = 9 * mm;  

    private:
        const G4String mName = {"ScintillationProcess"};
        G4ProcessType mProcessType = {fElectromagnetic};
        G4EmProcessSubType mProcessSubType = {fScintillation};

    };

    inline G4bool ScintillationProcess::IsApplicable(
        const G4ParticleDefinition& aParticleType
    ) 
    {
        if (
            aParticleType.GetParticleName() == "opticalphoton" || 
            aParticleType.GetParticleName() == "thermalelectron"
        ) { 
            return false;
        }
        if (aParticleType.IsShortLived()) return false;
        return true;
    }

    inline void ScintillationProcess::SetPhotonYieldFactor(const G4double yieldfactor)
    {
        mPhotonYieldFactor = yieldfactor;
    }
    inline void ScintillationProcess::SetElectronYieldFactor(const G4double yieldfactor)
    {
        mElectronYieldFactor = yieldfactor;
    }

    inline G4double ScintillationProcess::GetPhotonYieldFactor() const 
    {
        return mPhotonYieldFactor;
    }
    inline G4double ScintillationProcess::GetElectronYieldFactor() const 
    {
        return mElectronYieldFactor;
    }
}