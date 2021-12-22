#include "uart_help.h"
#include "uart/uart.h"
#include "uart_help.h"
#include <string.h>

// Linux headers
// #include <fcntl.h>
// #include <sys/mman.h>
// #include <unistd.h>
// #include <sys/ioctl.h>
// #include <cstdio>

SerialPort::SerialPort(void* base, uint32_t baud)
{
    //TODO (mmap)
    // ioctl(file,TTY);
    printf("mapped addr: %p\n",base);
    sifive_uart_init(base, 0, baud);
}

size_t SerialPort::write(const byte b)
{
    // if (len == 0) {
    //     len = strlen((const char*)b);
    // }
    // // return unix_write(tty_file, b, len);
    // for (int i = 0; i < len; i++) {
    //     sifive_uart_putc(b[i]);
    // }
    sifive_uart_putc(b);
    return 0;
}

byte SerialPort::read()
{
    //     static byte buf[256];
    //     static size_t blen = 0;
    //     static int i = 0;
    //     if (i >= blen) {
    //         blen = unix_read(tty_file, buf, sizeof(buf));
    //         // if (blen == 0) {
    //         //     usleep(1);
    //         // }
    // #if TEST_UART
    //         printf("new read, blen=%lu\n", blen);
    // #endif
    //         i = 0;
    //     }
    //     return buf[i++];
    return (byte)sifive_uart_getc();
}

bool SerialPort::available()
{
    return sifive_uart_getc() != -1;
}

#ifdef TEST_UART
int main()
{
    SerialPort serial = SerialPort();
    const byte reset_command[] = { '\x56', '\x00', '\x26', '\x00' };
    for (int i = 0; i < 4; i++) {
        serial.write(reset_command[i]);
    }
    byte b;
    while(serial.available()) {
        b = serial.read();
        printf("0x%x\n", b);
    } 
}
#endif