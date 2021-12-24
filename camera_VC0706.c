/***************************************************
  This is a library for the openjumper TTL JPEG Camera (VC0706 chipset)


  These displays use Serial to communicate, 2 pins are required to interface

 ****************************************************/

#include "camera_VC0706.h"
#include <unistd.h>
// #include <cstdio>
#include <string.h>
#include "common.h"

void cam_common_init(void)
{
    // serial = NULL;
    frameptr = 0;
    bufferLen = 0;
    serialNum = 0;
}

// // Constructor when using SoftwareSerial or NewSoftSerial
// #if ARDUINO >= 100
// cam_camera_VC0706(SoftwareSerial *ser) {
// #else
// cam_camera_VC0706(NewSoftSerial *ser) {
// #endif
// 	common_init();  // Set everything to common state, then...
// 	swSerial = ser; // ...override swSerial with value passed.
// }

// cam_camera_VC0706(SerialPort* ser)
// {
//     common_init();
//     serial = ser;
// }

void cam_VC0706_init(){
    cam_common_init();
}

// 硬件串口构造函数
// cam_camera_VC0706(HardwareSerial *ser) {
// 	common_init();  // Set everything to common state, then...
// 	hwSerial = ser; // ...override hwSerial with value passed.
// }

// 初始化摄像头,通信波特率
bool cam_begin(uint32_t baud)
{
    return cam_reset();
}

//复位
bool cam_reset()
{
    uint8_t args[] = { 0x0 };

    return cam_runCommand(VC0706_RESET, args, 1, 5, false);
}

//动作检测
bool cam_motionDetected()
{
    if (cam_readResponse(4, 200) != 4) {
        return false;
    }
    if (!cam_verifyResponse(VC0706_COMM_MOTION_DETECTED))
        return false;

    return true;
}

//设置动作状态
bool cam_setMotionStatus(uint8_t x, uint8_t d1, uint8_t d2)
{
    uint8_t args[] = { 0x03, x, d1, d2 };

    return cam_runCommand(VC0706_MOTION_CTRL, args, sizeof(args), 5, false);
}

//获取动作状态
uint8_t cam_getMotionStatus(uint8_t x)
{
    uint8_t args[] = { 0x01, x };

    return cam_runCommand(VC0706_MOTION_STATUS, args, sizeof(args), 5, false);
}

//打开动作检测
bool cam_setMotionDetect(bool flag)
{
    if (!cam_setMotionStatus(VC0706_MOTIONCONTROL,
            VC0706_UARTMOTION, VC0706_ACTIVATEMOTION))
        return false;

    uint8_t args[] = { 0x01, flag };

    cam_runCommand(VC0706_COMM_MOTION_CTRL, args, sizeof(args), 5, false);
}

//获取检测状态
bool cam_getMotionDetect(void)
{
    uint8_t args[] = { 0x0 };

    if (!cam_runCommand(VC0706_COMM_MOTION_STATUS, args, 1, 6, false))
        return false;

    return camerabuff[5];
}

//获取图片大小
uint8_t cam_getImageSize()
{
    uint8_t args[] = { 0x4, 0x4, 0x1, 0x00, 0x19 };
    if (!cam_runCommand(VC0706_READ_DATA, args, sizeof(args), 6, false))
        return -1;

    return camerabuff[5];
}

//设置图片尺寸
bool cam_setImageSize(uint8_t x)
{
    uint8_t args[] = { 0x05, 0x04, 0x01, 0x00, 0x19, x };

    return cam_runCommand(VC0706_WRITE_DATA, args, sizeof(args), 5, false);
}

/****************** downsize image control */

uint8_t cam_getDownsize(void)
{
    uint8_t args[] = { 0x0 };
    if (!cam_runCommand(VC0706_DOWNSIZE_STATUS, args, 1, 6, false))
        return -1;

    return camerabuff[5];
}

bool cam_setDownsize(uint8_t newsize)
{
    uint8_t args[] = { 0x01, newsize };

    return cam_runCommand(VC0706_DOWNSIZE_CTRL, args, 2, 5, false);
}

/***************** other high level commands */
//摄像头版本
char* cam_getVersion(void)
{
    uint8_t args[] = { 0x11 };

    cam_sendCommand(VC0706_GEN_VERSION, args, 1);
    // get reply
    if (!cam_readResponse(CAMERABUFFSIZ, 200))
        return 0;
    camerabuff[bufferLen] = 0; // end it!
    return (char*)camerabuff; // return it!
}

/****************** high level photo comamnds */

void cam_OSD(uint8_t x, uint8_t y, char* str)
{
    if (strlen(str) > 14) {
        str[13] = 0;
    }

    uint8_t args[17] = { strlen(str), strlen(str) - 1, (y & 0xF) | ((x & 0x3) << 4) };

    for (uint8_t i = 0; i < strlen(str); i++) {
        char c = str[i];
        if ((c >= '0') && (c <= '9')) {
            str[i] -= '0';
        } else if ((c >= 'A') && (c <= 'Z')) {
            str[i] -= 'A';
            str[i] += 10;
        } else if ((c >= 'a') && (c <= 'z')) {
            str[i] -= 'a';
            str[i] += 36;
        }

        args[3 + i] = str[i];
    }

    cam_runCommand(VC0706_OSD_ADD_CHAR, args, strlen(str) + 3, 5, false);
    cam_printBuff();
}

bool cam_setCompression(uint8_t c)
{
    uint8_t args[] = { 0x5, 0x1, 0x1, 0x12, 0x04, c };
    return cam_runCommand(VC0706_WRITE_DATA, args, sizeof(args), 5, false);
}

