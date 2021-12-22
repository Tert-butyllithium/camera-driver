/*************************************************** 
  This is a library for the openjumper TTL JPEG Camera (VC0706 chipset)

 ****************************************************/
#pragma once

#include "common.h"
#include "uart_help.h"

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


// struct camera_VC0706 {
  void cam_VC0706_init(void);
  bool cam_begin(uint32_t baud);
  bool cam_reset(void);
  bool cam_TVon(void);
  bool cam_TVoff(void);
  bool cam_takePicture(void);
  uint8_t* cam_readPicture(uint8_t n);
  bool cam_resumeVideo(void);
  uint32_t cam_frameLength(void);
  char *cam_getVersion(void);
  uint8_t cam_available(void);
  uint8_t cam_getDownsize(void);
  bool cam_setDownsize(uint8_t);
  uint8_t cam_getImageSize(void);
  bool cam_setImageSize(uint8_t);
  bool cam_getMotionDetect(void);
  uint8_t cam_getMotionStatus(uint8_t);
  bool cam_motionDetected(void);
  bool cam_setMotionDetect(bool f);
  bool cam_setMotionStatus(uint8_t x, uint8_t d1, uint8_t d2);
  bool cam_cameraFrameBuffCtrl(uint8_t command);
  uint8_t cam_getCompression(void);
  bool cam_setCompression(uint8_t c);
  
  // bool cam_getPTZ(uint16_t &w, uint16_t &h, uint16_t &wz, uint16_t &hz, uint16_t &pan, uint16_t &tilt);
  bool cam_setPTZ(uint16_t wz, uint16_t hz, uint16_t pan, uint16_t tilt);

  // void cam_OSD(uint8_t x, uint8_t y, char *s); // isnt supported by the chip :(
  static uint8_t  serialNum;
  static uint8_t  camerabuff[CAMERABUFFSIZ+1];
  static uint8_t  bufferLen;
  static uint16_t frameptr;
  // SerialPort* serial;

  void cam_common_init(void);
  bool cam_runCommand(uint8_t cmd, uint8_t args[], uint8_t argn, uint8_t resp, bool flushflag); 
  void cam_sendCommand(uint8_t cmd, uint8_t args[], uint8_t argn); 
  uint8_t cam_readResponse(uint8_t numbytes, uint8_t timeout);
  bool cam_verifyResponse(uint8_t command);
  void cam_printBuff(void);
// };s
