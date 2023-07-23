/**
 * @file Target.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#pragma once

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4GenericMessenger.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4SubtractionSolid.hh"
#include "G4Material.hh"
#include "G4SDManager.hh"
#include "G4Cache.hh"
#include "G4AutoDelete.hh"

#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UIparameter.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithABool.hh"
#include "G4GDMLParser.hh"

#ifdef MAREX_YAML
#include "yaml-cpp/yaml.h"
#endif

#include "Material.hh"
#include "SensitiveDetector.hh"

namespace marex
{
    class Target
    {
    public:
        Target();
        ~Target();

#ifdef MAREX_YAML
        Target(YAML::Node config);
#endif

        void DefineMaterials();

        void Construct(G4LogicalVolume* logicalWorld);

    private:
        G4String mWorldMaterialName = {"high_vacuum"};

        // main target volume
        G4double mTargetEntrance = {16.0 * m};
        G4double mTargetX = {1.5 * cm};
        G4double mTargetY = {1.5 * cm};
        G4String mActiveVolumeMaterialName = {"liquid_argon"};
        G4double mTargetRadius = {1.1049 * cm};
        G4double mTargetLength = {158.60268 * cm};

        // outer container mother volume
        G4bool mConstructContainer = {true};
        G4String mContainerOuterMaterialName = {"stainless_steel"};
        G4String mContainerInnerMaterialName = {"high_vacuum"};
        G4double mContainerOuterRadius = {7.62 * cm};
        G4double mContainerInnerRadius = {7.4549 * cm};
        G4double mContainerLength = {160.6169 * cm};

        G4bool mConstructAnnulus = {true};
        G4String mAnnulusInnerMaterialName = {"liquid_argon"};
        G4String mAnnulusContainerMaterialName = {"stainless_steel"};
        G4String mAnnulusLeftFaceMaterialName = {"stainless_steel"};
        G4String mAnnulusRightFaceMaterialName = {"stainless_steel"};
        G4double mAnnulusInnerRadius = {5.08 * cm};
        G4double mAnnulusContainerRadius = {4.9149 * cm};
        G4double mAnnulusLength = {139.21 * cm};
        G4double mAnnulusFaceThickness = {0.3302 * cm};

        // stainless steel tube
        G4bool mConstructActiveVolumeContainer = {true};
        G4String mActiveVolumeContainerMaterialName = {"stainless_steel"};
        G4double mActiveVolumeContainerRadius = {1.27 * cm};

        // left side inner flange
        G4bool mConstructInnerFlangeLeftSide = {true};
        G4String mInnerFlangeLeftSideMaterialName = {"aluminum"};
        G4double mInnerFlangeLeftSideRadius = {4.8133 / 2.0 * cm};
        G4double mInnerFlangeLeftSideThickness = {0.20066};

        // right side inner flange
        G4bool mConstructInnerFlangeRightSide = {true};
        G4String mInnerFlangeRightSideMaterialName = {"aluminum"};
        G4double mInnerFlangeRightSideRadius = {4.8133 / 2.0 * cm};
        G4double mInnerFlangeRightSideThickness = {0.20066};

        // inner flange gap
        G4bool mConstructInnerFlangeLeftSideGap = {true};
        G4bool mConstructInnerFlangeRightSideGap = {true};
        G4String mInnerFlangeLeftSideGapMaterialName = {"high_vacuum"};
        G4String mInnerFlangeRightSideGapMaterialName = {"high_vacuum"};
        G4double mInnerFlangeLeftSideGapThickness = {1.87706};
        G4double mInnerFlangeRightSideGapThickness = {1.87706};

        // left side Outer flange
        G4bool mConstructOuterFlangeLeftSide = {true};
        G4String mOuterFlangeLeftSideMaterialName = {"aluminum"};
        G4double mOuterFlangeLeftSideRadius = {17.10436 / 2.0 * cm};
        G4double mOuterFlangeLeftSideThickness = {0.20066};

        // right side Outer flange
        G4bool mConstructOuterFlangeRightSide = {true};
        G4String mOuterFlangeRightSideMaterialName = {"aluminum"};
        G4double mOuterFlangeRightSideRadius = {17.10436 / 2.0 * cm};
        G4double mOuterFlangeRightSideThickness = {0.20066};

        G4double mTZeroLocation = {-30.0 * m};

        // Container Mother Volume
        G4Material *mContainerOuterMaterial;
        G4Material *mContainerInnerMaterial;
        G4Tubs *mSolidContainerOuter;
        G4Tubs *mSolidContainerInner;
        G4LogicalVolume* mLogicalContainerOuter;
        G4LogicalVolume* mLogicalContainerInner;
        G4VPhysicalVolume* mPhysicalContainerOuter;
        G4VPhysicalVolume* mPhysicalContainerInner;

        // Annulus
        G4Material *mAnnulusInnerMaterial;
        G4Material *mAnnulusContainerMaterial;
        G4Material *mAnnulusLeftFaceMaterial;
        G4Material *mAnnulusRightFaceMaterial;
        G4Tubs *mSolidAnnulusInner;
        G4Tubs *mSolidAnnulusContainer;
        G4Tubs *mSolidAnnulusLeftFace;
        G4Tubs *mSolidAnnulusRightFace;
        G4LogicalVolume* mLogicalAnnulusInner;
        G4LogicalVolume* mLogicalAnnulusContainer;
        G4LogicalVolume* mLogicalAnnulusLeftFace;
        G4LogicalVolume* mLogicalAnnulusRightFace;
        G4VPhysicalVolume* mPhysicalAnnulusInner;
        G4VPhysicalVolume* mPhysicalAnnulusContainer;
        G4VPhysicalVolume* mPhysicalAnnulusLeftFace;
        G4VPhysicalVolume* mPhysicalAnnulusRightFace;

        // Active Volume
        G4Material *mActiveVolumeMaterial;
        G4Tubs *mSolidActiveVolume;
        G4LogicalVolume* mLogicalActiveVolume;
        G4VPhysicalVolume* mPhysicalActiveVolume;

        // Active Volume Container
        G4Material* mActiveVolumeContainerMaterial;
        G4Tubs *mSolidActiveVolumeContainer;
        G4LogicalVolume* mLogicalActiveVolumeContainer;
        G4VPhysicalVolume* mPhysicalActiveVolumeContainer;

        // Inner flange left side
        G4Material* mInnerFlangeLeftSideMaterial;
        G4Tubs *mSolidInnerFlangeLeftSide;
        G4LogicalVolume* mLogicalInnerFlangeLeftSide;
        G4VPhysicalVolume* mPhysicalInnerFlangeLeftSide;

        // Inner flange right side
        G4Material* mInnerFlangeRightSideMaterial;
        G4Tubs *mSolidInnerFlangeRightSide;
        G4LogicalVolume* mLogicalInnerFlangeRightSide;
        G4VPhysicalVolume* mPhysicalInnerFlangeRightSide;

        // Inner flange left side gap
        G4Material* mInnerFlangeLeftSideGapMaterial;
        G4Tubs *mSolidInnerFlangeLeftSideGap;
        G4LogicalVolume* mLogicalInnerFlangeLeftSideGap;
        G4VPhysicalVolume* mPhysicalInnerFlangeLeftSideGap;

        // Inner flange right side gap
        G4Material* mInnerFlangeRightSideGapMaterial;
        G4Tubs *mSolidInnerFlangeRightSideGap;
        G4LogicalVolume* mLogicalInnerFlangeRightSideGap;
        G4VPhysicalVolume* mPhysicalInnerFlangeRightSideGap;

        // Outer flange left side
        G4Material* mOuterFlangeLeftSideMaterial;
        G4Tubs *mSolidOuterFlangeLeftSide;
        G4LogicalVolume* mLogicalOuterFlangeLeftSide;
        G4VPhysicalVolume* mPhysicalOuterFlangeLeftSide;

        // Outer flange right side
        G4Material* mOuterFlangeRightSideMaterial;
        G4Tubs *mSolidOuterFlangeRightSide;
        G4LogicalVolume* mLogicalOuterFlangeRightSide;
        G4VPhysicalVolume* mPhysicalOuterFlangeRightSide;

#ifdef MAREX_YAML
        YAML::Node mConfig;
#endif
    };
}