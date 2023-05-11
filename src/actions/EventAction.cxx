/**
 * @file EventAction.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#include "EventAction.hh"

namespace marex
{
    EventAction::EventAction()
    : G4UserEventAction()
    {
        // generate list of primaries
        auto Manager = EventManager::GetEventManager();
        auto primaries = Manager->GeneratePrimaryList();
        Manager->ClearEventData();

        auto RunManager = G4RunManager::GetRunManager();
        auto Generator = (PrimaryGeneratorAction*)RunManager->GetUserPrimaryGeneratorAction();
        Generator->SetPrimaries(primaries);
    }

    EventAction::~EventAction()
    {}

    void EventAction::BeginOfEventAction(const G4Event*)
    {
        // generate list of primaries
        auto Manager = EventManager::GetEventManager();
        auto primaries = Manager->GeneratePrimaryList();
        Manager->ClearEventData();

        auto RunManager = G4RunManager::GetRunManager();
        auto Generator = (PrimaryGeneratorAction*)RunManager->GetUserPrimaryGeneratorAction();
        Generator->SetPrimaries(primaries);
    }

    void EventAction::EndOfEventAction(const G4Event* event)
    {
        auto Manager = EventManager::GetEventManager();
        auto AnalysisManager = G4AnalysisManager::Instance();
        Manager->FillParticleMaps(event->GetEventID());
        Manager->FillPrimaryInfo(event->GetEventID());
        Manager->FillParticleInfo(event->GetEventID());
        Manager->FillOpticalPhotons(event->GetEventID());
        Manager->FillEnergyDeposits(event->GetEventID());
        Manager->FillOpticalPhotons(event->GetEventID());
        Manager->FillHits(event->GetEventID());
        
        // Send out tuples to analysis functions
        Manager->EvaluateEvent();
        Manager->ClearEventData();
    }
}