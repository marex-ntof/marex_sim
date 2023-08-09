/**
 * @file Analysis.hh
 * @author Yashwanth Bezawada [ysbezawada@ucdavis.edu]
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#pragma once
#include "EventManager.hh"
#include "Randomize.hh"

#include "TRandom3.h"

namespace marex
{
    struct bkgdAnalysisTuple
    {
        G4double n_energy = 0;
        G4bool detectionStatus = false;
        // G4double n_energy_detected = 0;
        G4double n_tof_bkgd = 0;
    };

    void bkgdAnalysisFunctionRunBegin();
    void bkgdAnalysisFunctionRunEnd();
    void bkgdAnalysisFunctionEventBegin();
    void bkgdAnalysisFunctionEventEnd();

}