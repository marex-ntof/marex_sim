/**
 * @file Material.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-30
 */
#include "Material.hh"

namespace marex
{
    G4Material* CreateMaterial(G4String material_name, G4String volume_name)
    {
        G4Material* material;
        if(material_name == "gaseous_argon") {
            material = G4NistManager::Instance()->FindOrBuildMaterial("G4_Ar");
        }
        else if(material_name == "liquid_argon") {
            if(EventManager::GetEventManager()->UseG4Definition()) {
                material = G4NistManager::Instance()->FindOrBuildMaterial("G4_lAr");
            }
            else {
                auto argon = new Argon("LAr");
                material = argon->GetMaterial();
            }
        }
        else if(material_name == "g4_liquid_argon") {
            material = G4NistManager::Instance()->FindOrBuildMaterial("G4_lAr");
        }
        else if(material_name == "concrete") {
            material = G4NistManager::Instance()->FindOrBuildMaterial("G4_CONCRETE");
        }
        else if(material_name == "kapton") {
            material = G4NistManager::Instance()->FindOrBuildMaterial("G4_KAPTON");
        }
        else if(material_name == "polyurethane") {
            G4NistManager* man = G4NistManager::Instance();
            material = new G4Material("Polyurethane", 1.005*g/cm3, 4);
            G4Element* C = man->FindOrBuildElement("C");
            G4Element* H = man->FindOrBuildElement("H");
            G4Element* N = man->FindOrBuildElement("N");
            G4Element* O = man->FindOrBuildElement("O");
            material->AddElement(C, 3);
            material->AddElement(H, 8);
            material->AddElement(N, 2);
            material->AddElement(O, 1);
        }
        else if(material_name == "stainless_steel") {
            material = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");
        }
        else if(material_name == "rough_vacuum") 
        {
            auto air = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
            material = new G4Material(
                "VacuumRough", air->GetDensity() * 0.1, 1
            );
            material->AddMaterial(air, 1.0);
        }
        else if(material_name == "high_vacuum") 
        {
            auto air = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
            material = new G4Material(
                "VacuumHigh", air->GetDensity() * 1.0E-11, 1
            );
            material->AddMaterial(air, 1.0);
        }
        else if(material_name == "water") {
            material = G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER");
        }
        else if(material_name == "air") {
            material = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
        }
        else if(material_name == "aluminum") {
            material = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");
        }
        else if(material_name == "carbon") {
            material = G4NistManager::Instance()->FindOrBuildMaterial("G4_C");
        }
        else if(material_name == "brass") {
            material = G4NistManager::Instance()->FindOrBuildMaterial("G4_BRASS");
        }
        else
        {
            std::cerr << "ERROR! Material (" + material_name + ") not recognized!" << G4endl;
            exit(0);
        }
        material->SetName(volume_name + "_" + material_name);
        return material;
    }
}