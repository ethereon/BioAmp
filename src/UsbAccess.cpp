/*
================================================================

	USB Access
	
	A general USB I/O class built on top of libusb
	
	Copyright (C) 2010 Saumitro Dasgupta.
	This code is made available under the MIT License.
	<http://www.opensource.org/licenses/mit-license.html>

================================================================
*/

#include "UsbAccess.h"
#include <assert.h>
#include <iostream>

//-----------------------------------------------------------------------------

using namespace std;
using namespace xinverse;

//-----------------------------------------------------------------------------

#ifndef NDEBUG
#define TRACE(msg) cout << "[TRACE] " << msg << endl
#else
#define TRACE(msg)
#endif

//-----------------------------------------------------------------------------

UsbAccess::UsbAccess() {

  this->devices = NULL;
  this->context = NULL;
  this->hDev = NULL;

}

//-----------------------------------------------------------------------------
 
UsbAccess::~UsbAccess() {


  if(this->devices!=NULL)
    endSession();

}

//-----------------------------------------------------------------------------

bool UsbAccess::isConnected() const {

  return this->hDev!=NULL;

}

//-----------------------------------------------------------------------------

void UsbAccess::beginSession() {

  assert(this->context==NULL);

  if(libusb_init(&this->context)<0)
    return; //ERROR HANDLE

  libusb_set_debug(this->context, 3);

  
}

//-----------------------------------------------------------------------------
 
void UsbAccess::endSession() {

  assert(this->context!=NULL);

  if(this->hDev!=NULL)
    closeDevice();


  libusb_exit(this->context);

  this->context = NULL;
  this->devices = NULL;



}

//-----------------------------------------------------------------------------

void UsbAccess::openPrologue() {

  assert(devices==NULL && context!=NULL);

  deviceCount = libusb_get_device_list(this->context, &this->devices);

  //libusb returns 1 extra because of null termination
  deviceCount--;

}

//-----------------------------------------------------------------------------

void UsbAccess::openEpilogue() {

  assert(this->devices!=NULL);

  libusb_free_device_list(this->devices,1);
  this->devices=NULL;

}


//-----------------------------------------------------------------------------

bool UsbAccess::onSuccessfulOpen(libusb_device* dev) {

  assert(this->hDev!=NULL);

  this->resolveKernelDriver();

  libusb_config_descriptor* cfg = NULL;

  libusb_get_active_config_descriptor(dev, &cfg);
  
  if(cfg==NULL) {

    TRACE("HALT! Unable to acquire configuration descriptor!");
    return false;
    
  }

  assert(cfg->bNumInterfaces>0);
  const libusb_interface* firstInterface = &(cfg->interface[0]);

  assert(firstInterface->num_altsetting>0);

  const libusb_interface_descriptor* firstSetting = &(firstInterface->altsetting[0]);

  if(libusb_claim_interface(this->hDev,firstSetting->bInterfaceNumber)!=0) {

    TRACE("HALT! Failed on attempting to claim interface!");
    return false;

  }

  TRACE("Successfully claimed interface");

  this->epOutAddress=255;
  this->epInAddress=255;

  TRACE("Discovering endpoint addresses...");

  for(int i=0;i<firstSetting->bNumEndpoints;++i) {

    const libusb_endpoint_descriptor *ep = &(firstSetting->endpoint[i]);
    uint8_t epAddr = ep->bEndpointAddress;

    if(epAddr>>6)
      this->epInAddress=epAddr;
    else
      this->epOutAddress=epAddr;

  }

  assert(epOutAddress!=255);
  assert(epInAddress!=255);

  TRACE("All endpoint addresses found.");

  libusb_free_config_descriptor(cfg);

  return true;


}

//-----------------------------------------------------------------------------

void UsbAccess::resolveKernelDriver() {

  
  if(libusb_kernel_driver_active(this->hDev,0) == 1) {
    
    TRACE("Kerner driver detected! Attempting to detach it...");
    
    if(libusb_detach_kernel_driver(this->hDev, 0) == 0) {
      
      TRACE("Kernel driver successfully detached.");
      
    }
    
  } else {
    
    
    TRACE("No kernel driver detected. (It's a good thing).");
    
  }
  
}


//-----------------------------------------------------------------------------

bool UsbAccess::openDeviceWithDescription(std::string desc) {


  //Make sure no open device exists.
  assert(this->hDev==NULL);

  this->openPrologue();

  unsigned char buffer[1024];
  libusb_device_descriptor descriptor;


    
  for(int i=0; i<this->deviceCount; ++i) {

    libusb_get_device_descriptor(devices[i],&descriptor);

    //Attempt to open device...
    if(libusb_open(devices[i], &this->hDev))
      continue;

    //Get the device's description string
    if(libusb_get_string_descriptor_ascii(this->hDev, descriptor.iProduct, buffer, 1024)>0) {

      //Check if it matches
      if(desc==(const char*)buffer) {

	TRACE("Target device found.");
	
	this->onSuccessfulOpen(devices[i]);
	break;
      
      }
      
    }
    
    //Device did not match. Close it.
    libusb_close(this->hDev);
    this->hDev = NULL;
  
  } 

  if(this->hDev==NULL) {
    
    TRACE("No matching devices found");

  }
 
  return (this->hDev!=NULL);

}

//-----------------------------------------------------------------------------

void UsbAccess::closeDevice() {


  assert(this->hDev!=NULL);
  libusb_close(this->hDev);


}

//-----------------------------------------------------------------------------

void UsbAccess::sendControlTransfer(const ControlTransfer& cx) {

  assert(this->hDev);

  libusb_control_transfer(this->hDev,
			  cx.type,
			  cx.request,
			  cx.value,
			  0,
			  (unsigned char*)cx.data,
			  cx.length,
			  cx.timeout);
			  

  
}

//-----------------------------------------------------------------------------

void UsbAccess::sendBulkTransfer(BulkTransfer& bx) {

  assert(this->hDev);

  while(1) {
    int retCode = libusb_interrupt_transfer(this->hDev,
					    bx.endpoint,
					    (unsigned char*)bx.data,
					    bx.length,
					    &bx.transferred,
					    bx.timeout);

    //Check for Pipe stall...
    if(retCode==LIBUSB_ERROR_PIPE)
      libusb_clear_halt(this->hDev, bx.endpoint);
    else
      return;


  }

}

//-----------------------------------------------------------------------------
