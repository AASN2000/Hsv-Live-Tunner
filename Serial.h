#pragma once
#include <Windows.h>
#include <string>

class Serial {
public:
    Serial(const char* portName, int baudRate);
    ~Serial();
    bool write(int dx, int dy);
    bool isConnected() const;

private:
    HANDLE hSerial;
    bool connected;
};