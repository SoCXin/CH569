/********************************** (C) COPYRIGHT *******************************
* File Name          : usb30_desc.c
* Author             : WCH
* Version            : V1.0
* Date               : 2020/07/31
* Description 		 :
*******************************************************************************/
#include "usb30_desc.h"


const UINT8 DeviceDescriptor[] =
{
	0x12, 	// bLength
	0x01, 	// DEVICE descriptor type
	0x00, 	// 3.00
	0x03,
	0xff, 	// device class
	0x80, 	// device sub-class
	0x55, 	// vendor specific protocol
	0x09, 	// max packet size 512B
	0x86, 	// vendor id-0x1A86(qinheng)
	0x1A,
	0x37, 	// product id 0x8080
	0x55,
	0x00, 	//bcdDevice 0x0011
	0x01,
	0x01, 	// manufacturer index string
	0x02, 	// product index string
	0x03, 	// serial number index string
	0x01 	// number of configurations
};

const UINT8 ConfigDescriptor[] =
{
	0x09,   // length of this descriptor
	0x02,   // CONFIGURATION (2)
	0xc8,   // total length includes endpoint descriptors (should be 1 more than last address)
	0x00,   // total length high byte
	0x01,   // number of interfaces
	0x01,	// configuration value for this one
	0x00,   // configuration - string is here, 0 means no string
	0x80,	// attributes - bus powered, no wakeup
	0x64,	// max power - 800 ma is 100 (64 hex)

//interface_descriptor
	0x09,	// length of the interface descriptor
	0x04,	// INTERFACE (4)
	0x00,	// Zero based index 0f this interface
	0x00,	// Alternate setting value (?)
	0x0e,	// Number of endpoints (not counting 0)
	0xFF,	// Interface class, ff is vendor specific
	0x80,	// Interface sub-class
	0x55,	// Interface protocol
	0x00, 	// Index to string descriptor for this interface

//Endpoint 1 Descriptor
	0x07,	// length of this endpoint descriptor
	0x05,	// ENDPOINT (5)
	0x81,	// endpoint direction (80 is in) and address
	0x03,	// transfer type - 00 = control, 01 = iso, 10 = bulk, 11 = int
	0x00,	// max packet size - 1024 bytes
	0x04,	// max packet size - high
	0x00, 	// polling interval in milliseconds (1 for iso)

//endp1_compansion_desc
	0x06,	// length of this endpoint compansion descriptor
	0x30,
	0x01, 	// max burst size
	0x00, 	// no stream
	0x00,
	0x00,

//endp2_descriptor
	0x07, 	// length of this endpoint descriptor
	0x05, 	// ENDPOINT (5)
	0x01, 	// endpoint direction (00 is out) and address
	0x02,	// transfer type - 00 = control, 01 = iso, 10 = bulk, 11 = int
	0x00,	// max packet size - 1024 bytes
	0x04,   // max packet size - high
	0x00,    // polling interval in milliseconds (1 for iso)

//endp2_compansion_desc
	0x06,	// length of this endpoint compansion descriptor
	0x30,
	0x01, 	// max burst size
	0x00, 	// no stream
	0x00,
	0x00,

	0x07,	// length of this endpoint descriptor
	0x05,	// ENDPOINT (5)
	0x82,	// endpoint direction (80 is in) and address
	0x02,	// transfer type - 00 = control, 01 = iso, 10 = bulk, 11 = int
	0x00,	// max packet size - 1024 bytes
	0x04,	// max packet size - high
	0x00, 	// polling interval in milliseconds (1 for iso)

//endp1_compansion_desc

	0x06,	// length of this endpoint compansion descriptor
	0x30,
	0x01, 	// max burst size
	0x00, 	// no stream
	0x00,
	0x00,

//endp2_descriptor

	0x07, 	// length of this endpoint descriptor
	0x05, 	// ENDPOINT (5)
	0x02, 	// endpoint direction (00 is out) and address
	0x02,	// transfer type - 00 = control, 01 = iso, 10 = bulk, 11 = int
	0x00,	// max packet size - 1024 bytes
	0x04,   // max packet size - high
	0x00,    // polling interval in milliseconds (1 for iso)

//endp2_compansion_desc
	0x06,	// length of this endpoint compansion descriptor
	0x30,
	0x01, 	// max burst size
	0x00, 	// no stream
	0x00,
	0x00,

	0x07,	// length of this endpoint descriptor
	0x05,	// ENDPOINT (5)
	0x83,	// endpoint direction (80 is in) and address
	0x02,	// transfer type - 00 = control, 01 = iso, 10 = bulk, 11 = int
	0x00,	// max packet size - 1024 bytes
	0x04,	// max packet size - high
	0x00, 	// polling interval in milliseconds (1 for iso)

//endp1_compansion_desc
	0x06,	// length of this endpoint compansion descriptor
	0x30,
	0x01, 	// max burst size
	0x00, 	// no stream
	0x00,
	0x00,

//endp2_descriptor
	0x07, 	// length of this endpoint descriptor
	0x05, 	// ENDPOINT (5)
	0x03, 	// endpoint direction (00 is out) and address
	0x02,	// transfer type - 00 = control, 01 = iso, 10 = bulk, 11 = int
	0x00,	// max packet size - 1024 bytes
	0x04,   // max packet size - high
	0x00,    // polling interval in milliseconds (1 for iso)

//endp2_compansion_desc
	0x06,	// length of this endpoint compansion descriptor
	0x30,
	0x01, 	// max burst size
	0x00, 	// no stream
	0x00,
	0x00,

	0x07,	// length of this endpoint descriptor
	0x05,	// ENDPOINT (5)
	0x84,	// endpoint direction (80 is in) and address
	0x02,	// transfer type - 00 = control, 01 = iso, 10 = bulk, 11 = int
	0x00,	// max packet size - 1024 bytes
	0x04,	// max packet size - high
	0x00, 	// polling interval in milliseconds (1 for iso)

//endp1_compansion_desc
	0x06,	// length of this endpoint compansion descriptor
	0x30,
	0x01, 	// max burst size
	0x00, 	// no stream
	0x00,
	0x00,

//UINT8 endp2_descriptor
	0x07, 	// length of this endpoint descriptor
	0x05, 	// ENDPOINT (5)
	0x04, 	// endpoint direction (00 is out) and address
	0x02,	// transfer type - 00 = control, 01 = iso, 10 = bulk, 11 = int
	0x00,	// max packet size - 1024 bytes
	0x04,   // max packet size - high
	0x00,    // polling interval in milliseconds (1 for iso)

//endp2_compansion_desc
	0x06,	// length of this endpoint compansion descriptor
	0x30,
	0x01, 	// max burst size
	0x00, 	// no stream
	0x00,
	0x00,

	0x07,	// length of this endpoint descriptor
	0x05,	// ENDPOINT (5)
	0x85,	// endpoint direction (80 is in) and address
	0x02,	// transfer type - 00 = control, 01 = iso, 10 = bulk, 11 = int
	0x00,	// max packet size - 1024 bytes
	0x04,	// max packet size - high
	0x00, 	// polling interval in milliseconds (1 for iso)

//endp1_compansion_desc
	0x06,	// length of this endpoint compansion descriptor
	0x30,
	0x01, 	// max burst size
	0x00, 	// no stream
	0x00,
	0x00,

//endp2_descriptor
	0x07, 	// length of this endpoint descriptor
	0x05, 	// ENDPOINT (5)
	0x05, 	// endpoint direction (00 is out) and address
	0x02,	// transfer type - 00 = control, 01 = iso, 10 = bulk, 11 = int
	0x00,	// max packet size - 1024 bytes
	0x04,   // max packet size - high
	0x00,    // polling interval in milliseconds (1 for iso)

//endp2_compansion_desc
	0x06,	// length of this endpoint compansion descriptor
	0x30,
	0x01, 	// max burst size
	0x00, 	// no stream
	0x00,
	0x00,

	0x07,	// length of this endpoint descriptor
	0x05,	// ENDPOINT (5)
	0x86,	// endpoint direction (80 is in) and address
	0x02,	// transfer type - 00 = control, 01 = iso, 10 = bulk, 11 = int
	0x00,	// max packet size - 1024 bytes
	0x04,	// max packet size - high
	0x00, 	// polling interval in milliseconds (1 for iso)

//endp1_compansion_desc
	0x06,	// length of this endpoint compansion descriptor
	0x30,
	0x01, 	// max burst size
	0x00, 	// no stream
	0x00,
	0x00,

//endp2_descriptor
	0x07, 	// length of this endpoint descriptor
	0x05, 	// ENDPOINT (5)
	0x06, 	// endpoint direction (00 is out) and address
	0x02,	// transfer type - 00 = control, 01 = iso, 10 = bulk, 11 = int
	0x00,	// max packet size - 1024 bytes
	0x04,   // max packet size - high
	0x00,    // polling interval in milliseconds (1 for iso)

//endp2_compansion_desc
	0x06,	// length of this endpoint compansion descriptor
	0x30,
	0x01, 	// max burst size
	0x00, 	// no stream
	0x00,
	0x00,

	0x07,	// length of this endpoint descriptor
	0x05,	// ENDPOINT (5)
	0x87,	// endpoint direction (80 is in) and address
	0x02,	// transfer type - 00 = control, 01 = iso, 10 = bulk, 11 = int
	0x00,	// max packet size - 1024 bytes
	0x04,	// max packet size - high
	0x00, 	// polling interval in milliseconds (1 for iso)

//endp1_compansion_desc
	0x06,	// length of this endpoint compansion descriptor
	0x30,
	0x01, 	// max burst size
	0x00, 	// no stream
	0x00,
	0x00,

//endp2_descriptor
	0x07, 	// length of this endpoint descriptor
	0x05, 	// ENDPOINT (5)
	0x07, 	// endpoint direction (00 is out) and address
	0x02,	// transfer type - 00 = control, 01 = iso, 10 = bulk, 11 = int
	0x00,	// max packet size - 1024 bytes
	0x04,   // max packet size - high
	0x00,    // polling interval in milliseconds (1 for iso)

//endp2_compansion_desc
	0x06,	// length of this endpoint compansion descriptor
	0x30,
	0x01, 	// max burst size
	0x00, 	// no stream
	0x00,
	0x00
};

