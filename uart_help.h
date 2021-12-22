#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
// using byte = unsigned char;
typedef unsigned char byte;

void serial_init(void* base, uint32_t baud);
size_t serial_write(const byte);
byte serial_read();
bool serial_available();
