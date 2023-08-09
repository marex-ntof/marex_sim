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

        if(mConfig["nTOF"]["beam_pipe_middle_z_start"])        { mBeamPipeMidZStart = mConfig["nTOF"]["beam_pipe_middle_z_start"].as<G4double>() * m; }
        if(mConfig["nTOF"]["beam_pipe_middle_z_end"])          { mBeamPipeMidZEnd = mConfig["nTOF"]["beam_pipe_middle_z_end"].as<G4double>() * m; }
        if(mConfig["nTOF"]["beam_pipe_middle_inner_radius"])   { mBeamPipeMidInnerRadius = mConfig["nTOF"]["beam_pipe_middle_inner_radius"].as<G4double>() * cm; }
        if(mConfig["nTOF"]["beam_pipe_middle_outer_radius"])   { mBeamPipeMidOuterRadius = mConfig["nTOF"]["beam_pipe_middle_outer_radius"].as<G4double>() * cm; }

        if(mConfig["hall"]["construct_hall"])           { mConstructHall = mConfig["hall"]["construct_hall"].as<G4bool>(); }
        if(mConfig["hall"]["hall_material"])            { mHallMaterialName = mConfig["hall"]["hall_material"].as<std::string>(); }
        if(mConfig["hall"]["hall_width"])               { mHallWidth = mConfig["hall"]["hall_width"].as<G4double>() * m; }
        if(mConfig["hall"]["hall_height"])              { mHallHeight = mConfig["hall"]["hall_height"].as<G4double>() * m; }
        if(mConfig["hall"]["hall_length"])              { mHallLength = mConfig["hall"]["hall_length"].as<G4double>() * m; }
        if(mConfig["hall"]["hall_thickness"])           { mHallThickness = mConfig["hall"]["hall_thickness"].as<G4double>() * m; }

        if(mConfig["hall"]["construct_detector_wall"])  { mConstructDetectorWall = mConfig["hall"]["construct_detector_wall"].as<G4bool>(); }
        if(mConfig["hall"]["detector_wall_thickness"])  { mDetectorWallThickness = mConfig["hall"]["detector_wall_thickness"].as<G4double>() * m; }
        if(mConfig["hall"]["detector_wall_z"])          { mDetectorWallZ = mConfig["hall"]["detector_wall_z"].as<G4double>() * m; } 
        if(mConfig["hall"]["detector_wall_hole_dia"])   { mDetectorWallHoleDia = mConfig["hall"]["detector_wall_hole_dia"].as<G4double>() * cm; }

        if(mConfig["collimators"]["construct_collimator_upstream"])     { mConstructUpCollimator = mConfig["collimators"]["construct_collimator_upstream"].as<G4bool>(); }
        if(mConfig["collimators"]["collimator_upstream_length"])        { mUpCollimatorLength = mConfig["collimators"]["collimator_upstream_length"].as<G4double>() * cm; }
        if(mConfig["collimators"]["collimator_upstream_radius"])        { mUpCollimatorRadius = mConfig["collimators"]["collimator_upstream_radius"].as<G4double>() * cm; }
        if(mConfig["collimators"]["collimator_upstream_hole_dia"])      { mUpCollimatorHoleDia = mConfig["collimators"]["collimator_upstream_hole_dia"].as<G4double>() * mm; }
        if(mConfig["collimators"]["collimator_upstream_z"])             { mUpCollimatorZ = mConfig["collimators"]["collimator_upstream_z"].as<G4double>() * m; }
        if(mConfig["collimators"]["collimator_upstream_material"])      { mUpCollimatorMaterialName = mConfig["collimators"]["collimator_upstream_material"].as<std::string>(); }

        if(mConfig["collimators"]["construct_collimator_downstream"])   { mConstructDownCollimator = mConfig["collimators"]["construct_collimator_downstream"].as<G4bool>(); }
        if(mConfig["collimators"]["collimator_downstream_length"])      { mDownCollimatorLength = mConfig["collimators"]["collimator_downstream_length"].as<G4double>() * cm; }
        if(mConfig["collimators"]["collimator_downstream_radius"])      { mDownCollimatorRadius = mConfig["collimators"]["collimator_downstream_radius"].as<G4double>() * cm; }
        if(mConfig["collimators"]["collimator_downstream_hole_dia"])    { mDownCollimatorHoleDia = mConfig["collimators"]["collimator_downstream_hole_dia"].as<G4double>() * mm; }
        if(mConfig["collimators"]["collimator_downstream_z"])           { mDownCollimatorZ = mConfig["collimators"]["collimator_downstream_z"].as<G4double>() * m; }
        if(mConfig["collimators"]["collimator_downstream_material"])    { mDownCollimatorMaterialName = mConfig["collimators"]["collimator_downstream_material"].as<std::string>(); }

        if(mConfig["collimators"]["construct_boratedPE"])   { mConstructBoratedPE = mConfig["collimators"]["construct_boratedPE"].as<G4bool>(); }
        if(mConfig["collimators"]["boratedPE_length"])      { mBoratedPELength = mConfig["collimators"]["boratedPE_length"].as<G4double>() * cm; }
        if(mConfig["collimators"]["boratedPE_material"])    { mBoratedPEMaterialName = mConfig["collimators"]["boratedPE_material"].as<std::string>(); }

        // if(mConfig["hall"]["world_material"])           { mWorldMaterialName = mConfig["hall"]["world_material"].as<std::string>(); }
        // if(mConfig["generator"]["t_zero_location"])     { mTZeroLocation = mConfig["generator"]["t_zero_location"].as<G4double>() * m;}

        DefineMaterials();
    }
