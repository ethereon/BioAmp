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
#include "BandpassFilters.h"
#include "NotchFilters.h"
#include <sstream>
#include <assert.h>

using namespace std;
using namespace xinverse;


#define CX_IN  LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_ENDPOINT_IN

#define CX_OUT LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_ENDPOINT_OUT

#define GUSBAMP_ID "g.USBamp"


//-----------------------------------------------------------------------------

AmpLink::AmpLink() {


  isConnected = false;

  this->amp.beginSession();

}

//-----------------------------------------------------------------------------

AmpLink::~AmpLink() {

  if(isConnected)
    disconnect();

  amp.endSession();
  
}

//-----------------------------------------------------------------------------

bool AmpLink::connect(int index) {

  if(!this->amp.openDeviceWithDescription(GUSBAMP_ID,index))

    return false;

  isConnected = true;
  return true;

}

//-----------------------------------------------------------------------------

void AmpLink::disconnect() {

  assert(isConnected);

  amp.closeDevice();
  isConnected = false;

}

//-----------------------------------------------------------------------------


void AmpLink::getAvailableAmps(vector<string>* devices, bool verbose) {

  AmpLink ampLink;

  if(verbose)
    ampLink.setVerbosityLevel(3);
  else
    ampLink.setVerbosityLevel(0);

  bool success = true;

  for(int i=0;success;++i) {

    success = ampLink.connect(i);

    if(success) {

      string ampSerial = ampLink.getSerial();
      devices->push_back(ampSerial);
      ampLink.disconnect();

    }

  }

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

string AmpLink::getSerial() {

  char buffer[AMP_GET_SERIAL_LENGTH];

  cmd.request = AMP_GET_SERIAL_REQUEST;
  cmd.value = 0;
  cmd.length = AMP_GET_SERIAL_LENGTH;
  cmd.data = buffer;
  cmd.type = CX_IN;

  amp.sendControlTransfer(cmd);

  string serialPacket = string((const char*)&buffer);

  string part, serial;
  istringstream iss(serialPacket, istringstream::in);

  iss >> part; //The first word should be a set of dashes. discard.
  iss >> part; //This should be the version
  iss >> serial; //This should be the actual serial number

  serial +=" ( " + part + " )";

  return serial;

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


  assert(count>0 && count<=16);

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
  cmd.value = isEnabled;
  cmd.length = 0;
  cmd.data = NULL;
  cmd.type = CX_OUT;

  amp.sendControlTransfer(cmd);

}

//-----------------------------------------------------------------------------

void AmpLink::setVerbosityLevel(int level) {

  amp.setVerbosityLevel(level);

}

//-----------------------------------------------------------------------------

const FilterSpec* AmpLink::getBandpassFilters() {

  return bandpassFilters;
  
}

//-----------------------------------------------------------------------------

const FilterSpec* AmpLink::getNotchFilters() {

  return notchFilters;

}

//-----------------------------------------------------------------------------
void AmpLink::setBandpass(int filterIndex, int channel) {

  char* filterData = (char*)disableBandpass;

  assert(filterIndex<BANDPASS_FILTER_COUNT);

  if(filterIndex!=-1)
    filterData = (char*)bandpassFilters[filterIndex].details;

  cmd.request = AMP_SET_BANDPASS_REQUEST;
  cmd.value = channel;
  cmd.length = AMP_SET_BANDPASS_LENGTH;
  cmd.data = filterData;
  cmd.type = CX_OUT;

  amp.sendControlTransfer(cmd);


}

//-----------------------------------------------------------------------------
void AmpLink::setNotch(int filterIndex, int channel) {

  char* filterData = (char*)disableNotch;

  assert(filterIndex<NOTCH_FILTER_COUNT);

  if(filterIndex!=-1)
    filterData = (char*)notchFilters[filterIndex].details;

  cmd.request = AMP_SET_NOTCH_REQUEST;
  cmd.value = channel;
  cmd.length = AMP_SET_NOTCH_LENGTH;
  cmd.data = filterData;
  cmd.type = CX_OUT;

  amp.sendControlTransfer(cmd);

}


//-----------------------------------------------------------------------------
