#include "ScreenCapture.h"

ScreenCapture::ScreenCapture(int width, int height) : capture_width(width), capture_height(height) {
    hScreenDC = GetDC(NULL);
    hMemoryDC = CreateCompatibleDC(hScreenDC);
    hBitmap = CreateCompatibleBitmap(hScreenDC, capture_width, capture_height);
    hOldBitmap = SelectObject(hMemoryDC, hBitmap);
}

ScreenCapture::~ScreenCapture() {
    SelectObject(hMemoryDC, hOldBitmap);
    DeleteObject(hBitmap);
    DeleteDC(hMemoryDC);
    ReleaseDC(NULL, hScreenDC);
}

cv::Mat ScreenCapture::capture(int x, int y) {
    BitBlt(hMemoryDC, 0, 0, capture_width, capture_height, hScreenDC, x, y, SRCCOPY);
    BITMAPINFOHEADER bi;
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = capture_width;
    bi.biHeight = -capture_height;
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;
    cv::Mat mat(capture_height, capture_width, CV_8UC4);
    GetDIBits(hMemoryDC, hBitmap, 0, capture_height, mat.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);
    cv::Mat bgrMat;
    cv::cvtColor(mat, bgrMat, cv::COLOR_BGRA2BGR);
    return bgrMat;
}