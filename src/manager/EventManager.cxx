/**
 * @file EventManager.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-20
 */
#include "EventManager.hh"

namespace marex
{
    std::shared_ptr<EventManager> EventManager::sInstance;
    std::mutex EventManager::sMutex;

    EventManager::EventManager()
    {
    }

    EventManager::~EventManager()
    {
    }

    void EventManager::SetConfig(YAML::Node config)
    {
        mConfig = config;
        if(mConfig["manager"])
        {
            if(mConfig["manager"]["number_of_threads"]) { 
                sNumberOfThreads = mConfig["manager"]["number_of_threads"].as<G4int>(); 
            }
            if(mConfig["manager"]["number_of_runs"])    { 
                sNumberOfRuns = mConfig["manager"]["number_of_runs"].as<G4int>(); 
            }
            if(mConfig["manager"]["number_of_events"])  { 
                sNumberOfEvents = mConfig["manager"]["number_of_events"].as<G4int>(); 
            }
            if(mConfig["manager"]["output_filename"])   { 
                sOutputFileName = mConfig["manager"]["output_filename"].as<std::string>(); 
            }
            if(mConfig["manager"]["save_profile_data"]) { 
                sSaveProfileData = mConfig["manager"]["save_profile_data"].as<G4bool>(); 
            }
            if(mConfig["argon"]["use_g4_definition"])   { mUseG4Definition = mConfig["argon"]["use_g4_definition"].as<G4bool>(); }
            if(mConfig["argon"]["argon_36_ratio"])      { mArgon36Ratio = mConfig["argon"]["argon_36_ratio"].as<G4double>(); }
            if(mConfig["argon"]["argon_38_ratio"])      { mArgon38Ratio = mConfig["argon"]["argon_38_ratio"].as<G4double>(); }
            if(mConfig["argon"]["argon_40_ratio"])      { mArgon40Ratio = mConfig["argon"]["argon_40_ratio"].as<G4double>(); }
            if(mConfig["argon"]["lar_density"])         { mLArDensity = mConfig["argon"]["lar_density"].as<G4double>() * g/cm3; }
            if(mConfig["argon"]["lar_temperature"])     { mLArTemperature = mConfig["argon"]["lar_temperature"].as<G4double>() * kelvin; }
            if(mConfig["argon"]["lar_pressure"])        { mLArPressure = mConfig["argon"]["lar_pressure"].as<G4double>() * atmosphere; }
            if(mConfig["detector"]["detector_entrance"]) { mDetectorEntrance = mConfig["detector"]["detector_entrance"].as<G4double>() * m; }
        }

        if(mConfig["generator"]["energy_cut_low"])  { mEnergyCutLow = mConfig["generator"]["energy_cut_low"].as<G4double>() * keV; }
        if(mConfig["generator"]["energy_cut_high"]) { mEnergyCutHigh = mConfig["generator"]["energy_cut_high"].as<G4double>() * keV; }

        // Set up energy distributions
        mnTOFBeamProfileFileName = mConfig["generator"]["beam_profile"]["profile_file"].as<std::string>();
        mnTOFBeamProfileName = mConfig["generator"]["beam_profile"]["profile_name"].as<std::string>();
        mnTOFBeamProfileFile = new TFile(mnTOFBeamProfileFileName);
        mnTOFBeamProfile.reset((TH2D*)mnTOFBeamProfileFile->Get(mnTOFBeamProfileName));

        mnTOFTOFFileName = mConfig["generator"]["time_of_flight"]["tof_file"].as<std::string>();
        mnTOFTOFFile = new TFile(mnTOFTOFFileName);
        // Set up tof profile and projections
        mnTOFTOF.reset((TH2D*)mnTOFTOFFile->Get(mnTOFTOFName));
        
        auto beam_x = mnTOFTOF->GetXaxis();
        auto num_bins = beam_x->GetNbins();
        for(G4int ii = 0; ii < num_bins; ii++)
        {
            std::string projection_name = "profile_" + std::to_string(ii);
            TH1D* projection = (TH1D*)mnTOFTOF->ProjectionY(
                projection_name.c_str(),
                ii, ii+1
            );
            mnTOFTOFProjections.emplace_back(projection);
        }

        G4GDMLParser* mGDMLParser;
    }

    void EventManager::SetPhysicsList(PhysicsList* physicsList)
    {
        sPhysicsList.reset(physicsList);
    }

    std::vector<PrimaryGeneration> EventManager::GeneratePrimaryList()
    {
        StartFunctionProfile();
        std::vector<PrimaryGeneration> primaries;
        primaries.emplace_back(
            PrimaryGeneration(
                "Am241",
                95, 241,
                0.0 * eplus,
                0.,
                {0., 0., 0.},
                0.0 * keV,
                {0., 0., 1.}
            )
        );
        EndFunctionProfile("GeneratePrimaryList");
        return primaries;
    }