// ===== String Descriptor Lang ID=====
// ====================================//

const UINT8 StringLangID[] =
{
	0x04,	// this descriptor length
	0x03,	// descriptor type
	0x09,	// Language ID 0 low byte
	0x04	// Language ID 0 high byte
};


// ====================================
// =====   String Descriptor 1    =====
// ====================================
const UINT8 StringVendor[] =
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
const UINT8 StringProduct[]=
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
    0x33, 0x00, //3
    0x2e, 0x00, //.
    0x30, 0x00, //0
    0x20, 0x00, //
    0x44, 0x00, //D
    0x45, 0x00, //E
    0x56, 0x00, //V
    0x49, 0x00, //I
    0x43, 0x00, //C
    0x45, 0x00, //E
    0x20, 0x00
};

// ====================================
// =====   String Descriptor 3    =====
// ====================================
UINT8 StringSerial[] =
{
    0x16,	// length of this descriptor
	0x03,
	'0',
	0x00,
	'1',
	0x00,
	'2',
	0x00,
	'3',
	0x00,
	'4',
	0x00,
	'5',
	0x00,
	'6',
	0x00,
	'7',
	0x00,
	'8',
	0x00,
	'9',
	0x00,
};

const UINT8 OSStringDescriptor[] =
{
    0x12,	// length of this descriptor
	0x03,
	'M',
	0x00,
	'S',
	0x00,
	'F',
	0x00,
	'T',
	0x00,
	'1',
	0x00,
	'0',
	0x00,
	'0',
	0x00,
	0x01,
	0x00
};

