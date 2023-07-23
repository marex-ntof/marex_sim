/**
 * @file nTOF.cxx
 * @author Yashwanth Bezawada [ysbezawada@ucdavis.edu]
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#include "nTOF.hh"

namespace marex
{
    nTOF::nTOF()
    {
        DefineMaterials();
    }

#ifdef MAREX_YAML
    nTOF::nTOF(YAML::Node config)
    : mConfig(config)
    {
        if(mConfig["nTOF"]["construct_beam_pipe"])      { mConstructBeamPipe = mConfig["nTOF"]["construct_beam_pipe"].as<G4bool>(); }
        if(mConfig["nTOF"]["beam_pipe_length"])         { mBeamPipeLength = mConfig["nTOF"]["beam_pipe_length"].as<G4double>() * m; }
        if(mConfig["nTOF"]["beam_pipe_inner_radius"])   { mBeamPipeInnerRadius = mConfig["nTOF"]["beam_pipe_inner_radius"].as<G4double>() * cm; }
        if(mConfig["nTOF"]["beam_pipe_outer_radius"])   { mBeamPipeOuterRadius = mConfig["nTOF"]["beam_pipe_outer_radius"].as<G4double>() * cm; }
        if(mConfig["nTOF"]["beam_material"])            { mBeamPipeVacuumMaterialName = mConfig["nTOF"]["beam_material"].as<std::string>(); }
        if(mConfig["nTOF"]["beam_pipe_material"])       { mBeamPipePipeMaterialName = mConfig["nTOF"]["beam_pipe_material"].as<std::string>(); }
        if(mConfig["nTOF"]["beam_pipe_entrance"])       { mBeamPipeEntrance = mConfig["nTOF"]["beam_pipe_entrance"].as<G4double>() * m; }
        
        // if(mConfig["hall"]["world_material"])           { mWorldMaterialName = mConfig["hall"]["world_material"].as<std::string>(); }
        // if(mConfig["generator"]["t_zero_location"])     { mTZeroLocation = mConfig["generator"]["t_zero_location"].as<G4double>() * m;}

        DefineMaterials();
    }
#endif

    void nTOF::DefineMaterials()
    {
        mBeamPipeVacuumMaterial = CreateMaterial(mBeamPipeVacuumMaterialName, "BeamPipeVacuum");
        mBeamPipePipeMaterial = CreateMaterial(mBeamPipePipeMaterialName, "BeamPipePipe");
    }

    nTOF::~nTOF()
    {
    }

    void nTOF::Construct(G4LogicalVolume* logicalWorld)
    {
        DefineMaterials();

        // // Construct the hall
        // if(mConstructHall)
        // {
        //     G4Box* OuterWall = new G4Box(
        //         "Solid_MArEXHall_OuterWall",
        //         mWorldX / 2.0,
        //         mWorldY / 2.0,
        //         mWorldZ / 2.0
        //     );
        //     G4Box* InnerWall = new G4Box(
        //         "Solid_MArEXHall_InnerWall",
        //         mWorldX / 2.0 - mWallThickness,
        //         mWorldY / 2.0 - mWallThickness,
        //         mWorldZ / 2.0 - mWallThickness
        //     );
        //     mSolidHall = new G4SubtractionSolid(
        //         "Solid_MArEXHall", 
        //         OuterWall,
        //         InnerWall
        //     );
        //     mLogicalHall = new G4LogicalVolume(
        //         mSolidHall, 
        //         mHallMaterial, 
        //         "Logical_MArEXHall"
        //     );
        //     mPhysicalHall = new G4PVPlacement(
        //         0, 
        //         G4ThreeVector(), 
        //         mLogicalHall, 
        //         "Physical_MArEXHall", 
        //         logicalWorld, 
        //         false, 
        //         0, 
        //         true
        //     );
        // }

        mBeamPipePosition = {
            0.,
            0.,
            (mBeamPipeLength - mBeamPipeEntrance) / 2.0
        };

        // Construct the beampipe
        if(mConstructBeamPipe)
        {
            // Beam pipe vacuum
            mSolidBeamPipeVacuum = new G4Tubs(
                "Solid_MArEXBeamPipeVacuum", 
                0,
                mBeamPipeInnerRadius, 
                0.5 * mBeamPipeLength, 
                0,
                2*M_PI
            );
            mLogicalBeamPipeVacuum = new G4LogicalVolume(
                mSolidBeamPipeVacuum, 
                mBeamPipeVacuumMaterial, 
                "Logical_MArEXBeamPipeVacuum"
            );
            mPhysicalBeamPipeVacuum = new G4PVPlacement(
                0, 
                mBeamPipePosition, 
                mLogicalBeamPipeVacuum, 
                "Physical_MArEXBeamPipeVacuum", 
                logicalWorld, 
                false, 
                0, 
                true
            );

            // Beam Pipe 
            mSolidBeamPipePipe = new G4Tubs(
                "Solid_MArEXBeamPipePipe", 
                mBeamPipeInnerRadius,
                mBeamPipeOuterRadius, 
                0.5 * mBeamPipeLength, 
                0,
                2*M_PI
            );
            mLogicalBeamPipePipe = new G4LogicalVolume(
                mSolidBeamPipePipe, 
                mBeamPipePipeMaterial, 
                "Logical_MArEXBeamPipePipe"
            );
            mPhysicalBeamPipePipe = new G4PVPlacement(
                0, 
                mBeamPipePosition,
                mLogicalBeamPipePipe, 
                "Physical_MArEXBeamPipePipe", 
                logicalWorld, 
                false, 
                0, 
                true
            );
        }
    }
}