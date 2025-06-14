// For the Tuner Project
#pragma once

#include <string>
#include <vector>
#include "json.hpp"

struct TunerSettings {
    // Hotkey
    int menu_key;

    // Screen & FOV
    int screen_width;
    int screen_height;
    int fov_size;

    // Detection
    std::vector<int> hsv_lower;
    std::vector<int> hsv_upper;
    int min_contour_area;

    // Visuals
    std::string aim_target;
    int aim_offset_x;
    int aim_offset_y;
    bool show_aiming_visuals; // NEW: The master toggle for aiming visuals
};

class Config {
public:
    static bool load(const std::string& filename, TunerSettings& settings);
    static void createDefault(const std::string& filename);
    static bool save(const std::string& filename, const TunerSettings& settings);
    static int stringToVirtualKey(const std::string& key);
    static std::string virtualKeyToString(int vKey);
};