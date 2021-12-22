KDIR := /lib/modules/5.13.0-7620-generic/build 
PWD := $(shell pwd)
obj-m := camdriver.o 

camdriver-y := take_picture.o
camdriver-y += uart_help.o
camdriver-y += camera_VC0706.o
camdriver-y += uart/uart.o
camdriver-y += base/base.o


all:
	make -C $(KDIR) M=$(PWD) modules 
clean:
	make -C $(KDIR) M=$(PWD) clean