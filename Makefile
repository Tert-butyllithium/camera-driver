CFLAGS = -g 

src := take_picture.c
src += camera_VC0706.c
src += uart/uart.c
src += uart_help.c
src += base64/base64.c

objs := $(src:%.cpp=%.o)

all:
	c++ $(CFLAGS) $(src) -o take_picture
clean:
	-@rm take_picture uart_test
uart: 
	c++ $(CFLAGS) uart_help.cpp uart/uart.c -o uart_test -DTEST_UART
.PHONY: clean uart