    G4int EventManager::GetIndex(G4String tuple)
    {
        for(size_t ii = 0; ii < sTuples.size(); ii++)
        {
            if(sTuples[ii].name == tuple) {
                return ii;
            }
        }
        sCurrentTupleIndex += 1;
        sTuples.emplace_back(Tuple(tuple, sCurrentTupleIndex));
        return sCurrentTupleIndex;
    }

    void EventManager::OpenOutputFile(G4int RunID)
    {
        auto AnalysisManager = G4AnalysisManager::Instance();
        G4String OutputFile = OutputFileName() + "_" + std::to_string(RunID) + ".root";
        G4bool fileopen = AnalysisManager->OpenFile(OutputFile);
        if (!fileopen) {
            G4cout << "File - " + OutputFile 
                << " - not opened!" << G4endl;
        }
        else {
            G4cout << "File - " + OutputFile
                << " - opened successfully." << G4endl;
        }
    }
    void EventManager::CloseOutputFile(G4int RunID)
    {
        auto AnalysisManager = G4AnalysisManager::Instance();
        AnalysisManager->Write();
        AnalysisManager->CloseFile();

#ifdef LARGEANT_PROFILING
        std::ofstream ProfilingFile;
        std::filesystem::create_directory(".logs");
        auto ThreadID = G4Threading::G4GetThreadId();
        if(ThreadID < 0) { 
            return;
        }
        ProfilingFile.open(
            ".logs/" + OutputFileName() + "_" + std::to_string(RunID)
            + "_t" + std::to_string(ThreadID) + ".profile"  
        );
        ProfilingFile << "function,number_of_calls,total_time[ms]\n"; 
        
        auto Profiles = GetFunctionProfiles();
        for(auto const& [key, val] : Profiles)
        {
            ProfilingFile << key << ","; 
            ProfilingFile << val.calls << ","; 
            ProfilingFile << val.time << "\n";
        }
        ProfilingFile.close();
#endif
    }

