/**
 * @file PrimaryGeneratorAction.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#include "PrimaryGeneratorAction.hh"

namespace marex
{
    PrimaryGeneratorAction::PrimaryGeneratorAction()
    {
        mParticleGun = new G4ParticleGun();
        mParticleTable = G4ParticleTable::GetParticleTable();
        mParticle = mParticleTable->FindParticle("geantino");
        mParticleGun->SetNumberOfParticles(1);
        mParticleGun->SetParticleMomentum(1. * MeV);
        mParticleGun->SetParticleEnergy(1. * MeV);
        mParticleGun->SetParticlePosition(G4ThreeVector(0.,0.,0.));
        mParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,1.,0.));
        mParticleGun->SetParticleDefinition(mParticle);
    }

    PrimaryGeneratorAction::~PrimaryGeneratorAction()
    {
    }
        
    PrimaryGeneratorAction::PrimaryGeneratorAction(
        G4int numberOfParticles, 
        G4String particleName,
        G4double momentum,
        G4double energy,
        G4ThreeVector position, 
        G4ThreeVector momentumDirection
    )
    : mNumberOfParticles(numberOfParticles)
    , mParticleName(particleName)
    , mParticleMomentum(momentum)
    , mParticleEnergy(energy)
    , mParticlePosition(position)
    , mParticleMomentumDirection(momentumDirection)
    {
        mParticleGun = new G4ParticleGun(mNumberOfParticles);
        mParticleTable = G4ParticleTable::GetParticleTable();
        mIonTable = G4IonTable::GetIonTable();
        mParticle = mParticleTable->FindParticle(mParticleName);

        mParticleGun->SetParticleMomentum(mParticleMomentum);
        mParticleGun->SetParticleEnergy(mParticleEnergy);
        mParticleGun->SetParticlePosition(mParticlePosition);
        mParticleGun->SetParticleMomentumDirection(mParticleMomentumDirection);
        
        mParticleGun->SetParticleDefinition(mParticle);
    }

    void PrimaryGeneratorAction::SetPrimaries(std::vector<PrimaryGeneration> primaries)
    {
        mPrimaries = primaries;
    }

    void PrimaryGeneratorAction::SetNumberOfParticles(G4int numberOfParticles)
    {
        mNumberOfParticles = numberOfParticles;
        mParticleGun->SetNumberOfParticles(mNumberOfParticles);
    }
    void PrimaryGeneratorAction::SetParticleName(G4String particleName)
    {
        mParticleName = particleName;
        mParticle = mParticleTable->FindParticle(mParticleName);
        mParticleGun->SetParticleDefinition(mParticle);
    }
    void PrimaryGeneratorAction::SetParticleMomentum(G4double momentum)
    {
        mParticleMomentum = momentum;
        mParticleGun->SetParticleMomentum(mParticleMomentum);
    }
    void PrimaryGeneratorAction::SetParticleEnergy(G4double energy)
    {
        mParticleEnergy = energy;
        mParticleGun->SetParticleEnergy(mParticleEnergy);
    }
    void PrimaryGeneratorAction::SetParticlePosition(G4ThreeVector position)
    {
        mParticlePosition = position;
        mParticleGun->SetParticlePosition(mParticlePosition);
    }
    void PrimaryGeneratorAction::SetParticleMomentumDirection(G4ThreeVector momentumDirection)
    {
        mParticleMomentumDirection = momentumDirection;
        mParticleGun->SetParticleMomentumDirection(mParticleMomentumDirection);
    }
    
    void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
    {
        for (auto primary : mPrimaries)
        {
            if(primary.ion == true)
            {
                auto ion = G4IonTable::GetIonTable()->GetIon(primary.Z, primary.A, primary.energy);
                mParticleGun->SetParticleDefinition(ion);
            }
            else {
                mParticleGun->SetParticleDefinition(primary.definition);
            }
            mParticleGun->SetNumberOfParticles(1);
            mParticleGun->SetParticleCharge(primary.charge);
            mParticleGun->SetParticleTime(primary.time);
            mParticleGun->SetParticlePosition(primary.position);
            mParticleGun->SetParticleEnergy(primary.energy);
            mParticleGun->SetParticleMomentumDirection(primary.momentum_direction);
            mParticleGun->GeneratePrimaryVertex(event);
        }
    }
}