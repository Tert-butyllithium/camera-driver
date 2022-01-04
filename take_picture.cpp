#include "camera_VC0706.h"
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <time.h>
#include <unistd.h>
using std::min;

// camera_VC0706 cam = camera_VC0706(&cameraconnection);
// SerialPort uart = SerialPort(115200, "/dev/ttySIF1");
SerialPort uart = SerialPort();
VC0706 cam = VC0706(&uart);
void setup()
{

    printf("VC0706 Camera test\n");

    // reset cam
    // if (cam.begin()) {
    //     printf("Camera Found:\n");
    // } else {
    //     printf("No camera found?\n");
    //     return;
    // }

    // get version
    time_t start = time(NULL);
    char* reply = cam.getVersion();
    time_t end = time(NULL);
    printf("get reply time %lu\n", end - start);
    if (reply == 0) {
        printf("Failed to get version");
    } else {
        printf("-----------------\n");
        printf("%s\n", reply);
        printf("-----------------\n");
    }

    // sleep(3);

    // start = time(NULL);
    // cam.setImageSize(VC0706_640x480);
    // end = time(NULL);
    printf("set resolution 640 x 480\n");
    printf("set resolution time %lu\n", end - start);
    // cam.setImageSize(VC0706_320x240);
    // cam.setImageSize(VC0706_160x120);

    // uint8_t imgsize = cam.getImageSize();
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
    if (!cam.takePicture())
        printf("Failed to snap!\n");
    else
        printf("Picture taken!\n");
    time_t end = time(NULL);
    char filename[13];
    printf("taking picture %lu\n", end - start);
    strcpy(filename, "IMAGE00.JPG");
    FILE* imgFile = fopen(filename, "wb");

    start = time(NULL);
    uint32_t jpglen = cam.frameLength();
    end = time(NULL);

    printf("image size: %u bytes, time: %lu\n", jpglen, end - start);

    printf("Writing image to %s\n", filename);

    while (jpglen > 0) {
        // read 2048 bytes each time
        uint8_t* buffer;
        uint32_t bytesToRead = min(64u, jpglen);
        buffer = cam.readPicture(bytesToRead);
        // imgFile.write(buffer, bytesToRead);
        // printf("buf: %p\n",buffer);
        fwrite(buffer, 1, bytesToRead, imgFile);
        jpglen -= bytesToRead;
    }
    fclose(imgFile);
    printf("...Done!\n");
    cam.resumeVideo();
}

#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define UART_REG_ADDR 0x10011000
#define UART_REG_SIZE 0x400
void* base;
void inspect()
{
    int fd = open("/dev/mem", O_RDWR);
    printf("fd: %d, offest: %lx\n", fd, UART_REG_ADDR);
    base = mmap(NULL, 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED, fd, UART_REG_ADDR);

    unsigned int* txctrl = (unsigned int*)(base + 0x08);
    unsigned int* rxctrl = (unsigned int*)(base + 0x0c);
    unsigned int* ie = (unsigned int*)(base + 0x10);
    unsigned int* ip = (unsigned int*)(base + 0x14);
    unsigned int* div = (unsigned int*)(base + 0x18);

    printf("txctrl: 0x%x\n", *txctrl);
    printf("rxctrl: 0x%x\n", *rxctrl);
    printf("ie: 0x%x\n", *ie);
    printf("ip: 0x%x\n", *ip);
    printf("div: 0x%x\n", *div);

    munmap(base, 0x1000);
}

int main()
{
#ifdef INSPECT
    inspect();
#endif
    setup();
    loop();
}