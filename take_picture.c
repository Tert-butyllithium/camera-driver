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

void setup(void)
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
    // char* reply = getVersion();
    // printf("version reply: %p", reply);
    // if (reply == 0) {
    //     printf("Failed to get version");
    // } else {
    //     printf("-----------------\n");
    //     printf("%s\n",reply);
    //     printf("-----------------\n");
    // }

    // setImageSize(VC0706_640x480);
    //setImageSize(VC0706_320x240);
    //setImageSize(VC0706_160x120);

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

void loop(void)
{
    unsigned int i = 0;
    // msleep(3000);
    if (!takePicture())
        printf("Failed to snap!\n");
    else
        printf("Picture taken!\n");

    uint32_t jpglen = frameLength();
    buf_len = jpglen;
    printf("%d  byte image\n", jpglen);

    while (jpglen > 0) {
        uint8_t* buffer;
        uint32_t bytesToRead = min((uint32_t)1024, jpglen); // 调节一次性读取数据大小，从32-64byte ，过大容易不工作
        buffer = readPicture(bytesToRead);
        strncpy(super_buf + i, buffer, bytesToRead);
        i += bytesToRead;
        // imgFile.write(buffer, bytesToRead);
        // fwrite(buffer, 1, bytesToRead, imgFile);
        jpglen -= bytesToRead;
    }
    printf("...Done!\n");
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
int main(){
    // base = ioremap(UART_REG_ADDR, UART_REG_SIZE);
    serial_init(base, 115200);
    VC0706();

    setup();
    loop();
    return 0;
}
#endif