    void EventManager::CreateTuples()
    {
        StartFunctionProfile();
        auto AnalysisManager = G4AnalysisManager::Instance();
        AnalysisManager->SetDefaultFileType("root");
        AnalysisManager->SetVerboseLevel(0);
        AnalysisManager->SetNtupleMerging(true);

        if(SaveParticleMaps())
        {
            G4int index = GetIndex("ParticleMaps");
            AnalysisManager->CreateNtuple("ParticleMaps", "ParticleMaps");
            AnalysisManager->CreateNtupleIColumn("event");
            AnalysisManager->CreateNtupleIColumn("track_id");
            AnalysisManager->CreateNtupleSColumn("particle");
            AnalysisManager->CreateNtupleIColumn("pdg");
            AnalysisManager->CreateNtupleIColumn("parent_track_id");
            AnalysisManager->CreateNtupleIColumn("ancestor_track_id");
            AnalysisManager->CreateNtupleIColumn("scintillation_ancestor_track_id");
            AnalysisManager->CreateNtupleIColumn("scintillation_ancestor_pdg");
            AnalysisManager->FinishNtuple(index);
        }
        if(SavePrimaryInfo())
        {
            G4int index = GetIndex("Primaries");
            AnalysisManager->CreateNtuple("Primaries", "Primaries");
            AnalysisManager->CreateNtupleIColumn("event");
            AnalysisManager->CreateNtupleIColumn("track_id");
            AnalysisManager->CreateNtupleSColumn("particle");
            AnalysisManager->CreateNtupleIColumn("pdg");
            AnalysisManager->CreateNtupleDColumn("init_t");
            AnalysisManager->CreateNtupleDColumn("init_x");
            AnalysisManager->CreateNtupleDColumn("init_y");
            AnalysisManager->CreateNtupleDColumn("init_z");
            AnalysisManager->CreateNtupleDColumn("init_energy");
            AnalysisManager->CreateNtupleDColumn("final_t");
            AnalysisManager->CreateNtupleDColumn("final_x");
            AnalysisManager->CreateNtupleDColumn("final_y");
            AnalysisManager->CreateNtupleDColumn("final_z");
            AnalysisManager->CreateNtupleDColumn("final_energy");
            AnalysisManager->CreateNtupleIColumn("number_of_daughters");
            AnalysisManager->CreateNtupleDColumn("total_daughter_init_energy");
            AnalysisManager->CreateNtupleDColumn("total_daughter_final_energy");
            AnalysisManager->CreateNtupleDColumn("total_daughter_edep");
            AnalysisManager->CreateNtupleIColumn("number_of_edeps");
            AnalysisManager->CreateNtupleDColumn("total_edep");
            AnalysisManager->CreateNtupleIColumn("number_of_photons");
            AnalysisManager->CreateNtupleDColumn("total_optical_photon_init_energy");
            AnalysisManager->CreateNtupleDColumn("total_optical_photon_final_energy");
            AnalysisManager->CreateNtupleDColumn("total_optical_photon_edep");
            AnalysisManager->CreateNtupleIColumn("number_of_electrons");
            AnalysisManager->CreateNtupleDColumn("total_thermal_electron_init_energy");
            AnalysisManager->CreateNtupleDColumn("total_thermal_electron_final_energy");
            AnalysisManager->CreateNtupleDColumn("total_thermal_electron_edep");
            AnalysisManager->CreateNtupleIColumn("number_of_hits");
            AnalysisManager->FinishNtuple(index);
        }
        if(SaveParticleInfo())
        {
            G4int index = GetIndex("Particles");
            AnalysisManager->CreateNtuple("Particles", "Particles");
            AnalysisManager->CreateNtupleIColumn("event");
            AnalysisManager->CreateNtupleIColumn("track_id");
            AnalysisManager->CreateNtupleIColumn("pdg");
            AnalysisManager->CreateNtupleIColumn("parent_track_id");
            AnalysisManager->CreateNtupleSColumn("creation_process");
            AnalysisManager->CreateNtupleDColumn("global_init_time");
            AnalysisManager->CreateNtupleDColumn("local_init_time");
            AnalysisManager->CreateNtupleDColumn("init_x");
            AnalysisManager->CreateNtupleDColumn("init_y");
            AnalysisManager->CreateNtupleDColumn("init_z");
            AnalysisManager->CreateNtupleDColumn("init_energy");
            AnalysisManager->CreateNtupleDColumn("init_px");
            AnalysisManager->CreateNtupleDColumn("init_py");
            AnalysisManager->CreateNtupleDColumn("init_pz");
            AnalysisManager->FinishNtuple(index);
        }
        if(SaveEnergyDeposits())
        {
            G4int index = GetIndex("EnergyDeposits");
            AnalysisManager->CreateNtuple("EnergyDeposits", "EnergyDeposits");
            AnalysisManager->CreateNtupleIColumn("event");
            AnalysisManager->CreateNtupleIColumn("track_id");
            AnalysisManager->CreateNtupleSColumn("particle");
            AnalysisManager->CreateNtupleIColumn("pdg");
            AnalysisManager->CreateNtupleDColumn("local_time");
            AnalysisManager->CreateNtupleDColumn("global_time");
            AnalysisManager->CreateNtupleDColumn("init_x");
            AnalysisManager->CreateNtupleDColumn("init_y");
            AnalysisManager->CreateNtupleDColumn("init_z");
            AnalysisManager->CreateNtupleDColumn("final_x");
            AnalysisManager->CreateNtupleDColumn("final_y");
            AnalysisManager->CreateNtupleDColumn("final_z");
            AnalysisManager->CreateNtupleDColumn("edep");
            AnalysisManager->FinishNtuple(index);
        }
        if(SaveOpticalPhotons())
        {
            G4int index = GetIndex("OpticalPhotons");
            AnalysisManager->CreateNtuple("OpticalPhotons", "OpticalPhotons");
            AnalysisManager->CreateNtupleIColumn("event");
            AnalysisManager->CreateNtupleIColumn("track_id");
            AnalysisManager->CreateNtupleIColumn("parent_track_id");
            AnalysisManager->CreateNtupleIColumn("parent_pdg");
            AnalysisManager->CreateNtupleDColumn("energy");
            AnalysisManager->CreateNtupleDColumn("track_length");
            AnalysisManager->FinishNtuple(index);
        }
        if(SaveThermalElectrons())
        {
            G4int index = GetIndex("ThermalElectrons");
            AnalysisManager->CreateNtuple("ThermalElectrons", "ThermalElectrons");
            AnalysisManager->CreateNtupleIColumn("event");
            AnalysisManager->CreateNtupleDColumn("energy");
            AnalysisManager->CreateNtupleDColumn("track_length");
            AnalysisManager->FinishNtuple(index);
        }
        if(SaveHits())
        {
            G4int index = GetIndex("Hits");
            AnalysisManager->CreateNtuple("Hits", "Hits");
            AnalysisManager->CreateNtupleIColumn("event");
            AnalysisManager->CreateNtupleIColumn("copy_number");
            AnalysisManager->CreateNtupleIColumn("track_id");
            AnalysisManager->CreateNtupleIColumn("parent_id");
            AnalysisManager->CreateNtupleDColumn("local_time");
            AnalysisManager->CreateNtupleDColumn("global_time");
            AnalysisManager->CreateNtupleDColumn("x_particle");
            AnalysisManager->CreateNtupleDColumn("y_particle");
            AnalysisManager->CreateNtupleDColumn("z_particle");
            AnalysisManager->CreateNtupleDColumn("px_particle");
            AnalysisManager->CreateNtupleDColumn("py_particle");
            AnalysisManager->CreateNtupleDColumn("pz_particle");
            AnalysisManager->CreateNtupleDColumn("energy");
            AnalysisManager->CreateNtupleIColumn("detected");
            AnalysisManager->CreateNtupleIColumn("scintillation_ancestor_pdg");
            AnalysisManager->FinishNtuple(index);
        }
        EndFunctionProfile("CreateTuples");
    }

