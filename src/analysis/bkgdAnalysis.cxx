/**
 * @file Analysis.cxx
 * @author Yashwanth Bezawada [ysbezawada@ucdavis.edu]
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#include "bkgdAnalysis.hh"

namespace marex
{
    bkgdAnalysisTuple bkgdTuple;

    void bkgdAnalysisFunctionRunBegin()
    {
        auto AnalysisManager = G4AnalysisManager::Instance();
        auto Manager = EventManager::GetEventManager();
        G4int index = Manager->GetIndex("bkgdAnalysis");
        AnalysisManager->CreateNtuple("bkgdAnalysis", "bkgdAnalysis");
        AnalysisManager->CreateNtupleDColumn("n_energy");
        AnalysisManager->CreateNtupleIColumn("detectionStatus");
        // AnalysisManager->CreateNtupleDColumn("n_energy_detected");
        AnalysisManager->CreateNtupleDColumn("n_tof_bkgd");
        AnalysisManager->FinishNtuple(index);
    }
    void bkgdAnalysisFunctionRunEnd()
    {
        auto AnalysisManager = G4AnalysisManager::Instance();
        auto Manager = EventManager::GetEventManager();
        G4int index = Manager->GetIndex("bkgdAnalysis");

        TRandom3* rand3 = new TRandom3();

        G4int numThreads = Manager->NumberOfThreads();
        G4int numBkgdEvts = Manager->GetNumBkgdEvents();
        G4String bkgdDistType = Manager->GetBkgdDistType();
        G4double eCutLow = Manager->GetEnergyCutLow();
        G4double eCutHigh = Manager->GetEnergyCutHigh();

        G4double tofLow = Manager->GetNominalTOF(eCutHigh);
        G4double tofHigh = Manager->GetNominalTOF(eCutLow);

        if (bkgdDistType == "Uniform" && numBkgdEvts != 0)
        {
            G4int evtsLoop = std::round(numBkgdEvts/numThreads);
            for (G4int i = 0; i < evtsLoop; i++)
            {
                G4double randEvt = rand3->Uniform(tofLow, tofHigh);
                // bkgdTuple.n_energy = randEvt;
                // bkgdTuple.n_energy_detected = randEvt;
                bkgdTuple.n_tof_bkgd = randEvt;
                bkgdTuple.detectionStatus = true;

                // G4cout << "Random bkgd energy [ " << i <<  " ] : " << randEvt << G4endl;
                // AnalysisManager->FillNtupleDColumn(index, 0, bkgdTuple.n_energy);
                // AnalysisManager->FillNtupleDColumn(index, 1, bkgdTuple.n_energy_detected);
                AnalysisManager->FillNtupleIColumn(index, 1, bkgdTuple.detectionStatus);
                AnalysisManager->FillNtupleDColumn(index, 2, bkgdTuple.n_tof_bkgd);
                AnalysisManager->AddNtupleRow(index);
            }
        }
    }
    void bkgdAnalysisFunctionEventBegin()
    {
        auto AnalysisManager = G4AnalysisManager::Instance();        
    }
    void bkgdAnalysisFunctionEventEnd()
    {
        auto AnalysisManager = G4AnalysisManager::Instance();
        auto Manager = EventManager::GetEventManager();
        auto primaries = Manager->GetPrimaries();
        auto particles = Manager->GetParticles();
        G4int index = Manager->GetIndex("bkgdAnalysis");

        G4double detEntrance = Manager->GetDetEntrance()/1000; //m
        G4double detLength = Manager->GetDetLength()/1000;

        // G4cout << "Detector Z start: " << detEntrance << G4endl;
        // G4cout << "Detector Z end: " << (detEntrance + detLength) << G4endl;

        for (auto primary:primaries)
        {
            if(primary.pdg != 2112){continue;} //Only Neutrons
            G4int track_id = primary.track_id;
            auto particle = particles[Manager->GetParticleTrackID(track_id)];
            // Getting the trajectory of the neutrons
            auto trajectory = particle.GetTrajectory();

            // Fill Neutron Energy
            bkgdTuple.n_energy = trajectory.E[0];
            AnalysisManager->FillNtupleDColumn(index, 0, bkgdTuple.n_energy);

            // start points
            auto x_start = trajectory.x[0]/1000.0;  //m
            auto y_start = trajectory.y[0]/1000.0;
            // auto z_start = trajectory.z[0]/1000.0;

            for (G4int i = 0; i < trajectory.size()-1; i++)
            {
                // start points
                auto x_i = trajectory.x[i]/1000.0;  //m
                auto y_i = trajectory.y[i]/1000.0;
                auto z_i = trajectory.z[i]/1000.0;
                // end points
                // auto x_f = trajectory.x[i+1]/1000.0;  //m
                // auto y_f = trajectory.y[i+1]/1000.0;
                // auto z_f = trajectory.z[i+1]/1000.0;

                if (x_i != x_start || y_i != y_start)
                {
                    break;
                }

                if (x_i == x_start && y_i == y_start)
                {
                    // if (z_i >= detEntrance && z_i <= detEntrance + detLength) // Checking if the point is in the detector
                    // {
                    //     if (x_f != x_start || y_f != y_start) // Checking if it scatters in the detector
                    //     {
                    //         bkgdTuple.n_energy_detected = trajectory.E[0];
                    //         AnalysisManager->FillNtupleDColumn(index, 1, bkgdTuple.n_energy_detected);
                    //         break;
                    //     }
                    // }

                    if (z_i >= detEntrance)
                    {
                        // bkgdTuple.n_energy_detected = trajectory.E[0];
                        // AnalysisManager->FillNtupleDColumn(index, 1, bkgdTuple.n_energy_detected);

                        bkgdTuple.detectionStatus = true;
                        AnalysisManager->FillNtupleIColumn(index, 1, bkgdTuple.detectionStatus);
                        break;
                    }
                }
            }
            AnalysisManager->AddNtupleRow(index);
        }
    }
}