/**
 * @file Filters.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#include "Filters.hh"

namespace marex
{
    Filters::Filters()
    {
        DefineMaterials();
    }

#ifdef MAREX_YAML
    Filters::Filters(YAML::Node config)
    : mConfig(config)
    {
        if(mConfig["filters"]["construct_filters"])    { mConstructFilters = mConfig["filters"]["construct_filters"].as<G4bool>(); }
        if(mConfig["filters"]["filter_materials"])     { mFilterMaterialNames = mConfig["filters"]["filter_materials"].as<std::vector<std::string>>(); }
        if(mConfig["filters"]["filter_x_position"])    { mFilterXPos = mConfig["filters"]["filter_x_position"].as<std::vector<G4double>>(); }
        if(mConfig["filters"]["filter_z_position"])    { mFilterZPos = mConfig["filters"]["filter_z_position"].as<std::vector<G4double>>(); }
        if(mConfig["filters"]["filter_thickness"])     { mFilterThickness = mConfig["filters"]["filter_thickness"].as<std::vector<G4double>>(); }
        if(mConfig["filters"]["filter_radius"])        { mFilterRadius = mConfig["filters"]["filter_radius"].as<std::vector<G4double>>(); }

        DefineMaterials();
    }
#endif

    void Filters::DefineMaterials()
    {
        for (G4int i = 0; i < mFilterMaterialNames.size(); i++)
        {
            mFilterMaterials.emplace_back(CreateMaterial(mFilterMaterialNames[i], "Filter_"+std::to_string(i)));
        }
    }

    Filters::~Filters()
    {
    }

    void Filters::Construct(G4LogicalVolume* logicalWorld)
    {
        DefineMaterials();

        if(mConstructFilters)
        {
            for (G4int i = 0; i < mFilterMaterialNames.size(); i++)
            {
                // Solid
                G4Tubs* solidFilter = new G4Tubs(
                    "Solid_ArtieIIFilter_"+mFilterMaterialNames[i]+"_"+std::to_string(i),
                    0,
                    mFilterRadius[i] * cm / 2.0, 
                    0.5 * mFilterThickness[i] * cm, 
                    0,
                    2*M_PI
                );
                // Logical Volume
                G4LogicalVolume* LogicalFilter = new G4LogicalVolume(
                    solidFilter, 
                    mFilterMaterials[i], 
                    "Logical_ArtieIIFilter_"+mFilterMaterialNames[i]+"_"+std::to_string(i)
                );
                // Physical Volume
                G4VPhysicalVolume* PhysicalFilter = new G4PVPlacement(
                    0, 
                    G4ThreeVector(
                        mFilterXPos[i] * cm,
                        0., 
                        mFilterZPos[i] * m + 0.5 * mFilterThickness[i] * cm),
                    LogicalFilter, 
                    "Physical_ArtieIIFilter_"+mFilterMaterialNames[i]+"_"+std::to_string(i), 
                    logicalWorld, 
                    false, 
                    0, 
                    true
                );
            }
        }
    }
}