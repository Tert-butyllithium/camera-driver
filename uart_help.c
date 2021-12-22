#include "uart/uart.h"
#include "uart_help.h"
#include <string.h>

#include "common.h"

void serial_init(void* base, uint32_t baud)
{
    //TODO (mmap)
    // ioctl(file,TTY);
    printf("mapped addr: %p\n",base);
    sifive_uart_init(base, 0, baud);
}

size_t serial_write(const byte b)
{
    sifive_uart_putc(b);
    return 0;
}

byte serial_read()
{
    return (byte)sifive_uart_getc();
}

bool serail_available()
{
    return sifive_uart_getc() != -1;
}

#ifdef TEST_UART
int main()
{
    // SerialPort serial = SerialPort();
    
    // const byte reset_command[] = { '\x56', '\x00', '\x26', '\x00' };
    // for (int i = 0; i < 4; i++) {
    //     serial.write(reset_command[i]);
    // }
    // byte b;
    // while(serial.available()) {
    //     b = serial.read();
    //     printf("0x%x\n", b);
    // } 
}
#endif
