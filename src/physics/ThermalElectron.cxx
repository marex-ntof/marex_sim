/**
 * @file ThermalElectron.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @date 2022-10-05
 */
#include "ThermalElectron.hh"

namespace marex
{
    ThermalElectron* ThermalElectron::sInstance = 0;

    const G4String ThermalElectron::sName = "thermalelectron";

    ThermalElectron* ThermalElectron::Definition() 
    {
        if (sInstance != 0) {
            return sInstance;
        }

        // otherwise create the particle
        G4ParticleTable* pTable = G4ParticleTable::GetParticleTable();
        G4ParticleDefinition* ThermalElectronInstance = pTable->FindParticle(sName);
        if (ThermalElectronInstance == 0) 
        {
            ThermalElectronInstance = new G4ParticleDefinition(
                sName,              // name
                electron_mass_c2,   // mass (0.51099906 * MeV)
                0.0 * MeV,          // width
                -1. * eplus,        // charge
                1,                  // 2 x spin
                0,                  // parity
                0,                  // charge-conjugation
                0,                  // 2 x isospin
                0,                  // 2 x isospin3
                0,                  // g-parity
                "lepton",           // type
                1,                  // lepton number
                0,                  // baryon number
                11,                 // PDG encoding
                true,               // stable?
                -1.0,               // lifetime
                NULL,               // decay table
                false,              // shortlived?
                "e"                 // sub-type
            );   
            // Bohr Magnetron
            G4double MagneticMoment = (-0.5 * eplus * hbar_Planck * c_squared) / (electron_mass_c2);
            G4double g = 2. * 1.0011596521859;
            ThermalElectronInstance->SetPDGMagneticMoment(g * MagneticMoment);
        }
        sInstance = reinterpret_cast<ThermalElectron*>(ThermalElectronInstance);
        return sInstance;
    }
}