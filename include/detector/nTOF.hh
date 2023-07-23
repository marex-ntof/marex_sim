/**
 * @file nTOF.hh
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
    class nTOF
    {
    public:
        nTOF();
        ~nTOF();

#ifdef MAREX_YAML
        nTOF(YAML::Node config);
#endif

        void DefineMaterials();
        void Construct(G4LogicalVolume* logicalWorld);
    
    private:
        // G4String mWorldMaterialName = {"high_vacuum"};
        // G4double mTZeroLocation = { -30.0 * m };

        //////////// Config Parameters
        //Beam Pipe
        G4bool mConstructBeamPipe = {true};
        G4double mBeamPipeLength = { 100 * m };
        G4double mBeamPipeInnerRadius = { 10 * cm };
        G4double mBeamPipeOuterRadius = { 12 * cm };
        G4String mBeamPipeVacuumMaterialName = { "high_vacuum" };
        G4String mBeamPipePipeMaterialName = { "stainless_steel" };
        G4double mBeamPipeEntrance = { 0 * m };

        G4double mBeamPipeMidZStart = { 185 * m };
        G4double mBeamPipeMidZEnd = { 195 * m };

        //Hall
        G4bool mConstructHall = {true};
        G4String mHallMaterialName = { "concrete" };
        G4double mHallWidth = { 3 * m };
        G4double mHallHeight = { 3 * m };
        G4double mHallLength = { 200 * m};
        G4double mHallThickness = { 0.5 * m };

        //Detector Wall
        G4bool mConstructDetectorWall = {true};
        G4double mDetectorWallThickness = { 1.6 * m };
        G4double mDetectorWallZ = { 189 * m};

        //Upstream Collimator
        G4bool mConstructUpCollimator = {true};
        G4double mUpCollimatorLength = { 20.0 * cm };
        G4double mUpCollimatorRadius = { 5.0 * cm };
        G4double mUpCollimatorHoleDia = { 5.0 * mm };
        G4double mUpCollimatorZ = { 184.0 * m };
        G4String mUpCollimatorMaterialName = {"brass"};

        //Downstream Collimator
        G4bool mConstructDownCollimator = {true};
        G4double mDownCollimatorLength = { 20.0 * cm };
        G4double mDownCollimatorRadius = { 5.0 * cm };
        G4double mDownCollimatorHoleDia = { 5.0 * mm };
        G4double mDownCollimatorZ = { 184.7 * m };
        G4String mDownCollimatorMaterialName = {"brass"};
        ////////////////////////////////////

        //Position vectors for objects
        G4ThreeVector mBeamPipePosition;
        G4ThreeVector mBeamPipeMidPosition;
        G4ThreeVector mHallPosition;
        G4ThreeVector mDetectorWallPosition;
        G4ThreeVector mUpCollimatorPosition;
        G4ThreeVector mDownCollimatorPosition;

        //Beam Pipe Vacuum
        G4Material* mBeamPipeVacuumMaterial;
        G4Tubs* mSolidBeamPipeVacuum;
        G4LogicalVolume* mLogicalBeamPipeVacuum;
        G4VPhysicalVolume* mPhysicalBeamPipeVacuum;

        //Beam Pipe Pipe
        G4Material* mBeamPipePipeMaterial;
        G4Tubs* mSolidBeamPipePipe;
        G4LogicalVolume* mLogicalBeamPipePipe;
        G4VPhysicalVolume* mPhysicalBeamPipePipe;

        //Beam Pipe Mid Vacuum
        G4Tubs* mSolidBeamPipeMidVacuum;
        G4LogicalVolume* mLogicalBeamPipeMidVacuum;
        G4VPhysicalVolume* mPhysicalBeamPipeMidVacuum;

        //Beam Pipe Mid Pipe
        G4Tubs* mSolidBeamPipeMidPipe;
        G4LogicalVolume* mLogicalBeamPipeMidPipe;
        G4VPhysicalVolume* mPhysicalBeamPipeMidPipe;

        //nTOF Hall
        G4Material* mHallMaterial;
        G4SubtractionSolid* mSolidHall;
        G4LogicalVolume* mLogicalHall;
        G4VPhysicalVolume* mPhysicalHall;

        //Detector Wall
        G4SubtractionSolid* mSolidDetectorWall;
        G4LogicalVolume* mLogicalDetectorWall;
        G4VPhysicalVolume* mPhysicalDetectorWall;

        //Upstream Collimator
        G4Material* mUpCollimatorMaterial;
        G4Tubs* mSolidUpCollimator;
        G4LogicalVolume* mLogicalUpCollimator;
        G4VPhysicalVolume* mPhysicalUpCollimator;

        //Downstream Collimator
        G4Material* mDownCollimatorMaterial;
        G4Tubs* mSolidDownCollimator;
        G4LogicalVolume* mLogicalDownCollimator;
        G4VPhysicalVolume* mPhysicalDownCollimator;

#ifdef MAREX_YAML
        YAML::Node mConfig;
#endif
    };
}