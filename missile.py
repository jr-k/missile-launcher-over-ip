import usb.core
import usb.util
import sys

# find our device

dev = usb.core.find(idVendor=0x1130, idProduct=0x0202)

# was it found?
if dev is None:
    raise ValueError('Device not found')

# Linux kernel sets up a device driver for USB device, which you have
# to detach. Otherwise trying to interact with the device gives a
# 'Resource Busy' error.
try:
    dev.detach_kernel_driver(0)
except Exception as e:
    print(e)
    pass  # already unregistered

# set the active configuration. With no arguments, the first
# configuration will be the active one
dev.set_configuration()

print("ok")