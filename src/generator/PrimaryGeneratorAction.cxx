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
        if(mConfig["generator"]["energy_cut_low"])   { 
            mEnergyCutLow = mConfig["generator"]["energy_cut_low"].as<G4double>() * keV; 
        }
        if(mConfig["generator"]["energy_cut_high"])  { 
            mEnergyCutHigh = mConfig["generator"]["energy_cut_high"].as<G4double>() * keV; 
        }
        
        std::string distribution_type = mConfig["generator"]["energy_distribution"]["distribution_type"].as<std::string>();
        std::string profile_type = mConfig["generator"]["beam_profile"]["profile_type"].as<std::string>();
        std::string tof_type = mConfig["generator"]["time_of_flight"]["tof_type"].as<std::string>();

        // Set up energy distribution 
        if(distribution_type == "ntof")
        {
            mUsenTOFDistribution = true;
            mnTOFTOFDistribution = EventManager::GetEventManager()->GetnTOFTOFDistribution();
        }
        else
        {
            mUseUniformDistribution = true;
        }
        if(profile_type == "ntof")
        {
            mUsenTOFBeamProfile = true;
            mnTOFBeamProfile = EventManager::GetEventManager()->GetnTOFBeamProfile();
        }
        if(tof_type == "ntof")
        {
            mUsenTOFTOF = true;
        }        
        
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
        if(mUsenTOFDistribution) {
            G4double ranTOF = mnTOFTOFDistribution->GetRandom() * ns;
            return EventManager::GetEventManager()->GetEnergyFromTOF(ranTOF) * MeV;
        }
        else {
            // Uniform in TOF
            // auto Manager = EventManager::GetEventManager();
            // G4double lenFlightPath = mDetEntrance - mTZeroLocation;
            // G4double tofLow = Manager->GetNominalTOF(mEnergyCutHigh);
            // G4double tofHigh = Manager->GetNominalTOF(mEnergyCutLow);
            // G4double n_tof = tofLow + (tofHigh - tofLow) * G4UniformRand();
            // G4double n_energy = 0.5 * NeutronMassMeV() * lenFlightPath * lenFlightPath / (SpeedOfLight() * SpeedOfLight() * n_tof * n_tof);
            // return (n_energy * MeV);

            // Uniform in Energy
            return (mEnergyCutLow + (mEnergyCutHigh - mEnergyCutLow) * G4UniformRand());
        }
    }

    G4double PrimaryGeneratorAction::SampleTOF(G4double beam_energy)
    {
        if(mUsenTOFTOF)
        {
            Double_t nominalTOF = EventManager::GetEventManager()->GetNominalTOF(beam_energy);
            Double_t nominalVelocity = EventManager::GetEventManager()->GetNominalVelocity(beam_energy);
            mnTOFTOF = EventManager::GetEventManager()->GetnTOFTOF();
            Int_t energy_bin = mnTOFTOF->GetXaxis()->FindBin(beam_energy * MeV);
            TH1D* TOF = EventManager::GetEventManager()->GetnTOFTOFProjection(energy_bin);
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

        if(mUsenTOFBeamProfile) {
            mnTOFBeamProfile->GetRandom2(x, y, mTRandom3);
        }
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
        G4double deltaTOF = SampleTOF(BeamEnergy);
        G4ThreeVector StartPosition = SampleBeamProfile(mTZeroLocation);
        G4ThreeVector StartMomentum = SampleBeamMomentum(StartPosition);
        mParticleGun->SetParticleTime(deltaTOF);
        mParticleGun->SetParticlePosition(StartPosition);
        mParticleGun->SetParticleMomentumDirection(StartMomentum);
        mParticleGun->SetParticleEnergy(BeamEnergy);
        mParticleGun->GeneratePrimaryVertex(event);
    }
}