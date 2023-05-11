/**
 * @file RunAction.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#include "RunAction.hh"

namespace marex
{
    RunAction::RunAction()
    : G4UserRunAction()
    {
        EventManager::GetEventManager()->CreateTuples();
    }

    RunAction::~RunAction()
    {}

    void RunAction::BeginOfRunAction(const G4Run* run)
    {
        auto Manager = EventManager::GetEventManager();
        Manager->ResetProfiling();
        Manager->OpenOutputFile(run->GetRunID());
    }

    void RunAction::EndOfRunAction(const G4Run* run)
    {
        auto Manager = EventManager::GetEventManager();
        Manager->CloseOutputFile(run->GetRunID());
    }
}