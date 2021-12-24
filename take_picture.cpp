#include "camera_VC0706.h"
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <unistd.h>
using std::min;

// camera_VC0706 cam = camera_VC0706(&cameraconnection);
SerialPort uart = SerialPort();

camera_VC0706 cam = camera_VC0706(&uart);
void setup()
{

    // #if !defined(SOFTWARE_SPI)
    // #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    // 	if(chipSelect != 53) pinMode(53, OUTPUT); // SS on Mega
    // #else
    // 	if(chipSelect != 4) pinMode(4, OUTPUT); // SS on Uno, etc.
    // #endif
    // #endif

    // pinMode(7,INPUT_PULLUP);
    // Serial.begin(9600);
    printf("VC0706 Camera test\n");

    // //SD卡检测
    // if (!SD.begin(chipSelect)) {
    // 	printf("Card failed, or not present\n");
    // 	return;
    // }

    // 查询摄像头
    if (cam.begin()) {
        printf("Camera Found:\n");
    } else {
        printf("No camera found?\n");
        return;
    }

    sleep(4);
    // 摄像头版本号
    char* reply = cam.getVersion();
    if (reply == 0) {
        printf("Failed to get version");
    } else {
        printf("-----------------\n");
        printf(reply);
        printf("-----------------\n");
    }

    // 选择合适的图片尺寸 640x480, 320x240 or 160x120
    // 图片越大，传输速度越慢
    cam.setImageSize(VC0706_640x480);
    //cam.setImageSize(VC0706_320x240);
    //cam.setImageSize(VC0706_160x120);

    uint8_t imgsize = cam.getImageSize();
    printf("Image size: %02X\n", imgsize);
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
    uint16_t jpglen = cam.frameLength();
    printf("%d ", jpglen);
    printf(" byte image\n");

    printf("Writing image to ");
    printf(filename);

    while (jpglen > 0) {
        // 一次读取32bytes
        uint8_t* buffer;
        uint8_t bytesToRead = min((uint16_t)32, jpglen); // 调节一次性读取数据大小，从32-64byte ，过大容易不工作
        buffer = cam.readPicture(bytesToRead);
        // imgFile.write(buffer, bytesToRead);
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