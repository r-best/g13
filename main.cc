#include <stdio.h>
#include <iostream>
#include <libusb-1.0/libusb.h>

#include "g13.h"
using namespace std;
using namespace G13;

int getG13s(libusb_device **devs, ssize_t count, libusb_device_handle **g13s){
	int errcode, num_g13s = 0;
	for(int i = 0; i < count; i++){
		libusb_device_descriptor desc;
		if((errcode = libusb_get_device_descriptor(devs[i], &desc)) < 0){
			cerr << "Error getting device descriptor, code: " << errcode << " (" << libusb_error_name(errcode) << ")" << endl;
			continue;
		}
		
		if(desc.idVendor == LOGITECH_VENDOR_ID && desc.idProduct == G13_PRODUCT_ID){
			libusb_device_handle *handle;
			if((errcode = libusb_open(devs[i], &handle)) != 0){
				cerr << "Error opening device, code " << errcode << " (" << libusb_error_name(errcode) << ")" << endl;
				continue;
			}
			
			if (libusb_kernel_driver_active(handle, 0) == 1)
				if (libusb_detach_kernel_driver(handle, 0) == 0)
					cout << "Kernel driver detached" << endl;
			
			if((errcode = libusb_claim_interface(handle, 0)) < 0){
				cerr << "Error claiming usb interface" << errcode << " (" << libusb_error_name(errcode) << ")" << endl;
				continue;
			}

			g13s[num_g13s++] = handle;
		}
	}
	return num_g13s;
}

int main(void){
	// Initialize libusb
	if (libusb_init(NULL) < 0){
		cerr << "Error initializing libusb" << endl;
		return -1;
	}
	
	// Find all connected USB devices
	printf("Finding connected USB devices...\n");
	libusb_device **devs;
	int cnt = libusb_get_device_list(NULL, &devs);
	if (cnt < 0){
		cerr << "No usb devices found" << endl;
		return -1;
	}
	printf("Found %d USB devices\n", cnt);

	// Filter out USB devices that aren't G13s
	libusb_device_handle **g13s;
	int num_g13s = 0;
	if((num_g13s = getG13s(devs, cnt, g13s)) < 1){
		cerr << "No G13 devices found" << endl;
		return -1;
	}
	libusb_free_device_list(devs, 1);
	printf("%d device(s) are Logitech G13s\n", num_g13s);

	libusb_exit(NULL);
	return 0;
}