/*
 ================================================================
 
 Amplifier Protocol
 The Myelin Project
 
 Reverse engineered g.UsbAmp protocol.
 The primary device used during reverse engineering was a
 UA-2008 model. The protocol (to the extent described below)
 appears to be identical in the UB-2009 model.
 
 Copyright (C) 2010 Saumitro Dasgupta.
 This code is made available under the MIT License.
 <http://www.opensource.org/licenses/mit-license.html>
 
 ================================================================
 */

#ifndef __AMP_PROTOCOL_H__
#define __AMP_PROTOCOL_H__


#define AMP_GET_SERIAL_REQUEST 0xd2
#define AMP_GET_SERIAL_LENGTH 0x100

#define AMP_SET_MODE_COMMON_REQUEST 0xc2

#define AMP_SET_MODE_NORMAL_REQUEST 0xc0
#define AMP_SET_MODE_NORMAL_COMMON_VALUE 0x1

#define AMP_SET_MODE_CALIBRATE_REQUEST 0xc1
#define AMP_SET_MODE_CALIBRATE_COMMON_VALUE 0x2

#define AMP_SET_MODE_IMPEDANCE_REQUEST 0xc9
#define AMP_SET_MODE_IMPEDANCE_COMMON_VALUE 0x3

#define AMP_START_REQUEST 0xf7

#define AMP_STOP_REQUEST 0xb8

//The  value is a boolean flag, 0 or 1
#define AMP_SET_TRIGGER_LINE_ENABLED_REQUEST 0xd3

//The  value is a boolean flag, 0 or 1
#define AMP_ENABLE_SC_REQUEST 0xca

//The  value is the buffer size
#define AMP_SET_BUFFER_SIZE_REQUEST 0xb5

#define AMP_SET_SAMPLING_RATE_REQUEST 0xb6

//Data = a byte buffer containing active channels
//1 byte per channel index, 1 based.
#define AMP_SET_CHANNELS_REQUEST 0xb9

#define AMP_SET_SLAVE_REQUEST 0xcd //Investigate this

//data = 1 bit boolean flag PER CHANNEL
#define AMP_SET_BIPOLAR_REQUEST 0xc8

#define AMP_GET_MODE_REQUEST 0xcf
#define AMP_GET_MODE_LENGTH 0x1


#endif
