// For the Tuner Project
#include "Config.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <sstream>

bool Config::load(const std::string& filename, TunerSettings& settings) {
    std::ifstream f(filename);
    if (!f.is_open()) return false;
    try {
        nlohmann::json j;
        f >> j;
        settings.menu_key = stringToVirtualKey(j.at("menu_key").get<std::string>());
        settings.screen_width = j.at("screen_width").get<int>();
        settings.screen_height = j.at("screen_height").get<int>();
        settings.fov_size = j.at("fov_size").get<int>();
        settings.hsv_lower = j.at("hsv_lower").get<std::vector<int>>();
        settings.hsv_upper = j.at("hsv_upper").get<std::vector<int>>();
        settings.min_contour_area = j.at("min_contour_area").get<int>();
        settings.aim_target = j.at("aim_target").get<std::string>();
        settings.aim_offset_x = j.at("aim_offset_x").get<int>();
        settings.aim_offset_y = j.at("aim_offset_y").get<int>();
        settings.show_aiming_visuals = j.at("show_aiming_visuals").get<bool>(); // Load the new setting
    }
    catch (nlohmann::json::exception& e) {
        std::cerr << "Error parsing JSON file '" << filename << "': " << e.what() << std::endl;
        return false;
    }
    return true;
}

void Config::createDefault(const std::string& filename) {
    std::cout << "Config file not found or invalid. Creating a default '" << filename << "'." << std::endl;
    nlohmann::ordered_json j;
    j["menu_key"] = "VK_F12";
    j["screen_width"] = 1920;
    j["screen_height"] = 1080;
    j["fov_size"] = 300;
    j["hsv_upper"] = { 160, 255, 255 };
    j["hsv_lower"] = { 140, 120, 150 };
    j["min_contour_area"] = 50;
    j["aim_target"] = "head";
    j["aim_offset_x"] = 0;
    j["aim_offset_y"] = -10;
    j["show_aiming_visuals"] = false; // Add the new setting to the default file
    std::ofstream o(filename);
    o << std::setw(4) << j << std::endl;
}

bool Config::save(const std::string& filename, const TunerSettings& settings) {
    nlohmann::ordered_json j;
    j["menu_key"] = virtualKeyToString(settings.menu_key);
    j["screen_width"] = settings.screen_width;
    j["screen_height"] = settings.screen_height;
    j["fov_size"] = settings.fov_size;
    j["hsv_upper"] = settings.hsv_upper;
    j["hsv_lower"] = settings.hsv_lower;
    j["min_contour_area"] = settings.min_contour_area;
    j["aim_target"] = settings.aim_target;
    j["aim_offset_x"] = settings.aim_offset_x;
    j["aim_offset_y"] = settings.aim_offset_y;
    j["show_aiming_visuals"] = settings.show_aiming_visuals; // Save the new setting
    try {
        std::ofstream o(filename);
        o << std::setw(4) << j << std::endl;
    }
    catch (...) {
        return false;
    }
    return true;
}

// ... (stringToVirtualKey and virtualKeyToString functions are unchanged) ...
int Config::stringToVirtualKey(const std::string& key) {
    if (key == "VK_LBUTTON") return VK_LBUTTON;
    if (key == "VK_RBUTTON") return VK_RBUTTON;
    if (key == "VK_MBUTTON") return VK_MBUTTON;
    if (key == "VK_XBUTTON1") return VK_XBUTTON1;
    if (key == "VK_XBUTTON2") return VK_XBUTTON2;
    if (key == "VK_SHIFT") return VK_SHIFT;
    if (key == "VK_CONTROL") return VK_CONTROL;
    if (key == "VK_INSERT") return VK_INSERT;
    if (key == "VK_END") return VK_END;
    if (key == "VK_F12") return VK_F12;
    try {
        return std::stoi(key, nullptr, 0);
    }
    catch (...) {
        return VK_F12;
    }
}

std::string Config::virtualKeyToString(int vKey) {
    switch (vKey) {
    case VK_LBUTTON: return "VK_LBUTTON";
    case VK_RBUTTON: return "VK_RBUTTON";
    case VK_MBUTTON: return "VK_MBUTTON";
    case VK_XBUTTON1: return "VK_XBUTTON1";
    case VK_XBUTTON2: return "VK_XBUTTON2";
    case VK_SHIFT: return "VK_SHIFT";
    case VK_CONTROL: return "VK_CONTROL";
    case VK_INSERT: return "VK_INSERT";
    case VK_END: return "VK_END";
    case VK_F12: return "VK_F12";
    default:
        std::stringstream ss;
        ss << "0x" << std::hex << std::uppercase << vKey;
        return ss.str();
    }
}