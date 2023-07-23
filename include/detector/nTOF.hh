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

        G4bool mConstructBeamPipe = {true};
        G4double mBeamPipeLength = { 100 * m };
        G4double mBeamPipeInnerRadius = { 10 * cm };
        G4double mBeamPipeOuterRadius = { 12 * cm };
        G4String mBeamPipeVacuumMaterialName = { "high_vacuum" };
        G4String mBeamPipePipeMaterialName = { "stainless_steel" };
        G4double mBeamPipeEntrance = { 0 * m };

        G4ThreeVector mBeamPipePosition;

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

#ifdef MAREX_YAML
        YAML::Node mConfig;
#endif
    };
}