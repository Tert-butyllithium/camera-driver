/***************************************************
  This is a library for the Adafruit TTL JPEG Camera (VC0706 chipset)

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/397

  These displays use Serial to communicate, 2 pins are required to interface

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution

  This version has been modified by Haonan in 2022 to make it suitable for
  Linux/Linux kernel using it

 ****************************************************/

#include "camera_VC0706.h"
// #include <cstdio>
#include "common.h"

#define HI(X) (uint8_t)((X) >> 8)
#define LO(X) (uint8_t)((X)&0xFF)

inline bool runCommand_help(uint8_t cmd, uint8_t* args, uint8_t argn,
    uint8_t resplen)
{
    return runCommand(cmd, args, argn, resplen, false);
}

// Initialization code used by all constructor types
void common_init(void)
{
    // hwSerial = NULL;
    frameptr = 0;
    bufferLen = 0;
    serialNum = 0;
}

// Constructor when using HardwareSerial
void VC0706()
{
    common_init(); // Set everything to common state, then...
    // hwSerial = ser; // ...override hwSerial with value passed.
}

bool begin(uint32_t baud)
{
    return reset();
}

bool reset()
{
    uint8_t args[] = { 0x0 };
    bool res = runCommand_help(VC0706_RESET, args, 1, 4);
    printBuff();
    return res;
}

bool motionDetected()
{
    if (readResponse(4, 200) != 4) {
        return false;
    }

    if (!verifyResponse(VC0706_COMM_MOTION_DETECTED)) {
        return false;
    }

    return true;
}

bool setMotionStatus(uint8_t x, uint8_t d1, uint8_t d2)
{
    uint8_t args[] = { 0x03, x, d1, d2 };
    return runCommand_help(VC0706_MOTION_CTRL, args, sizeof(args), 5);
}

uint8_t getMotionStatus(uint8_t x)
{
    uint8_t args[] = { 0x01, x };
    return runCommand_help(VC0706_MOTION_STATUS, args, sizeof(args), 5);
}

bool setMotionDetect(bool flag)
{
    if (!setMotionStatus(VC0706_MOTIONCONTROL,
            VC0706_UARTMOTION, VC0706_ACTIVATEMOTION)) {
        return false;
    }

    uint8_t args[] = { 0x01, flag };
    return runCommand_help(VC0706_COMM_MOTION_CTRL, args, sizeof(args), 5);
}

bool getMotionDetect(void)
{
    uint8_t args[] = { 0x0 };

    if (!runCommand_help(VC0706_COMM_MOTION_STATUS, args, 1, 6)) {
        return false;
    }

    return camerabuff[5];
}

uint8_t getImageSize()
{
    uint8_t args[] = { 0x4, 0x4, 0x1, 0x00, 0x19 };

    if (!runCommand_help(VC0706_READ_DATA, args, sizeof(args), 6)) {
        return -1;
    }

    return camerabuff[5];
}

bool setImageSize(uint8_t x)
{
    uint8_t v = x <= ((uint8_t)0x22u) ? 0x04 : 0x05;
    uint8_t args[] = { 0x05, v, 0x01, 0x00, 0x19, x };
    bool res = runCommand_help(VC0706_WRITE_DATA, args, sizeof(args), 5);
    printBuff();
    return res;
}

/****************** downsize image control */

uint8_t getDownsize(void)
{
    uint8_t args[] = { 0x0 };

    if (!runCommand_help(VC0706_DOWNSIZE_STATUS, args, 1, 6)) {
        return -1;
    }

    return camerabuff[5];
}

bool setDownsize(uint8_t newsize)
{
    uint8_t args[] = { 0x01, newsize };
    return runCommand_help(VC0706_DOWNSIZE_CTRL, args, 2, 5);
}

/***************** other high level commands */

char* getVersion(void)
{
    uint8_t args[] = { 0x00 };
    sendCommand(VC0706_GEN_VERSION, args, 1);

    if (!readResponse(5, 200)) {
        return 0;
    }
    printBuff();
    printf("version len: %d\n", camerabuff[4]);

    // get reply
    if (!readResponse(camerabuff[4], 200)) {
        return 0;
    }

    printBuff();

    camerabuff[bufferLen] = 0; // end it!
    return (char*)camerabuff; // return it!
}

