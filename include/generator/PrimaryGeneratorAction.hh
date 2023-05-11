/**
 * @file PrimaryGeneratorAction.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#pragma once
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleGun.hh"
#include "G4IonTable.hh"
#include "G4Event.hh"

#include "Core.hh"

namespace marex
{
    class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
    {   
    public:
        PrimaryGeneratorAction();
        ~PrimaryGeneratorAction();

        PrimaryGeneratorAction(
            G4int numberOfParticles, 
            G4String particleName,
            G4double momentum = {0. * MeV},
            G4double energy = {0. * MeV},
            G4ThreeVector position = {G4ThreeVector(0.,0.,0.)}, 
            G4ThreeVector momentumDirection = {G4ThreeVector(0.,0.,1.)}
        );

        void SetPrimaries(std::vector<PrimaryGeneration> primaries);

        void SetNumberOfParticles(G4int numberOfParticles);
        void SetParticleName(G4String particleName);
        void SetParticleMomentum(G4double momentum);
        void SetParticleEnergy(G4double energy);
        void SetParticlePosition(G4ThreeVector position);
        void SetParticleMomentumDirection(G4ThreeVector momentumDirection);

        // method to access particle gun
        G4int GetNumberOfParticles() const                  { return mNumberOfParticles; }
        G4ParticleDefinition* GetParticle() const           { return mParticle; }

        G4String GetParticleName() const                    { return mParticleName; }
        G4double GetParticleMomentum() const                { return mParticleMomentum; }
        G4double GetParticleEnergy() const                  { return mParticleEnergy; }
        G4ThreeVector GetParticlePosition() const           { return mParticlePosition; }
        G4ThreeVector GetParticleMomentumDirection() const  { return mParticleMomentumDirection; }
        G4ParticleGun* GetParticleGun() const               { return mParticleGun; }

        virtual void GeneratePrimaries(G4Event* event);

    private:
        G4ParticleGun* mParticleGun;

        G4int mNumberOfParticles;
        G4ParticleTable* mParticleTable;
        G4IonTable* mIonTable;
        G4ParticleDefinition* mParticle;

        G4String mParticleName;
        G4double mParticleMomentum;
        G4double mParticleEnergy;
        G4ThreeVector mParticlePosition;
        G4ThreeVector mParticleMomentumDirection;
        
        std::vector<PrimaryGeneration> mPrimaries;
    };
}