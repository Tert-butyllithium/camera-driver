#include "base64/base64.h"
#include "camera_VC0706.h"
#include "common.h"
#include "uart/uart.h"
#ifdef __MY_KMOD__
MODULE_LICENSE("MIT");
#endif

static char super_buf[1024 * 128];
static unsigned int buf_len;
void* base;

#include <time.h>

void setup()
{

    printf("VC0706 Camera test\n");

    // reset cam
    if (begin(115200)) {
        printf("Camera Found:\n");
    } else {
        printf("No camera found?\n");
        return;
    }

    // get version
    time_t start = time(NULL);
    char* reply = getVersion();
    time_t end = time(NULL);
    printf("get reply time %lu\n", end - start);
    if (reply == 0) {
        printf("Failed to get version\n");
    } else {
        printf("-----------------\n");
        printf("%s\n", reply);
        printf("-----------------\n");
    }

    sleep(3);

    // start = time(NULL);
    // setImageSize(VC0706_640x480);
    // end = time(NULL);
    printf("set resolution 640 x 480\n");
    printf("set resolution time %lu\n", end - start);
    // setImageSize(VC0706_320x240);
    // setImageSize(VC0706_160x120);

    // uint8_t imgsize = getImageSize();
    // printf("Image size: %02X\n", imgsize);
    // if (imgsize == VC0706_640x480)
    //     printf("640x480\n");
    // if (imgsize == VC0706_320x240)
    //     printf("320x240\n");
    // if (imgsize == VC0706_160x120)
    //     printf("160x120\n");

    printf("Get ready !\n");
}

void loop()
{
    time_t start = time(NULL);
    if (!takePicture())
        printf("Failed to snap!\n");
    else
        printf("Picture taken!\n");
    time_t end = time(NULL);
    char filename[13];
    printf("taking picture %lu\n", end - start);
    strcpy(filename, "IMAGE00.JPG");
    FILE* imgFile = fopen(filename, "wb");

    start = time(NULL);
    uint32_t jpglen = frameLength();
    end = time(NULL);

    printf("image size: %u bytes, time: %lu\n", jpglen, end - start);

    printf("Writing image to %s\n", filename);
    int flag = 0;
    int rept_cnt = 0;
    while (jpglen > 0) {
        // read 2048 bytes each time
        uint8_t* buffer;
        uint32_t bytesToRead = min(32u, jpglen);
        rept:
        buffer = readPicture(bytesToRead);
        // imgFile.write(buffer, bytesToRead);
        // printf("buf: %p\n",buffer);
        if (!buffer){
            // if(flag){
            //     break;
            // }
            flag++;
            rept_cnt++;
            printf("jpglen: %d, flag = %d\n", jpglen,flag);
            goto rept;
        }
        flag = 0;
        fwrite(buffer, 1, bytesToRead, imgFile);
        jpglen -= bytesToRead;
        // printf("jpglen: %d\n", jpglen);
    }
    fclose(imgFile);
    printf("...Done! with repat times: %d\n",rept_cnt);
    // resumeVideo();
}

#ifdef __MY_KMOD__
static int __init camera_drv_init(void)
{
    base = ioremap(UART_REG_ADDR, UART_REG_SIZE);
    serial_init(base, 115200);
    VC0706();
    // uart = SerialPort(base);
    // cam = camera_VC0706(&uart);

    setup();
    loop();
    return 0;
}

static void __exit camera_drv_exit(void)
{
    print_base64_encode(super_buf, buf_len);
    iounmap(base);
}

module_init(camera_drv_init);
module_exit(camera_drv_exit);
#else

#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE - 1)
int main()
{
    // base = ioremap(UART_REG_ADDR, UART_REG_SIZE);
    int fd = open("/dev/mem", O_RDWR);
    printf("fd: %d, offest: %lx\n", fd, UART_REG_ADDR & ~MAP_MASK);
    base = mmap(NULL, 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED, fd, UART_REG_ADDR & ~MAP_MASK);
    printf("base: %p\n", base);
    // getchar();
    serial_init(base, 115200);
    VC0706();

    setup();
    loop();
    munmap(base, 0x1000);
    return 0;
}
#endif
