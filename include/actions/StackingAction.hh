/**
 * @file StackingAction.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-09-14
 */
#pragma once
#include "globals.hh"
#include "G4ClassificationOfNewTrack.hh"
#include "G4StackManager.hh"
#include "G4Track.hh"
#include "G4UserStackingAction.hh"
#include "G4OpticalPhoton.hh"

namespace marex
{
    class StackingAction : public G4UserStackingAction 
    {
    public:
        StackingAction();
        virtual ~StackingAction();
        static G4StackManager* savedManager;
        static StackingAction* theStackingAction;

        bool isUrgentEmpty();

        virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track*);
        virtual void NewStage();

    private:
    };
}