    void EventManager::FillParticleMaps(G4int EventID)
    {
        StartFunctionProfile();
        if(!SaveParticleMaps()) {
            return;
        }
        auto AnalysisManager = G4AnalysisManager::Instance();
        G4int index = GetIndex("ParticleMaps");
        for(auto const& [key, val] : mParticleName)
        {
            AnalysisManager->FillNtupleIColumn(index, 0, EventID);
            AnalysisManager->FillNtupleIColumn(index, 1, key);
            AnalysisManager->FillNtupleSColumn(index, 2, GetParticleName(key));
            AnalysisManager->FillNtupleIColumn(index, 3, GetParticlePDG(key));
            AnalysisManager->FillNtupleIColumn(index, 4, GetParticleParentTrackID(key));
            AnalysisManager->FillNtupleIColumn(index, 5, GetParticleAncestorTrackID(key));
            AnalysisManager->FillNtupleIColumn(index, 6, GetScintillationAncestorTrackID(key));
            AnalysisManager->FillNtupleIColumn(index, 7, GetScintillationAncestorPDG(key));
            AnalysisManager->AddNtupleRow(index);
        }
        EndFunctionProfile("FillParticleMaps");
    }
    void EventManager::FillPrimaryInfo(G4int EventID)
    {
        StartFunctionProfile();
        if(!SavePrimaryInfo()) {
            return;
        }
        auto AnalysisManager = G4AnalysisManager::Instance();
        G4int index = GetIndex("Primaries");
        for(size_t ii = 0; ii < mPrimaryData.size(); ii++)
        {
            AnalysisManager->FillNtupleIColumn(index, 0, EventID);
            AnalysisManager->FillNtupleIColumn(index, 1, mPrimaryData[ii].track_id);
            AnalysisManager->FillNtupleSColumn(index, 2, mPrimaryData[ii].name);
            AnalysisManager->FillNtupleIColumn(index, 3, mPrimaryData[ii].pdg);
            AnalysisManager->FillNtupleDColumn(index, 4, mPrimaryData[ii].init_t);
            AnalysisManager->FillNtupleDColumn(index, 5, mPrimaryData[ii].init_x);
            AnalysisManager->FillNtupleDColumn(index, 6, mPrimaryData[ii].init_y);
            AnalysisManager->FillNtupleDColumn(index, 7, mPrimaryData[ii].init_z);
            AnalysisManager->FillNtupleDColumn(index, 8, mPrimaryData[ii].init_energy);
            AnalysisManager->FillNtupleDColumn(index, 9, mPrimaryData[ii].final_t);
            AnalysisManager->FillNtupleDColumn(index, 10, mPrimaryData[ii].final_x);
            AnalysisManager->FillNtupleDColumn(index, 11, mPrimaryData[ii].final_y);
            AnalysisManager->FillNtupleDColumn(index, 12, mPrimaryData[ii].final_z);
            AnalysisManager->FillNtupleDColumn(index, 13, mPrimaryData[ii].final_energy);
            AnalysisManager->FillNtupleIColumn(index, 14, mPrimaryData[ii].number_of_daughters);
            AnalysisManager->FillNtupleDColumn(index, 15, mPrimaryData[ii].total_daughter_init_energy);
            AnalysisManager->FillNtupleDColumn(index, 16, mPrimaryData[ii].total_daughter_final_energy);
            AnalysisManager->FillNtupleDColumn(index, 17, mPrimaryData[ii].total_daughter_edep);
            AnalysisManager->FillNtupleIColumn(index, 18, mPrimaryData[ii].number_of_edeps);
            AnalysisManager->FillNtupleDColumn(index, 19, mPrimaryData[ii].total_edep);
            // AnalysisManager->FillNtupleIColumn(index, 20, mPrimaryData[ii].number_of_photons);
            // AnalysisManager->FillNtupleDColumn(index, 21, mPrimaryData[ii].total_optical_photon_init_energy);
            // AnalysisManager->FillNtupleDColumn(index, 22, mPrimaryData[ii].total_optical_photon_final_energy);
            // AnalysisManager->FillNtupleDColumn(index, 23, mPrimaryData[ii].total_optical_photon_edep);
            // AnalysisManager->FillNtupleIColumn(index, 24, mPrimaryData[ii].number_of_electrons);
            // AnalysisManager->FillNtupleDColumn(index, 25, mPrimaryData[ii].total_thermal_electron_init_energy);
            // AnalysisManager->FillNtupleDColumn(index, 26, mPrimaryData[ii].total_thermal_electron_final_energy);
            // AnalysisManager->FillNtupleDColumn(index, 27, mPrimaryData[ii].total_thermal_electron_edep);
            // AnalysisManager->FillNtupleIColumn(index, 28, mPrimaryData[ii].number_of_hits);
            AnalysisManager->AddNtupleRow(index);
        }
        EndFunctionProfile("FillPrimaryInfo");
    }
    void EventManager::FillParticleInfo(G4int EventID)
    {
        StartFunctionProfile();
        if(!SaveParticleInfo()) {
            return;
        }
        auto AnalysisManager = G4AnalysisManager::Instance();
        G4int index = GetIndex("Particles");
        for(size_t ii = 0; ii < mParticles.size(); ii++)
        {
            AnalysisManager->FillNtupleIColumn(index, 0, EventID);
            AnalysisManager->FillNtupleIColumn(index, 1, mParticles[ii].TrackID());
            AnalysisManager->FillNtupleIColumn(index, 2, mParticles[ii].PDG());
            AnalysisManager->FillNtupleIColumn(index, 3, mParticles[ii].ParentTrackID());
            AnalysisManager->FillNtupleSColumn(index, 4, mParticles[ii].CreationProcess());
            AnalysisManager->FillNtupleDColumn(index, 5, mParticles[ii].GetGlobalCreationTime());
            AnalysisManager->FillNtupleDColumn(index, 6, mParticles[ii].GetT());
            AnalysisManager->FillNtupleDColumn(index, 7, mParticles[ii].GetX());
            AnalysisManager->FillNtupleDColumn(index, 8, mParticles[ii].GetY());
            AnalysisManager->FillNtupleDColumn(index, 9, mParticles[ii].GetZ());
            AnalysisManager->FillNtupleDColumn(index, 10, mParticles[ii].GetE());
            AnalysisManager->FillNtupleDColumn(index, 11, mParticles[ii].GetPx());
            AnalysisManager->FillNtupleDColumn(index, 12, mParticles[ii].GetPy());
            AnalysisManager->FillNtupleDColumn(index, 13, mParticles[ii].GetPz());
            AnalysisManager->AddNtupleRow(index);
        }
        EndFunctionProfile("FillParticleInfo");
    }
    void EventManager::FillEnergyDeposits(G4int EventID)
    {
        StartFunctionProfile();
        if(!SaveEnergyDeposits()) {
            return;
        } 
        auto AnalysisManager = G4AnalysisManager::Instance();
        G4int index = GetIndex("EnergyDeposits");
        for(size_t ii = 0; ii < mEnergyDeposits.size(); ii++)
        {
            AnalysisManager->FillNtupleIColumn(index, 0, EventID);
            AnalysisManager->FillNtupleIColumn(index, 1, mEnergyDeposits[ii].track_id);
            AnalysisManager->FillNtupleSColumn(index, 2, mEnergyDeposits[ii].name);
            AnalysisManager->FillNtupleIColumn(index, 3, mEnergyDeposits[ii].pdg);
            AnalysisManager->FillNtupleDColumn(index, 4, mEnergyDeposits[ii].local_time);
            AnalysisManager->FillNtupleDColumn(index, 5, mEnergyDeposits[ii].global_time);
            AnalysisManager->FillNtupleDColumn(index, 6, mEnergyDeposits[ii].pre_step_position[0]);
            AnalysisManager->FillNtupleDColumn(index, 7, mEnergyDeposits[ii].pre_step_position[1]);
            AnalysisManager->FillNtupleDColumn(index, 8, mEnergyDeposits[ii].pre_step_position[2]);
            AnalysisManager->FillNtupleDColumn(index, 9, mEnergyDeposits[ii].post_step_position[0]);
            AnalysisManager->FillNtupleDColumn(index, 10, mEnergyDeposits[ii].post_step_position[1]);
            AnalysisManager->FillNtupleDColumn(index, 11, mEnergyDeposits[ii].post_step_position[2]);
            AnalysisManager->FillNtupleDColumn(index, 12, mEnergyDeposits[ii].energy);
            AnalysisManager->AddNtupleRow(index);
        }
        EndFunctionProfile("FillEnergyDeposits");
    }
    void EventManager::FillOpticalPhotons(G4int EventID)
    {
        StartFunctionProfile();
        if(!SaveOpticalPhotons()) {
            return;
        }
        auto AnalysisManager = G4AnalysisManager::Instance();
        G4int index = GetIndex("OpticalPhotons");
        for(size_t ii = 0; ii < mOpticalPhotonData.size(); ii++)
        {
            AnalysisManager->FillNtupleIColumn(index, 0, EventID);
            AnalysisManager->FillNtupleIColumn(index, 1, mOpticalPhotonData[ii].track_id);
            AnalysisManager->FillNtupleIColumn(index, 2, mOpticalPhotonData[ii].parent_track_id);
            AnalysisManager->FillNtupleIColumn(index, 3, mOpticalPhotonData[ii].parent_pdg);
            AnalysisManager->FillNtupleDColumn(index, 4, mOpticalPhotonData[ii].total_energy);
            AnalysisManager->FillNtupleDColumn(index, 5, mOpticalPhotonData[ii].track_length);
            AnalysisManager->AddNtupleRow(index);
        }
        EndFunctionProfile("FillOpticalPhotons");
    }

