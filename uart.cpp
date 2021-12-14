// C library headers
#include <stdio.h>
#include <string.h>

// Linux headers
#include <errno.h> // Error integer and strerror() function
#include <fcntl.h> // Contains file controls like O_RDWR
#include <termios.h>
#include <unistd.h> // write(), read(), close()

#define SERIAL_PORT "/dev/ttyS0"

inline size_t unix_write(int __fd, const void* __buf, size_t __n)
{
    return write(__fd, __buf, __n);
}

inline size_t unix_read(int __fd, void* __buf, size_t __nbytes)
{
    return read(__fd, __buf, __nbytes);
}

#include "uart.h"

int set_interface_attribs(int fd, int speed)
{
    struct termios tty;

    if (tcgetattr(fd, &tty) < 0) {
        printf("Error from tcgetattr: %s\n", strerror(errno));
        return -1;
    }

    cfsetospeed(&tty, (speed_t)speed);
    cfsetispeed(&tty, (speed_t)speed);

    tty.c_cflag |= (CLOCAL | CREAD); /* ignore modem controls */
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8; /* 8-bit characters */
    tty.c_cflag &= ~PARENB; /* no parity bit */
    tty.c_cflag &= ~CSTOPB; /* only need 1 stop bit */
    tty.c_cflag &= ~CRTSCTS; /* no hardware flowcontrol */

    /* setup for non-canonical mode */
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tty.c_oflag &= ~OPOST;

    /* fetch bytes as they become available */
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 1;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        printf("Error from tcsetattr: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

void set_mincount(int fd, int mcount)
{
    struct termios tty;

    if (tcgetattr(fd, &tty) < 0) {
        printf("Error tcgetattr: %s\n", strerror(errno));
        return;
    }

    tty.c_cc[VMIN] = mcount ? 1 : 0;
    tty.c_cc[VTIME] = 1; /* half second timer */

    if (tcsetattr(fd, TCSANOW, &tty) < 0)
        printf("Error tcsetattr: %s\n", strerror(errno));
}

SerialPort::SerialPort(uint32_t baud)
{
    int wlen;
    tty_file = open(SERIAL_PORT, O_RDWR | O_NOCTTY | O_SYNC);
    // memset(&tty, 0, sizeof(tty));
    set_interface_attribs(tty_file, B115200);
    set_mincount(tty_file, 0);
    tcdrain(tty_file);
}

size_t SerialPort::write(const byte* b, int len)
{
    if(len == 0){
        len = strlen((const char*) b);
    }
    return unix_write(tty_file, b, len);
}

byte SerialPort::read()
{
    static byte buf[256];
    static size_t blen = 0;
    static int i = 0;
    if (i >= blen) {
        blen = unix_read(tty_file, buf, sizeof(buf));
        // if (blen == 0) {
        //     usleep(1);
        // }
#if TEST_UART
        printf("new read, blen=%lu\n", blen);
#endif
        i = 0;
    }
    return buf[i++];
}

bool SerialPort::available()
{
    return true;
}
#ifdef TEST_UART
int main()
{
    SerialPort serial = SerialPort();
    const byte reset_command[] = { '\x56', '\x00', '\x26', '\x00' };
    for (int i = 0; i < 4; i++) {
        serial.write(reset_command, 4);
    }
    byte b;
    do {
        b = serial.read();
        printf("0x%x\n", b);
    } while (1);
}
#endif