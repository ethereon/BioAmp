
#ifndef __USB_ACCESS_H__
#define __USB_ACCESS_H__

#include <iostream>
#include <libusb.h>
#include "ControlTransfer.h"
#include "BulkTransfer.h"

namespace xinverse {

class UsbAccess {


 protected:

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


 public:

  UsbAccess();
  ~UsbAccess();


  void beginSession();
  void endSession();

  bool isConnected() const;

  bool openDeviceWithDescription(std::string desc);
  void sendControlTransfer(const ControlTransfer& cx);
  void sendBulkTransfer(BulkTransfer& bx);
  void closeDevice();

  uint8_t getOutEndpoint() { return epOutAddress; }
  uint8_t getInEndpoint()  { return epInAddress; }
  

  
  

};

}

#endif
