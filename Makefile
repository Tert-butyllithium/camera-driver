CFLAGS = -Ofast
CC := /opt/riscv/bin/riscv64-unknown-linux-gnu-g++

src := take_picture.c
src += camera_VC0706.c
src += uart/uart.c
src += uart_help.c
src += base64/base64.c

objs := $(src:%.cpp=%.o)

all:
	$(CC) $(CFLAGS) $(src) -o take_picture
clean:
	-@rm take_picture uart_test
uart: 
	$(CC) $(CFLAGS) uart_help.c uart/uart.c -o uart_test -DTEST_UART
.PHONY: clean uart