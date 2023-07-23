/**
 * @file LiGDetector.cxx
 * @author Yashwanth Bezawada [ysbezawada@ucdavis.edu]
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#include "LiGDetector.hh"

namespace marex
{
    LiGDetector::LiGDetector()
    {
        DefineMaterials();
    }

#ifdef MAREX_YAML
    LiGDetector::LiGDetector(YAML::Node config)
    : mConfig(config)
    {
        if(mConfig["detector"]["construct_detector"])   { mConstructDetector = mConfig["detector"]["construct_detector"].as<G4bool>(); }
        if(mConfig["detector"]["detector_material"])    { mDetectorMaterialName = mConfig["detector"]["detector_material"].as<std::string>(); }
        if(mConfig["detector"]["detector_radius"])      { mDetectorRadius = mConfig["detector"]["detector_radius"].as<G4double>() * cm; }
        if(mConfig["detector"]["detector_length"])      { mDetectorLength = mConfig["detector"]["detector_length"].as<G4double>() * cm; }
        if(mConfig["detector"]["detector_entrance"])    { mDetectorEntrance = mConfig["detector"]["detector_entrance"].as<G4double>() * m; }    

        if(mConfig["hall"]["world_material"])           { mWorldMaterialName = mConfig["hall"]["world_material"].as<std::string>(); }

        DefineMaterials();
    }
#endif

    void LiGDetector::DefineMaterials()
    {
        mDetectorMaterial = CreateMaterial(mDetectorMaterialName, "Detector");
    }

    LiGDetector::~LiGDetector()
    {
    }

    void LiGDetector::Construct(G4LogicalVolume* logicalWorld)
    {
        DefineMaterials();
        // Construct the detector
        if(mConstructDetector)
        {
            mSolidDetector = new G4Tubs(
                "Solid_ArtieIIDetector", 
                0,
                mDetectorRadius, 
                0.5 * mDetectorLength, 
                0,
                2*M_PI
            );
            mLogicalDetector = new G4LogicalVolume(
                mSolidDetector, 
                mDetectorMaterial, 
                "Logical_ArtieIIDetector"
            );
            mPhysicalDetector = new G4PVPlacement(
                0, 
                G4ThreeVector(
                    0.,
                    0., 
                    + mDetectorEntrance + mDetectorLength * 0.5),
                mLogicalDetector, 
                "Physical_ArtieIIDetector", 
                logicalWorld, 
                false, 
                0, 
                true
            );
        }
    }
}