
#include "CH56x_common.h"
const UINT8 hs_device_descriptor[] =
{
	0x12, 	// bLength
	0x01, 	// DEVICE descriptor type
	0x10, 	// 2.00
	0x02,
	0x00, 	// device class
	0x00, 	// device sub-class
	0x00, 	// vendor specific protocol
	0x40, 	// max packet size 512B
	0x86, 	// vendor id-0x1A86(qinheng)
	0x1A,
	0x37, 	// product id 0x5537
	0x55,
	0x00, 	//bcdDevice 0x0111
	0x11,
	0x01, 	// manufacturer index string
	0x02, 	// product index string
	0x00, 	// serial number index string
	0x01 	// number of configurations
};

const UINT8 hs_config_descriptor[] =
{
	0x09,   // length of this descriptor                                                        
	0x02,   // CONFIGURATION (2)                                                                
	0x74,   // total length includes endpoint descriptors (should be 1 more than last address)
	0x00,   // total length high byte                                                           
	0x01,   // number of interfaces                                                             
	0x01,	// configuration value for this one                                                   
	0x00,   // configuration - string is here, 0 means no string                                
	0x80,	// attributes - bus powered, no wakeup                                                
	0x64,	// max power - 800 ma is 100 (64 hex)

	0x09,	// length of the interface descriptor
	0x04,	// INTERFACE (4)
	0x00,	// Zero based index 0f this interface
	0x00,	// Alternate setting value (?)
	0x0e,	// Number of endpoints (not counting 0)
	0xFF,	// Interface class, ff is vendor specific
	0xFF,	// Interface sub-class
	0xFF,	// Interface protocol
	0x00, 	// Index to string descriptor for this interface

	0x07,	// length of this endpoint descriptor
	0x05,	// ENDPOINT (5)
	0x81,	// endpoint direction (80 is in) and address
	0x02,	// transfer type - 00 = control, 01 = iso, 10 = bulk, 11 = int
	0x00,	// max packet size - 512 bytes
	0x02,	// max packet size - high
	0x00, 	// polling interval in milliseconds (1 for iso)

	0x07, 	// length of this endpoint descriptor
	0x05, 	// ENDPOINT (5)
	0x01, 	// endpoint direction (00 is out) and address
	0x02,	// transfer type - 00 = control, 01 = iso, 10 = bulk, 11 = int
	0x00,	// max packet size - 512 bytes
	0x02,   // max packet size - high
	0x00,    // polling interval in milliseconds (1 for iso)

	0x07,	// length of this endpoint descriptor
	0x05,	// ENDPOINT (5)
	0x82,	// endpoint direction (80 is in) and address
	0x02,	// transfer type - 00 = control, 01 = iso, 10 = bulk, 11 = int
	0x00,	// max packet size - 512 bytes
	0x02,	// max packet size - high
	0x00, 	// polling interval in milliseconds (1 for iso)

	0x07,	// length of this endpoint descriptor
	0x05,	// ENDPOINT (5)
	0x02,	// endpoint direction (80 is in) and address
	0x02,	// transfer type - 00 = control, 01 = iso, 10 = bulk, 11 = int
	0x00,	// max packet size - 512 bytes
	0x02,	// max packet size - high
	0x00, 	// polling interval in milliseconds (1 for iso)

	0x07,	// length of this endpoint descriptor
	0x05,	// ENDPOINT (5)
	0x83,	// endpoint direction (80 is in) and address
	0x02,	// transfer type - 00 = control, 01 = iso, 10 = bulk, 11 = int
	0x00,	// max packet size - 512 bytes
	0x02,	// max packet size - high
	0x00, 	// polling interval in milliseconds (1 for iso)

	0x07,	// length of this endpoint descriptor
	0x05,	// ENDPOINT (5)
	0x03,	// endpoint direction (80 is in) and address
	0x02,	// transfer type - 00 = control, 01 = iso, 10 = bulk, 11 = int
	0x00,	// max packet size - 512 bytes
	0x02,	// max packet size - high
	0x00, 	// polling interval in milliseconds (1 for iso)

	0x07,	// length of this endpoint descriptor
	0x05,	// ENDPOINT (5)
	0x84,	// endpoint direction (80 is in) and address
	0x02,	// transfer type - 00 = control, 01 = iso, 10 = bulk, 11 = int
	0x00,	// max packet size - 512 bytes
	0x02,	// max packet size - high
	0x00, 	// polling interval in milliseconds (1 for iso)

	0x07,	// length of this endpoint descriptor
	0x05,	// ENDPOINT (5)
	0x04,	// endpoint direction (80 is in) and address
	0x02,	// transfer type - 00 = control, 01 = iso, 10 = bulk, 11 = int
	0x00,	// max packet size - 512 bytes
	0x02,	// max packet size - high
	0x00, 	// polling interval in milliseconds (1 for iso)

	0x07,	// length of this endpoint descriptor
	0x05,	// ENDPOINT (5)
	0x85,	// endpoint direction (80 is in) and address
	0x02,	// transfer type - 00 = control, 01 = iso, 10 = bulk, 11 = int
	0x00,	// max packet size - 512 bytes
	0x02,	// max packet size - high
	0x00, 	// polling interval in milliseconds (1 for iso)

	0x07,	// length of this endpoint descriptor
	0x05,	// ENDPOINT (5)
	0x05,	// endpoint direction (80 is in) and address
	0x02,	// transfer type - 00 = control, 01 = iso, 10 = bulk, 11 = int
	0x00,	// max packet size - 512 bytes
	0x02,	// max packet size - high
	0x00, 	// polling interval in milliseconds (1 for iso)

	0x07,	// length of this endpoint descriptor
	0x05,	// ENDPOINT (5)
	0x86,	// endpoint direction (80 is in) and address
	0x02,	// transfer type - 00 = control, 01 = iso, 10 = bulk, 11 = int
	0x00,	// max packet size - 512 bytes
	0x02,	// max packet size - high
	0x00, 	// polling interval in milliseconds (1 for iso)

	0x07,	// length of this endpoint descriptor
	0x05,	// ENDPOINT (5)
	0x06,	// endpoint direction (80 is in) and address
	0x02,	// transfer type - 00 = control, 01 = iso, 10 = bulk, 11 = int
	0x00,	// max packet size - 512 bytes
	0x02,	// max packet size - high
	0x00, 	// polling interval in milliseconds (1 for iso)

	0x07,	// length of this endpoint descriptor
	0x05,	// ENDPOINT (5)
	0x87,	// endpoint direction (80 is in) and address
	0x02,	// transfer type - 00 = control, 01 = iso, 10 = bulk, 11 = int
	0x00,	// max packet size - 512 bytes
	0x02,	// max packet size - high
	0x00, 	// polling interval in milliseconds (1 for iso)

	0x07,	// length of this endpoint descriptor
	0x05,	// ENDPOINT (5)
	0x07,	// endpoint direction (80 is in) and address
	0x02,	// transfer type - 00 = control, 01 = iso, 10 = bulk, 11 = int
	0x00,	// max packet size - 512 bytes
	0x02,	// max packet size - high
	0x00 	// polling interval in milliseconds (1 for iso)
};
// ====================================
// ===== String Descriptor Lang ID=====
// ====================================
const UINT8 hs_string_descriptor0[] =
{
	0x04,	// this descriptor length
	0x03,	// descriptor type
	0x09,	// Language ID 0 low byte
	0x04	// Language ID 0 high byte
};


