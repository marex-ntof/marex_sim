/**
 * @file SensitiveDetector.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#pragma once
#include <memory>

#include "G4VSolid.hh"
#include "G4Box.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VSensitiveDetector.hh"
#include "G4StepPoint.hh"
#include "G4PVPlacement.hh"
#include "G4LogicalVolume.hh"
#include "G4RunManager.hh"
#ifdef ARTIE_GEANT_10
#include "g4root.hh"
#else
#include "G4AnalysisManager.hh"
#endif

namespace Artie
{
    class EventManager;
    
    class SensitiveDetector : public G4VSensitiveDetector
    {
    public:
        SensitiveDetector(G4String);
        ~SensitiveDetector();

    private:
        G4String mName = {"default"};
        virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*); 

    };
}