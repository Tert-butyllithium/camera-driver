#include "uart_help.h"
#include "uart/uart.h"

#include "common.h"

void serial_init(void* base, uint32_t baud)
{
    // TODO (mmap)
    //  ioctl(file,TTY);
    printf("mapped addr: %p\n", base);
    sifive_uart_init(base, 0, baud);
}

size_t serial_write(const byte b)
{
    sifive_uart_putc(b);
    return 0;
}

static u8 small_buf[128];
static int buf_idx = 0;
byte serial_read()
{
    if (buf_idx) {
        return small_buf[--buf_idx];
    }
    return (byte)sifive_uart_getc();
}

bool serial_available(void)
{
    unsigned long cnt = 200000000UL;
    int ch;
    while (cnt--) {
        ch = _sifive_uart_getc();
        if (ch != -1) {
            small_buf[buf_idx++] = (u8)ch;
            return true;
        }
    }
    return false;
}

#ifdef TEST_UART
#ifdef __MY_KMOD__

#include "base64/base64.h"
MODULE_LICENSE("MIT");
static void* base;
static int __init uart_init(void)
{
    // const  static char str[]="hello, world! from uart driver\n";
    byte reset_command[] = { '\x56', '\x00', '\x26', '\x00' };
    byte ver_command[] = { '\x56', '\x00', '\x11', '\x00' };
    char buf[20];
    unsigned int len = 4;
    int i;

    base = ioremap(UART_REG_ADDR, UART_REG_SIZE);
    serial_init(base, 115200);
    printf("test get version");

    for (i = 0; i < len; i++) {
        serial_write(reset_command[i]);
    }
    for (i = 0; i < 4; i++) {
        buf[i] = serial_read();
    }
    // // printf("%s\n",buf);
    // print_base64_encode(buf,20);
    printf("\\x%02X\\x%02X\\x%02X\\x%02X\n", buf[0], buf[1], buf[2], buf[3]);

    for (i = 0; i < len; i++) {
        serial_write(ver_command[i]);
    }

    for (i = 0; i < 5; i++) {
        buf[i] = serial_read();
    }
    printf("\\x%02X\\x%02X\\x%02X\\x%02X\\x%02X\n", buf[0], buf[1], buf[2], buf[3], buf[4]);

    for (i = 0; i < 11; i++) {
        buf[i] = serial_read();
    }
    buf[11] = '\0';
    printf("%s", buf);

    // print_base64_encode(buf,20);

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
    int fd = open("/dev/mem", O_RDWR);
    printf("fd: %d, offest: %lx\n", fd, UART_REG_ADDR);
    void* base = mmap(NULL, 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED, fd, UART_REG_ADDR);
    serial_init(base, 115200);
    const byte reset_command[] = { '\x56', '\x00', '\x26', '\x00' };
    for (int i = 0; i < 4; i++) {
        serial_write(reset_command[i]);
    }
    byte b;
    int cnt = 4;
    while (cnt--) {
        b = serial_read();
        printf("\\x%02X", b);
    }
    puts("");
    byte ver_command[] = { '\x56', '\x00', '\x11', '\x00' };
    for (int i = 0; i < 4; i++) {
        serial_write(ver_command[i]);
    }

    // cnt = 100;
    while (serial_available()) {
        b = serial_read();
        // if(isalnum(b)||b=='\n'){
        //     putchar(b);
        // }
        // else{
        //     printf("\\x%02X", b);
        // }
        putchar(b);
    }
    puts("");
    puts("-----end------");
}
#endif
#endif
