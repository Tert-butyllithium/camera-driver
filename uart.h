#pragma

#include <stdint.h>
#include <cstddef>
using byte = unsigned char;
class SerialPort {
public:
    SerialPort(uint32_t baud = 115200);
    size_t write(const byte b);
    byte read();
    bool available();

    int tty_file;
};