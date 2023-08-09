/**
 * @file MArEXTarget.hh
 * @author Yashwanth Bezawada [ysbezawada@ucdavis.edu]
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
    class MArEXTarget
    {
    public:
        MArEXTarget();
        ~MArEXTarget();

#ifdef MAREX_YAML
        MArEXTarget(YAML::Node config);
#endif

        void DefineMaterials();

        void Construct(G4LogicalVolume* logicalWorld);

    private:
        G4String mWorldMaterialName = {"high_vacuum"};

        // main target volume
        G4double mTargetPlacement = {16.0 * m};
        G4double mTargetX = {1.5 * cm};
        G4double mTargetY = {1.5 * cm};
        G4String mActiveVolumeMaterialName = {"liquid_argon"};
        G4double mGasPressure = {0};
        G4double mTargetRadius = {1.1049 * cm};
        G4double mTargetLength = {158.60268 * cm};

        // outer container mother volume
        G4bool mConstructContainer = {true};
        G4String mContainerShellMaterialName = {"stainless_steel"};
        G4String mContainerInnerMaterialName = {"high_vacuum"};
        G4double mContainerOuterRadius = {7.62 * cm};
        G4double mContainerInnerRadius = {7.4549 * cm};
        G4double mContainerLength = {160.6169 * cm};

        G4double mTZeroLocation = {-30.0 * m};

        // Container Mother Volume
        G4Material *mContainerInnerMaterial;
        G4Tubs *mSolidContainerInner;
        G4LogicalVolume* mLogicalContainerInner;
        G4VPhysicalVolume* mPhysicalContainerInner;

        // Active Volume
        G4Material *mActiveVolumeMaterial;
        G4Tubs *mSolidActiveVolume;
        G4LogicalVolume* mLogicalActiveVolume;
        G4VPhysicalVolume* mPhysicalActiveVolume;

        // Container Shell Volume
        G4Material *mContainerShellMaterial;
        G4Tubs *mSolidContainerShell;
        G4LogicalVolume* mLogicalContainerShell;
        G4VPhysicalVolume* mPhysicalContainerShell;

        // Container Right End Volume
        G4Tubs *mSolidContainerRightEnd;
        G4LogicalVolume* mLogicalContainerRightEnd;
        G4VPhysicalVolume* mPhysicalContainerRightEnd;

        // Container Left End Volume
        G4Tubs *mSolidContainerLeftEnd;
        G4LogicalVolume* mLogicalContainerLeftEnd;
        G4VPhysicalVolume* mPhysicalContainerLeftEnd;

#ifdef MAREX_YAML
        YAML::Node mConfig;
#endif
    };
}