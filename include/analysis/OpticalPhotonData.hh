/**
 * @file OpticalPhoton.hh
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
    struct OpticalPhotonData
    {
        G4int track_id = {0};
        G4int parent_track_id = {0};
        G4int parent_pdg = {0};
        G4double total_energy = {0};
        G4double track_length = {0};

        OpticalPhotonData(
            G4int _track_id, G4int _parent_track_id,
            G4int _parent_pdg, G4double _total_energy,
            G4double _track_length
        )
        : track_id(_track_id), parent_track_id(_parent_track_id)
        , parent_pdg(_parent_pdg), total_energy(_total_energy)
        , track_length(_track_length)
        {
        }
    };
}
