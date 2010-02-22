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

  static void getAvailableAmps(std::vector<std::string>* devices, bool verbose=false);
  static const FilterSpec* getBandpassFilters();
  static const FilterSpec* getNotchFilters();

  bool connect(int index=0);
  void disconnect();

  void setSamplingRate(unsigned int argRate);
  void setBufferSize(unsigned int argSize);
  

  
  void start();
  void stop();
  void getData(void* buffer, unsigned int bufferLength);

  std::string getSerial();

  double getImpedance(unsigned int channel);


  void setMode(AmpMode mode);
  void setActiveChannels(char* channels,unsigned int count);
  void setTriggerLineEnabled(bool isEnabled);
  void setBandpass(int filterIndex, int channel);
  void setNotch(int filterIndex, int channel);

  void setVerbosityLevel(int level);

};

#endif