// ====================================
// =====   String Descriptor 1    =====
// ====================================
const UINT8 hs_string_descriptor1[] =
{		
    0x08,	// length of this descriptor 
	0x03,
	'W',  
	0x00,
	'C',  
	0x00,
	'H',  
	0x00
};	      
         
// ====================================
// =====   String Descriptor 2    =====
// ====================================
const UINT8 hs_string_descriptor2[]=
{
    38,         //该描述符的长度为38字节
    0x03,       //字符串描述符的类型编码为0x03
    0x57, 0x00, //W
    0x43, 0x00, //C
    0x48, 0x00, //H
    0x20, 0x00, //
    0x55, 0x00, //U
    0x53, 0x00, //S
    0x42, 0x00, //B
    0x32, 0x00, //2
    0x2e, 0x00, //.
    0x30, 0x00, //0
    0x20, 0x00, //
    0x44, 0x00, //D
    0x45, 0x00, //E
    0x56, 0x00, //V
    0x49, 0x00, //I
    0x43, 0x00, //C
    0x45, 0x00, //E
    0x20, 0x00  //
};
  
const UINT8 hs_bos_descriptor[] =
{
	0x05,   // length of this descriptor                                                        
	0x0f,   // CONFIGURATION (2)                                                                
	0x16,   // total length includes endpoint descriptors (should be 1 more than last address)
	0x00,   // total length high byte                                                           
	0x02,   	// number of device cap

	0x07,
	0x10,	// DEVICE CAPABILITY type
	0x02,	// USB2.0 EXTENSION
	0x02,
	0x00,
	0x00,
	0x00,

	0x0a,	// length of this descriptor 
	0x10,	// DEVICE CAPABILITY type
	0x03,	// superspeed usb device capability
	0x00,	// 
	0x0e,	// ss/hs/fs
	0x00,
	0x01,	// the lowest speed is full speed
	0x0a,	// u1 exit latency is 10us
	0xff,	// u1 exit latency is 8us
	0x07
};