    void EventManager::FillHits(G4int EventID)
    {
        StartFunctionProfile();
        if (!SaveHits()) {
            return;
        }
        auto AnalysisManager = G4AnalysisManager::Instance();
        G4int index = GetIndex("Hits");
        for(size_t ii = 0; ii < mHits.size(); ii++)
        {
            AnalysisManager->FillNtupleIColumn(index, 0, EventID);
            AnalysisManager->FillNtupleIColumn(index, 1, mHits[ii].copy_number);
            AnalysisManager->FillNtupleIColumn(index, 2, mHits[ii].track_id);
            AnalysisManager->FillNtupleIColumn(index, 3, mHits[ii].parent_track_id);
            AnalysisManager->FillNtupleDColumn(index, 4, mHits[ii].local_time);
            AnalysisManager->FillNtupleDColumn(index, 5, mHits[ii].global_time);
            AnalysisManager->FillNtupleDColumn(index, 6, mHits[ii].particle_position[0]);
            AnalysisManager->FillNtupleDColumn(index, 7, mHits[ii].particle_position[1]);
            AnalysisManager->FillNtupleDColumn(index, 8, mHits[ii].particle_position[2]);
            AnalysisManager->FillNtupleDColumn(index, 9, mHits[ii].particle_momentum[0]);
            AnalysisManager->FillNtupleDColumn(index, 10,mHits[ii].particle_momentum[1]);
            AnalysisManager->FillNtupleDColumn(index, 11,mHits[ii].particle_momentum[2]);
            AnalysisManager->FillNtupleDColumn(index, 12,mHits[ii].energy);
            AnalysisManager->FillNtupleIColumn(index, 13,mHits[ii].detected);
            // AnalysisManager->FillNtupleIColumn(index, 14,mHits[ii].scintillation_ancestor_pdg);
            AnalysisManager->AddNtupleRow(index);
        }
        EndFunctionProfile("FillHits");
    }


