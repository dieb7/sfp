// Default SFP Parameters  Robert Chapman III  Feb 21, 2015

// make a copy in application directory
// change parameters for application
// make sure application copy is first in the include list

#ifndef _PARAMETERS_H_
#define _PARAMETERS_H_

#define MAX_FRAME_LENGTH 254	// no more than 254; set according to link resources
#define MAX_FRAMES 10			// number of frames floating around system

// timeouts
#define SFP_POLL_TIME		(   2 TO_MSEC)		// polling in link down
#define SFP_RESEND_TIME		( 250 TO_MSECS)	// time between retransmissions
#define SFP_GIVEUP_TIME		(SFP_RESEND_TIME * 50)	// time for link to die
#define SPS_STARTUP_TIME	( 300 TO_MSECS)	// time to start sps
#define SFP_FRAME_TIME		(  50 TO_MSECS)	// maximum time to wait between bytes for a frame
#define SFP_FRAME_PROCESS	(1000 TO_MSECS)	// maximum time to wait for frame processing
#define STALE_RX_FRAME 		(1000 TO_MSECS)	// number of milliseconds to hang onto a received frame


#define NUM_LINKS 1				// number of links in this node

// Routing
enum {
	HOST = 0x0,
	ME = 0x0,
	YOU = 0x0,
	DIRECT = 0x0,
	ROUTING_POINTS,
};

#endif