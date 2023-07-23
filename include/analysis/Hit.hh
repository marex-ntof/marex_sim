/**
 * @file Hit.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#pragma once

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

namespace Artie
{
    struct Hit
    {
        G4int copy_number = {0};
        G4int track_id = {0};
        G4int parent_track_id = {0};
        G4double local_time = {0};
        G4double global_time = {0};
        G4ThreeVector particle_position = {0,0,0};
        G4ThreeVector particle_momentum = {0,0,0};
        G4double energy = {0};
        G4bool detected = {false};

        Hit(
            G4int _copy_number, G4int _track_id,
            G4int _parent_track_id, G4double _local_time,
            G4double _global_time, G4ThreeVector _particle_position,
            G4ThreeVector _particle_momentum, G4double _energy
        )
        : copy_number(_copy_number), track_id(_track_id)
        , parent_track_id(_parent_track_id), local_time(_local_time)
        , global_time(_global_time), particle_position(_particle_position)
        , particle_momentum(_particle_momentum), energy(_energy)
        {
        }
    };
}