    void EventManager::AddParticleMapsFromTrack(const G4Track* track)
    {
        StartFunctionProfile();
        G4int track_id = track->GetTrackID();
        G4String particle_name = track->GetParticleDefinition()->GetParticleName();
        G4int particle_pdg = track->GetParticleDefinition()->GetPDGEncoding();
        G4int parent_track_id = track->GetParentID();
        
        const G4VProcess* creator_process = track->GetCreatorProcess();
        G4String process = "none";
        if(creator_process) {
            process = creator_process->GetProcessName();
        }

        // Save Particle Info    
        AddParticleName(track_id, particle_name);
        AddParticlePDG(track_id, particle_pdg);
        AddParticleParentTrackID(track_id, parent_track_id);

        // Add ancestor info
        if (parent_track_id == 0) {
            AddParticleAncestorTrackID(track_id, 0);
        }
        else if (GetParticleParentTrackID(parent_track_id) == 0) {
            AddParticleAncestorTrackID(track_id, parent_track_id);
        }
        else {
            AddParticleAncestorTrackID(track_id, GetParticleAncestorTrackID(parent_track_id));
        }

        // Add scintillation information
        if(process == "ScintillationProcess")
        {
            AddScintillationAncestorTrackID(track_id, parent_track_id);
            AddScintillationAncestorPDG(track_id, GetParticlePDG(parent_track_id));
        }
        else
        {
            AddScintillationAncestorTrackID(track_id, -1);
            AddScintillationAncestorPDG(track_id, -1);
        }
        EndFunctionProfile("AddParticleMapsFromTrack");
    }

    void EventManager::AddPrimaryInfoFromTrackBegin(const G4Track* track)
    {
        StartFunctionProfile();
        G4String particle_name = track->GetParticleDefinition()->GetParticleName();
        G4int particle_pdg = track->GetParticleDefinition()->GetPDGEncoding();
        G4ThreeVector particle_position = track->GetVertexPosition();

        G4double local_time = track->GetLocalTime();
        G4double global_time = track->GetGlobalTime();
        G4double kinetic_energy = track->GetKineticEnergy();
        G4int track_status = (int)track->GetTrackStatus();
        G4int track_id = track->GetTrackID();
        G4int parent_track_id = track->GetParentID();

        const G4VProcess* creator_process = track->GetCreatorProcess();
        G4String process = "none";
        if(creator_process) {
            process = creator_process->GetProcessName();
        }

        if(parent_track_id == 0)
        {
            AddPrimaryData(
                PrimaryData(
                    particle_name, track_id, particle_pdg,
                    global_time, particle_position[0], particle_position[1],
                    particle_position[2], kinetic_energy 
                )
            );
        }
        else
        {
            // if(particle_name == "opticalphoton")
            // {
            //     GetPrimaryData(GetParticleAncestorTrackID(track_id)).number_of_photons += 1;
            //     GetPrimaryData(GetParticleAncestorTrackID(track_id)).total_optical_photon_init_energy += kinetic_energy;
            // }
            // else if(particle_name == "thermalelectron")
            // {
            //     GetPrimaryData(GetParticleAncestorTrackID(track_id)).number_of_electrons += 1;
            //     GetPrimaryData(GetParticleAncestorTrackID(track_id)).total_thermal_electron_init_energy += kinetic_energy;
            // }
            // else
            // {
            //     GetPrimaryData(GetParticleAncestorTrackID(track_id)).total_daughter_init_energy += kinetic_energy;
            //     GetPrimaryData(GetParticleAncestorTrackID(track_id)).number_of_daughters += 1;
            // }
        }
        EndFunctionProfile("AddPrimaryInfoFromTrackBegin");
    }

