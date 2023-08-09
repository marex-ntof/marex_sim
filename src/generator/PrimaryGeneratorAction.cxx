/**
 * @file PrimaryGeneratorAction.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-12-13
 */
#include "PrimaryGeneratorAction.hh"

namespace marex
{
    PrimaryGeneratorAction::PrimaryGeneratorAction()
    : G4VUserPrimaryGeneratorAction()
    {

        mParticleGun = new G4ParticleGun();
        G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle("neutron");
        mParticleGun->SetNumberOfParticles(1);
        mParticleGun->SetParticleEnergy(57*keV);
        mParticleGun->SetParticlePosition(G4ThreeVector(0.,0.,0.));
        mParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
        mParticleGun->SetParticleDefinition(particle);
    }

    PrimaryGeneratorAction::~PrimaryGeneratorAction()
    {
    }

    PrimaryGeneratorAction::PrimaryGeneratorAction(YAML::Node config)
    : G4VUserPrimaryGeneratorAction()
    , mConfig(config)
    {
        if(mConfig["detector"]["detector_entrance"]) {
            mDetEntrance = mConfig["detector"]["detector_entrance"].as<G4double>() * m;
        }        
        if(mConfig["generator"]["t_zero_location"])  { 
            mTZeroLocation = mConfig["generator"]["t_zero_location"].as<G4double>() * m; 
        }
        
        std::string distribution_type = mConfig["generator"]["distribution_type"].as<std::string>();
        std::string moderator_type = mConfig["generator"]["moderator_distribution"]["tof_type"].as<std::string>();
        
        // Set up energy distribution 
        if(distribution_type == "tof")
        {
            mUseTOFDistribution = true;
            mTOFDistribution = EventManager::GetEventManager()->GetTOFDistribution();
        }
        else {
            mUseTOFDistribution = false;
            mEnergyDistribution = EventManager::GetEventManager()->GetEnergyDistribution();
        }
        if(moderator_type == "ntof")
        {
            mUseModerator = true;
        }

        mBeamProfile = EventManager::GetEventManager()->GetBeamProfile();
        
        mTRandom3 = new TRandom3();
        mParticleGun = new G4ParticleGun();
        G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle("neutron");
        mParticleGun->SetNumberOfParticles(1);
        mParticleGun->SetParticleEnergy(57*keV);
        mParticleGun->SetParticlePosition(G4ThreeVector(0.,0., mTZeroLocation));
        mParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
        mParticleGun->SetParticleDefinition(particle);
    }

    G4double PrimaryGeneratorAction::SampleBeamEnergy()
    {
        if(mUseTOFDistribution) {
            G4double ranTOF = mTOFDistribution->GetRandom();
            return EventManager::GetEventManager()->GetEnergyFromTOF(ranTOF) * MeV;
        }
        else {
            return mEnergyDistribution->GetRandom();
        }
    }

    G4double PrimaryGeneratorAction::SampleModerator(G4double beam_energy)
    {
        if(mUseModerator)
        {
            Double_t nominalTOF = EventManager::GetEventManager()->GetNominalTOF(beam_energy);
            Double_t nominalVelocity = EventManager::GetEventManager()->GetNominalVelocity(beam_energy);
            mModerator = EventManager::GetEventManager()->GetModeratorDistribution();
            Int_t energy_bin = mModerator->GetXaxis()->FindBin(beam_energy * MeV);
            TH1D* TOF = EventManager::GetEventManager()->GetModeratorProjection(energy_bin);
            Double_t lambda = TOF->GetRandom() * cm;   
            return lambda / (nominalVelocity);
        }
        return 0.0;
    }

    G4ThreeVector PrimaryGeneratorAction::SampleBeamProfile(
        G4double t_zero_location
    )
    {
        Double_t x = 0.0;
        Double_t y = 0.0;
        mBeamProfile->GetRandom2(x, y, mTRandom3);
        return G4ThreeVector(x, y, t_zero_location);
    }

    G4ThreeVector PrimaryGeneratorAction::SampleBeamMomentum(
        G4ThreeVector StartPosition
    )
    {
        return G4ThreeVector(0, 0, 1);
    }

    void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
    {
        mParticleGun->SetNumberOfParticles(1);
        G4double BeamEnergy = SampleBeamEnergy();
        G4double deltaTOF = SampleModerator(BeamEnergy);
        G4ThreeVector StartPosition = SampleBeamProfile(mTZeroLocation);
        G4ThreeVector StartMomentum = SampleBeamMomentum(StartPosition);
        mParticleGun->SetParticleTime(deltaTOF);
        mParticleGun->SetParticlePosition(StartPosition);
        mParticleGun->SetParticleMomentumDirection(StartMomentum);
        mParticleGun->SetParticleEnergy(BeamEnergy);
        mParticleGun->GeneratePrimaryVertex(event);
    }
}