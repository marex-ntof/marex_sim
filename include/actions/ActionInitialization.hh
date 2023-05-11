/**
 * @file ActionInitialization.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-12-13
 */
#pragma once
#include "G4VUserActionInitialization.hh"

#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "TrackingAction.hh"
#include "Core.hh"

namespace marex
{
    class ActionInitialization : public G4VUserActionInitialization
    {
    public:
        ActionInitialization();
        ~ActionInitialization();

        ActionInitialization(YAML::Node config);

        virtual void Build() const;
        virtual void BuildForMaster() const;

    private:
        YAML::Node mConfig;

    };
}