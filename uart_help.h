#pragma once

#include "common.h"

void serial_init(void* base, uint32_t baud);
size_t serial_write(const byte);
byte serial_read(void);
bool serial_available(void);
