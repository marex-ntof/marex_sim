/**
 * @file ArtieIDetectorConstruction.hh
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

#ifdef ARTIE_YAML
#include "yaml-cpp/yaml.h"
#endif

#include "Material.hh"
#include "SensitiveDetector.hh"

namespace Artie
{
    class ArtieIDetectorConstruction : public G4VUserDetectorConstruction
    {
    public:
        ArtieIDetectorConstruction();
        ~ArtieIDetectorConstruction();

#ifdef ARTIE_YAML
        ArtieIDetectorConstruction(YAML::Node config);
#endif

        void DefineMaterials();

        virtual G4VPhysicalVolume* Construct();
    
    private:
        virtual void ConstructSDandField();

    private:
        G4String mWorldMaterialName = {"air"};
        G4double mExperimentalHallX = {500 * m};
        G4double mExperimentalHallY = {500 * m};
        G4double mExperimentalHallZ = {500 * m};

        // Artie-I target
        G4String mActiveVolumeMaterialName = {"liquid_argon"};
        G4double mTargetRadius = {2.5 / 2.0 * cm};
        G4double mTargetLength = {168 * cm};

        // Container
        G4bool mConstructContainer = {false};
        G4String mContainerMaterialName = {"stainless_steel"};
        G4String mInsulationMaterialName = {"polyurethane"};
        G4String mLeftWindowMaterialName = {"kapton"};
        G4String mRightWindowMaterialName = {"kapton"};
        G4String mLeftBufferMaterialName = {"gaseous_argon"};
        G4String mRightBufferMaterialName = {"gaseous_argon"};
        G4double mContainerRadius = {3.49 / 2.0 * cm};
        G4double mInsulationThickness = {10.0 * cm};
        G4double mWindowThickness = {0.00762 * cm};
        G4double mBufferLength = {5.0 * cm};

        // Beam pipes
        G4bool mConstructBeamPipe = {false};
        G4String mBeamPipeLeftBeamMaterialName = {"high_vacuum"};
        G4String mBeamPipeRightBeamMaterialName = {"high_vacuum"};
        G4String mBeamPipeLeftPipeMaterialName = {"rough_vacuum"};
        G4String mBeamPipeRightPipeMaterialName = {"rough_vacuum"};
        G4double mBeamPipeInnerRadius = {18.0 * cm};
        G4double mBeamPipeOuterRadius = {20.0 * cm};
        G4double mBeamPipeLeftHalfLength = 0.;
        G4double mBeamPipeRightHalfLength = 0.;
        G4ThreeVector mBeamPipeLeftPosition = {0., 0., 0.};
        G4ThreeVector mBeamPipeRightPosition = {0., 0., 0.};
        G4double mGap = {2.5 * m};

        // Hall
        G4bool mConstructHall = {false};
        G4String mHallMaterialName = {"concrete"};
        G4double mWorldX = {4 * m};
        G4double mWorldY = {4 * m};
        G4double mWorldZ = {200 * m};
        G4double mWallThickness = {1 * m};

        // Detector
        G4bool mConstructDetector = {true};
        G4String mDetectorMaterialName = {"water"};
        G4double mDetectorRadius = {2.0 * cm};
        G4double mDetectorLength = {20.0 * cm};
        G4double mDetectorEntrance = {69.0 * m};

        // Experimental Hall
        G4Material *mWorldMaterial;
        G4Box* mSolidExperimentalHall;
        G4LogicalVolume* mLogicalExperimentalHall;
        G4VPhysicalVolume* mPhysicalExperimentalHall;

        // Active Volume
        G4Material *mActiveVolumeMaterial;
        G4Tubs *mSolidActiveVolume;
        G4LogicalVolume* mLogicalActiveVolume;
        G4VPhysicalVolume* mPhysicalActiveVolume;

        // Container
        G4Material* mContainerMaterial;
        G4Tubs *mSolidContainer;
        G4LogicalVolume* mLogicalContainer;
        G4VPhysicalVolume* mPhysicalContainer;

        // Insulation
        G4Material* mInsulationMaterial;
        G4Tubs *mSolidInsulation;
        G4LogicalVolume* mLogicalInsulation;
        G4VPhysicalVolume* mPhysicalInsulation;

        // LeftWindow
        G4Material* mLeftWindowMaterial;
        G4Tubs *mSolidLeftWindow;
        G4LogicalVolume* mLogicalLeftWindow;
        G4VPhysicalVolume* mPhysicalLeftWindow;

        // RightWindow
        G4Material* mRightWindowMaterial;
        G4Tubs *mSolidRightWindow;
        G4LogicalVolume* mLogicalRightWindow;
        G4VPhysicalVolume* mPhysicalRightWindow;

        // LeftBuffer
        G4Material* mLeftBufferMaterial;
        G4Tubs *mSolidLeftBuffer;
        G4LogicalVolume* mLogicalLeftBuffer;
        G4VPhysicalVolume* mPhysicalLeftBuffer;

        // RightBuffer
        G4Material* mRightBufferMaterial;
        G4Tubs *mSolidRightBuffer;
        G4LogicalVolume* mLogicalRightBuffer;
        G4VPhysicalVolume* mPhysicalRightBuffer;

        // BeamPipeLeftBeam
        G4Material* mBeamPipeLeftBeamMaterial;
        G4Tubs *mSolidBeamPipeLeftBeam;
        G4LogicalVolume* mLogicalBeamPipeLeftBeam;
        G4VPhysicalVolume* mPhysicalBeamPipeLeftBeam;

        // BeamPipeRightBeam
        G4Material* mBeamPipeRightBeamMaterial;
        G4Tubs *mSolidBeamPipeRightBeam;
        G4LogicalVolume* mLogicalBeamPipeRightBeam;
        G4VPhysicalVolume* mPhysicalBeamPipeRightBeam;

        // BeamPipeLeftPipe
        G4Material* mBeamPipeLeftPipeMaterial;
        G4Tubs *mSolidBeamPipeLeftPipe;
        G4LogicalVolume* mLogicalBeamPipeLeftPipe;
        G4VPhysicalVolume* mPhysicalBeamPipeLeftPipe;

        // BeamPipeRightPipe
        G4Material* mBeamPipeRightPipeMaterial;
        G4Tubs *mSolidBeamPipeRightPipe;
        G4LogicalVolume* mLogicalBeamPipeRightPipe;
        G4VPhysicalVolume* mPhysicalBeamPipeRightPipe;

        // Detector
        G4Material* mDetectorMaterial;
        G4Tubs *mSolidDetector;
        G4LogicalVolume* mLogicalDetector;
        G4VPhysicalVolume* mPhysicalDetector;

        // Hall
        G4Material* mHallMaterial;
        G4SubtractionSolid *mSolidHall;
        G4LogicalVolume* mLogicalHall;
        G4VPhysicalVolume* mPhysicalHall;

#ifdef ARTIE_YAML
        YAML::Node mConfig;
#endif
    };
}