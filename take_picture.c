// #include <string.h>

#include "base64/base64.h"
#include "camera_VC0706.h"
#include "common.h"
#include "uart/uart.h"

static char super_buf[1024 * 100];
static unsigned int buf_len;

void setup()
{

    // pinMode(7,INPUT_PULLUP);
    // Serial.begin(9600);
    printf("VC0706 Camera test\n");

    // 查询摄像头
    if (cam_begin(115200)) {
        printf("Camera Found:\n");
    } else {
        printf("No camera found?\n");
        return;
    }
    // 摄像头版本号
    char* reply = cam_getVersion();
    if (reply == 0) {
        printf("Failed to get version");
    } else {
        printf("-----------------\n");
        printf(reply);
        printf("-----------------\n");
    }

    // 选择合适的图片尺寸 640x480, 320x240 or 160x120
    // 图片越大，传输速度越慢
    cam_setImageSize(VC0706_640x480);
    //cam_setImageSize(VC0706_320x240);
    //cam_setImageSize(VC0706_160x120);

    uint8_t imgsize = cam_getImageSize();
    printf("Image size: ");
    if (imgsize == VC0706_640x480)
        printf("640x480\n");
    if (imgsize == VC0706_320x240)
        printf("320x240\n");
    if (imgsize == VC0706_160x120)
        printf("160x120\n");

    printf("Get ready !\n");
}

void loop()
{
    unsigned int i = 0;
    if (1) { //按键检测
        // usleep(10);
        if (1) {
            if (!cam_takePicture())
                printf("Failed to snap!\n");
            else
                printf("Picture taken!\n");

            uint16_t jpglen = cam_frameLength();
            buf_len = jpglen;
            printf("%d  byte image\n", jpglen);

            while (jpglen > 0) {
                // 一次读取32bytes
                uint8_t* buffer;
                uint8_t bytesToRead = min((uint16_t)32, jpglen); // 调节一次性读取数据大小，从32-64byte ，过大容易不工作
                buffer = cam_readPicture(bytesToRead);
                strncpy(super_buf + i, buffer, bytesToRead);
                i += bytesToRead;
                // imgFile.write(buffer, bytesToRead);
                // fwrite(buffer, 1, bytesToRead, imgFile);
                jpglen -= bytesToRead;
            }
            printf("...Done!\n");
            cam_resumeVideo();
        }
    }
}

#ifdef __MY_KMOD__
static int __init camera_drv_init(void)
{
    base = ioremap(UART_REG_ADDR, UART_REG_SIZE);
    uart = SerialPort(base);
    cam = camera_VC0706(&uart);
    setup();
    loop();
    return 0;
}

static void __exit camera_drv_exit(void)
{
    print_base64_encode(super_buf, buf_len);
}

module_init(camera_drv_init);
module_exit(camera_drv_exit);

#endif