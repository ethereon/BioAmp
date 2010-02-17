/*
================================================================

	AmpLink
	The Myelin Project
	
	User space driver for the Austrian g.UsbAmp
	
	Copyright (C) 2010 Saumitro Dasgupta.
	This code is made available under the MIT License.
	<http://www.opensource.org/licenses/mit-license.html>

================================================================
*/

#include "AmpLink.h"
#include "AmpProtocol.h"
#include <assert.h>

using namespace std;
using namespace xinverse;


#define CX_IN  LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_ENDPOINT_IN

#define CX_OUT LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_ENDPOINT_OUT

#define GUSBAMP_ID "g.USBamp"


//-----------------------------------------------------------------------------

AmpLink::AmpLink() {

  
  

}

//-----------------------------------------------------------------------------

AmpLink::~AmpLink() {

}

//-----------------------------------------------------------------------------

bool AmpLink::connect() {

  this->amp.beginSession();

  if(!this->amp.openDeviceWithDescription("g.USBamp"))
    return false;
  //TODO: Check the hw version packet with the newer amp

  return true;

}

//-----------------------------------------------------------------------------

void AmpLink::disconnect() {

  this->amp.endSession();

}

//-----------------------------------------------------------------------------

void AmpLink::setSamplingRate(unsigned int argRate) {


  cmd.request = AMP_SET_SAMPLING_RATE_REQUEST;
  cmd.value = argRate;
  cmd.length = 0;
  cmd.data = NULL;
  cmd.type = CX_OUT;

  amp.sendControlTransfer(cmd);

}

//-----------------------------------------------------------------------------

void AmpLink::setBufferSize(unsigned int argSize) {

  cmd.request = AMP_SET_BUFFER_SIZE_REQUEST;
  cmd.value = argSize;
  cmd.length = 0;
  cmd.data = NULL;
  cmd.type = CX_OUT;

  amp.sendControlTransfer(cmd);
  


}

//-----------------------------------------------------------------------------
 
void AmpLink::start() {

  cmd.request = AMP_START_REQUEST;
  cmd.value = 0;
  cmd.length = 0;
  cmd.data = NULL;
  cmd.type = CX_OUT;

  amp.sendControlTransfer(cmd);



}

//-----------------------------------------------------------------------------

void AmpLink::stop() {

  cmd.request = AMP_STOP_REQUEST;
  cmd.value = 0;
  cmd.length = 0;
  cmd.data = NULL;
  cmd.type = CX_OUT;

  amp.sendControlTransfer(cmd);


}

//-----------------------------------------------------------------------------

void AmpLink::getData(void* buffer, unsigned int bufferLength) {

  bx.endpoint = amp.getInEndpoint();
  bx.data = buffer;
  bx.length =bufferLength;
  
  amp.sendBulkTransfer(bx);
  
}

//-----------------------------------------------------------------------------

string AmpLink::getAmpSerial() {

}

//-----------------------------------------------------------------------------

double AmpLink::getImpedance(unsigned int channel) { 

}

//-----------------------------------------------------------------------------

void AmpLink::setMode(AmpMode mode) {


  uint8_t req;
  //First, send the common set mode request...

  if(mode==Impedance) {

    cmd.value = AMP_SET_MODE_IMPEDANCE_COMMON_VALUE;
    req = AMP_SET_MODE_IMPEDANCE_REQUEST;

  }
  else if(mode==Calibrate) {

    cmd.value = AMP_SET_MODE_CALIBRATE_COMMON_VALUE;
    req = AMP_SET_MODE_CALIBRATE_REQUEST;

  }
  else {

    cmd.value = AMP_SET_MODE_NORMAL_COMMON_VALUE;
    req = AMP_SET_MODE_NORMAL_REQUEST;

  }

  cmd.request = AMP_SET_MODE_COMMON_REQUEST;
  cmd.length = 0;
  cmd.data = NULL;
  cmd.type = CX_OUT;

  amp.sendControlTransfer(cmd);


  //...then the mode specific request

  cmd.request = req;
  cmd.value = 0;

  amp.sendControlTransfer(cmd);

  



}

//-----------------------------------------------------------------------------

void AmpLink::setActiveChannels(char* channels,unsigned int count) {


  assert(count>0 && count<16);

  cmd.request = AMP_SET_CHANNELS_REQUEST;
  cmd.value = 0;
  cmd.length = count;
  cmd.data = (void*)channels;
  cmd.type = CX_OUT;

  amp.sendControlTransfer(cmd);


}

//-----------------------------------------------------------------------------

void AmpLink::setTriggerLineEnabled(bool isEnabled) {


  cmd.request = AMP_SET_TRIGGER_LINE_ENABLED_REQUEST;
  cmd.value = 0;
  cmd.length = 0;
  cmd.data = NULL;
  cmd.type = CX_OUT;

  amp.sendControlTransfer(cmd);
  


}
