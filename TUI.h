// For the Tuner Project
#pragma once

#include "Config.h"
#include <string>

class TUI {
public:
    static void showTunerSettings(TunerSettings& settings);
    static void showHsvTuner(TunerSettings& settings); // New function
    static void clearScreen();
    static void printHeader(const std::string& title);
};