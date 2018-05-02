default: main

main: main.cc g13.h
	g++ $< `pkg-config --libs --cflags libusb-1.0`

clean:
	rm *.out