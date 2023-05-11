/**
 * @file ThermalElectronData.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @date 2022-10-17
 */
#pragma once
#include <vector>

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

namespace marex
{
    struct ThermalElectronData
    {
        G4int track_id = {0};
    };
}