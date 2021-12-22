// #include "uart_help.h"
// #include "uart/uart.h"
// #include "uart_help.h"
// #include <string.h>

// // Linux headers
// // #include <fcntl.h>
// // #include <sys/mman.h>
// // #include <unistd.h>
// // #include <sys/ioctl.h>
// // #include <cstdio>
// #include "common.h"

// SerialPort::SerialPort(void* base, uint32_t baud)
// {
//     //TODO (mmap)
//     // ioctl(file,TTY);
//     printf("mapped addr: %p\n",base);
//     sifive_uart_init(base, 0, baud);
// }

// size_t SerialPort::write(const byte b)
// {
//     sifive_uart_putc(b);
//     return 0;
// }

// byte SerialPort::read()
// {
//     return (byte)sifive_uart_getc();
// }

// bool SerialPort::available()
// {
//     return sifive_uart_getc() != -1;
// }

// #ifdef TEST_UART
// int main()
// {
//     SerialPort serial = SerialPort();
//     const byte reset_command[] = { '\x56', '\x00', '\x26', '\x00' };
//     for (int i = 0; i < 4; i++) {
//         serial.write(reset_command[i]);
//     }
//     byte b;
//     while(serial.available()) {
//         b = serial.read();
//         printf("0x%x\n", b);
//     } 
// }
// #endif