#endif

    void nTOF::DefineMaterials()
    {
        mBeamPipeVacuumMaterial = CreateMaterial(mBeamPipeVacuumMaterialName, "BeamPipeVacuum");
        mBeamPipePipeMaterial = CreateMaterial(mBeamPipePipeMaterialName, "BeamPipePipe");
        mHallMaterial = CreateMaterial(mHallMaterialName, "Hall");
        mUpCollimatorMaterial = CreateMaterial(mUpCollimatorMaterialName, "UpCollimator");
        mDownCollimatorMaterial = CreateMaterial(mDownCollimatorMaterialName, "DownCollimator");
        mBoratedPEMaterial = CreateMaterial(mBoratedPEMaterialName, "BoratedPE");
    }

    nTOF::~nTOF()
    {
    }

    void nTOF::Construct(G4LogicalVolume* logicalWorld)
    {
        DefineMaterials();

        // Construct the hall
        if(mConstructHall)
        {
            G4double mHallOffset = 2.0 * m;
            mHallPosition = {
                0.,
                0.,
                (mHallLength/2.0 - mHallOffset)
            };

            G4Box* OuterWall = new G4Box(
                "Solid_MArEXHall_OuterWall",
                mHallWidth / 2.0,
                mHallHeight / 2.0,
                mHallLength / 2.0
            );
            G4Box* InnerWall = new G4Box(
                "Solid_MArEXHall_InnerWall",
                mHallWidth / 2.0 - mHallThickness,
                mHallHeight / 2.0 - mHallThickness,
                mHallLength / 2.0 - mHallThickness
            );
            mSolidHall = new G4SubtractionSolid(
                "Solid_MArEXHall", 
                OuterWall,
                InnerWall
            );
            mLogicalHall = new G4LogicalVolume(
                mSolidHall, 
                mHallMaterial, 
                "Logical_MArEXHall"
            );
            mPhysicalHall = new G4PVPlacement(
                0, 
                mHallPosition, 
                mLogicalHall, 
                "Physical_MArEXHall", 
                logicalWorld, 
                false, 
                0, 
                true
            );
        }

        //Construct Detector Wall
        if(mConstructDetectorWall)
        {
            mDetectorWallPosition = {
                0.,
                0.,
                (mDetectorWallZ + mDetectorWallThickness/2.0)
            };

            G4Box* MainWall = new G4Box(
                "Solid_DetectorWall_MainWall",
                mHallWidth / 2.0 - mHallThickness,
                mHallHeight / 2.0 - mHallThickness,
                mDetectorWallThickness / 2.0
            );

            G4Tubs* WallHole = new G4Tubs(
                "Solid_DetectorWall_WallHole",
                0,
                mDetectorWallHoleDia / 2.0,
                mDetectorWallThickness / 2.0,
                0,
                2*M_PI
            );

            mSolidDetectorWall = new G4SubtractionSolid(
                "Solid_MArEXDetectorWall", 
                MainWall,
                WallHole
            );

            mLogicalDetectorWall = new G4LogicalVolume(
                mSolidDetectorWall, 
                mHallMaterial, 
                "Logical_MArEXDetectorWall"
            );

            mPhysicalDetectorWall = new G4PVPlacement(
                0, 
                mDetectorWallPosition, 
                mLogicalDetectorWall, 
                "Physical_MArEXDetectorWall", 
                logicalWorld, 
                false, 
                0, 
                true
            );
        }

        // Construct the beampipe
        if(mConstructBeamPipe)
        {
            mBeamPipePosition = {
                0.,
                0.,
                (mBeamPipeLength - mBeamPipeEntrance) / 2.0
            };

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

            ///////// Beam Pipe after the target
            G4double mBeamPipeMidLength = mBeamPipeMidZEnd - mBeamPipeMidZStart;
            mBeamPipeMidPosition = {
                0.,
                0.,
                mBeamPipeMidZStart + mBeamPipeMidLength * 0.5
            };

            // Beam pipe vacuum
            mSolidBeamPipeMidVacuum = new G4Tubs(
                "Solid_MArEXBeamPipeMidVacuum", 
                0,
                mBeamPipeMidInnerRadius, 
                0.5 * mBeamPipeMidLength, 
                0,
                2*M_PI
            );
            mLogicalBeamPipeMidVacuum = new G4LogicalVolume(
                mSolidBeamPipeMidVacuum, 
                mBeamPipeVacuumMaterial, 
                "Logical_MArEXBeamPipeMidVacuum"
            );
            mPhysicalBeamPipeMidVacuum = new G4PVPlacement(
                0, 
                mBeamPipeMidPosition, 
                mLogicalBeamPipeMidVacuum, 
                "Physical_MArEXBeamPipeMidVacuum", 
                logicalWorld, 
                false, 
                0, 
                true
            );

            // Beam Pipe 
            mSolidBeamPipeMidPipe = new G4Tubs(
                "Solid_MArEXBeamPipeMidPipe", 
                mBeamPipeMidInnerRadius,
                mBeamPipeMidOuterRadius, 
                0.5 * mBeamPipeMidLength, 
                0,
                2*M_PI
            );
            mLogicalBeamPipeMidPipe = new G4LogicalVolume(
                mSolidBeamPipeMidPipe, 
                mBeamPipePipeMaterial, 
                "Logical_MArEXBeamPipeMidPipe"
            );
            mPhysicalBeamPipeMidPipe = new G4PVPlacement(
                0, 
                mBeamPipeMidPosition,
                mLogicalBeamPipeMidPipe, 
                "Physical_MArEXBeamPipeMidPipe", 
                logicalWorld, 
                false, 
                0, 
                true
            );
        }

        // Collimators
        if(mConstructUpCollimator)
        {
            mUpCollimatorPosition = {
                0.,
                0.,
                (mUpCollimatorZ + mUpCollimatorLength / 2.0)
            };

            mSolidUpCollimator = new G4Tubs(
                "Solid_MArEXUpCollimator", 
                mUpCollimatorHoleDia / 2.0,
                mUpCollimatorRadius, 
                mUpCollimatorLength / 2.0, 
                0,
                2*M_PI
            );

            mLogicalUpCollimator = new G4LogicalVolume(
                mSolidUpCollimator, 
                mUpCollimatorMaterial, 
                "Logical_MArEXUpCollimator"
            );

            mPhysicalUpCollimator = new G4PVPlacement(
                0, 
                mUpCollimatorPosition,
                mLogicalUpCollimator, 
                "Physical_MArEXUpCollimator", 
                logicalWorld, 
                false, 
                0, 
                true
            );
        }

        if(mConstructDownCollimator)
        {
            mDownCollimatorPosition = {
                0.,
                0.,
                (mDownCollimatorZ + mDownCollimatorLength / 2.0)
            };

            mSolidDownCollimator = new G4Tubs(
                "Solid_MArEXDownCollimator", 
                mDownCollimatorHoleDia / 2.0,
                mDownCollimatorRadius, 
                mDownCollimatorLength / 2.0, 
                0,
                2*M_PI
            );

            mLogicalDownCollimator = new G4LogicalVolume(
                mSolidDownCollimator, 
                mDownCollimatorMaterial, 
                "Logical_MArEXDownCollimator"
            );

            mPhysicalDownCollimator = new G4PVPlacement(
                0, 
                mDownCollimatorPosition,
                mLogicalDownCollimator, 
                "Physical_MArEXDownCollimator", 
                logicalWorld, 
                false, 
                0, 
                true
            );
        }

        if(mConstructBoratedPE)
        {
            mBoratedPEPosition = {
                0.,
                0.,
                (mDownCollimatorZ + mDownCollimatorLength + mBoratedPELength * 0.5)
            };

            mSolidBoratedPE = new G4Tubs(
                "Solid_MArEXBoratedPE", 
                mDownCollimatorHoleDia / 2.0,
                mDownCollimatorRadius, 
                mBoratedPELength / 2.0, 
                0,
                2*M_PI
            );

            mLogicalBoratedPE = new G4LogicalVolume(
                mSolidBoratedPE, 
                mBoratedPEMaterial, 
                "Logical_MArEXBoratedPE"
            );

            mPhysicalBoratedPE = new G4PVPlacement(
                0, 
                mBoratedPEPosition,
                mLogicalBoratedPE, 
                "Physical_MArEXBoratedPE", 
                logicalWorld, 
                false, 
                0, 
                true
            );
        }
    }
}