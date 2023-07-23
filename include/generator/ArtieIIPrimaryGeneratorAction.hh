/**
 * @file ArtieIIPrimaryGeneratorAction.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#pragma once
#include <memory>
#include <random>
#include <array>
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleGun.hh"
#include "G4IonTable.hh"
#include "G4Event.hh"
#include "Randomize.hh"
#include "EventManager.hh"

#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TGraph.h"
#include "TMath.h"
#include "TRandom3.h"

#include "Core.hh"
#include "EventManager.hh"

#include "yaml-cpp/yaml.h"

namespace Artie
{
    class ArtieIIPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
    {   
    public:
        ArtieIIPrimaryGeneratorAction();
        ~ArtieIIPrimaryGeneratorAction();

        virtual void GeneratePrimaries(G4Event* event);
        void ConstructEnergyDistribution();
        G4double SampleBeamEnergy();
        G4double SampleTOF(G4double beam_energy);
        G4ThreeVector SampleBeamProfile(G4double t_zero_location);
        G4ThreeVector SampleBeamMomentum(G4ThreeVector StartPosition);

        ArtieIIPrimaryGeneratorAction(YAML::Node config);
        YAML::Node Config() const { return mConfig; }

    private:
        G4ParticleGun* mParticleGun;

        G4String mParticleName;
        G4double mParticleMomentum;
        G4double mParticleEnergy;
        G4ThreeVector mParticlePosition;
        G4ThreeVector mParticleMomentumDirection;

        G4double mDetEntrance =     { 188 * m };
        G4double mTZeroLocation =   { -30.0 * m };
        G4double mEnergyCutLow =    { 40 * keV };
        G4double mEnergyCutHigh =   { 70 * keV };

        G4bool mUseLANLDistribution = { false };
        G4bool mUsenTOFDistribution = { false };
        G4bool mUseUniformDistribution = { false };

        G4bool mUseLANLBeamProfile = { false };
        G4bool mUsenTOFBeamProfile = { false };
        
        G4bool mUseLANLTOF = { false };
        G4bool mUsenTOFTOF = { false };

        G4double mBeamHoleSeparation =  { 3.0 * cm};
        G4int mLANLBeamLocation = {0};

        TH1D* mLANLEnergyDistribution = {0};
        TH2D* mLANLBeamProfile = {0};
        TH2D* mLANLTOF = {0};
        
        TH1D* mnTOFTOFDistribution = {0};
        TH2D* mnTOFBeamProfile = {0};
        TH2D* mnTOFTOF = {0};

        TRandom3* mTRandom3 = {0};

        YAML::Node mConfig;
    };
}