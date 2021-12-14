/*************************************************** 
  This is a library for the openjumper TTL JPEG Camera (VC0706 chipset)

 ****************************************************/
#pragma once

#include <stdint.h>
#include "uart.h"

#define VC0706_RESET  0x26
#define VC0706_GEN_VERSION 0x11
#define VC0706_READ_FBUF 0x32
#define VC0706_GET_FBUF_LEN 0x34
#define VC0706_FBUF_CTRL 0x36
#define VC0706_DOWNSIZE_CTRL 0x54
#define VC0706_DOWNSIZE_STATUS 0x55
#define VC0706_READ_DATA 0x30
#define VC0706_WRITE_DATA 0x31
#define VC0706_COMM_MOTION_CTRL 0x37
#define VC0706_COMM_MOTION_STATUS 0x38
#define VC0706_COMM_MOTION_DETECTED 0x39
#define VC0706_MOTION_CTRL 0x42
#define VC0706_MOTION_STATUS 0x43
#define VC0706_TVOUT_CTRL 0x44
#define VC0706_OSD_ADD_CHAR 0x45

#define VC0706_STOPCURRENTFRAME 0x0
#define VC0706_STOPNEXTFRAME 0x1
#define VC0706_RESUMEFRAME 0x3
#define VC0706_STEPFRAME 0x2

#define VC0706_640x480 0x00
#define VC0706_320x240 0x11
#define VC0706_160x120 0x22

#define VC0706_MOTIONCONTROL 0x0
#define VC0706_UARTMOTION 0x01
#define VC0706_ACTIVATEMOTION 0x01

#define VC0706_SET_ZOOM 0x52
#define VC0706_GET_ZOOM 0x53

#define CAMERABUFFSIZ 100
#define CAMERADELAY 10


struct camera_VC0706 {
// #if ARDUINO >= 100
//   camera_VC0706(SoftwareSerial *ser); // Constructor when using SoftwareSerial
// #else
//   camera_VC0706(NewSoftSerial  *ser); // Constructor when using NewSoftSerial
// #endif
//   camera_VC0706(HardwareSerial *ser); // Constructor when using HardwareSerial
  camera_VC0706(SerialPort *ser);
  bool begin(uint32_t baud = 115200);
  bool reset(void);
  bool TVon(void);
  bool TVoff(void);
  bool takePicture(void);
  uint8_t *readPicture(uint8_t n);
  bool resumeVideo(void);
  uint32_t frameLength(void);
  char *getVersion(void);
  uint8_t available();
  uint8_t getDownsize(void);
  bool setDownsize(uint8_t);
  uint8_t getImageSize();
  bool setImageSize(uint8_t);
  bool getMotionDetect();
  uint8_t getMotionStatus(uint8_t);
  bool motionDetected();
  bool setMotionDetect(bool f);
  bool setMotionStatus(uint8_t x, uint8_t d1, uint8_t d2);
  bool cameraFrameBuffCtrl(uint8_t command);
  uint8_t getCompression();
  bool setCompression(uint8_t c);
  
  bool getPTZ(uint16_t &w, uint16_t &h, uint16_t &wz, uint16_t &hz, uint16_t &pan, uint16_t &tilt);
  bool setPTZ(uint16_t wz, uint16_t hz, uint16_t pan, uint16_t tilt);

  void OSD(uint8_t x, uint8_t y, char *s); // isnt supported by the chip :(
  uint8_t  serialNum;
  uint8_t  camerabuff[CAMERABUFFSIZ+1];
  uint8_t  bufferLen;
  uint16_t frameptr;
// #if ARDUINO >= 100
//   SoftwareSerial *swSerial;
// #else
//   NewSoftSerial  *swSerial;
// #endif
//   HardwareSerial *hwSerial;
  SerialPort* serial;

  void common_init(void);
  bool runCommand(uint8_t cmd, uint8_t args[], uint8_t argn, uint8_t resp, bool flushflag = false); 
  void sendCommand(uint8_t cmd, uint8_t args[], uint8_t argn); 
  uint8_t readResponse(uint8_t numbytes, uint8_t timeout);
  bool verifyResponse(uint8_t command);
  void printBuff(void);
};