/***************** baud rate commands */

char* setBaud9600()
{
    uint8_t args[] = { 0x03, 0x01, 0xAE, 0xC8 };
    sendCommand(VC0706_SET_PORT, args, sizeof(args));

    // get reply
    if (!readResponse(CAMERABUFFSIZ, 200)) {
        return 0;
    }

    camerabuff[bufferLen] = 0; // end it!
    return (char*)camerabuff; // return it!
}

char* setBaud19200()
{
    uint8_t args[] = { 0x03, 0x01, 0x56, 0xE4 };
    sendCommand(VC0706_SET_PORT, args, sizeof(args));

    // get reply
    if (!readResponse(CAMERABUFFSIZ, 200)) {
        return 0;
    }

    camerabuff[bufferLen] = 0; // end it!
    return (char*)camerabuff; // return it!
}

char* setBaud38400()
{
    uint8_t args[] = { 0x03, 0x01, 0x2A, 0xF2 };
    sendCommand(VC0706_SET_PORT, args, sizeof(args));

    // get reply
    if (!readResponse(CAMERABUFFSIZ, 200)) {
        return 0;
    }

    camerabuff[bufferLen] = 0; // end it!
    return (char*)camerabuff; // return it!
}

char* setBaud57600()
{
    uint8_t args[] = { 0x03, 0x01, 0x1C, 0x1C };
    sendCommand(VC0706_SET_PORT, args, sizeof(args));

    // get reply
    if (!readResponse(CAMERABUFFSIZ, 200)) {
        return 0;
    }

    camerabuff[bufferLen] = 0; // end it!
    return (char*)camerabuff; // return it!
}

char* setBaud115200()
{
    uint8_t args[] = { 0x03, 0x01, 0x0D, 0xA6 };
    sendCommand(VC0706_SET_PORT, args, sizeof(args));

    // get reply
    if (!readResponse(CAMERABUFFSIZ, 200)) {
        return 0;
    }

    camerabuff[bufferLen] = 0; // end it!
    return (char*)camerabuff; // return it!
}

/****************** high level photo comamnds */

