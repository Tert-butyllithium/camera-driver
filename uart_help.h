#pragma once

#include <stdint.h>
#include <cstddef>
using byte = unsigned char;
class SerialPort {
public:
    SerialPort(void* base, uint32_t baud = 115200);
    size_t write(const byte);
    byte read();
    bool available();

    int tty_file;
};