const UINT8 BOSDescriptor[] =
{
	0x05,   // length of this descriptor
	0x0f,   // CONFIGURATION (2)
	0x16,   // total length includes endpoint descriptors (should be 1 more than last address)
	0x00,   // total length high byte
	0x02,   // number of device cap

//dev_cap_descriptor1
	0x07,
	0x10,	// DEVICE CAPABILITY type
	0x02,	// USB2.0 EXTENSION
	0x06,
	0x00,
	0x00,
	0x00,

//dev_cap_descriptor2
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

const UINT8 MSOS20DescriptorSet[] =
{
    //
    // Microsoft OS 2.0 Descriptor Set Header
    //
    0x0A, 0x00,             // wLength - 10 bytes
    0x00, 0x00,             // MSOS20_SET_HEADER_DESCRIPTOR
	0x00, 0x00, 0x03, 0x06, // dwWindowsVersion 每 0x06030000 for Windows Blue
    0x48, 0x00,             // wTotalLength 每 72 bytes
    //
    // Microsoft OS 2.0 Registry Value Feature Descriptor
    //
    0x3E, 0x00,             // wLength - 62 bytes
    0x04, 0x00,             // wDescriptorType 每 4 for Registry Property
    0x04, 0x00,             // wPropertyDataType - 4 for REG_DWORD
    0x30, 0x00,              // wPropertyNameLength 每 48 bytes
    0x53, 0x00, 0x65, 0x00, // Property Name - ※SelectiveSuspendEnabled§
    0x6C, 0x00, 0x65, 0x00,
    0x63, 0x00, 0x74, 0x00,
    0x69, 0x00, 0x76, 0x00,
    0x65, 0x00, 0x53, 0x00,
    0x75, 0x00, 0x73, 0x00,
    0x70, 0x00, 0x65, 0x00,
    0x6E, 0x00, 0x64, 0x00,
    0x45, 0x00, 0x6E, 0x00,
    0x61, 0x00, 0x62, 0x00,
    0x6C, 0x00, 0x65, 0x00,
    0x64, 0x00, 0x00, 0x00,
    0x04, 0x00,             // wPropertyDataLength 每 4 bytes
    0x01, 0x00, 0x00, 0x00  // PropertyData - 0x00000001
};

const UINT8 PropertyHeader[] =
{
	0x8e, 0x00, 0x00, 0x00, 0x00, 01, 05, 00, 01, 00,
	0x84, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x00,
	0x28, 0x00,
	0x44, 0x00, 0x65, 0x00, 0x76, 0x00, 0x69, 0x00, 0x63, 0x00, 0x65, 0x00, 0x49, 0x00, 0x6e,
	0x00, 0x74, 0x00, 0x65, 0x00, 0x72, 0x00, 0x66, 0x00, 0x61, 0x00, 0x63, 0x00, 0x65, 0x00, 0x47, 0x00, 0x55, 0x00, 0x49, 0x00, 0x44, 0x00, 0x00, 0x00,

	0x4e, 0x00, 0x00, 0x00, //L"{12345678-1234-1234-1234-123456789ABC"
	0x7b, 0x00, 0x31, 0x00, 0x32, 0x00, 0x33, 0x00, 0x34, 0x00, 0x35, 0x00, 0x36, 0x00, 0x37, 0x00, 0x38, 0x00,
	0x2d, 0x00, 0x31, 0x00, 0x32, 0x00, 0x33, 0x00, 0x34, 0x00,
	0x2d ,0x00, 0x31, 0x00, 0x32, 0x00, 0x33, 0x00, 0x34, 0x00,
	0x2d, 0x00, 0x31, 0x00, 0x32, 0x00, 0x33, 0x00, 0x34, 0x00, 0x2d, 0x00, 0x31, 0x00, 0x32, 0x00, 0x33, 0x00,
	0x34, 0x00, 0x35, 0x00, 0x36, 0x00, 0x37, 0x00, 0x38, 0x00, 0x39, 0x00, 0x41, 0x00, 0x42, 0x00, 0x43, 0x00,
	0x7d, 0x00, 0x00, 0x00
};

const UINT8 CompactId[] =
{
	0x28, 0x00, 0x00, 0x00, 0x00, 0x01, 0x04, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
	0x57, 0x49, 0x4e, 0x55, 0x53, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

UINT8 GetStatus[] =
{
	0x01, 0x00
};

