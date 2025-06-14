#include <iostream>
#include <fstream>
#include <Windows.h>
#include <memory>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>

#include "Config.h"
#include "ScreenCapture.h"
#include "ColorDetector.h"
#include "UserInput.h"
#include "TUI.h"

void saveHsvToTxt(const TunerSettings& settings) {
    std::ofstream file("tuned_hsv_values.txt");
    if (!file.is_open()) {
        std::cerr << "Error: Could not open tuned_hsv_values.txt for writing." << std::endl;
        return;
    }
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    char time_str[26];
    ctime_s(time_str, sizeof(time_str), &now_time);
    file << "// HSV values tuned on " << time_str;
    file << "// You can copy these values into your config.json\n\n";
    file << "\"hsv_lower\": [" << settings.hsv_lower[0] << ", " << settings.hsv_lower[1] << ", " << settings.hsv_lower[2] << "],\n";
    file << "\"hsv_upper\": [" << settings.hsv_upper[0] << ", " << settings.hsv_upper[1] << ", " << settings.hsv_upper[2] << "]\n";
    std::cout << "Final HSV values have been saved to tuned_hsv_values.txt" << std::endl;
}

int main() {
    // ... (setup is unchanged) ...
    SetConsoleTitle(L"Aimbot Tuner");
    TunerSettings settings;
    if (!Config::load("config.json", settings)) {
        Config::createDefault("config.json");
        std::cout << "\nDefault config.json created. Please edit it and restart." << std::endl;
        std::cout << "Press Enter to exit...";
        std::cin.get();
        return -1;
    }
    TUI::clearScreen();
    TUI::printHeader("Aimbot Tuner");
    std::cout << "Tuner is active. Showing visual feedback." << std::endl;
    std::cout << "Press F1 to Hot-Swap HSV values." << std::endl;
    std::cout << "Press F12 to edit other settings." << std::endl;
    std::cout << "Press END in this console window to exit and save." << std::endl;
    auto screen_capture = std::make_unique<ScreenCapture>(settings.fov_size, settings.fov_size);
    ColorDetector color_detector(settings.hsv_lower, settings.hsv_upper);
    int screen_width = settings.screen_width > 0 ? settings.screen_width : GetSystemMetrics(SM_CXSCREEN);
    int screen_height = settings.screen_height > 0 ? settings.screen_height : GetSystemMetrics(SM_CYSCREEN);
    const std::string debug_window_name = "Aimbot Debug";
    bool last_menu_key_state = false;
    bool last_hsv_key_state = false;

    while (!isKeyDown(VK_END)) {
        // ... (F1 and F12 menu logic is unchanged) ...
        bool menu_key_down = isKeyDown(settings.menu_key);
        if (menu_key_down && !last_menu_key_state) {
            cv::destroyWindow(debug_window_name);
            cv::waitKey(1);
            TUI::showTunerSettings(settings);
            screen_capture.reset(new ScreenCapture(settings.fov_size, settings.fov_size));
            color_detector = ColorDetector(settings.hsv_lower, settings.hsv_upper);
            TUI::clearScreen();
            TUI::printHeader("Aimbot Tuner");
            std::cout << "Tuner is active. Showing visual feedback." << std::endl;
            std::cout << "Press F1 to Hot-Swap HSV values." << std::endl;
            std::cout << "Press F12 to edit other settings." << std::endl;
            std::cout << "Press END in this console window to exit and save." << std::endl;
        }
        last_menu_key_state = menu_key_down;
        bool hsv_key_down = isKeyDown(VK_F1);
        if (hsv_key_down && !last_hsv_key_state) {
            cv::destroyWindow(debug_window_name);
            cv::waitKey(1);
            TUI::showHsvTuner(settings);
            color_detector = ColorDetector(settings.hsv_lower, settings.hsv_upper);
            TUI::clearScreen();
            TUI::printHeader("Aimbot Tuner");
            std::cout << "Tuner is active. Showing visual feedback." << std::endl;
            std::cout << "Press F1 to Hot-Swap HSV values." << std::endl;
            std::cout << "Press F12 to edit other settings." << std::endl;
            std::cout << "Press END in this console window to exit and save." << std::endl;
        }
        last_hsv_key_state = hsv_key_down;

        int fov_x = (screen_width - settings.fov_size) / 2;
        int fov_y = (screen_height - settings.fov_size) / 2;
        int fov_center_x = settings.fov_size / 2;
        int fov_center_y = settings.fov_size / 2;

        cv::Mat frame = screen_capture->capture(fov_x, fov_y);
        if (frame.empty()) continue;

        cv::Mat debug_frame;
        frame.copyTo(debug_frame);

        cv::Point target_pos;
        if (color_detector.findTarget(frame, settings, target_pos, &debug_frame)) {

            // Conditional drawing logic
            if (settings.show_aiming_visuals) {
                // ADVANCED MODE: Show aim target and offset
                cv::Point final_aim_pos(target_pos.x + settings.aim_offset_x, target_pos.y + settings.aim_offset_y);
                cv::circle(debug_frame, target_pos, 6, cv::Scalar(255, 0, 0), 1); // Blue circle on calculated target
                cv::circle(debug_frame, final_aim_pos, 2, cv::Scalar(0, 0, 255), -1); // Red dot on final aim point
                cv::line(debug_frame, { final_aim_pos.x - 5, final_aim_pos.y }, { final_aim_pos.x + 5, final_aim_pos.y }, { 0, 0, 255 }, 1);
                cv::line(debug_frame, { final_aim_pos.x, final_aim_pos.y - 5 }, { final_aim_pos.x, final_aim_pos.y + 5 }, { 0, 0, 255 }, 1);
            }
            else {
                // SIMPLE MODE: Show only the raw detected point
                cv::circle(debug_frame, target_pos, 3, cv::Scalar(255, 255, 255), -1); // Solid white dot
            }
        }

        cv::line(debug_frame, { fov_center_x - 10, fov_center_y }, { fov_center_x + 10, fov_center_y }, { 0, 255, 0 }, 1);
        cv::line(debug_frame, { fov_center_x, fov_center_y - 10 }, { fov_center_x, fov_center_y + 10 }, { 0, 255, 0 }, 1);

        cv::imshow(debug_window_name, debug_frame);
        cv::waitKey(1);
    }

    saveHsvToTxt(settings);
    cv::destroyAllWindows();
    return 0;
}