    void EventManager::AddPrimaryInfoFromTrackEnd(const G4Track* track)
    {
        StartFunctionProfile();
        G4String particle_name = track->GetParticleDefinition()->GetParticleName();
        G4ThreeVector particle_position = track->GetVertexPosition();
        G4double kinetic_energy = track->GetKineticEnergy();
        G4int track_id = track->GetTrackID();
        G4double global_time = track->GetGlobalTime();
        
        if(GetParticleParentTrackID(track_id) == 0)
        {
            GetPrimaryData(track_id).AddFinalTrackData(
                global_time, particle_position[0], particle_position[1],
                particle_position[2], kinetic_energy
            );
        }
        // else if(particle_name == "opticalphoton") {
        //     GetPrimaryData(GetParticleAncestorTrackID(track_id)).total_optical_photon_final_energy += kinetic_energy;
        // }
        // else if (particle_name == "thermalelectron") {
        //     GetPrimaryData(GetParticleAncestorTrackID(track_id)).total_thermal_electron_final_energy += kinetic_energy;
        // }
        // else {
        //     GetPrimaryData(GetParticleAncestorTrackID(track_id)).total_daughter_final_energy += kinetic_energy;
        // }
        EndFunctionProfile("AddPrimaryInfoFromTrackEnd");
    }

    void EventManager::AddParticleInfoFromTrackBegin(const G4Track* track)
    {
        StartFunctionProfile();
        G4int track_id = track->GetTrackID();
        G4int particle_pdg = track->GetParticleDefinition()->GetPDGEncoding();
        G4int parent_track_id = track->GetParentID();
        const G4VProcess* creator_process = track->GetCreatorProcess();
        G4String process = "none";
        if(creator_process) {
            process = creator_process->GetProcessName();
        }
        G4double global_time = track->GetGlobalTime();
        G4double local_time = track->GetLocalTime();
        G4ThreeVector particle_position = track->GetVertexPosition();
        G4double kinetic_energy = track->GetKineticEnergy();
        G4ThreeVector particle_momentum = track->GetMomentum();

        mParticles.emplace_back(
            Particle(
                track_id, 
                particle_pdg,
                parent_track_id,
                process,
                global_time,
                local_time,
                particle_position,
                kinetic_energy,
                particle_momentum
            )
        );
        AddParticleTrackID(track_id, mParticles.size()-1);
        EndFunctionProfile("AddParticleInfoFromTrackBegin");
    }
    void EventManager::AddParticleInfoFromTrackEnd(const G4Track* track)
    {
        StartFunctionProfile();
        EndFunctionProfile("AddParticleInfoFromTrackEnd");
    }
    void EventManager::AddParticleInfoFromStep(const G4Step *step)
    {
        StartFunctionProfile();
        G4VPhysicalVolume* physicalVolume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
        G4LogicalVolume *logicalVolume = physicalVolume->GetLogicalVolume();
        G4Track* track = step->GetTrack();
        G4StepPoint* pre_step_point = step->GetPreStepPoint();
        G4StepPoint* post_step_point = step->GetPostStepPoint();
        const G4VProcess* pre_process = pre_step_point->GetProcessDefinedStep();
        const G4VProcess* post_process = post_step_point->GetProcessDefinedStep();

        G4int           event = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
        G4String        particle_name = step->GetTrack()->GetParticleDefinition()->GetParticleName();
        G4int           particle_pdg = step->GetTrack()->GetParticleDefinition()->GetPDGEncoding();
        G4ThreeVector   particleStartPosition = step->GetPreStepPoint()->GetPosition();
        G4double        local_time = track->GetLocalTime();
        G4double        global_time = track->GetGlobalTime();
        G4double        pre_step_energy = pre_step_point->GetKineticEnergy();
        G4double        post_step_energy = post_step_point->GetKineticEnergy();
        G4int           track_status = (int)track->GetTrackStatus();
        G4int           track_id = track->GetTrackID();
        G4int           parent_track_id = track->GetParentID();
        G4ThreeVector   pre_step_position = pre_step_point->GetPosition();
        G4ThreeVector   post_step_position = post_step_point->GetPosition();
        G4ThreeVector   pre_step_momentum = pre_step_point->GetMomentum();
        G4ThreeVector   post_step_momentum = post_step_point->GetMomentum();
        G4double        edep = step->GetTotalEnergyDeposit();
        G4double        kinetic_energy = track->GetKineticEnergy();

        if(GetParticleParentTrackID(track_id) == 0)
        {
            if(parent_track_id == 0) {
                GetPrimaryData(track_id).number_of_edeps += 1;
                GetPrimaryData(track_id).total_edep += edep;
            }
            else if(particle_name == "opticalphoton") {
                GetPrimaryData(GetParticleAncestorTrackID(track_id)).total_optical_photon_edep += edep;
            }
            else if(particle_name == "thermalelectron") {
                GetPrimaryData(GetParticleAncestorTrackID(track_id)).total_thermal_electron_edep += edep;
            }
            else {
                GetPrimaryData(GetParticleAncestorTrackID(track_id)).total_daughter_edep += edep;
            }
        }
        else
        {
            mParticles[GetParticleTrackID(track_id)].AddTrajectoryPoint(
                local_time, pre_step_position, kinetic_energy, pre_step_momentum
            );
        }
        EndFunctionProfile("AddParticleInfoFromStep");
    }

