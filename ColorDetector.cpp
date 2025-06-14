// For the Tuner Project
#include "ColorDetector.h"
#include <opencv2/imgproc.hpp>
#include <limits>

ColorDetector::ColorDetector(const std::vector<int>& lower, const std::vector<int>& upper) {
    hsv_lower = cv::Scalar(lower[0], lower[1], lower[2]);
    hsv_upper = cv::Scalar(upper[0], upper[1], upper[2]);
}

bool ColorDetector::findTarget(const cv::Mat& frame, const TunerSettings& settings, cv::Point& target_center, cv::Mat* debug_frame) {
    cv::Mat hsv, mask;
    cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);
    cv::inRange(hsv, hsv_lower, hsv_upper, mask);

    cv::erode(mask, mask, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3)));
    cv::dilate(mask, mask, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

    if (debug_frame) {
        cv::cvtColor(mask, *debug_frame, cv::COLOR_GRAY2BGR);
    }

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    if (contours.empty()) {
        return false;
    }

    const cv::Point fov_center(frame.cols / 2, frame.rows / 2);
    double min_distance = std::numeric_limits<double>::max();
    int closest_contour_index = -1;

    for (size_t i = 0; i < contours.size(); i++) {
        if (cv::contourArea(contours[i]) < settings.min_contour_area) {
            continue;
        }
        cv::Moments mu = cv::moments(contours[i]);
        if (mu.m00 == 0) continue;
        cv::Point current_center(static_cast<int>(mu.m10 / mu.m00), static_cast<int>(mu.m01 / mu.m00));
        double distance = cv::norm(current_center - fov_center);

        if (distance < min_distance) {
            min_distance = distance;
            closest_contour_index = static_cast<int>(i);
        }
        if (debug_frame) {
            cv::drawContours(*debug_frame, contours, static_cast<int>(i), cv::Scalar(0, 255, 255), 1);
        }
    }

    if (closest_contour_index == -1) {
        return false;
    }

    const auto& target_contour = contours[closest_contour_index];
    if (settings.aim_target == "head" || settings.aim_target == "body") {
        cv::Rect bounding_box = cv::boundingRect(target_contour);
        if (settings.aim_target == "head") {
            target_center.x = bounding_box.x + bounding_box.width / 2;
            target_center.y = bounding_box.y;
        }
        else {
            target_center.x = bounding_box.x + bounding_box.width / 2;
            target_center.y = bounding_box.y + bounding_box.height / 2;
        }
        if (debug_frame) {
            cv::rectangle(*debug_frame, bounding_box, cv::Scalar(0, 255, 0), 2);
        }
    }
    else {
        cv::Moments final_mu = cv::moments(target_contour);
        target_center.x = static_cast<int>(final_mu.m10 / final_mu.m00);
        target_center.y = static_cast<int>(final_mu.m01 / final_mu.m00);
    }

    if (debug_frame) {
        cv::circle(*debug_frame, target_center, 5, cv::Scalar(0, 0, 255), -1);
        cv::drawContours(*debug_frame, contours, closest_contour_index, cv::Scalar(255, 0, 255), 2);
    }
    return true;
}