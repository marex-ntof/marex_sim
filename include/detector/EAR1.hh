/**
 * @file EAR1.hh
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

#include "yaml-cpp/yaml.h"

#include "Material.hh"

namespace marex
{
    class EAR1
    {
    public:
        EAR1();
        ~EAR1();

#ifdef MAREX_YAML
        EAR1(YAML::Node config);
#endif

        void DefineMaterials();
        void Construct(G4LogicalVolume* logicalWorld);
    
    private:
        // G4String mWorldMaterialName = {"high_vacuum"};
        // G4double mTZeroLocation = { -30.0 * m };

        //////////// Config Parameters
        //Beam Pipe Mid
        G4bool mConstructBeamPipeMid = {true};
        G4String mBeamPipeVacuumMaterialName = { "high_vacuum" };
        G4String mBeamPipePipeMaterialName = { "stainless_steel" };
        G4double mBeamPipeMidZStart = { 185 * m };
        G4double mBeamPipeMidZEnd = { 195 * m };
        G4double mBeamPipeMidInnerRadius = { 5.0 * cm };
        G4double mBeamPipeMidOuterRadius = { 9.75 * cm };

        //Hall
        G4bool mConstructHall = {true};
        G4double mBeamPipeOuterRadius = { 12 * cm };
        G4String mHallMaterialName = { "concrete" };
        G4double mHallWidth = { 3 * m };
        G4double mHallHeight = { 3 * m };
        G4double mHallZStart = { 200 * m};
        G4double mHallZEnd = { 200 * m};
        G4double mHallThickness = { 0.5 * m };

        //Detector Wall
        G4bool mConstructDetectorWall = {true};
        G4String mWallMaterialName = { "concrete" };
        G4double mDetectorWallThickness = { 1.6 * m };
        G4double mDetectorWallZ = { 189 * m};
        G4double mDetectorWallHoleDia = {20.0 * cm};

        //Beam Collimator
        G4bool mConstructBeamCollimator = {true};
        G4String mBeamCollimatorMaterialName = {"stainless_steel"};
        G4double mBeamCollimatorZ = {170 * m};
        G4double mBeamCollimatorThickness = {66 * mm};
        G4double mBeamCollimatorRadius = {150 * mm};
        G4double mBeamCollimatorHole1Dia = {218 * mm};
        G4double mBeamCollimatorHole2Dia = {90 * mm};

        //Collimator
        G4bool mConstructCollimator = {true};
        G4double mCollimatorLength = { 20.0 * cm };
        G4double mCollimatorRadius = { 5.0 * cm };
        G4double mCollimatorHoleDia = { 5.0 * mm };
        G4double mCollimatorZ = { 184.7 * m };
        G4String mCollimatorMaterialName = {"brass"};

        //Borated PE
        G4bool mConstructBoratedPE = {true};
        G4double mBoratedPELength = {50*cm};
        G4String mBoratedPEMaterialName = {"borated_polyethylene"};
        ////////////////////////////////////

        //Position vectors for objects
        G4ThreeVector mBeamPipeMidPosition;
        G4ThreeVector mHallPosition;
        G4ThreeVector mDetectorWallPosition;
        // G4ThreeVector mBeamCollimatorPosition;
        G4ThreeVector mCollimatorPosition;
        G4ThreeVector mBoratedPEPosition;

        //Beam Pipe Mid Vacuum
        G4Material* mBeamPipeVacuumMaterial;
        G4Tubs* mSolidBeamPipeMidVacuum;
        G4LogicalVolume* mLogicalBeamPipeMidVacuum;
        G4VPhysicalVolume* mPhysicalBeamPipeMidVacuum;

        //Beam Pipe Mid Pipe
        G4Material* mBeamPipePipeMaterial;
        G4Tubs* mSolidBeamPipeMidPipe;
        G4LogicalVolume* mLogicalBeamPipeMidPipe;
        G4VPhysicalVolume* mPhysicalBeamPipeMidPipe;

        //nTOF Hall
        G4Material* mHallMaterial;
        G4SubtractionSolid* mSolidHall;
        G4LogicalVolume* mLogicalHall;
        G4VPhysicalVolume* mPhysicalHall;

        //Detector Wall
        G4Material* mWallMaterial;
        G4SubtractionSolid* mSolidDetectorWall;
        G4LogicalVolume* mLogicalDetectorWall;
        G4VPhysicalVolume* mPhysicalDetectorWall;

        //Beam Collimator
        G4Material* mBeamCollimatorMaterial;
        G4Tubs* mSolidBeamCollimator1;
        G4LogicalVolume* mLogicalBeamCollimator1;
        G4VPhysicalVolume* mPhysicalBeamCollimator1;
        G4Tubs* mSolidBeamCollimator2;
        G4LogicalVolume* mLogicalBeamCollimator2;
        G4VPhysicalVolume* mPhysicalBeamCollimator2;
        
        //Upstream Collimator
        G4Material* mUpCollimatorMaterial;
        G4Tubs* mSolidUpCollimator;
        G4LogicalVolume* mLogicalUpCollimator;
        G4VPhysicalVolume* mPhysicalUpCollimator;

        //Collimator
        G4Material* mCollimatorMaterial;
        G4Tubs* mSolidCollimator;
        G4LogicalVolume* mLogicalCollimator;
        G4VPhysicalVolume* mPhysicalCollimator;

        //Borated PE
        G4Material* mBoratedPEMaterial;
        G4Tubs* mSolidBoratedPE;
        G4LogicalVolume* mLogicalBoratedPE;
        G4VPhysicalVolume* mPhysicalBoratedPE;

#ifdef MAREX_YAML
        YAML::Node mConfig;
#endif
    };
}