void OSD(uint8_t x, uint8_t y, char* str)
{
    uint8_t i;
    if (strlen(str) > 14) {
        str[13] = 0;
    }

    uint8_t args[17] = { LO(strlen(str)), LO(strlen(str) - 1), LO((y & 0xF) | ((x & 0x3) << 4)) };

    for (i = 0; i < strlen(str); i++) {
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

    runCommand_help(VC0706_OSD_ADD_CHAR, args, strlen(str) + 3, 5);
    printBuff();
}

bool setCompression(uint8_t c)
{
    uint8_t args[] = { 0x5, 0x1, 0x1, 0x12, 0x04, c };
    return runCommand_help(VC0706_WRITE_DATA, args, sizeof(args), 5);
}

uint8_t getCompression(void)
{
    uint8_t args[] = { 0x4, 0x1, 0x1, 0x12, 0x04 };
    runCommand_help(VC0706_READ_DATA, args, sizeof(args), 6);
    printBuff();
    return camerabuff[5];
}

// bool setPTZ(uint16_t wz, uint16_t hz, uint16_t pan, uint16_t tilt)
// {
//     uint8_t args[] = { 0x08, HI(wz), LO(wz),
//         HI(hz), LO(hz),
//         HI(pan), LO(pan),
//         HI(tilt), LO(tilt) };
//     return (!runCommand_help(VC0706_SET_ZOOM, args, sizeof(args), 5));
// }

// bool getPTZ(uint16_t& w, uint16_t& h, uint16_t& wz, uint16_t& hz, uint16_t& pan, uint16_t& tilt)
// {
//     uint8_t args[] = { 0x0 };

//     if (!runCommand_help(VC0706_GET_ZOOM, args, sizeof(args), 16)) {
//         return false;
//     }

//     printBuff();
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

bool takePicture()
{
    frameptr = 0;
    return cameraFrameBuffCtrl(VC0706_STOPCURRENTFRAME);
}

bool resumeVideo()
{
    return cameraFrameBuffCtrl(VC0706_RESUMEFRAME);
}

bool TVon()
{
    uint8_t args[] = { 0x1, 0x1 };
    return runCommand_help(VC0706_TVOUT_CTRL, args, sizeof(args), 5);
}
bool TVoff()
{
    uint8_t args[] = { 0x1, 0x0 };
    return runCommand_help(VC0706_TVOUT_CTRL, args, sizeof(args), 5);
}

bool cameraFrameBuffCtrl(uint8_t command)
{
    uint8_t args[] = { 0x1, command };
    return runCommand_help(VC0706_FBUF_CTRL, args, sizeof(args), 5);
}

uint32_t frameLength(void)
{
    uint8_t args[] = { 0x01, 0x00 };

    if (!runCommand_help(VC0706_GET_FBUF_LEN, args, sizeof(args), 9)) {
        return 0;
    }

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

uint8_t available(void)
{
    return bufferLen;
}

// TODO (look like n>128 will cause some faults)
uint8_t* readPicture(uint32_t n)
{
    uint8_t args[] = { 0x0C, 0x0, 0x0A,
        0, 0, HI(frameptr), LO(frameptr),
        (uint8_t)(n >> 24u), (uint8_t)((n >> 16) & 0xffu), (uint8_t)((n >> 8) & 0xffu), (uint8_t)(n & 0xffu),
        HI(CAMERADELAY), LO(CAMERADELAY) };

    if (!runCommand(VC0706_READ_FBUF, args, sizeof(args), 5, false)) {
        return 0;
    }

    // read into the buffer PACKETLEN!
    if (readResponse(n + 5, CAMERADELAY) == 0) {
        return 0;
    }

    frameptr += n;
    return camerabuff;
}

/**************** low level commands */

bool runCommand(uint8_t cmd, uint8_t* args, uint8_t argn,
    uint8_t resplen, bool flushflag)
{
    // flush out anything in the buffer?
    if (flushflag) {
        readResponse(100, 10);
    }

    sendCommand(cmd, args, argn);

    if (readResponse(resplen, 200) != resplen) {
        return false;
    }

    if (!verifyResponse(cmd)) {
        return false;
    }

    return true;
}

void sendCommand(uint8_t cmd, uint8_t args[], uint8_t argn)
{
    uint8_t i;
    serial_write((byte)0x56);
    serial_write((byte)serialNum);
    serial_write((byte)cmd);

    for (i = 0; i < argn; i++) {
        serial_write((byte)args[i]);
    }
}

#include <chrono>
#include <thread>
uint32_t readResponse(uint8_t numbytes, uint8_t timeout)
{
    uint8_t counter = 0;
    bufferLen = 0;
    int avail;

    while ((timeout != counter) && (bufferLen != numbytes)) {
        avail = serial_available();

        if (avail <= 0) {
            // delay(1);
            // msleep(1);
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
            counter++;
            continue;
        }

        counter = 0;
        // there's a byte!
        camerabuff[bufferLen++] = serial_read();
        // printf("there's a byte! \\x%02X", camerabuff[bufferLen-1]);
    }

    printBuff();
    return bufferLen;
}

bool verifyResponse(uint8_t command)
{
    if ((camerabuff[0] != 0x76) || (camerabuff[1] != serialNum) || (camerabuff[2] != command) || (camerabuff[3] != 0x0)) {
        return false;
    }

    return true;
}

void printBuff()
{
    // #ifdef TEST_CAM
    uint8_t i = 0;
    printf("[buflen: %d] ", bufferLen);
    for (; i < bufferLen; i++) {
        // if (isalnum(camerabuff[i])) {
        //     printf("%c", camerabuff[i]);
        // } else {
        printf("\\x%02X", camerabuff[i]);
        // }
    }

    printf("\n");
    // #endif
}
