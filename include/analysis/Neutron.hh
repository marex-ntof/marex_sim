/**
 * @file Neutron.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#pragma once

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

namespace Artie
{
    struct NeutronEventData
    {
        G4int event = {0};
        G4int track_id = {0};
        G4double neutron_energy = {0};
        G4double nominal_tof = {0};
        G4double start_time = {0};
        G4double start_x = {0};
        G4double start_y = {0};
        G4double start_z = {0};

        G4int detector = {-1};
        G4double arrival_time = {0};
        G4double arrival_energy = {0};
        G4int num_elastic = {0};
        G4int num_inelastic = {0};
        G4int num_capture = {0};
        G4int num_fission = {0};
        G4int num_scatter = {0};
        G4int num_scatter_out = {0};
        G4int gas_first = {0};
        G4double first_scatter_x = {0};
        G4double first_scatter_y = {0};
        G4double first_scatter_z = {0};
        G4double first_scatter_t = {0};
        G4double second_scatter_x = {0};
        G4double second_scatter_y = {0};
        G4double second_scatter_z = {0};
        G4double second_scatter_t = {0};
        G4double max_dphi = {0};
        G4double max_dp = {0};
        G4double max_dE = {0};
        G4double first_target_step_time = {0};
        G4double first_target_step_energy = {0};
        G4double first_target_step_z = {0};
        G4int reached_lar = {0};
        G4int exited_lar = {0};
        G4int safe_passage = {0};

        NeutronEventData()
        {}

        NeutronEventData(
            G4int _event, G4int _track_id,
            G4double _neutron_energy,
            G4double _nominal_tof,
            G4double _start_time,
            G4ThreeVector _start_pos
        )
        : event(_event), track_id(_track_id)
        , neutron_energy(_neutron_energy)
        , nominal_tof(_nominal_tof)
        , start_time(_start_time)
        {
            start_x = _start_pos[0];
            start_y = _start_pos[1];
            start_z = _start_pos[2];
        }

        NeutronEventData(
            G4int _event, G4int _track_id,
            G4double _neutron_energy, G4double _nominal_tof,
            G4double _start_time, G4double _arrival_time,
            G4double _arrival_energy, G4int _num_elastic, G4int _num_inelastic,
            G4int _num_capture, G4int _num_fission,
            G4int _num_scatter, G4int _num_scatter_out,
            G4int _gas_first, G4double _first_scatter_z,
            G4double _max_dphi, G4double _max_dp, G4double _max_dE
        )
        : event(_event), track_id(_track_id)
        , neutron_energy(_neutron_energy), nominal_tof(_nominal_tof)
        , start_time(_start_time), arrival_time(_arrival_time)
        , arrival_energy(_arrival_energy), num_elastic(_num_elastic), num_inelastic(_num_inelastic)
        , num_capture(_num_capture), num_fission(_num_fission)
        , num_scatter(_num_scatter), num_scatter_out(_num_scatter_out)
        , gas_first(_gas_first), first_scatter_z(_first_scatter_z)
        , max_dphi(_max_dphi), max_dp(_max_dp), max_dE(_max_dE)
        {
        }
    };

    struct NeutronRunData
    {
        G4int num_events = {0};
        G4int num_detected = {0};
        G4int num_elastic = {0};
        G4int num_inelastic = {0};
        G4int num_capture = {0};
        G4int num_fission = {0};
        G4int num_scatter = {0};
        G4int num_scatter_out = {0};
        G4int num_scatter_detector = {0};

        NeutronRunData()
        {}

        NeutronRunData(
            G4int _num_events, G4int _num_detected,
            G4int _num_elastic, G4int _num_inelastic,
            G4int _num_capture, G4int _num_fission,
            G4int _num_scatter, G4int _num_scatter_out,
            G4int _num_scatter_detector
        )
        : num_events(_num_events), num_detected(_num_detected)
        , num_elastic(_num_elastic), num_inelastic(_num_inelastic)
        , num_capture(_num_capture), num_fission(_num_fission)
        , num_scatter(_num_scatter), num_scatter_out(_num_scatter_out)
        , num_scatter_detector(_num_scatter_detector)
        {
        }
    };
}