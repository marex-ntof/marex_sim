/**
 * @file MArEXTarget.cxx
 * @author Yashwanth Bezawada [ysbezawada@ucdavis.edu]
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#include "MArEXTarget.hh"

namespace marex
{
    MArEXTarget::MArEXTarget()
    {
        DefineMaterials();
    }

#ifdef MAREX_YAML
    MArEXTarget::MArEXTarget(YAML::Node config)
    : mConfig(config)
    {
        if(mConfig["MArEX_target"]["target_entrance"]) { mTargetEntrance = mConfig["MArEX_target"]["target_entrance"].as<G4double>() * m; }
        if(mConfig["MArEX_target"]["target_x"]) { mTargetX = mConfig["MArEX_target"]["target_x"].as<G4double>() * cm; }
        if(mConfig["MArEX_target"]["target_y"]) { mTargetY = mConfig["MArEX_target"]["target_y"].as<G4double>() * cm; }
        if(mConfig["MArEX_target"]["active_volume_material"]) { mActiveVolumeMaterialName = mConfig["MArEX_target"]["active_volume_material"].as<std::string>(); }
        if(mConfig["MArEX_target"]["target_radius"])          { mTargetRadius = mConfig["MArEX_target"]["target_radius"].as<G4double>() * cm; }
        if(mConfig["MArEX_target"]["target_length"])          { mTargetLength = mConfig["MArEX_target"]["target_length"].as<G4double>() * cm; }
        
        if(mConfig["MArEX_target"]["construct_container"])  { mConstructContainer = mConfig["MArEX_target"]["construct_container"].as<G4bool>(); }
        if(mConfig["MArEX_target"]["container_shell_material"])   { mContainerShellMaterialName = mConfig["MArEX_target"]["container_shell_material"].as<std::string>(); }
        if(mConfig["MArEX_target"]["container_inner_material"])   { mContainerInnerMaterialName = mConfig["MArEX_target"]["container_inner_material"].as<std::string>(); }
        if(mConfig["MArEX_target"]["container_outer_radius"])     { mContainerOuterRadius = mConfig["MArEX_target"]["container_outer_radius"].as<G4double>() * cm; }
        if(mConfig["MArEX_target"]["container_inner_radius"])     { mContainerInnerRadius = mConfig["MArEX_target"]["container_inner_radius"].as<G4double>() * cm; }
        if(mConfig["MArEX_target"]["container_length"])     { mContainerLength = mConfig["MArEX_target"]["container_length"].as<G4double>() * cm; }

        if(mConfig["hall"]["world_material"])   { mWorldMaterialName = mConfig["hall"]["world_material"].as<std::string>(); }

        DefineMaterials();
    }
#endif

    void MArEXTarget::DefineMaterials()
    {
        mActiveVolumeMaterial = CreateMaterial(mActiveVolumeMaterialName, "ActiveVolume");

        mContainerShellMaterial = CreateMaterial(mContainerShellMaterialName, "ContainerOuter");
        mContainerInnerMaterial = CreateMaterial(mContainerInnerMaterialName, "ContainerInner");
    }

    MArEXTarget::~MArEXTarget()
    {
    }

    void MArEXTarget::Construct(G4LogicalVolume* logicalWorld)
    {
        DefineMaterials();
        G4double mContainerThickness = mContainerOuterRadius - mContainerInnerRadius;
        G4double mTargetZ = mTargetEntrance + 0.5 * mContainerLength;
        if (!mConfig["MArEX_target"]["target_length"])
        {
            mTargetLength = mContainerLength - 2.0 * mContainerThickness;
        }
        
        // Construct outer container mother volume
        mSolidContainerInner = new G4Tubs(
            "Solid_MArEXTargetContainerInner", 
            0,
            mContainerOuterRadius, 
            0.5 * mContainerLength, 
            0,
            2*M_PI
        );
        mLogicalContainerInner = new G4LogicalVolume(
            mSolidContainerInner, 
            mContainerInnerMaterial, 
            "Logical_MArEXTargetContainerInner"
        );
        mPhysicalContainerInner = new G4PVPlacement(
            0, 
            G4ThreeVector(mTargetX, mTargetY, mTargetZ),
            mLogicalContainerInner, 
            "Physical_MArEXTargetContainerInner", 
            logicalWorld, 
            false, 
            0, 
            true
        );

        // Construct the active volume
        mSolidActiveVolume = new G4Tubs(
            "Solid_MArEXActiveVolume", 
            0,
            mTargetRadius, 
            0.5 * mTargetLength, 
            0,
            2*M_PI
        );
        mLogicalActiveVolume = new G4LogicalVolume(
            mSolidActiveVolume, 
            mActiveVolumeMaterial, 
            "Logical_MArEXActiveVolume"
        );
        mPhysicalActiveVolume = new G4PVPlacement(
            0, 
            G4ThreeVector(0.,0.,0.), 
            mLogicalActiveVolume, 
            "Physical_MArEXActiveVolume", 
            mLogicalContainerInner, 
            false, 
            0, 
            true
        );

        if(mConstructContainer)
        {
            // construct container shell
            mSolidContainerShell = new G4Tubs(
                "Solid_MArEXTargetContainerShell", 
                mContainerInnerRadius,
                mContainerOuterRadius, 
                0.5 * mContainerLength, 
                0,
                2*M_PI
            );
            mLogicalContainerShell = new G4LogicalVolume(
                mSolidContainerShell, 
                mContainerShellMaterial, 
                "Logical_MArEXTargetContainerShell"
            );
            mPhysicalContainerShell = new G4PVPlacement(
                0, 
                G4ThreeVector(0,0,0),
                mLogicalContainerShell, 
                "Physical_MArEXTargetContainerShell", 
                mLogicalContainerInner,
                false, 
                0, 
                true
            );

            // construct container right end
            mSolidContainerRightEnd = new G4Tubs(
                "Solid_MArEXTargetContainerRightEnd", 
                0,
                mContainerInnerRadius, 
                0.5 * mContainerThickness, 
                0,
                2*M_PI
            );
            mLogicalContainerRightEnd = new G4LogicalVolume(
                mSolidContainerRightEnd, 
                mContainerShellMaterial, 
                "Logical_MArEXTargetContainerRightEnd"
            );
            mPhysicalContainerRightEnd = new G4PVPlacement(
                0, 
                G4ThreeVector(0,0, - (0.5 * mTargetLength + 0.5 * mContainerThickness)),
                mLogicalContainerRightEnd, 
                "Physical_MArEXTargetContainerRightEnd", 
                mLogicalContainerInner,
                false, 
                0, 
                true
            );

            // construct container left end
            mSolidContainerLeftEnd = new G4Tubs(
                "Solid_MArEXTargetContainerLeftEnd", 
                0,
                mContainerInnerRadius, 
                0.5 * mContainerThickness, 
                0,
                2*M_PI
            );
            mLogicalContainerLeftEnd = new G4LogicalVolume(
                mSolidContainerLeftEnd, 
                mContainerShellMaterial, 
                "Logical_MArEXTargetContainerLeftEnd"
            );
            mPhysicalContainerLeftEnd = new G4PVPlacement(
                0, 
                G4ThreeVector(0,0,(0.5 * mTargetLength + 0.5 * mContainerThickness)),
                mLogicalContainerLeftEnd, 
                "Physical_MArEXTargetContainerLeftEnd", 
                mLogicalContainerInner,
                false, 
                0, 
                true
            );
        }
    }
}