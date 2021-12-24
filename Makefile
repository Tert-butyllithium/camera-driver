KDIR := ../linux-5.12.4
CROSS_COMPILE := /opt/riscv/bin/riscv64-unknown-linux-gnu-
PWD := $(shell pwd)

T1      ?= n
T1y		?= n
T2      ?= n
T2y		?= n

# T1, primary compile for driver
ccflags-$(T1y) := -fno-stack-protector
obj-$(T1) := camdriver.o 

camdriver-$(T1y) := take_picture.o
camdriver-$(T1y) += uart_help.o
camdriver-$(T1y) += camera_VC0706.o
camdriver-$(T1y) += uart/uart.o
camdriver-$(T1y) += base64/base64.o

# T2, test

ccflags-$(T2y) := -fno-stack-protector -DTEST_UART=1
obj-$(T2) := uart_test.o
uart_test-$(T2y) := uart_help.o 
uart_test-$(T2y) += uart/uart.o
uart_test-$(T2y) += base64/base64.o

main:
	$(MAKE) T1=m T1y=y all
all:
	make -C $(KDIR) M=$(PWD) CROSS_COMPILE=$(CROSS_COMPILE) ARCH=riscv modules 
test:
	$(MAKE) T2=m T2y=y all
clean:
	make -C $(KDIR) M=$(PWD) clean