/**
 * @file Target.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#include "Target.hh"

namespace marex
{
    Target::Target()
    {
        DefineMaterials();
    }

#ifdef MAREX_YAML
    Target::Target(YAML::Node config)
    : mConfig(config)
    {
        if(mConfig["target"]["target_entrance"]) { mTargetEntrance = mConfig["target"]["target_entrance"].as<G4double>() * m; }
        if(mConfig["target"]["target_x"]) { mTargetX = mConfig["target"]["target_x"].as<G4double>() * cm; }
        if(mConfig["target"]["target_y"]) { mTargetY = mConfig["target"]["target_y"].as<G4double>() * cm; }
        if(mConfig["target"]["active_volume_material"]) { mActiveVolumeMaterialName = mConfig["target"]["active_volume_material"].as<std::string>(); }
        if(mConfig["target"]["target_radius"])          { mTargetRadius = mConfig["target"]["target_radius"].as<G4double>() * cm; }
        if(mConfig["target"]["target_length"])          { mTargetLength = mConfig["target"]["target_length"].as<G4double>() * cm; }
        
        if(mConfig["target"]["construct_container"])  { mConstructContainer = mConfig["target"]["construct_container"].as<G4bool>(); }
        if(mConfig["target"]["container_outer_material"])   { mContainerOuterMaterialName = mConfig["target"]["container_outer_material"].as<std::string>(); }
        if(mConfig["target"]["container_inner_material"])   { mContainerInnerMaterialName = mConfig["target"]["container_inner_material"].as<std::string>(); }
        if(mConfig["target"]["container_outer_radius"])     { mContainerOuterRadius = mConfig["target"]["container_outer_radius"].as<G4double>() * cm; }
        if(mConfig["target"]["container_inner_radius"])     { mContainerInnerRadius = mConfig["target"]["container_inner_radius"].as<G4double>() * cm; }
        if(mConfig["target"]["container_length"])     { mContainerLength = mConfig["target"]["container_length"].as<G4double>() * cm; }

        if(mConfig["target"]["construct_annulus"])          { mConstructAnnulus = mConfig["target"]["construct_annulus"].as<G4bool>(); }
        if(mConfig["target"]["annulus_container_material"]) { mAnnulusContainerMaterialName = mConfig["target"]["annulus_container_material"].as<std::string>(); }
        if(mConfig["target"]["annulus_inner_material"])     { mAnnulusInnerMaterialName = mConfig["target"]["annulus_inner_material"].as<std::string>(); }
        if(mConfig["target"]["annulus_container_radius"])   { mAnnulusContainerRadius = mConfig["target"]["annulus_container_radius"].as<G4double>() * cm; }
        if(mConfig["target"]["annulus_inner_radius"])       { mAnnulusInnerRadius = mConfig["target"]["annulus_inner_radius"].as<G4double>() * cm; }
        if(mConfig["target"]["annulus_length"])             { mAnnulusLength = mConfig["target"]["annulus_length"].as<G4double>() * cm; }
        if(mConfig["target"]["annulus_face_thickness"])     { mAnnulusFaceThickness = mConfig["target"]["annulus_face_thickness"].as<G4double>() * cm; }

        if(mConfig["target"]["construct_active_volume_container"])  { mConstructActiveVolumeContainer = mConfig["target"]["construct_active_volume_container"].as<G4bool>(); }
        if(mConfig["target"]["active_volume_container_material"])   { mActiveVolumeContainerMaterialName = mConfig["target"]["active_volume_container_material"].as<std::string>(); }
        if(mConfig["target"]["active_volume_container_radius"])     { mActiveVolumeContainerRadius = mConfig["target"]["active_volume_container_radius"].as<G4double>() * cm; }

        if(mConfig["target"]["construct_inner_flange_left_side"]) { mConstructInnerFlangeLeftSide = mConfig["target"]["construct_inner_flange_left_side"].as<G4bool>(); }
        if(mConfig["target"]["inner_flange_left_side_material"])  { mInnerFlangeLeftSideMaterialName = mConfig["target"]["inner_flange_left_side_material"].as<std::string>(); }
        if(mConfig["target"]["inner_flange_left_side_radius"])    { mInnerFlangeLeftSideRadius = mConfig["target"]["inner_flange_left_side_radius"].as<G4double>() * cm; }
        if(mConfig["target"]["inner_flange_left_side_thickness"]) { mInnerFlangeLeftSideThickness = mConfig["target"]["inner_flange_left_side_thickness"].as<G4double>() * cm; } 
        
        if(mConfig["target"]["construct_inner_flange_right_side"]) { mConstructInnerFlangeRightSide = mConfig["target"]["construct_inner_flange_right_side"].as<G4bool>(); }
        if(mConfig["target"]["inner_flange_right_side_material"])  { mInnerFlangeRightSideMaterialName = mConfig["target"]["inner_flange_right_side_material"].as<std::string>(); }
        if(mConfig["target"]["inner_flange_right_side_radius"])    { mInnerFlangeRightSideRadius = mConfig["target"]["inner_flange_right_side_radius"].as<G4double>() * cm; }
        if(mConfig["target"]["inner_flange_right_side_thickness"]) { mInnerFlangeRightSideThickness = mConfig["target"]["inner_flange_right_side_thickness"].as<G4double>() * cm; }
        
        if(mConfig["target"]["construct_inner_flange_left_side_gap"]) { mConstructInnerFlangeLeftSideGap = mConfig["target"]["construct_inner_flange_left_side_gap"].as<G4bool>(); }
        if(mConfig["target"]["inner_flange_left_side_gap_material"])  { mInnerFlangeLeftSideGapMaterialName = mConfig["target"]["inner_flange_left_side_gap_material"].as<std::string>(); }
        if(mConfig["target"]["inner_flange_left_side_gap_thickness"]) { mInnerFlangeLeftSideGapThickness = mConfig["target"]["inner_flange_left_side_gap_thickness"].as<G4double>() * cm; } 
        
        if(mConfig["target"]["construct_inner_flange_left_side_gap"]) { mConstructInnerFlangeLeftSideGap = mConfig["target"]["construct_inner_flange_left_side_gap"].as<G4bool>(); }
        if(mConfig["target"]["inner_flange_right_side_gap_material"])  { mInnerFlangeRightSideGapMaterialName = mConfig["target"]["inner_flange_right_side_gap_material"].as<std::string>(); }
        if(mConfig["target"]["inner_flange_right_side_gap_thickness"]) { mInnerFlangeRightSideGapThickness = mConfig["target"]["inner_flange_right_side_gap_thickness"].as<G4double>() * cm; } 

        if(mConfig["target"]["construct_outer_flange_left_side"]) { mConstructOuterFlangeLeftSide = mConfig["target"]["construct_outer_flange_left_side"].as<G4bool>(); }
        if(mConfig["target"]["outer_flange_left_side_material"])  { mOuterFlangeLeftSideMaterialName = mConfig["target"]["outer_flange_left_side_material"].as<std::string>(); }
        if(mConfig["target"]["outer_flange_left_side_radius"])    { mOuterFlangeLeftSideRadius = mConfig["target"]["outer_flange_left_side_radius"].as<G4double>() * cm; }
        if(mConfig["target"]["outer_flange_left_side_thickness"]) { mOuterFlangeLeftSideThickness = mConfig["target"]["outer_flange_left_side_thickness"].as<G4double>() * cm; }
        
        if(mConfig["target"]["construct_outer_flange_right_side"]) { mConstructOuterFlangeRightSide = mConfig["target"]["construct_outer_flange_right_side"].as<G4bool>(); }
        if(mConfig["target"]["outer_flange_right_side_material"])  { mOuterFlangeRightSideMaterialName = mConfig["target"]["outer_flange_right_side_material"].as<std::string>(); }
        if(mConfig["target"]["outer_flange_right_side_radius"])    { mOuterFlangeRightSideRadius = mConfig["target"]["outer_flange_right_side_radius"].as<G4double>() * cm; }
        if(mConfig["target"]["outer_flange_right_side_thickness"]) { mOuterFlangeRightSideThickness = mConfig["target"]["outer_flange_right_side_thickness"].as<G4double>() * cm; }

        if(mConfig["world"]["world_material"])   { mWorldMaterialName = mConfig["world"]["world_material"].as<std::string>(); }

        DefineMaterials();
    }
#endif

    void Target::DefineMaterials()
    {
        mActiveVolumeMaterial = CreateMaterial(mActiveVolumeMaterialName, "ActiveVolume");

        mContainerOuterMaterial = CreateMaterial(mContainerOuterMaterialName, "ContainerOuter");
        mContainerInnerMaterial = CreateMaterial(mContainerInnerMaterialName, "ContainerInner");

        mAnnulusInnerMaterial = CreateMaterial(mAnnulusInnerMaterialName, "AnnulusInner");
        mAnnulusContainerMaterial = CreateMaterial(mAnnulusContainerMaterialName, "AnnulusContainer");
        mAnnulusLeftFaceMaterial = CreateMaterial(mAnnulusLeftFaceMaterialName, "AnnulusLeftFace");
        mAnnulusRightFaceMaterial = CreateMaterial(mAnnulusRightFaceMaterialName, "AnnulusRightFace");

        mActiveVolumeContainerMaterial = CreateMaterial(mActiveVolumeContainerMaterialName, "ActiveVolumeContainer"); 
        mInnerFlangeLeftSideMaterial = CreateMaterial(mInnerFlangeLeftSideMaterialName, "InnerFlangeLeftSide");
        mInnerFlangeRightSideMaterial = CreateMaterial(mInnerFlangeRightSideMaterialName, "InnerFlangeRightSide");
        
        mOuterFlangeLeftSideMaterial = CreateMaterial(mOuterFlangeLeftSideMaterialName, "OuterFlangeLeftSide");
        mOuterFlangeRightSideMaterial = CreateMaterial(mOuterFlangeRightSideMaterialName, "OuterFlangeRightSide");
    }

    Target::~Target()
    {
    }

    void Target::Construct(G4LogicalVolume* logicalWorld)
    {
        DefineMaterials();
        G4double mTargetZ = mTargetEntrance + 0.5 * mContainerLength;
        // Construct outer container mother volume
        mSolidContainerInner = new G4Tubs(
            "Solid_TargetContainerInner", 
            0,
            mContainerOuterRadius, 
            0.5 * mContainerLength, 
            0,
            2*M_PI
        );
        mLogicalContainerInner = new G4LogicalVolume(
            mSolidContainerInner, 
            mContainerInnerMaterial, 
            "Logical_TargetContainerInner"
        );
        mPhysicalContainerInner = new G4PVPlacement(
            0, 
            G4ThreeVector(mTargetX, mTargetY, mTargetZ),
            mLogicalContainerInner, 
            "Physical_TargetContainerInner", 
            logicalWorld, 
            false, 
            0, 
            true
        );

        // Construct the active volume
        mSolidActiveVolume = new G4Tubs(
            "Solid_ActiveVolume", 
            0,
            mTargetRadius, 
            0.5 * mTargetLength, 
            0,
            2*M_PI
        );
        mLogicalActiveVolume = new G4LogicalVolume(
            mSolidActiveVolume, 
            mActiveVolumeMaterial, 
            "Logical_ActiveVolume"
        );
        mPhysicalActiveVolume = new G4PVPlacement(
            0, 
            G4ThreeVector(0.,0.,0.), 
            mLogicalActiveVolume, 
            "Physical_ActiveVolume", 
            mLogicalContainerInner, 
            false, 
            0, 
            true
        );

        if(mConstructContainer)
        {
            // construct outer container
            mSolidContainerOuter = new G4Tubs(
                "Solid_TargetContainerOuter", 
                mContainerInnerRadius,
                mContainerOuterRadius, 
                0.5 * mContainerLength, 
                0,
                2*M_PI
            );
            mLogicalContainerOuter = new G4LogicalVolume(
                mSolidContainerOuter, 
                mContainerOuterMaterial, 
                "Logical_TargetContainerOuter"
            );
            mPhysicalContainerOuter = new G4PVPlacement(
                0, 
                G4ThreeVector(0,0,0),
                mLogicalContainerOuter, 
                "Physical_TargetContainerOuter", 
                mLogicalContainerInner, 
                false, 
                0, 
                true
            );
        }

        if(mConstructAnnulus)
        {
            // construct annulus argon
            mSolidAnnulusInner = new G4Tubs(
                "Solid_TargetAnnulusInner", 
                mAnnulusInnerRadius,
                mContainerInnerRadius, 
                0.5 * mAnnulusLength, 
                0,
                2*M_PI
            );
            mLogicalAnnulusInner = new G4LogicalVolume(
                mSolidAnnulusInner, 
                mAnnulusInnerMaterial, 
                "Logical_TargetAnnulusInner"
            );
            mPhysicalAnnulusInner = new G4PVPlacement(
                0, 
                G4ThreeVector(0,0,0),
                mLogicalAnnulusInner, 
                "Physical_TargetAnnulusInner", 
                mLogicalContainerInner, 
                false, 
                0, 
                true
            );

            // construct annulus inner container
            mSolidAnnulusContainer = new G4Tubs(
                "Solid_TargetAnnulusContainer", 
                mAnnulusContainerRadius,
                mAnnulusInnerRadius, 
                0.5 * mAnnulusLength, 
                0,
                2*M_PI
            );
            mLogicalAnnulusContainer = new G4LogicalVolume(
                mSolidAnnulusContainer, 
                mAnnulusContainerMaterial, 
                "Logical_TargetAnnulusContainer"
            );
            mPhysicalAnnulusContainer = new G4PVPlacement(
                0, 
                G4ThreeVector(0,0,0),
                mLogicalAnnulusContainer, 
                "Physical_TargetAnnulusContainer", 
                mLogicalContainerInner, 
                false, 
                0, 
                true
            );

            // contruct annulus left face
            mSolidAnnulusLeftFace = new G4Tubs(
                "Solid_TargetAnnulusLeftFace", 
                mAnnulusContainerRadius,
                mContainerInnerRadius, 
                0.5 * mAnnulusFaceThickness, 
                0,
                2*M_PI
            );
            mLogicalAnnulusLeftFace = new G4LogicalVolume(
                mSolidAnnulusLeftFace, 
                mAnnulusLeftFaceMaterial, 
                "Logical_TargetAnnulusLeftFace"
            );
            mPhysicalAnnulusLeftFace = new G4PVPlacement(
                0, 
                G4ThreeVector(0,0,-(mAnnulusLength + mAnnulusFaceThickness) * 0.5),
                mLogicalAnnulusLeftFace, 
                "Physical_TargetAnnulusLeftFace", 
                mLogicalContainerInner, 
                false, 
                0, 
                true
            );
            // contruct annulus left face
            mSolidAnnulusRightFace = new G4Tubs(
                "Solid_TargetAnnulusRightFace", 
                mAnnulusContainerRadius,
                mContainerInnerRadius, 
                0.5 * mAnnulusFaceThickness, 
                0,
                2*M_PI
            );
            mLogicalAnnulusRightFace = new G4LogicalVolume(
                mSolidAnnulusRightFace, 
                mAnnulusRightFaceMaterial, 
                "Logical_TargetAnnulusRightFace"
            );
            mPhysicalAnnulusRightFace = new G4PVPlacement(
                0, 
                G4ThreeVector(0,0,+(mAnnulusLength + mAnnulusFaceThickness) * 0.5),
                mLogicalAnnulusRightFace, 
                "Physical_TargetAnnulusRightFace", 
                mLogicalContainerInner, 
                false, 
                0, 
                true
            );
        }

        // Construct the active volume container
        if(mConstructActiveVolumeContainer)
        {
            // ActiveVolumeContainer
            mSolidActiveVolumeContainer = new G4Tubs(
                "Solid_TargetActiveVolumeContainer", 
                mTargetRadius,
                mActiveVolumeContainerRadius, 
                0.5 * mTargetLength, 
                0,
                2*M_PI
            );
            mLogicalActiveVolumeContainer = new G4LogicalVolume(
                mSolidActiveVolumeContainer, 
                mActiveVolumeContainerMaterial, 
                "Logical_TargetActiveVolumeContainer"
            );
            mPhysicalActiveVolumeContainer = new G4PVPlacement(
                0, 
                G4ThreeVector(0,0,0),
                mLogicalActiveVolumeContainer, 
                "Physical_TargetActiveVolumeContainer", 
                mLogicalContainerInner, 
                false, 
                0, 
                true
            );

            // left side inner flange
            if(mConstructInnerFlangeLeftSide)
            {
                mSolidInnerFlangeLeftSide = new G4Tubs(
                    "Solid_TargetInnerFlangeLeftSide",
                    0,
                    mInnerFlangeLeftSideRadius,
                    0.5 * mInnerFlangeLeftSideThickness,
                    0,
                    2*M_PI
                );
                mLogicalInnerFlangeLeftSide = new G4LogicalVolume(
                    mSolidInnerFlangeLeftSide,
                    mInnerFlangeLeftSideMaterial,
                    "Logical_TargetInnerFlangeLeftSide"
                );
                mPhysicalInnerFlangeLeftSide = new G4PVPlacement(
                    0,
                    G4ThreeVector(
                        0,
                        0,
                        -(mTargetLength + mInnerFlangeLeftSideThickness) * 0.5),
                    mLogicalInnerFlangeLeftSide,
                    "Physical_TargetInnerFlangeLeftSide",
                    mLogicalContainerInner,
                    false,
                    0,
                    true 
                );
            }
            // right side inner flange
            if(mConstructInnerFlangeRightSide)
            {
                mSolidInnerFlangeRightSide = new G4Tubs(
                    "Solid_TargetInnerFlangeRightSide",
                    0,
                    mInnerFlangeRightSideRadius,
                    0.5 * mInnerFlangeRightSideThickness,
                    0,
                    2*M_PI
                );
                mLogicalInnerFlangeRightSide = new G4LogicalVolume(
                    mSolidInnerFlangeRightSide,
                    mInnerFlangeRightSideMaterial,
                    "Logical_TargetInnerFlangeRightSide"
                );
                mPhysicalInnerFlangeRightSide = new G4PVPlacement(
                    0,
                    G4ThreeVector(
                        0,
                        0,
                        + (mTargetLength + mInnerFlangeRightSideThickness) * 0.5),
                    mLogicalInnerFlangeRightSide,
                    "Physical_TargetInnerFlangeRightSide",
                    mLogicalContainerInner,
                    false,
                    0,
                    true 
                );
            }
            // left side Outer flange
            if(mConstructOuterFlangeLeftSide)
            {
                mSolidOuterFlangeLeftSide = new G4Tubs(
                    "Solid_TargetOuterFlangeLeftSide",
                    0,
                    mOuterFlangeLeftSideRadius,
                    0.5 * mOuterFlangeLeftSideThickness,
                    0,
                    2*M_PI
                );
                mLogicalOuterFlangeLeftSide = new G4LogicalVolume(
                    mSolidOuterFlangeLeftSide,
                    mOuterFlangeLeftSideMaterial,
                    "Logical_TargetOuterFlangeLeftSide"
                );
                mPhysicalOuterFlangeLeftSide = new G4PVPlacement(
                    0,
                    G4ThreeVector(
                        mTargetX,
                        mTargetY,
                        mTargetZ - (mContainerLength + mOuterFlangeLeftSideThickness) * 0.5),
                    mLogicalOuterFlangeLeftSide,
                    "Physical_TargetOuterFlangeLeftSide",
                    logicalWorld,
                    false,
                    0,
                    true 
                );
            }
            // right side Outer flange
            if(mConstructOuterFlangeRightSide)
            {
                mSolidOuterFlangeRightSide = new G4Tubs(
                    "Solid_TargetOuterFlangeRightSide",
                    0,
                    mOuterFlangeRightSideRadius,
                    0.5 * mOuterFlangeRightSideThickness,
                    0,
                    2*M_PI
                );
                mLogicalOuterFlangeRightSide = new G4LogicalVolume(
                    mSolidOuterFlangeRightSide,
                    mOuterFlangeRightSideMaterial,
                    "Logical_TargetOuterFlangeRightSide"
                );
                mPhysicalOuterFlangeRightSide = new G4PVPlacement(
                    0,
                    G4ThreeVector(
                        mTargetX,
                        mTargetY,
                        mTargetZ + (mContainerLength + mOuterFlangeRightSideThickness) * 0.5),
                    mLogicalOuterFlangeRightSide,
                    "Physical_TargetOuterFlangeRightSide",
                    logicalWorld,
                    false,
                    0,
                    true 
                );
            }
        }
    }
}