/**
 * @file StackingAction.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-09-14
 */
#include "StackingAction.hh"

namespace marex
{
    StackingAction::StackingAction() 
    {
    }

    StackingAction::~StackingAction() 
    {
    }

    StackingAction* StackingAction::theStackingAction = 0;
    G4StackManager* StackingAction::savedManager = 0;

    G4ClassificationOfNewTrack StackingAction::ClassifyNewTrack(
        const G4Track* track
    ) 
    {
        savedManager = stackManager;
        if (track->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()) {
            return fWaiting;
        }
        return fUrgent;
    }

    bool StackingAction::isUrgentEmpty() 
    {
        if (!savedManager) 
        {
            std::cerr
                << "savedManager not set by StackingAction::ClassifyNewTrack(). "
                "Did you set up StackingAction as your stacking action? Did you "
                "override ClassifyNewTrack and forget to set savedManager?"
                << std::endl;
        }
        return savedManager->GetNUrgentTrack() == 0;
    }

    void StackingAction::NewStage() 
    {
    }
}