    void EventManager::AddEnergyDepositInfoFromStep(const G4Step* step)
    {
        StartFunctionProfile();
        G4VPhysicalVolume* physicalVolume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
        G4LogicalVolume *logicalVolume = physicalVolume->GetLogicalVolume();
        G4Track* track = step->GetTrack();
        G4StepPoint* pre_step_point = step->GetPreStepPoint();
        G4StepPoint* post_step_point = step->GetPostStepPoint();
        const G4VProcess* pre_process = pre_step_point->GetProcessDefinedStep();
        const G4VProcess* post_process = post_step_point->GetProcessDefinedStep();

        G4int           event = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
        G4String        particle_name = step->GetTrack()->GetParticleDefinition()->GetParticleName();
        G4int           particle_pdg = step->GetTrack()->GetParticleDefinition()->GetPDGEncoding();
        G4ThreeVector   particleStartPosition = step->GetPreStepPoint()->GetPosition();
        G4double        local_time = track->GetLocalTime();
        G4double        global_time = track->GetGlobalTime();
        G4double        pre_step_energy = pre_step_point->GetKineticEnergy();
        G4double        post_step_energy = post_step_point->GetKineticEnergy();
        G4int           track_status = (int)track->GetTrackStatus();
        G4int           track_id = track->GetTrackID();
        G4int           parent_track_id = track->GetParentID();
        G4ThreeVector   pre_step_position = pre_step_point->GetPosition();
        G4ThreeVector   post_step_position = post_step_point->GetPosition();
        G4double        edep = step->GetTotalEnergyDeposit();

        mEnergyDeposits.emplace_back(
            EnergyDeposit(
                track_id, particle_name,
                particle_pdg, local_time, global_time,
                pre_step_position, post_step_position,
                edep
            )
        );
        EndFunctionProfile("AddEnergyDepositInfoFromStep");
    }

    void EventManager::AddOpticalPhotonInfoFromTrackEnd(const G4Track* track)
    {
        StartFunctionProfile();
        G4String particle_name = track->GetParticleDefinition()->GetParticleName();
        if(particle_name == "opticalphoton")
        {
            mOpticalPhotonData.emplace_back(OpticalPhotonData(
                track->GetTrackID(), track->GetParentID(),
                GetParticlePDG(track->GetParentID()),
                track->GetTotalEnergy(), track->GetTrackLength()/cm
            ));
        }
        EndFunctionProfile("AddOpticalPhotonInfoFromTrackEnd");
    }

    void EventManager::AddThermalElectronInfoFromTrackEnd(const G4Track* track)
    {
        StartFunctionProfile();
        EndFunctionProfile("AddThermalElectronInfoFromTrackEnd");
    }

    void EventManager::AddHitInfoFromStep(G4Step* step, G4TouchableHistory* history)
    {
        StartFunctionProfile();
        const G4VTouchable* touchable = step->GetPreStepPoint()->GetTouchable();
        const G4Track* track = step->GetTrack();
        const G4StepPoint *preStepPoint = step->GetPreStepPoint();

        G4int           copyNo = touchable->GetCopyNumber();
        G4double        globalTime = preStepPoint->GetGlobalTime();
        G4int           trackID = track->GetTrackID();
        G4int           parentID = track->GetParentID();
        G4double        localTime = preStepPoint->GetLocalTime();
        G4ThreeVector   particlePosition = preStepPoint->GetPosition();
        G4double        energy = preStepPoint->GetTotalEnergy();
        G4ThreeVector   particleMomentum = preStepPoint->GetMomentum();

        // G4bool detected_hit = GetComponentFromCopyNumber(copyNo)->ProcessHits(step, history);
        // mHits.emplace_back(
        //     Hit(
        //         copyNo, trackID,
        //         parentID, localTime, globalTime,
        //         particlePosition, particleMomentum,
        //         energy, detected_hit,
        //         GetScintillationAncestorPDG(trackID)
        //     )
        // );
        EndFunctionProfile("AddHitInfoFromStep");
    }

    void EventManager::EvaluateEvent()
    {
        for(size_t ii = 0; ii < mAnalysisFunctions.size(); ii++)
        {
            mAnalysisFunctions[ii]();
        }
    }
}