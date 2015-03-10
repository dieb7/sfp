// PID declarations  generated by parsepids.py  Feb 21, 2015  22:14:05

#ifndef PID_H
#define PID_H

#define ACK_BIT 0x80 	// used for tagging acked frames. High bit is set for acked frames. next high bit is sfs 1 or 2
#define SPS_BIT 0x40 	// used for indicating sfs type if an acked frame
#define PID_BITS (0xFF&(~(ACK_BIT|SPS_BIT))) 	// used to mask off upper bits
#define TEST_FRAME 0x00 	// test frame for throughput measurements
#define WHO_PIDS (TEST_FRAME) 	// all pids greater than, use the who header for routing
#define PING 0x1 	// to check other end
#define PING_BACK 0x2 	// expected response to a PING
#define SPS 0x3 	// used for initializing SPS frame acks and setting id
#define SPS_ACK 0x4 	// confirm sps; must contain sender - who.from
#define GET_VERSION 0x5 	// get the version number
#define VERSION_NO 0x6 	// return the version number
#define TALK_IN 0x7 	// keyboard input to be interpreted
#define TALK_OUT 0x8 	// used to send emits out usb port
#define EVAL 0x9 	// evaluate text with destination; who, text
#define CALL_CODE 0xa 	// call code starting at this location; who, addr32
#define MEM_READ 0xb 	// read memory request; who, addr32, len8
#define MEM_DATA 0xc 	// memory contents; who, addr32, len8
#define CHECK_MEM 0xd 	// check memory contents; who, addr32, len32, method8
#define MEM_CHECK 0xe 	// value returned from checking memory contents; who, addr32, len32, method8, code
#define FILL_MEM 0xf 	// fill memory chunks with pattern of length bytes; who, addr32, len32, len8, pattern
#define RAM_WRITE 0x10 	// request write to RAM; who, 32bit addr, 8bit length, bytes
#define FLASH_WRITE 0x11 	// request write to flash; who, 32bit addr, 8bit length, bytes
#define WRITE_CONF 0x12 	// confirm a write to memory; who, addr32, len8
#define ERASE_MEM 0x13 	// erase memory range froms start to end
#define ERASE_CONF 0x14 	// confirm memory erased (0) or not (!0)
#define MAX_PIDS 0x15 	// number of PIDS defined

#endif
