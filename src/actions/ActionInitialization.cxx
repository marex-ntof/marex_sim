/**
 * @file ActionInitialization.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#include "ActionInitialization.hh"

namespace marex
{
    ActionInitialization::ActionInitialization()
    {
    }

    ActionInitialization::~ActionInitialization()
    {
    }

#ifdef MAREX_YAML
    ActionInitialization::ActionInitialization(YAML::Node config)
    : mConfig(config)
    {
    }
#endif

    void ActionInitialization::Build() const
    {
#ifdef MAREX_YAML
        SetUserAction(new PrimaryGeneratorAction(mConfig));
#else
        SetUserAction(new PrimaryGeneratorAction());
#endif 
        SetUserAction(new RunAction());
        SetUserAction(new EventAction());
        SetUserAction(new SteppingAction());
        SetUserAction(new TrackingAction());
    }

    void ActionInitialization::BuildForMaster() const
    {
        SetUserAction(new RunAction());
    }
}