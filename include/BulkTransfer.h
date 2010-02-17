#ifndef __BULK_TRANSFER_H__
#define __BULK_TRANSFER_H__

namespace xinverse {

class BulkTransfer {

 public:

  
  BulkTransfer() { timeout = 0; transferred = 0;}
  ~BulkTransfer() {}

  unsigned char endpoint;
  void* data;
  int length;
  int transferred;
  unsigned int timeout;
  


};

}

#endif
