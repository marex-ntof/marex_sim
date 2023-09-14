/**
 * @file EAR1.cxx
 * @author Yashwanth Bezawada [ysbezawada@ucdavis.edu]
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#include "EAR1.hh"

namespace marex
{
    EAR1::EAR1()
    {
        DefineMaterials();
    }

#ifdef MAREX_YAML
    EAR1::EAR1(YAML::Node config)
    : mConfig(config)
    {
        if(mConfig["EAR1"]["construct_beam_pipe_middle"])      { mConstructBeamPipeMid = mConfig["EAR1"]["construct_beam_pipe_middle"].as<G4bool>(); }
        if(mConfig["nTOF"]["beam_pipe_material"])              { mBeamPipePipeMaterialName = mConfig["nTOF"]["beam_pipe_material"].as<std::string>(); }
        if(mConfig["nTOF"]["beam_material"])                   { mBeamPipeVacuumMaterialName = mConfig["nTOF"]["beam_material"].as<std::string>(); }
        if(mConfig["EAR1"]["beam_pipe_middle_z_start"])        { mBeamPipeMidZStart = mConfig["EAR1"]["beam_pipe_middle_z_start"].as<G4double>() * m; }
        if(mConfig["EAR1"]["beam_pipe_middle_z_end"])          { mBeamPipeMidZEnd = mConfig["EAR1"]["beam_pipe_middle_z_end"].as<G4double>() * m; }
        if(mConfig["EAR1"]["beam_pipe_middle_inner_radius"])   { mBeamPipeMidInnerRadius = mConfig["EAR1"]["beam_pipe_middle_inner_radius"].as<G4double>() * cm; }
        if(mConfig["EAR1"]["beam_pipe_middle_outer_radius"])   { mBeamPipeMidOuterRadius = mConfig["EAR1"]["beam_pipe_middle_outer_radius"].as<G4double>() * cm; }

        if(mConfig["EAR1"]["construct_hall"])           { mConstructHall = mConfig["EAR1"]["construct_hall"].as<G4bool>(); }
        if(mConfig["nTOF"]["beam_pipe_outer_radius"])   { mBeamPipeOuterRadius = mConfig["nTOF"]["beam_pipe_outer_radius"].as<G4double>() * cm; }
        if(mConfig["EAR1"]["hall_material"])            { mHallMaterialName = mConfig["EAR1"]["hall_material"].as<std::string>(); }
        if(mConfig["EAR1"]["hall_z_start"])             { mHallZStart = mConfig["EAR1"]["hall_z_start"].as<G4double>() * m; }
        if(mConfig["EAR1"]["hall_z_end"])               { mHallZEnd = mConfig["EAR1"]["hall_z_end"].as<G4double>() * m; }
        if(mConfig["EAR1"]["hall_width"])               { mHallWidth = mConfig["EAR1"]["hall_width"].as<G4double>() * m; }
        if(mConfig["EAR1"]["hall_height"])              { mHallHeight = mConfig["EAR1"]["hall_height"].as<G4double>() * m; }
        if(mConfig["EAR1"]["hall_thickness"])           { mHallThickness = mConfig["EAR1"]["hall_thickness"].as<G4double>() * m; }

        if(mConfig["EAR1"]["construct_detector_wall"])  { mConstructDetectorWall = mConfig["EAR1"]["construct_detector_wall"].as<G4bool>(); }
        if(mConfig["EAR1"]["wall_material"])            { mWallMaterialName = mConfig["EAR1"]["wall_material"].as<std::string>(); }
        if(mConfig["EAR1"]["detector_wall_thickness"])  { mDetectorWallThickness = mConfig["EAR1"]["detector_wall_thickness"].as<G4double>() * m; }
        if(mConfig["EAR1"]["detector_wall_z"])          { mDetectorWallZ = mConfig["EAR1"]["detector_wall_z"].as<G4double>() * m; } 
        if(mConfig["EAR1"]["detector_wall_hole_dia"])   { mDetectorWallHoleDia = mConfig["EAR1"]["detector_wall_hole_dia"].as<G4double>() * cm; }
        if(mConfig["EAR1"]["hall_width"])               { mHallWidth = mConfig["EAR1"]["hall_width"].as<G4double>() * m; }
        if(mConfig["EAR1"]["hall_height"])              { mHallHeight = mConfig["EAR1"]["hall_height"].as<G4double>() * m; }
        if(mConfig["EAR1"]["hall_thickness"])           { mHallThickness = mConfig["EAR1"]["hall_thickness"].as<G4double>() * m; }

        if(mConfig["EAR1"]["construct_beam_collimator"])   { mConstructBeamCollimator = mConfig["EAR1"]["construct_beam_collimator"].as<G4bool>(); }
        if(mConfig["EAR1"]["beam_collimator_material"])    { mBeamCollimatorMaterialName = mConfig["EAR1"]["beam_collimator_material"].as<std::string>(); }
        if(mConfig["EAR1"]["beam_collimator_z"])           { mBeamCollimatorZ = mConfig["EAR1"]["beam_collimator_z"].as<G4double>() * m; }
        if(mConfig["EAR1"]["beam_collimator_thickness"])   { mBeamCollimatorThickness = mConfig["EAR1"]["beam_collimator_thickness"].as<G4double>() * mm; }
        if(mConfig["EAR1"]["beam_collimator_radius"])      { mBeamCollimatorRadius = mConfig["EAR1"]["beam_collimator_radius"].as<G4double>() * mm; }
        if(mConfig["EAR1"]["beam_collimator_hole_1_dia"])  { mBeamCollimatorHole1Dia = mConfig["EAR1"]["beam_collimator_hole_1_dia"].as<G4double>() * mm; }
        if(mConfig["EAR1"]["beam_collimator_hole_2_dia"])  { mBeamCollimatorHole2Dia = mConfig["EAR1"]["beam_collimator_hole_2_dia"].as<G4double>() * mm; }
        
        if(mConfig["collimators"]["construct_collimator"])   { mConstructCollimator = mConfig["collimators"]["construct_collimator"].as<G4bool>(); }
        if(mConfig["collimators"]["collimator_length"])      { mCollimatorLength = mConfig["collimators"]["collimator_length"].as<G4double>() * cm; }
        if(mConfig["collimators"]["collimator_radius"])      { mCollimatorRadius = mConfig["collimators"]["collimator_radius"].as<G4double>() * cm; }
        if(mConfig["collimators"]["collimator_hole_dia"])    { mCollimatorHoleDia = mConfig["collimators"]["collimator_hole_dia"].as<G4double>() * mm; }
        if(mConfig["collimators"]["collimator_z"])           { mCollimatorZ = mConfig["collimators"]["collimator_z"].as<G4double>() * m; }
        if(mConfig["collimators"]["collimator_material"])    { mCollimatorMaterialName = mConfig["collimators"]["collimator_material"].as<std::string>(); }

        if(mConfig["collimators"]["construct_boratedPE"])   { mConstructBoratedPE = mConfig["collimators"]["construct_boratedPE"].as<G4bool>(); }
        if(mConfig["collimators"]["boratedPE_length"])      { mBoratedPELength = mConfig["collimators"]["boratedPE_length"].as<G4double>() * cm; }
        if(mConfig["collimators"]["boratedPE_material"])    { mBoratedPEMaterialName = mConfig["collimators"]["boratedPE_material"].as<std::string>(); }

        // if(mConfig["EAR1"]["world_material"])           { mWorldMaterialName = mConfig["EAR1"]["world_material"].as<std::string>(); }
        // if(mConfig["generator"]["t_zero_location"])     { mTZeroLocation = mConfig["generator"]["t_zero_location"].as<G4double>() * m;}

        DefineMaterials();
    }
#endif

    void EAR1::DefineMaterials()
    {
        mBeamPipeVacuumMaterial = CreateMaterial(mBeamPipeVacuumMaterialName, "BeamPipeVacuum");
        mBeamPipePipeMaterial = CreateMaterial(mBeamPipePipeMaterialName, "BeamPipePipe");
        mHallMaterial = CreateMaterial(mHallMaterialName, "Hall");
        mWallMaterial = CreateMaterial(mWallMaterialName, "Wall");
        mBeamCollimatorMaterial = CreateMaterial(mBeamCollimatorMaterialName, "BeamCollimator");
        mCollimatorMaterial = CreateMaterial(mCollimatorMaterialName, "DownCollimator");
        mBoratedPEMaterial = CreateMaterial(mBoratedPEMaterialName, "BoratedPE");
    }

    EAR1::~EAR1()
    {
    }

    void EAR1::Construct(G4LogicalVolume* logicalWorld)
    {
        DefineMaterials();

        // Construct the hall
        if(mConstructHall)
        {
            G4double mHallLength = mHallZEnd - mHallZStart;
            
            G4double mHallOffset = 2.0 * m;
            mHallPosition = {
                0.,
                0.,
                (mHallZStart + mHallLength/2.0)
            };

            G4Box* OuterHall = new G4Box(
                "Solid_MArEXHall_OuterHall",
                mHallWidth / 2.0,
                mHallHeight / 2.0,
                mHallLength / 2.0
            );
            G4Box* InnerHall = new G4Box(
                "Solid_MArEXHall_InnerHall",
                mHallWidth / 2.0 - mHallThickness,
                mHallHeight / 2.0 - mHallThickness,
                mHallLength / 2.0 - mHallThickness
            );

            G4SubtractionSolid* mHall = new G4SubtractionSolid(
                "Solid_MArEXHall", 
                OuterHall,
                InnerHall
            );

            G4Tubs* HallHole = new G4Tubs(
                "Solid_HallHole",
                0,
                mBeamPipeOuterRadius,
                mHallThickness / 2.0,
                0,
                2*M_PI
            );

            G4ThreeVector mHallHolePosition = {
                0.,
                0.,
                -(mHallLength / 2.0 - mHallThickness / 2.0)
            };

            G4RotationMatrix* rotMatrix = new G4RotationMatrix;
            rotMatrix->rotateX(0);
            rotMatrix->rotateY(0);
            rotMatrix->rotateZ(0);  

            mSolidHall = new G4SubtractionSolid(
                "Solid_MArEXHall", 
                mHall,
                HallHole,
                rotMatrix,
                mHallHolePosition
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
                mWallMaterial, 
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
        if(mConstructBeamPipeMid)
        {
            // mBeamPipePosition = {
            //     0.,
            //     0.,
            //     (mBeamPipeLength - mBeamPipeEntrance) / 2.0
            // };

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

        // Beam Collimator
        if(mConstructBeamCollimator)
        {
            // Hole 1
            mSolidBeamCollimator1 = new G4Tubs(
                "Solid_MArEXBeamCollimator1", 
                mBeamCollimatorHole1Dia / 2.0,
                mBeamCollimatorRadius, 
                mBeamCollimatorThickness / 2.0 / 2.0, 
                0,
                2*M_PI
            );

            mLogicalBeamCollimator1 = new G4LogicalVolume(
                mSolidBeamCollimator1, 
                mBeamCollimatorMaterial,
                "Logical_MArEXBeamCollimator1"
            );

            mPhysicalBeamCollimator1 = new G4PVPlacement(
                0, 
                G4ThreeVector{0., 0., mBeamCollimatorZ - mBeamCollimatorThickness / 2.0 / 2.0},
                mLogicalBeamCollimator1, 
                "Physical_MArEXBeamCollimator1", 
                logicalWorld, 
                false, 
                0, 
                true
            );

            // Hole 2
            mSolidBeamCollimator2 = new G4Tubs(
                "Solid_MArEXBeamCollimator2", 
                mBeamCollimatorHole2Dia / 2.0,
                mBeamCollimatorRadius, 
                mBeamCollimatorThickness / 2.0 / 2.0, 
                0,
                2*M_PI
            );

            mLogicalBeamCollimator2 = new G4LogicalVolume(
                mSolidBeamCollimator2, 
                mBeamCollimatorMaterial,
                "Logical_MArEXBeamCollimator2"
            );

            mPhysicalBeamCollimator2 = new G4PVPlacement(
                0, 
                G4ThreeVector{0., 0., mBeamCollimatorZ + mBeamCollimatorThickness / 2.0 / 2.0},
                mLogicalBeamCollimator2, 
                "Physical_MArEXBeamCollimator2", 
                logicalWorld, 
                false, 
                0, 
                true
            );
        }

        // Collimators
        if(mConstructCollimator)
        {
            mCollimatorPosition = {
                0.,
                0.,
                (mCollimatorZ + mCollimatorLength / 2.0)
            };

            mSolidCollimator = new G4Tubs(
                "Solid_MArEXCollimator", 
                mCollimatorHoleDia / 2.0,
                mCollimatorRadius, 
                mCollimatorLength / 2.0, 
                0,
                2*M_PI
            );

            mLogicalCollimator = new G4LogicalVolume(
                mSolidCollimator, 
                mCollimatorMaterial, 
                "Logical_MArEXCollimator"
            );

            mPhysicalCollimator = new G4PVPlacement(
                0, 
                mCollimatorPosition,
                mLogicalCollimator, 
                "Physical_MArEXCollimator", 
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
                (mCollimatorZ + mCollimatorLength + mBoratedPELength * 0.5)
            };

            mSolidBoratedPE = new G4Tubs(
                "Solid_MArEXBoratedPE", 
                mCollimatorHoleDia / 2.0,
                mCollimatorRadius, 
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