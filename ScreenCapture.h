// For the Tuner Project
#pragma once

#include <Windows.h>
#include <opencv2/opencv.hpp>

class ScreenCapture {
public:
    ScreenCapture(int width, int height);
    ~ScreenCapture();
    cv::Mat capture(int x, int y);

private:
    HDC hScreenDC;
    HDC hMemoryDC;
    HBITMAP hBitmap;
    HGDIOBJ hOldBitmap;
    int capture_width;
    int capture_height;
};