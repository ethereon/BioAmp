#ifndef __AMP_LINK_H__
#define __AMP_LINK_H__


#include "UsbAccess.h"
#include "AmpDefs.h"
#include <string>

class AmpLink {

 private:

  xinverse::UsbAccess amp;
  xinverse::ControlTransfer cmd;
  xinverse::BulkTransfer bx;

 public:

  AmpLink();
  ~AmpLink();

  bool connect();
  void disconnect();

  void setSamplingRate(unsigned int argRate);
  void setBufferSize(unsigned int argSize);
  
  
  void start();
  void stop();
  void getData(void* buffer, unsigned int bufferLength);

  std::string getAmpSerial();

  double getImpedance(unsigned int channel);
  //AmpMode getAmpMode() const;


  void setMode(AmpMode mode);
  void setActiveChannels(char* channels,unsigned int count);
  void setTriggerLineEnabled(bool isEnabled);

};

#endif
