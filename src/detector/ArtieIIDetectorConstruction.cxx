/**
 * @file ArtieIIDetectorConstruction.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#include "ArtieIIDetectorConstruction.hh"

namespace marex
{
    ArtieIIDetectorConstruction::ArtieIIDetectorConstruction()
    : G4VUserDetectorConstruction()
    {
        DefineMaterials();
    }

#ifdef MAREX_YAML
    ArtieIIDetectorConstruction::ArtieIIDetectorConstruction(YAML::Node config)
    : G4VUserDetectorConstruction()
    , mConfig(config)
    {

        if(mConfig["hall"]["world_material"])   { mWorldMaterialName = mConfig["hall"]["world_material"].as<std::string>(); }
        if(mConfig["hall"]["world_x"])          { mExperimentalHallX = mConfig["hall"]["world_x"].as<G4double>() * m; }
        if(mConfig["hall"]["world_y"])          { mExperimentalHallY = mConfig["hall"]["world_y"].as<G4double>() * m; }
        if(mConfig["hall"]["world_z"])          { mExperimentalHallZ = mConfig["hall"]["world_z"].as<G4double>() * m; }

        mTarget = ArtieIITarget(config);
        mMArEXTarget = MArEXTarget(config);
        mFilters = Filters(config);
        mDICER = DICER(config);
        mnTOF = nTOF(config);

        mSimpleDetector = SimpleDetector(config);
        mDICERDetector = DICERDetector(config);
        mLiGDetector = LiGDetector(config);

        DefineMaterials();
    }
#endif

    void ArtieIIDetectorConstruction::DefineMaterials()
    {
        mWorldMaterial = CreateMaterial(mWorldMaterialName, "World");
    }

    ArtieIIDetectorConstruction::~ArtieIIDetectorConstruction()
    {
    }

    G4VPhysicalVolume* ArtieIIDetectorConstruction::Construct()
    {
        DefineMaterials();
        // create the world volume
        mSolidExperimentalHall = new G4Box(
            "Solid_ArtieIExperimentalHall", 
            mExperimentalHallX, 
            mExperimentalHallY, 
            mExperimentalHallZ
        );
        mLogicalExperimentalHall = new G4LogicalVolume(
            mSolidExperimentalHall, 
            mWorldMaterial, 
            "Logical_ArtieIExperimentalHall"
        );
        mPhysicalExperimentalHall = new G4PVPlacement(
            0, 
            G4ThreeVector(0., 0., 0.),
            mLogicalExperimentalHall,
            "Physical_ArtieIExperimentalHall",
            0, 
            false,
            0
        );

        if(mConfig["filters"]) {
            mFilters.Construct(mLogicalExperimentalHall);
        }

        if(mConfig["dicer"]) {
            mDICER.Construct(mLogicalExperimentalHall);
        }
        else {
            mnTOF.Construct(mLogicalExperimentalHall);
        }

        if(mConfig["MArEX_target"])
        {
            mMArEXTarget.Construct(mLogicalExperimentalHall);
        } else {
            mTarget.Construct(mLogicalExperimentalHall);
        }

        if(mConfig["detector"]["detector_type"].as<std::string>() == "simple") {
            mSimpleDetector.Construct(mLogicalExperimentalHall);
        }
        else if (mConfig["detector"]["detector_type"].as<std::string>() == "dicer") {
            mDICERDetector.Construct(mLogicalExperimentalHall);
        }
        else if (mConfig["detector"]["detector_type"].as<std::string>() == "LiG")
        {
            mLiGDetector.Construct(mLogicalExperimentalHall);
        }
        

        return mPhysicalExperimentalHall;
    }

    void ArtieIIDetectorConstruction::ConstructSDandField()
    {
    }
}