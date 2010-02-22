/*
================================================================

	USB Access
	
	A general USB I/O class built on top of libusb
	
	Copyright (C) 2010 Saumitro Dasgupta.
	This code is made available under the MIT License.
	<http://www.opensource.org/licenses/mit-license.html>

================================================================
*/

#ifndef __USB_ACCESS_H__
#define __USB_ACCESS_H__

#include <iostream>
#include <libusb.h>
#include "ControlTransfer.h"
#include "BulkTransfer.h"

namespace xinverse {

class UsbAccess {


 private:

  libusb_device **devices;
  libusb_context * context;
  libusb_device_handle *hDev;
  ssize_t deviceCount;

  uint8_t epOutAddress;
  uint8_t epInAddress;

  void resolveKernelDriver();
  void openPrologue();
  void openEpilogue();
  bool onSuccessfulOpen(libusb_device* dev);

  int verbosityLevel;

  void trace(const char* msg);


 public:

  UsbAccess();
  ~UsbAccess();


  void beginSession();
  void endSession();

  bool isConnected() const;

  void setVerbosityLevel(int level);

  bool openDeviceWithDescription(std::string desc, int index=0);
  void devicesWithVidPid(uint16_t vId, uint16_t pid);
  void sendControlTransfer(const ControlTransfer& cx);
  void sendBulkTransfer(BulkTransfer& bx);
  void closeDevice();

  uint8_t getOutEndpoint() { return epOutAddress; }
  uint8_t getInEndpoint()  { return epInAddress; }

};

}

#endif
