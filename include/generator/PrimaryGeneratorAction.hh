/**
 * @file PrimaryGeneratorAction.hh
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

namespace marex
{
    class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
    {   
    public:
        PrimaryGeneratorAction();
        ~PrimaryGeneratorAction();

        virtual void GeneratePrimaries(G4Event* event);
        void ConstructEnergyDistribution();
        G4double SampleBeamEnergy();
        G4double SampleModerator(G4double beam_energy);
        G4ThreeVector SampleBeamProfile(G4double t_zero_location);
        G4ThreeVector SampleBeamMomentum(G4ThreeVector StartPosition);

        PrimaryGeneratorAction(YAML::Node config);
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

        G4bool mUseTOFDistribution = { false };
        G4bool mUseModerator = { false };
        
        TH1D* mEnergyDistribution = {0};
        TH1D* mTOFDistribution = {0};
        TH2D* mBeamProfile = {0};
        TH2D* mModerator = {0};

        TRandom3* mTRandom3 = {0};

        YAML::Node mConfig;
    };
}