uint8_t cam_getCompression(void)
{
    uint8_t args[] = { 0x4, 0x1, 0x1, 0x12, 0x04 };
    cam_runCommand(VC0706_READ_DATA, args, sizeof(args), 6, false);
    cam_printBuff();
    return camerabuff[5];
}

bool cam_setPTZ(uint16_t wz, uint16_t hz, uint16_t pan, uint16_t tilt)
{
    uint8_t args[] = { 0x08, wz >> 8, wz,
        hz >> 8, wz,
        pan >> 8, pan,
        tilt >> 8, tilt };

    return (!cam_runCommand(VC0706_SET_ZOOM, args, sizeof(args), 5, false));
}

// bool cam_getPTZ(uint16_t& w, uint16_t& h, uint16_t& wz, uint16_t& hz, uint16_t& pan, uint16_t& tilt)
// {
//     uint8_t args[] = { 0x0 };

//     if (!cam_runCommand(VC0706_GET_ZOOM, args, sizeof(args), 16))
//         return false;
//     cam_printBuff();

//     w = camerabuff[5];
//     w <<= 8;
//     w |= camerabuff[6];

//     h = camerabuff[7];
//     h <<= 8;
//     h |= camerabuff[8];

//     wz = camerabuff[9];
//     wz <<= 8;
//     wz |= camerabuff[10];

//     hz = camerabuff[11];
//     hz <<= 8;
//     hz |= camerabuff[12];

//     pan = camerabuff[13];
//     pan <<= 8;
//     pan |= camerabuff[14];

//     tilt = camerabuff[15];
//     tilt <<= 8;
//     tilt |= camerabuff[16];

//     return true;
// }

bool cam_takePicture()
{
    frameptr = 0;
    return cam_cameraFrameBuffCtrl(VC0706_STOPCURRENTFRAME);
}

bool cam_resumeVideo()
{
    return cam_cameraFrameBuffCtrl(VC0706_RESUMEFRAME);
}

bool cam_TVon()
{
    uint8_t args[] = { 0x1, 0x1 };
    return cam_runCommand(VC0706_TVOUT_CTRL, args, sizeof(args), 5, false);
}
bool cam_TVoff()
{
    uint8_t args[] = { 0x1, 0x0 };
    return cam_runCommand(VC0706_TVOUT_CTRL, args, sizeof(args), 5, false);
}

bool cam_cameraFrameBuffCtrl(uint8_t command)
{
    uint8_t args[] = { 0x1, command };
    return runCommand(VC0706_FBUF_CTRL, args, sizeof(args), 5);
}

uint32_t cam_frameLength(void)
{
    uint8_t args[] = { 0x01, 0x00 };
    if (!runCommand(VC0706_GET_FBUF_LEN, args, sizeof(args), 9))
        return 0;

    uint32_t len;
    len = camerabuff[5];
    len <<= 8;
    len |= camerabuff[6];
    len <<= 8;
    len |= camerabuff[7];
    len <<= 8;
    len |= camerabuff[8];

    return len;
}

uint8_t cam_available(void)
{
    return bufferLen;
}

uint8_t* cam_readPicture(uint8_t n)
{
    uint8_t args[] = { 0x0C, 0x0, 0x0A,
        0, 0, frameptr >> 8, frameptr & 0xFF,
        0, 0, 0, n,
        CAMERADELAY >> 8, CAMERADELAY & 0xFF };

    if (!cam_runCommand(VC0706_READ_FBUF, args, sizeof(args), 5, false))
        return 0;

    // read into the buffer PACKETLEN!
    if (cam_readResponse(n + 5, CAMERADELAY) == 0)
        return 0;

    frameptr += n;

    return camerabuff;
}

/**************** low level commands */

bool cam_runCommand(uint8_t cmd, uint8_t* args, uint8_t argn,
    uint8_t resplen, bool flushflag)
{
    // flush out anything in the buffer?
    if (flushflag) {
        cam_readResponse(100, 10);
    }

    cam_sendCommand(cmd, args, argn);
    if (cam_readResponse(resplen, 200) != resplen)
        return false;
    if (!cam_verifyResponse(cmd))
        return false;
    return true;
}

void cam_sendCommand(uint8_t cmd, uint8_t args[], uint8_t argn)
{
    serial_write((byte)0x56);
    serial_write((byte)serialNum);
    serial_write((byte)cmd);

    for (uint8_t i = 0; i < argn; i++) {
        serial_write((byte)args[i]);
        //Serial.print(" 0x");
        //Serial.print(args[i], HEX);
    }
}

uint8_t cam_readResponse(uint8_t numbytes, uint8_t timeout)
{
    uint8_t counter = 0;
    bufferLen = 0;
    int avail;

    while ((timeout != counter) && (bufferLen != numbytes)) {
        avail = serial_available();
        if (avail <= 0) {
            usleep(1);
            counter++;
            continue;
        }
        counter = 0;
        // there's a byte!
        camerabuff[bufferLen++] = serial_read();
    }
    return bufferLen;
}

bool cam_verifyResponse(uint8_t command)
{
    if ((camerabuff[0] != 0x76) || (camerabuff[1] != serialNum) || (camerabuff[2] != command) || (camerabuff[3] != 0x0))
        return false;
    return true;
}

void cam_printBuff()
{
    for (uint8_t i = 0; i < bufferLen; i++) {
        printf("0x%X",camerabuff[i]);
        // Serial.print(camerabuff[i], HEX);
		
    }
    printf("\n");
}
