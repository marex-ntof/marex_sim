/**
 * @file RunAction.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-12-13
 */
#pragma once
#include "G4UserRunAction.hh"
#include "G4Run.hh"
#include "G4GenericMessenger.hh"
#ifdef MAREX_GEANT_10
#include "g4root.hh"
#else
#include "G4AnalysisManager.hh"
#endif

#include "PrimaryGeneratorAction.hh"
#include "EventManager.hh"

namespace marex
{
    class RunAction : public G4UserRunAction
    {
    public:
        RunAction();
        ~RunAction();

        void ConstructRootTrees();
        
        virtual void BeginOfRunAction(const G4Run*);
        virtual void EndOfRunAction(const G4Run*);

    private:
        
    };
}