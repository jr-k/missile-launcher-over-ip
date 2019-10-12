/*
 * ctlmissile.c - simple code to control USB missile launchers.
 *
 * Copyright 2006 James Puderer <jpuderer@littlebox.ca>
 * Copyright 2006 Jonathan McDowell <noodles@earth.li>
 *
 * This is free software; you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation; version 2.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <usb.h>

int debug = 0;

/*
 * Command to control original M&S USB missile launcher.
 */
void send_command_ms(struct usb_device *dev, char *cmd)
{
        usb_dev_handle *launcher;
        char data[64];
        int ret;

	launcher = usb_open(dev);
	if (launcher == NULL) {
		perror("Unable to open device");
		exit(EXIT_FAILURE);
	}

	/* Detach kernel driver (usbhid) from device interface and claim */
	usb_detach_kernel_driver_np(launcher, 0);
	usb_detach_kernel_driver_np(launcher, 1);

	ret = usb_set_configuration(launcher, 1);
	if (ret < 0) {
		perror("Unable to set device configuration");
		exit(EXIT_FAILURE);
	}

	ret = usb_claim_interface(launcher, 1);
	if (ret < 0) {
		perror("Unable to claim interface");
		exit(EXIT_FAILURE);
	}

	ret = usb_set_altinterface(launcher, 0);
	if (ret < 0) {
		perror("Unable to set alternate interface");
		exit(EXIT_FAILURE);
	}

	data[0] = 'U';
	data[1] = 'S';
	data[2] = 'B';
	data[3] = 'C';
	data[4] = 0;
	data[5] = 0;
	data[6] = 4;
	data[7] = 0;
	ret = usb_control_msg(launcher,
			USB_DT_HID,			// request type
			USB_REQ_SET_CONFIGURATION,	// request
			USB_RECIP_ENDPOINT,		// value
			1,		// index
			data,		// data
			8,		// Length of data.
			500);		// Timeout
	if (ret != 8) {
		fprintf(stderr, "Error: %s\n", usb_strerror());
		exit(EXIT_FAILURE);
	}

	data[0] = 'U';
	data[1] = 'S';
	data[2] = 'B';
	data[3] = 'C';
	data[4] = 0;
	data[5] = 0x40;
	data[6] = 2;
	data[7] = 0;
	ret = usb_control_msg(launcher,
			USB_DT_HID,
			USB_REQ_SET_CONFIGURATION,
			USB_RECIP_ENDPOINT,
			1,
			data,
			8,		// Length of data.
			500);		// Timeout
	if (ret != 8) {
		fprintf(stderr, "Error: %s\n", usb_strerror());
		exit(EXIT_FAILURE);
	}

	usb_set_altinterface(launcher, 0);

	memset(data, 0, 64);
	if (!strcmp(cmd, "up")) {
		data[3] = 1;
	} else if (!strcmp(cmd, "down")) {
		data[4] = 1;
	} else if (!strcmp(cmd, "left")) {
		data[1] = 1;
	} else if (!strcmp(cmd, "right")) {
		data[2] = 1;
	} else if (!strcmp(cmd, "fire")) {
		data[5] = 1;
	} else if (strcmp(cmd, "stop")) {
		fprintf(stderr, "Unknown command: %s", cmd);
		exit(EXIT_FAILURE);
	}

	data[6] = 8;
	data[7] = 8;

	ret = usb_control_msg(launcher,
			USB_DT_HID,
			USB_REQ_SET_CONFIGURATION,
			USB_RECIP_ENDPOINT,
			1,
			data,
			64,		// Length of data.
			5000);		// Timeout
	if (ret != 64) {
		fprintf(stderr, "Error: %s\n", usb_strerror());
		exit(EXIT_FAILURE);
	}

	usb_release_interface(launcher, 1);

	usb_close(launcher);
}

/*
 * Command to control Dream Cheeky USB missile launcher
 */
void send_command_cheeky(struct usb_device *dev, char *cmd)
{
        usb_dev_handle *launcher;
        char data[8];
        int ret;

	launcher = usb_open(dev);
	if (launcher == NULL) {
		perror("Unable to open device");
		exit(EXIT_FAILURE);
	}

	/* Detach kernel driver (usbhid) from device interface and claim */
	usb_detach_kernel_driver_np(launcher, 0);
	usb_detach_kernel_driver_np(launcher, 1);

	ret = usb_set_configuration(launcher, 1);
	if (ret < 0) {
		perror("Unable to set device configuration");
		exit(EXIT_FAILURE);
	}
	ret = usb_claim_interface(launcher, 0);
	if (ret < 0) {
		perror("Unable to claim interface");
		exit(EXIT_FAILURE);
	}

	memset(data, 0, 8);
	if (!strcmp(cmd, "up")) {
		data[0] = 0x01;
	} else if (!strcmp(cmd, "down")) {
		data[0] = 0x02;
	} else if (!strcmp(cmd, "left")) {
		data[0] = 0x04;
	} else if (!strcmp(cmd, "right")) {
		data[0] = 0x08;
	} else if (!strcmp(cmd, "fire")) {
		data[0] = 0x10;
	} else if (strcmp(cmd, "stop")) {
		fprintf(stderr, "Unknown command: %s", cmd);
		exit(EXIT_FAILURE);
	}

	ret = usb_control_msg(launcher,
			USB_DT_HID,
			USB_REQ_SET_CONFIGURATION,
			USB_RECIP_ENDPOINT,
			0,
			data,
			8,		// Length of data.
			5000);		// Timeout
	if (ret != 8) {
		fprintf(stderr, "Error: %s\n", usb_strerror());
		exit(EXIT_FAILURE);
	}

	usb_release_interface(launcher, 0);
	usb_close(launcher);
}

int main(int argc, char *argv[])
{
	struct usb_bus *busses, *bus;
	struct usb_device *dev = NULL;

	if (argc != 2) {
		printf("Usage: ctlmissile [ up | down | left | right | fire ]\n");
		exit(EXIT_FAILURE);
	}

	usb_init();
	usb_find_busses();
	usb_find_devices();

	busses = usb_get_busses();

	for (bus = busses; bus && !dev; bus = bus->next) {
		for (dev = bus->devices; dev; dev = dev->next) {
			if (debug) {
				printf("Checking 0x%04x:0x%04x\n",
					dev->descriptor.idVendor,
					dev->descriptor.idProduct);
			}
			if (dev->descriptor.idVendor == 0x1130 &&
				dev->descriptor.idProduct == 0x0202) {
				send_command_ms(dev, argv[1]);
				break;
			}
			if (dev->descriptor.idVendor == 0x1941 &&
				dev->descriptor.idProduct == 0x8021) {
				send_command_cheeky(dev, argv[1]);
				break;
			}
		}
	}

	if (!dev) {
		fprintf(stderr, "Unable to find device.\n");
		exit(EXIT_FAILURE);
	}

	return 0;
}

