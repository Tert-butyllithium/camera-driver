#include "camera_VC0706.h"
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <unistd.h>
using std::min;

// camera_VC0706 cam = camera_VC0706(&cameraconnection);
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
    char* reply = cam.getVersion();
    if (reply == 0) {
        printf("Failed to get version");
    } else {
        printf("-----------------\n");
        printf("%s\n",reply);
        printf("-----------------\n");
    }

    // sleep(3);


    cam.setImageSize(VC0706_640x480);
    printf("set resolution 640 x 480\n");
    //cam.setImageSize(VC0706_320x240);
    //cam.setImageSize(VC0706_160x120);

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
    if (!cam.takePicture())
        printf("Failed to snap!\n");
    else
        printf("Picture taken!\n");
    char filename[13];
    strcpy(filename, "IMAGE00.JPG");
    FILE* imgFile = fopen(filename, "wb");
    uint32_t jpglen = cam.frameLength();

    printf("image size: %u bytes\n", jpglen);

    printf("Writing image to %s\n",filename);

    while (jpglen > 0) {
        // read 2048 bytes each time
        uint8_t* buffer;
        uint32_t bytesToRead = min(1024u, jpglen);
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

int main()
{
    setup();
    loop();
}