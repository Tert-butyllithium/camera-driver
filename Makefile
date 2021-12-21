CFLAGS = -g 

src := take_picture.cpp
src += camera_VC0706.cpp
src += uart/uart.c
src += uart_help.cpp

objs := $(src:%.cpp=%.o)

all:
	c++ $(CFLAGS) $(src) -o take_picture
clean:
	-@rm take_picture uart
uart: 
	c++ $(CFLAGS) uart.cpp -o uart -DTEST_UART
.PHONY: clean uart