#include "TUI.h"
#include <iostream>
#include <string>
#include <limits>
#include <vector>
#include <chrono>
#include <thread>

void TUI::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void TUI::printHeader(const std::string& title) {
    std::cout << "==================================================" << std::endl;
    std::cout << "          C++ Color Aimbot - " << title << std::endl;
    std::cout << "==================================================" << std::endl << std::endl;
}


void TUI::showTunerSettings(TunerSettings& settings) {
    int choice = -1;
    while (choice != 0) {
        clearScreen();
        printHeader("Tuner Settings");
        std::cout << "Editing config.json. Changes will be saved." << std::endl << std::endl;
        std::cout << "  [1] FOV Size             : " << settings.fov_size << std::endl;
        std::cout << "  [2] Min Contour Area     : " << settings.min_contour_area << std::endl;
        std::cout << "  [3] Aim Target           : " << settings.aim_target << std::endl;
        std::cout << "  [4] X Aim Offset         : " << settings.aim_offset_x << std::endl;
        std::cout << "  [5] Y Aim Offset         : " << settings.aim_offset_y << std::endl;
        // NEW: Option to toggle the advanced visuals
        std::cout << "  [6] Show Aiming Visuals  : " << (settings.show_aiming_visuals ? "ON" : "OFF") << " (Toggle)" << std::endl;
        std::cout << "--------------------------------------------------" << std::endl;
        std::cout << "  [0] Save and Resume Tuner" << std::endl << std::endl;

        std::cout << "Enter a number to edit, or 0 to save and resume: ";
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            choice = -1;
            continue;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
        case 1: std::cout << "Enter new FOV size (e.g., 300): "; std::cin >> settings.fov_size; break;
        case 2: std::cout << "Enter new min contour area (e.g., 50): "; std::cin >> settings.min_contour_area; break;
        case 3: {
            std::cout << "Enter new aim target ('head', 'body', or 'center'): ";
            std::string target;
            std::getline(std::cin, target);
            if (target == "head" || target == "body" || target == "center") settings.aim_target = target;
            else std::cout << "Invalid target. No changes made." << std::endl;
            break;
        }
        case 4: std::cout << "Enter new X aim offset (in pixels): "; std::cin >> settings.aim_offset_x; break;
        case 5: std::cout << "Enter new Y aim offset (in pixels): "; std::cin >> settings.aim_offset_y; break;
        case 6: // NEW: Toggle the boolean
            settings.show_aiming_visuals = !settings.show_aiming_visuals;
            break;
        }
    }

    if (Config::save("config.json", settings)) {
        std::cout << "Settings saved to config.json." << std::endl;
    }
    else {
        std::cout << "Error: Could not save settings." << std::endl;
    }
    std::cout << "Press Enter to continue...";
    std::cin.get();
}

void TUI::showHsvTuner(TunerSettings& settings) {
    clearScreen();
    printHeader("HSV Hot-Swap Tuner");

    std::cout << "Current Lower HSV: [" << settings.hsv_lower[0] << ", " << settings.hsv_lower[1] << ", " << settings.hsv_lower[2] << "]" << std::endl;
    std::cout << "Current Upper HSV: [" << settings.hsv_upper[0] << ", " << settings.hsv_upper[1] << ", " << settings.hsv_upper[2] << "]" << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;

    int h_l, s_l, v_l, h_u, s_u, v_u;

    std::cout << "Enter new LOWER HSV values (H S V), separated by spaces: ";
    std::cin >> h_l >> s_l >> v_l;
    if (std::cin.fail()) {
        std::cout << "Invalid input. No changes made." << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::this_thread::sleep_for(std::chrono::seconds(2));
        return;
    }
    settings.hsv_lower = { h_l, s_l, v_l };

    std::cout << "Enter new UPPER HSV values (H S V), separated by spaces: ";
    std::cin >> h_u >> s_u >> v_u;
    if (std::cin.fail()) {
        std::cout << "Invalid input. No changes made." << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::this_thread::sleep_for(std::chrono::seconds(2));
        return;
    }
    settings.hsv_upper = { h_u, s_u, v_u };

    std::cout << "\nHSV values updated for this session." << std::endl;
    std::cout << "The final values will be saved to a .txt file on exit." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
}