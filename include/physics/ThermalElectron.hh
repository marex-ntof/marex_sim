/**
 * @file ThermalElectron.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @date 2022-10-05
 */
#pragma once

#include "G4Electron.hh"
#include "G4ParticleTable.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

namespace marex
{
    class ThermalElectron : public G4ParticleDefinition 
    {
    public:
        static ThermalElectron* Definition();

    private:
        static ThermalElectron* sInstance;
        ThermalElectron() {}
        ~ThermalElectron() {}

        static const G4String sName;

    };
}