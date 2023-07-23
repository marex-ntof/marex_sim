/**
 * @file DICER.hh
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

#ifdef ARTIE_YAML
#include "yaml-cpp/yaml.h"
#endif

#include "Material.hh"
#include "SensitiveDetector.hh"

namespace Artie
{
    class DICER
    {
    public:
        DICER();
        ~DICER();

#ifdef ARTIE_YAML
        DICER(YAML::Node config);
#endif

        void DefineMaterials();
        void Construct(G4LogicalVolume* logicalWorld);
    
    private:
        G4String mWorldMaterialName = {"high_vacuum"};

        // RBB
        G4bool mConstructRotatingBeamBlocker = {true};
        G4String mRotatingBeamBlockerMaterialName = {"brass"};
        G4double mRotatingBeamBlockerEntrance = {14.35 * m};
        G4double mRotatingBeamBlockerDiameter = {21.5 * cm};
        G4double mRotatingBeamBlockerLength = {30.5 * cm};
        G4double mRotatingBeamBlockerHoleSize = {0.8 * cm};
        G4double mRotatingBeamBlockerHoleSeparation = {3.0 * cm};

        // BC
        G4bool mConstructBinocularCollimator = {true};
        G4String mBinocularCollimatorMaterialName = {"brass"};
        G4double mBinocularCollimatorEntrance = {14.85 * m};
        G4double mBinocularCollimatorWidth = {15.0 * cm};
        G4double mBinocularCollimatorHeight = {15.0 * cm};
        G4double mBinocularCollimatorLength = {30.0 * cm};
        G4double mBinocularCollimatorHoleSize = {0.1 * cm};
        G4double mBinocularCollimatorHoleSeparation = {3.0 * cm};

        // AS
        G4bool mConstructAperatureStop = {true};
        G4String mAperatureStopMaterialName = {"brass"};
        G4double mAperatureStopEntrance = {18.5 * m};
        G4double mAperatureStopWidth = {15.0 * cm};
        G4double mAperatureStopHeight = {15.0 * cm};
        G4double mAperatureStopLength = {15.0 * cm};
        G4double mAperatureStopHoleSize = {1.2 * cm};
        G4double mAperatureStopHoleSeparation = {3.0 * cm};

        // RBB
        G4Material* mRotatingBeamBlockerMaterial;
        G4Tubs* mSolidRotatingBeamBlocker;
        G4LogicalVolume* mLogicalRotatingBeamBlocker;
        G4VPhysicalVolume* mPhysicalRotatingBeamBlocker;

        G4Material* mRotatingBeamBlockerLeftHoleMaterial;
        G4Tubs* mSolidRotatingBeamBlockerLeftHole;
        G4LogicalVolume* mLogicalRotatingBeamBlockerLeftHole;
        G4VPhysicalVolume* mPhysicalRotatingBeamBlockerLeftHole;

        G4Material* mRotatingBeamBlockerRightHoleMaterial;
        G4Tubs* mSolidRotatingBeamBlockerRightHole;
        G4LogicalVolume* mLogicalRotatingBeamBlockerRightHole;
        G4VPhysicalVolume* mPhysicalRotatingBeamBlockerRightHole;

        // BC
        G4Material* mBinocularCollimatorMaterial;
        G4Box* mSolidBinocularCollimator;
        G4LogicalVolume* mLogicalBinocularCollimator;
        G4VPhysicalVolume* mPhysicalBinocularCollimator;

        G4Material* mBinocularCollimatorLeftHoleMaterial;
        G4Tubs* mSolidBinocularCollimatorLeftHole;
        G4LogicalVolume* mLogicalBinocularCollimatorLeftHole;
        G4VPhysicalVolume* mPhysicalBinocularCollimatorLeftHole;

        G4Material* mBinocularCollimatorRightHoleMaterial;
        G4Tubs* mSolidBinocularCollimatorRightHole;
        G4LogicalVolume* mLogicalBinocularCollimatorRightHole;
        G4VPhysicalVolume* mPhysicalBinocularCollimatorRightHole;

        // AS
        G4Material* mAperatureStopMaterial;
        G4Box* mSolidAperatureStop;
        G4LogicalVolume* mLogicalAperatureStop;
        G4VPhysicalVolume* mPhysicalAperatureStop;

        G4Material* mAperatureStopLeftHoleMaterial;
        G4Tubs* mSolidAperatureStopLeftHole;
        G4LogicalVolume* mLogicalAperatureStopLeftHole;
        G4VPhysicalVolume* mPhysicalAperatureStopLeftHole;

        G4Material* mAperatureStopRightHoleMaterial;
        G4Tubs* mSolidAperatureStopRightHole;
        G4LogicalVolume* mLogicalAperatureStopRightHole;
        G4VPhysicalVolume* mPhysicalAperatureStopRightHole;

        G4double mTZeroLocation = {-30.0 * m};

#ifdef ARTIE_YAML
        YAML::Node mConfig;
#endif
    };
}