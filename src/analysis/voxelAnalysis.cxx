/**
 * @file Analysis.cxx
 * @author Yashwanth Bezawada [ysbezawada@ucdavis.edu]
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#include "voxelAnalysis.hh"

namespace marex
{
    AnalysisTuple MyTuple;

    void AnalysisFunctionRunBegin()
    {
        auto AnalysisManager = G4AnalysisManager::Instance();
        auto Manager = EventManager::GetEventManager();
        G4int index = Manager->GetIndex("Analysis");
        AnalysisManager->CreateNtuple("Analysis", "Analysis");
        AnalysisManager->CreateNtupleDColumn("x_pos");
        AnalysisManager->CreateNtupleDColumn("y_pos");
        AnalysisManager->CreateNtupleDColumn("z_pos");
        AnalysisManager->CreateNtupleDColumn("n_energy");
        AnalysisManager->CreateNtupleIColumn("N_x");
        AnalysisManager->CreateNtupleIColumn("N_y");
        AnalysisManager->CreateNtupleIColumn("N_z");
        AnalysisManager->CreateNtupleDColumn("n_tof");
        AnalysisManager->CreateNtupleIColumn("n_track_id");
        AnalysisManager->CreateNtupleDColumn("x_tracking");
        AnalysisManager->CreateNtupleDColumn("y_tracking");
        AnalysisManager->CreateNtupleDColumn("z_tracking");
        AnalysisManager->FinishNtuple(index);
    }
    void AnalysisFunctionRunEnd()
    {
        auto AnalysisManager = G4AnalysisManager::Instance();
    }
    void AnalysisFunctionEventBegin()
    {
        auto AnalysisManager = G4AnalysisManager::Instance();        
    }
    void AnalysisFunctionEventEnd()
    {
        auto AnalysisManager = G4AnalysisManager::Instance();
        auto Manager = EventManager::GetEventManager();
        auto primaries = Manager->GetPrimaries();
        auto particles = Manager->GetParticles();
        G4int index = Manager->GetIndex("Analysis");

        G4double hall_x = Manager->GetHallX()/1000; //m
        G4double hall_y = Manager->GetHallY()/1000;
        G4double hall_z = Manager->GetHallZ()/1000;
        G4int n_x = Manager->GetNx();
        G4int n_y = Manager->GetNy();
        G4int n_z = Manager->GetNz();

        G4cout << "X, Y, Z = " << hall_x << ", " << hall_y << ", " << hall_z << G4endl;
        G4cout << "Nx, Ny, Nz = " << n_x << ", " << n_y << ", " << n_z << G4endl;

        // Voxel conversion factor
        G4double x_fac = static_cast<G4double>(n_x)/hall_x;
        G4double y_fac = static_cast<G4double>(n_y)/hall_y;
        G4double z_fac = static_cast<G4double>(n_z)/hall_z;

        G4cout << "Factors = " << x_fac << ", " << y_fac << ", " << z_fac << G4endl;

        for (auto primary:primaries)
        {
            if(primary.pdg != 2112){continue;} //Only Neutrons
            G4int track_id = primary.track_id;
            auto particle = particles[Manager->GetParticleTrackID(track_id)];
            // Getting the trajectory of the neutrons
            auto trajectory = particle.GetTrajectory();

            // Current Voxel
            std::vector<G4int> CurrentVoxel;
            CurrentVoxel.push_back(n_x + 1);
            CurrentVoxel.push_back(n_y + 1);
            CurrentVoxel.push_back(n_z + 1);

            // Time
            G4double global_time = 0;
            G4double voxel_time = 0;

            /////////////////////////////////////////// Algorithm /////////////////////////////////////////////
            // http://www.cse.yorku.ca/~amana/research/grid.pdf                                              //
            // https://gamedev.stackexchange.com/questions/47362/cast-ray-to-select-block-in-voxel-game?rq=1 //
            ///////////////////////////////////////////////////////////////////////////////////////////////////
            
            // Loop over start and end points in the trajectory
            for (G4int i = 0; i < trajectory.size()-1; i++)
            {
                // start points
                auto x_i = trajectory.x[i]/1000.0;  //m
                auto y_i = trajectory.y[i]/1000.0;
                auto z_i = trajectory.z[i]/1000.0;
                // end points
                auto x_f = trajectory.x[i+1]/1000.0;  //m
                auto y_f = trajectory.y[i+1]/1000.0;
                auto z_f = trajectory.z[i+1]/1000.0;

                // G4cout << "trajectory_i[ " << track_id << ", " << i << " ], " << x_i << ", " << y_i << ", " << z_i << G4endl;
                // G4cout << "trajectory_f[ " << track_id << ", " << i << " ], " << x_f << ", " << y_f << ", " << z_f << G4endl;
                // G4cout << "Curent Voxel before traj loop = ( " << CurrentVoxel[0] << ", " << CurrentVoxel[1] << ", " << CurrentVoxel[2] << " )" << G4endl;

                // Start Voxel
                auto V_i = Manager->FindVoxel(x_i, y_i, z_i, x_fac, y_fac, z_fac);
                // End Voxel
                auto V_f = Manager->FindVoxel(x_f, y_f, z_f, x_fac, y_fac, z_fac);

                if (i==0)
                {
                    CurrentVoxel[0] = V_i[0];
                    CurrentVoxel[1] = V_i[1];
                    CurrentVoxel[2] = V_i[2];

                    MyTuple.x_pos = x_i;//current_x;
                    MyTuple.y_pos = y_i;//current_y;
                    MyTuple.z_pos = z_i;//current_z;
                    MyTuple.n_energy = trajectory.E[0];
                    MyTuple.N_x = CurrentVoxel[0];
                    MyTuple.N_y = CurrentVoxel[1];
                    MyTuple.N_z = CurrentVoxel[2];
                    MyTuple.n_tof = global_time;
                    MyTuple.n_track_id = track_id;
                    MyTuple.x_tracking = x_i;
                    MyTuple.y_tracking = y_i;
                    MyTuple.z_tracking = z_i;

                    AnalysisManager->FillNtupleDColumn(index, 0, MyTuple.x_pos);
                    AnalysisManager->FillNtupleDColumn(index, 1, MyTuple.y_pos);
                    AnalysisManager->FillNtupleDColumn(index, 2, MyTuple.z_pos);
                    AnalysisManager->FillNtupleDColumn(index, 3, MyTuple.n_energy);
                    AnalysisManager->FillNtupleIColumn(index, 4, MyTuple.N_x);
                    AnalysisManager->FillNtupleIColumn(index, 5, MyTuple.N_y);
                    AnalysisManager->FillNtupleIColumn(index, 6, MyTuple.N_z);
                    AnalysisManager->FillNtupleDColumn(index, 7, MyTuple.n_tof);
                    AnalysisManager->FillNtupleIColumn(index, 8, MyTuple.n_track_id);
                    AnalysisManager->FillNtupleDColumn(index, 9, MyTuple.x_tracking);
                    AnalysisManager->FillNtupleDColumn(index, 10, MyTuple.y_tracking);
                    AnalysisManager->FillNtupleDColumn(index, 11, MyTuple.z_tracking);
                    AnalysisManager->AddNtupleRow(index);
                }
                
                // Step distance
                G4double d = std::sqrt( (x_f - x_i)*(x_f - x_i) + (y_f - y_i)*(y_f - y_i) + (z_f - z_i)*(z_f - z_i) );
                
                // Skip the loop if the start and the end voxels are the same
                if (V_i == V_f)
                {
                    global_time += d/Manager->GetNominalVelocity(trajectory.E[i]);
                    continue;                   
                }

                ////////// If V_i != V_f, do 

                //Number of times to iterate
                G4int x_steps = std::abs(V_f[0] - V_i[0]);
                G4int y_steps = std::abs(V_f[1] - V_i[1]);
                G4int z_steps = std::abs(V_f[2] - V_i[2]);

                // G4int NumSteps = x_steps + y_steps + z_steps; // std::max({x_steps, y_steps, z_steps});

                // Finding unit vector in the direction joining start and end points
                G4double u_x = (x_f - x_i)/d;
                G4double u_y = (y_f - y_i)/d;
                G4double u_z = (z_f - z_i)/d;

                // current point
                G4double current_x = x_i;
                G4double current_y = y_i;
                G4double current_z = z_i;

                G4cout << "Curent Voxel before Step loop = ( " << CurrentVoxel[0] << ", " << CurrentVoxel[1] << ", " << CurrentVoxel[2] << " )" << G4endl;

                // distance traversed
                G4double d_travel = 0;

                // for (G4int j = 0; j < NumSteps; j++)
                while (d_travel < d)
                {
                    // Finding point to check for next voxel
                    // Finding t (distance need to be travesed along the line)
                    
                    G4int Vx_next = CurrentVoxel[0] + Manager->sgn(u_x);
                    G4int Vy_next = CurrentVoxel[1] + Manager->sgn(u_y);
                    G4int Vz_next = CurrentVoxel[2] + Manager->sgn(u_z);

                    G4cout << "[ " << d_travel << " ]" << "Next possible voxel values = " << Vx_next << ", " << Vy_next << ", " << Vz_next << G4endl;

                    // Finding t to reach each of the next Vx, Vy and Vz
                    G4double tx = 0;
                    G4double ty = 0;
                    G4double tz = 0;
                    // Avoiding dividing by 0
                    (u_x == 0) ? tx = d+1 : tx = ( (static_cast<G4double>(Vx_next)/x_fac) - current_x )/u_x;
                    (u_y == 0) ? ty = d+1 : ty = ( (static_cast<G4double>(Vy_next)/y_fac) - current_y )/u_y;
                    (u_z == 0) ? tz = d+1 : tz = ( (static_cast<G4double>(Vz_next)/z_fac) - current_z )/u_z;

                    G4cout << "[ " << d_travel << " ]" << "Values of ts for x, y and z = " << tx << ", " << ty << ", " << tz << G4endl;

                    // Update the current point with the new point
                    // t_min is the distance traversed in this loop
                    G4double t_min = std::min({tx, ty, tz});
                    d_travel += t_min;

                    if (d_travel > d)
                    {
                        G4double d_extra = std::sqrt( (x_f - current_x)*(x_f - current_x) + (y_f - current_y)*(y_f - current_y) + (z_f - current_z)*(z_f - current_z) );
                        global_time += d_extra/Manager->GetNominalVelocity(trajectory.E[i]);
                        break; //break out of the while loop
                    }
                    
                    current_x += t_min*u_x;
                    current_y += t_min*u_y;
                    current_z += t_min*u_z;

                    global_time += t_min/Manager->GetNominalVelocity(trajectory.E[i]);

                    G4cout << "[ " << d_travel << " ]" "Current point on the line = " << current_x << ", " << current_y << ", " <<current_z << G4endl;

                    // Update the current voxel
                    std::vector<G4int> new_voxel = Manager->FindVoxel(current_x, current_y, current_z, x_fac, y_fac, z_fac);
                    CurrentVoxel[0] = new_voxel[0];
                    CurrentVoxel[1] = new_voxel[1];
                    CurrentVoxel[2] = new_voxel[2];

                    G4cout << "[ " << d_travel << " ]" << "new voxel = ( " << new_voxel[0] << ", " << new_voxel[1] << ", " << new_voxel[2] << " )" << G4endl;
                    G4cout << "[ " << d_travel << " ]" << "Updated Curent Voxel = ( " << CurrentVoxel[0] << ", " << CurrentVoxel[1] << ", " << CurrentVoxel[2] << " )" << G4endl;

                    // Filling the tuple
                    MyTuple.x_pos = x_i;//current_x;
                    MyTuple.y_pos = y_i;//urrent_y;
                    MyTuple.z_pos = z_i;//current_z;
                    MyTuple.n_energy = trajectory.E[0];
                    MyTuple.N_x = CurrentVoxel[0];
                    MyTuple.N_y = CurrentVoxel[1];
                    MyTuple.N_z = CurrentVoxel[2];
                    MyTuple.n_tof = global_time;
                    MyTuple.n_track_id = track_id;
                    MyTuple.x_tracking = current_x;
                    MyTuple.y_tracking = current_y;
                    MyTuple.z_tracking = current_z;

                    AnalysisManager->FillNtupleDColumn(index, 0, MyTuple.x_pos);
                    AnalysisManager->FillNtupleDColumn(index, 1, MyTuple.y_pos);
                    AnalysisManager->FillNtupleDColumn(index, 2, MyTuple.z_pos);
                    AnalysisManager->FillNtupleDColumn(index, 3, MyTuple.n_energy);
                    AnalysisManager->FillNtupleIColumn(index, 4, MyTuple.N_x);
                    AnalysisManager->FillNtupleIColumn(index, 5, MyTuple.N_y);
                    AnalysisManager->FillNtupleIColumn(index, 6, MyTuple.N_z);
                    AnalysisManager->FillNtupleDColumn(index, 7, MyTuple.n_tof);
                    AnalysisManager->FillNtupleIColumn(index, 8, MyTuple.n_track_id);
                    AnalysisManager->FillNtupleDColumn(index, 9, MyTuple.x_tracking);
                    AnalysisManager->FillNtupleDColumn(index, 10, MyTuple.y_tracking);
                    AnalysisManager->FillNtupleDColumn(index, 11, MyTuple.z_tracking);
                    AnalysisManager->AddNtupleRow(index);
                }

                // Storing the last end point
                if (i == trajectory.size()-2)
                {
                    
                    MyTuple.x_pos = x_f;
                    MyTuple.y_pos = y_f;
                    MyTuple.z_pos = z_f;
                    MyTuple.n_energy = trajectory.E[0];
                    MyTuple.N_x = V_f[0];
                    MyTuple.N_y = V_f[1];
                    MyTuple.N_z = V_f[2];
                    MyTuple.n_tof = global_time;
                    MyTuple.n_track_id = track_id;
                    MyTuple.x_tracking = x_f;
                    MyTuple.y_tracking = y_f;
                    MyTuple.z_tracking = z_f;

                    AnalysisManager->FillNtupleDColumn(index, 0, MyTuple.x_pos);
                    AnalysisManager->FillNtupleDColumn(index, 1, MyTuple.y_pos);
                    AnalysisManager->FillNtupleDColumn(index, 2, MyTuple.z_pos);
                    AnalysisManager->FillNtupleDColumn(index, 3, MyTuple.n_energy);
                    AnalysisManager->FillNtupleIColumn(index, 4, MyTuple.N_x);
                    AnalysisManager->FillNtupleIColumn(index, 5, MyTuple.N_y);
                    AnalysisManager->FillNtupleIColumn(index, 6, MyTuple.N_z);
                    AnalysisManager->FillNtupleDColumn(index, 7, MyTuple.n_tof);
                    AnalysisManager->FillNtupleIColumn(index, 8, MyTuple.n_track_id);
                    AnalysisManager->FillNtupleDColumn(index, 9, MyTuple.x_tracking);
                    AnalysisManager->FillNtupleDColumn(index, 10, MyTuple.y_tracking);
                    AnalysisManager->FillNtupleDColumn(index, 11, MyTuple.z_tracking);
                    AnalysisManager->AddNtupleRow(index);
                }
                // G4cout << "Curent Voxel after step loop = ( " << CurrentVoxel[0] << ", " << CurrentVoxel[1] << ", " << CurrentVoxel[2] << " )" << G4endl;             
            }
        }
    }
}


// // Loop over start and end points in the trajectory
//             for (G4int i = 0; i < trajectory.size()-1; i++)
//             {
//                 // start points
//                 auto x_i = trajectory.x[i]/1000.0;  //m
//                 auto y_i = trajectory.y[i]/1000.0;
//                 auto z_i = trajectory.z[i]/1000.0;
//                 // end points
//                 auto x_f = trajectory.x[i+1]/1000.0;  //m
//                 auto y_f = trajectory.y[i+1]/1000.0;
//                 auto z_f = trajectory.z[i+1]/1000.0;

//                 G4cout << "trajectory_i[ " << track_id << ", " << i << " ], " << x_i << ", " << y_i << ", " << z_i << G4endl;
//                 G4cout << "trajectory_f[ " << track_id << ", " << i << " ], " << x_f << ", " << y_f << ", " << z_f << G4endl;
//                 G4cout << "Curent Voxel before traj loop = ( " << CurrentVoxel[0] << ", " << CurrentVoxel[1] << ", " << CurrentVoxel[2] << " )" << G4endl;

//                 // Start Voxel
//                 auto V_i = Manager->FindVoxel(x_i, y_i, z_i, x_fac, y_fac, z_fac);
//                 // End Voxel
//                 auto V_f = Manager->FindVoxel(x_f, y_f, z_f, x_fac, y_fac, z_fac);

//                 if (i==0)
//                 {
//                     CurrentVoxel[0] = V_i[0];
//                     CurrentVoxel[1] = V_i[1];
//                     CurrentVoxel[2] = V_i[2];
//                 }
                
//                 // Step distance
//                 G4double d = std::sqrt( (x_f - x_i)*(x_f - x_i) + (y_f - y_i)*(y_f - y_i) + (z_f - z_i)*(z_f - z_i) );
                
//                 // Skip the loop if the start and the end voxels are the same
//                 if (V_i == V_f)
//                 {
                    // if (V_i == CurrentVoxel)
                    // {
                    //     // G4cout << "initial voxel is current voxel" << G4endl;
                    //     global_time += d/Manager->GetNominalVelocity(trajectory.E[i]);
                    //     continue;
                    // } else {
                    //     // G4cout << "initial voxel is not current voxel" << G4endl;
                    //     global_time += d/Manager->GetNominalVelocity(trajectory.E[i]);

                    //     // Filling the tuple
                    //     MyTuple.x_pos = x_i;
                    //     MyTuple.y_pos = y_i;
                    //     MyTuple.z_pos = z_i;
                    //     MyTuple.n_energy = trajectory.E[0];
                    //     MyTuple.N_x = V_i[0];
                    //     MyTuple.N_y = V_i[1];
                    //     MyTuple.N_z = V_i[2];

                    //     AnalysisManager->FillNtupleDColumn(index, 0, MyTuple.x_pos);
                    //     AnalysisManager->FillNtupleDColumn(index, 1, MyTuple.y_pos);
                    //     AnalysisManager->FillNtupleDColumn(index, 2, MyTuple.z_pos);
                    //     AnalysisManager->FillNtupleDColumn(index, 3, MyTuple.n_energy);
                    //     AnalysisManager->FillNtupleIColumn(index, 4, MyTuple.N_x);
                    //     AnalysisManager->FillNtupleIColumn(index, 5, MyTuple.N_y);
                    //     AnalysisManager->FillNtupleIColumn(index, 6, MyTuple.N_z);
                    //     AnalysisManager->AddNtupleRow(index);

                    //     continue;
                    // }                    
//                 }

//                 ////////// If V_i != V_f, do the 

//                 // Set V_i to the current voxel
//                 if ( V_i != CurrentVoxel )
//                 {
//                     // G4cout << "initial voxel is not current voxel-----------" << G4endl;

//                     CurrentVoxel[0] = V_i[0];
//                     CurrentVoxel[1] = V_i[1];
//                     CurrentVoxel[2] = V_i[2];

//                     // Filling the tuple
//                     MyTuple.x_pos = x_i;
//                     MyTuple.y_pos = y_i;
//                     MyTuple.z_pos = z_i;
//                     MyTuple.n_energy = trajectory.E[0];
//                     MyTuple.N_x = V_i[0];
//                     MyTuple.N_y = V_i[1];
//                     MyTuple.N_z = V_i[2];

//                     AnalysisManager->FillNtupleDColumn(index, 0, MyTuple.x_pos);
//                     AnalysisManager->FillNtupleDColumn(index, 1, MyTuple.y_pos);
//                     AnalysisManager->FillNtupleDColumn(index, 2, MyTuple.z_pos);
//                     AnalysisManager->FillNtupleDColumn(index, 3, MyTuple.n_energy);
//                     AnalysisManager->FillNtupleIColumn(index, 4, MyTuple.N_x);
//                     AnalysisManager->FillNtupleIColumn(index, 5, MyTuple.N_y);
//                     AnalysisManager->FillNtupleIColumn(index, 6, MyTuple.N_z);
//                     AnalysisManager->AddNtupleRow(index);
//                 }

//                 //Number of times to iterate
//                 G4int x_steps = std::abs(V_f[0] - V_i[0]);
//                 G4int y_steps = std::abs(V_f[1] - V_i[1]);
//                 G4int z_steps = std::abs(V_f[2] - V_i[2]);

//                 G4int NumSteps = x_steps + y_steps + z_steps; // std::max({x_steps, y_steps, z_steps});

//                 // Finding unit vector in the direction joining start and end points
//                 G4double u_x = (x_f - x_i)/d;
//                 G4double u_y = (y_f - y_i)/d;
//                 G4double u_z = (z_f - z_i)/d;

//                 // current point
//                 G4double current_x = x_i;
//                 G4double current_y = y_i;
//                 G4double current_z = z_i;

//                 G4cout << "Curent Voxel before Step loop = ( " << CurrentVoxel[0] << ", " << CurrentVoxel[1] << ", " << CurrentVoxel[2] << " )" << G4endl;

//                 // while (V_f != CurrentVoxel)
//                 for (G4int j = 0; j < NumSteps; j++)
//                 {
//                     // Finding point to check for next voxel
//                     // Finding t (distance need to be travesed along the line)
                    
//                     G4int Vx_next = CurrentVoxel[0] + Manager->sgn(u_x);
//                     G4int Vy_next = CurrentVoxel[1] + Manager->sgn(u_y);
//                     G4int Vz_next = CurrentVoxel[2] + Manager->sgn(u_z);

//                     // G4cout << "Next possible voxel values = " << Vx_next << ", " << Vy_next << ", " << Vz_next << G4endl;

//                     // Finding t to reach each of the next Vx, Vy and Vz
//                     G4double tx = 0;
//                     G4double ty = 0;
//                     G4double tz = 0;
//                     // Avoiding dividing by 0
//                     (u_x == 0) ? tx = d+1 : tx = ( (static_cast<G4double>(Vx_next)/x_fac) - current_x )/u_x;
//                     (u_y == 0) ? ty = d+1 : ty = ( (static_cast<G4double>(Vy_next)/y_fac) - current_y )/u_y;
//                     (u_z == 0) ? tz = d+1 : tz = ( (static_cast<G4double>(Vz_next)/z_fac) - current_z )/u_z;

//                     // G4cout << "Values of ts for x, y and z = " << tx << ", " << ty << ", " << tz << G4endl;

//                     // Update the current point with the new point
//                     G4double t_min = std::min({tx, ty, tz});
//                     current_x += t_min*u_x;
//                     current_y += t_min*u_y;
//                     current_z += t_min*u_z;

//                     // G4cout << "Current point on the line = " << current_x << ", " << current_y << ", " <<current_z << G4endl;

//                     // Update the current voxel
//                     std::vector<G4int> new_voxel = Manager->FindVoxel(current_x, current_y, current_z, x_fac, y_fac, z_fac);
//                     CurrentVoxel[0] = new_voxel[0];
//                     CurrentVoxel[1] = new_voxel[1];
//                     CurrentVoxel[2] = new_voxel[2];

//                     // G4cout << "new voxel = ( " << new_voxel[0] << ", " << new_voxel[1] << ", " << new_voxel[2] << " )" << G4endl;
//                     // G4cout << "Updated Curent Voxel = ( " << CurrentVoxel[0] << ", " << CurrentVoxel[1] << ", " << CurrentVoxel[2] << " )" << G4endl;

//                     // Filling the tuple
//                     MyTuple.x_pos = current_x;
//                     MyTuple.y_pos = current_y;
//                     MyTuple.z_pos = current_z;
//                     MyTuple.n_energy = trajectory.E[0];
//                     MyTuple.N_x = CurrentVoxel[0];
//                     MyTuple.N_y = CurrentVoxel[1];
//                     MyTuple.N_z = CurrentVoxel[2];

//                     AnalysisManager->FillNtupleDColumn(index, 0, MyTuple.x_pos);
//                     AnalysisManager->FillNtupleDColumn(index, 1, MyTuple.y_pos);
//                     AnalysisManager->FillNtupleDColumn(index, 2, MyTuple.z_pos);
//                     AnalysisManager->FillNtupleDColumn(index, 3, MyTuple.n_energy);
//                     AnalysisManager->FillNtupleIColumn(index, 4, MyTuple.N_x);
//                     AnalysisManager->FillNtupleIColumn(index, 5, MyTuple.N_y);
//                     AnalysisManager->FillNtupleIColumn(index, 6, MyTuple.N_z);
//                     AnalysisManager->AddNtupleRow(index);
//                 }

//                 // G4cout << "Curent Voxel after step loop = ( " << CurrentVoxel[0] << ", " << CurrentVoxel[1] << ", " << CurrentVoxel[2] << " )" << G4endl;
                
//                 // Storing the last end voxel if it is not stored in the above loop
//                 if (i == trajectory.size()-2)
//                 {
//                     if (V_f[0] == CurrentVoxel[0] && V_f[1] == CurrentVoxel[1] && V_f[2] == CurrentVoxel[2])
//                     {
//                         continue;
//                     }
                    
//                     MyTuple.x_pos = x_f;
//                     MyTuple.y_pos = y_f;
//                     MyTuple.z_pos = z_f;
//                     MyTuple.n_energy = trajectory.E[0];
//                     MyTuple.N_x = V_f[0];
//                     MyTuple.N_y = V_f[1];
//                     MyTuple.N_z = V_f[2];

//                     AnalysisManager->FillNtupleDColumn(index, 0, MyTuple.x_pos);
//                     AnalysisManager->FillNtupleDColumn(index, 1, MyTuple.y_pos);
//                     AnalysisManager->FillNtupleDColumn(index, 2, MyTuple.z_pos);
//                     AnalysisManager->FillNtupleDColumn(index, 3, MyTuple.n_energy);
//                     AnalysisManager->FillNtupleIColumn(index, 4, MyTuple.N_x);
//                     AnalysisManager->FillNtupleIColumn(index, 5, MyTuple.N_y);
//                     AnalysisManager->FillNtupleIColumn(index, 6, MyTuple.N_z);
//                     AnalysisManager->AddNtupleRow(index);
//                 }