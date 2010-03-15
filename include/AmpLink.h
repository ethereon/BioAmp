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

#ifndef __AMP_LINK_H__
#define __AMP_LINK_H__


#include "UsbAccess.h"
#include "AmpDefs.h"
#include "AmpFilters.h"
#include <string>
#include <vector>

class AmpLink {
	
private:
	
	xinverse::UsbAccess amp;
	xinverse::ControlTransfer cmd;
	xinverse::BulkTransfer bx;
	
	int verbosityLevel;
	
	bool isConnected;
	
public:
	
	AmpLink();
	~AmpLink();
	
	//Returns a list of detected g.USBAmps
	static void getAvailableAmps(std::vector<std::string>* devices, bool verbose=false);
	
	//Returns a list of available Bandpass Filters ( offline method )
	static const FilterSpec* getBandpassFilters();
	
	//Returns a list of available Notch Filters ( offline method )
	static const FilterSpec* getNotchFilters();
	
	//Connect to an amplifier. The index parameter corresponds to the index of the desired
	//amplifier in the vector returned by the getAvailableAmps method.
	bool connect(int index=0);
	
	//Disconnect from the amplifier
	void disconnect();
	
	//Set the sampling rate
	void setSamplingRate(unsigned int argRate);
	
	//Set the buffer size
	//The buffer size is equal to the number of samples per getData call per channel
	void setBufferSize(unsigned int argSize);
	
	//Enter acquisition mode
	//Must be called before getData
	void start();
	
	//End acquisition mode
	void stop();
	
	//Acquire data and store into buffer
	void getData(void* buffer, unsigned int bufferLength);
	
	//Calculates the impedance ( in Ohms) based on the measured amplitude
	//and the calibration signal amplitude. ( see manual for more details)
	float calculateImpedance(float measuredAmplitude, float calibrationAmplitude=1E4); 
	
	//Get the serial of the amplifier
	std::string getSerial();
	
	//Set the amplifier mode ( see manual for more details)
	void setMode(AmpMode mode);
	
	//Set the channels from which data will be acquired
	//channels is an character array containing the channel numbers (one-based).
	//count is the number of elements in the array.
	void setActiveChannels(char* channels,unsigned int count);
	
	//Enable/Disable the digital input
	void setTriggerLineEnabled(bool isEnabled);
	
	//Set the Bandpass filter. Use filterIndex=-1 to disable.
	void setBandpass(int filterIndex, int channel);
	
	//Set the Notch filter. Use filterIndex=-1 to disable.
	void setNotch(int filterIndex, int channel);
	
	//Verbosity Level. Use 0 for silent, 3 to debug mode.
	//Correspond directly to libusb verbosity levels.
	void setVerbosityLevel(int level);
	
};

#endif
