#ifndef __CONTROL_TRANSFER_H__
#define __CONTROL_TRANSFER_H__

#include <libusb.h>

namespace xinverse {

class ControlTransfer {

 public:

  ControlTransfer() { timeout = 0; }
  ~ControlTransfer() {}    

		  

  
  uint8_t type;
  uint8_t request;
  uint16_t value;
  void* data;
  uint16_t length;

  unsigned int timeout;

};

}

#endif
