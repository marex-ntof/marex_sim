/**
 * @file EventAction.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-12-13
 */
#pragma once
#include <memory>
#include <map>
#include "globals.hh"
#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"

#include "RunAction.hh"
#include "EventManager.hh"

namespace marex
{
    class EventAction : public G4UserEventAction
    {
    public:
        EventAction();
        ~EventAction();
        
        virtual void BeginOfEventAction(const G4Event*);
        virtual void EndOfEventAction(const G4Event*);

    private:

    };
}