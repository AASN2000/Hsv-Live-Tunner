#include "Serial.h"
#include <iostream>

Serial::Serial(const char* portName, int baudRate) {
    this->connected = false;
    this->hSerial = CreateFileA(portName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (this->hSerial == INVALID_HANDLE_VALUE) {
        if (GetLastError() == ERROR_FILE_NOT_FOUND) {
            std::cerr << "ERROR: Handle was not attached. Reason: " << portName << " not available.\n";
        }
        else {
            std::cerr << "ERROR: Unknown serial error.\n";
        }
    }
    else {
        DCB dcbSerialParams = { 0 };
        if (!GetCommState(this->hSerial, &dcbSerialParams)) {
            std::cerr << "ERROR: Failed to get current serial parameters.\n";
        }
        else {
            dcbSerialParams.BaudRate = baudRate;
            dcbSerialParams.ByteSize = 8;
            dcbSerialParams.StopBits = ONESTOPBIT;
            dcbSerialParams.Parity = NOPARITY;
            dcbSerialParams.fDtrControl = DTR_CONTROL_ENABLE;

            if (!SetCommState(hSerial, &dcbSerialParams)) {
                std::cout << "ALERT: Could not set Serial Port parameters\n";
            }
            else {
                this->connected = true;
                PurgeComm(this->hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR);
            }
        }
    }
}

Serial::~Serial() {
    if (this->connected) {
        this->connected = false;
        CloseHandle(this->hSerial);
    }
}

bool Serial::write(int dx, int dy) {
    if (!this->connected) return false;

    std::string data = std::to_string(dx) + "," + std::to_string(dy) + "\n";
    DWORD bytes_written;
    return WriteFile(this->hSerial, data.c_str(), static_cast<DWORD>(data.length()), &bytes_written, NULL);
}

bool Serial::isConnected() const {
    return this->connected;
}