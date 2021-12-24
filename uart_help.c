#include "uart/uart.h"
#include "uart_help.h"

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

bool serial_available(void)
{
    // return sifive_uart_getc() != -1;
    return 1;
}

#ifdef TEST_UART
#ifdef __MY_KMOD__

#include "base64/base64.h"
MODULE_LICENSE("MIT");
static void* base;
static int __init uart_init(void){
    // const  static char str[]="hello, world! from uart driver\n";
    const byte reset_command[] = { '\x56', '\x00', '\x26', '\x00' };
    char buf[20];
    unsigned int len = 4;
    int i;

    base = ioremap(UART_REG_ADDR, UART_REG_SIZE);
    serial_init(base, 115200);
    
    for(i=0;i<len;i++){
        serial_write(reset_command[i]);
    }
    for(i=0;i<20;i++){
        buf[i]=serial_read();
    }
    // // printf("%s\n",buf);
    print_base64_encode(buf,20);
    // printf("\\x%2x\\x%2x\\x%2x\\x%2x\n",buf[0], buf[1],buf[2],buf[3]);

    return 0;
}

static void __exit uart_exit(void)
{
    iounmap(base);
}

module_init(uart_init);
module_exit(uart_exit);
#else
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
#endif
