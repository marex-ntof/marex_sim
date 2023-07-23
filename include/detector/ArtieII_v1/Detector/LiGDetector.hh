/**
 * @file LiGDetector.hh
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

#ifdef ARTIE_YAML
#include "yaml-cpp/yaml.h"
#endif

#include "Material.hh"
#include "SensitiveDetector.hh"

namespace Artie
{
    class LiGDetector
    {
    public:
        LiGDetector();
        ~LiGDetector();

#ifdef ARTIE_YAML
        LiGDetector(YAML::Node config);
#endif

        void DefineMaterials();
        void Construct(G4LogicalVolume* logicalWorld);
    
    private:
        G4String mWorldMaterialName = {"high_vacuum"};

        // Detector
        G4bool mConstructDetector = {true};
        G4String mDetectorMaterialName = {"water"};
        G4double mDetectorRadius = {2.0 * cm};
        G4double mDetectorLength = {20.0 * cm};
        G4double mDetectorEntrance = {30.0 * m};

        // Detectors
        G4Material* mDetectorMaterial;
        G4Tubs *mSolidDetector;
        G4LogicalVolume* mLogicalDetector;
        G4VPhysicalVolume* mPhysicalDetector;

        G4double mTZeroLocation = {-30.0 * m};

#ifdef ARTIE_YAML
        YAML::Node mConfig;
#endif
    };
}