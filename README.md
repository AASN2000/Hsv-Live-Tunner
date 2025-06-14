# HsvTunner
This application is a dedicated visual tuning tool for the Color Detecion Aimbot. Its sole purpose is to help you find the settings for color detection in a real-time environment. It reads from and saves to a config.json

This tool does not move your mouse or interact with an Arduino. It is for visual feedback only.

#Features

    Real-Time Visual Feedback: See exactly what the color detector sees inside the Field of View (FOV).
    Live HSV Hot-Swapping: Instantly test new HSV color ranges without restarting the application.
    Live Settings Editor: Adjust FOV size, target selection, and offsets on the fly.
    Aiming Logic Visualization: Clearly see the difference between the calculated target point and the final aim point after offsets are applied.
    Simple & Advanced Modes: Toggle aiming visuals on or off to focus purely on color detection or to fine-tune the aiming logic.
    Automatic HSV Export: On exit, the last used HSV values are saved to a clean tuned_hsv_values.txt file for easy reference.
    
#Prerequisites if you wanna build from source
1.  Visual Studio 2019 or newwer
2.  opencv: this project was build with version 4.11
3.  project configuration: The Visual Studio project must be configured to find your OpenCV installation.
     -  C/C++ -> General -> Additional Include Directories: Pointing to C:\opencv\build\include (or your custom path).

#How to Compile
1.  Open the solution file (.sln) in Visual Studio.
2.  Select a build configuration (Debug and x64).
3.  Build the solution

#Hotkeys
F1 - HSV Hot-Swap: Opens a menu in the console to let you type in new HSV lower and upper bounds.
F12 - Settings Editor: Opens a menu to adjust other visual parameters like FOV size, aim target, and offsets.
END - Exit and Save: Closes the application and saves the last used HSV values to tuned_hsv_values.txt.


#How to Use
1.  First record your game play using obs or other recording software (Using highest quality video is recommanded to reduce noise)
2.  Start with a general idea of the color you want to detect.
3.  Enter your first general hsv in config.json or F1 and enter broad HSV ranges to start.
4.  Observe the debug window. If too much is being detected, narrow your HSV range. If nothing is detected, broaden it.
5.  Repeat the F1 hot-swap process until only your desired target is consistently highlighted.
6.  Once you are satisfied, press the END key.

#Understanding the Visuals
-    Green Crosshair: The exact center of the Field of View (FOV).
-    Yellow Contours: An outline around every shape that matches your current HSV color range.
-    Purple Contour: The specific contour that has been chosen as the primary target (the one closest to the green crosshair).

If "Show Aiming Visuals" is ON (Not recomanded to turn on because your aiming logic might be diffrent then mine)
-    Blue Circle: The calculated aim point based on your aim_target setting (e.g., the "head" or "body" of the purple contour).
-    ed Crosshair: The final aiming position after applying the aim_offset_x and aim_offset_y values to the blue circle. This is where the aimbot would aim.

#Configuration (config.json)
The Tuner uses a config.json file to store its settings.

-  menu_key: The virtual key code for the settings editor.
-  fov_size: The width and height of the square capture area in pixels.
-  hsv_lower / hsv_upper: The color range to detect.
-  min_contour_area: Used to filter out small, noisy color detections.
-  aim_target: How to calculate the aim point (head, body, center).
-  aim_offset_x / aim_offset_y: Pixel offset applied to the calculated aim point.
-  show_aiming_visuals: Toggles between Simple and Advanced visual modes.
