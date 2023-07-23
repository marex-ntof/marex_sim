/**
 * @file Filters.hh
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
    class Filters
    {
    public:
        Filters();
        ~Filters();

#ifdef ARTIE_YAML
        Filters(YAML::Node config);
#endif

        void DefineMaterials();
        void Construct(G4LogicalVolume* logicalWorld);
    
    private:
        G4bool mConstructFilters = {false};
        std::vector<std::string> mFilterMaterialNames = {};
        std::vector<G4double> mFilterRadius = {};
        std::vector<G4double> mFilterThickness = {};
        std::vector<G4double> mFilterXPos = {};
        std::vector<G4double> mFilterZPos = {};
        std::vector<G4Material*> mFilterMaterials = {};

        G4double mTZeroLocation = {-30 * m};

#ifdef ARTIE_YAML
        YAML::Node mConfig;
#endif
    };
}