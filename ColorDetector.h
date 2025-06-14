// For the Tuner Project
#pragma once

#include <opencv2/opencv.hpp>
#include "Config.h"

class ColorDetector {
public:
    ColorDetector(const std::vector<int>& lower, const std::vector<int>& upper);
    bool findTarget(const cv::Mat& frame, const TunerSettings& settings, cv::Point& target_center, cv::Mat* debug_frame);

private:
    cv::Scalar hsv_lower;
    cv::Scalar hsv_upper;
};