KDIR := ../linux-5.12-rc4
CROSS_COMPILE := /opt/riscv/bin/riscv64-unknown-linux-gnu-
PWD := $(shell pwd)
obj-m := camdriver.o 

camdriver-y := take_picture.o
camdriver-y += uart_help.o
camdriver-y += camera_VC0706.o
camdriver-y += uart/uart.o
camdriver-y += base64/base64.o


all:
	make -C $(KDIR) M=$(PWD) CROSS_COMPILE=$(CROSS_COMPILE) ARCH=riscv modules 
clean:
	make -C $(KDIR) M=$(PWD) clean