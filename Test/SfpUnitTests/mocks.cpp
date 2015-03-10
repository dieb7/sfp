// Mocks

#include <qdebug.h>

extern "C" {
#include <stdio.h>
#include "printers.h"
#include "mocks.h"
#include "sfpRxSm.h"
#include "sfpTxSm.h"

void print(const char * message)
{
    printf("%s", message);
}

void printHex(unsigned int hex)
{
    printf("%8X ", hex);
}

void printnHex(unsigned int n, unsigned int hex)
{
    if (n)
        printf("%*X ", n, hex);
    else
        printf("%X ", hex);
}

void printDec(unsigned int dec)
{
    printf("%d ", dec);
}

#include "framepool.c"

bool framePoolFull()
{
    return queryq(poolq) == MAX_FRAMES;
}

#include "stats.c"
#include "stats.h"

#define GET_NODE_STAT(stat) Long get##stat() { sfpNode_t * node = getNode(); return node->stat; }
#define GET_LINK_STAT(stat) Long get##stat(sfpLink_t * link) { return link->stat; }

FOR_EACH_NODE_STAT(GET_NODE_STAT)
FOR_EACH_LINK_STAT(GET_LINK_STAT)

#include "services.c"

void rxFrame(sfpFrame * frame)
{
    pushq((Cell)frame, alink.frameq);
}

#include "machines.c"

#include "timeout.c"

#include "byteq.c"

static Long ticktime;
Long getTime()
{
    return ticktime;
}

void setTime(Long t)
{
    ticktime = t;
}

#include "node.c"

Byte packet[3];
sfpLink_t alink;
sfpNode_t anode;
Long acksin, acksout;
bool spsaccept;

QUEUE(MAX_FRAMES, frameinq);
QUEUE(MAX_FRAMES, npsq);
QUEUE(MAX_FRAMES, spsq);

void initLink()
{
    setTime(0);
    initSfpRxSM(&alink, frameinq);
    initSfpTxSM(&alink, npsq, spsq);
    initSfpStats();
    initFramePool();
    packet[0] = TEST_FRAME;    // pid
    packet[1] = DIRECT;        // to
    packet[2] = DIRECT;        // from
}

void initNode()
{
    setNode(&anode);
    addLink(DIRECT, &alink);
    initLink();
    setRouteTo(DIRECT, &alink);
    setWhoami(DIRECT);
    setWhatami(0);
    acksin = acksout = 0;
    spsaccept = true;
    for (int i = 0; i < MAX_PIDS; i++)
        setPacketHandler(i, NULL);
}

#include "sfpRxSm.c"
#include "sfpTxSm.c"

Long acksIn()
{
    sfpLink_t * link = &alink;

    if (testAckReceived(link)) {
        acksin++;
        clearAckReceived(link);
    }
    return acksin;
}

Long acksOut()
{
    sfpLink_t * link = &alink;

    if (testAckSend(link)) {
        acksout++;
        clearAckSend(link);
    }
    return acksout;
}

void breakPoint1(void)
{
    exitflag = false;
}

bool verbose = false;

void frameSay(sfpFrame * frame, const char * dir)
{
    Byte pid = frame->pid;
    Byte to = frame->who.to;
    Byte from = frame->who.from;
    Byte me = whoami();
    Long time = getTime();

    if (!verbose) return;

    switch(pid&PID_BITS) {
    case TEST_FRAME:     qDebug() << time << "Node" << me << "Frame" << dir << " - PID: TEST_FRAME " << to << "," << from; break;
    case PING:     qDebug() << time << "Node" << me << "Frame" << dir << "- PID: PING " << to << "," << from; break;
    case PING_BACK:     qDebug() << time << "Node" << me << "Frame" << dir << "- PID: PING_BACK " << to << "," << from; break;
    case SPS:     qDebug() << time << "Node" << me << "Frame" << dir << "- PID: SPS " << to << "," << from; break;
    case SPS_ACK:     qDebug() << time << "Node" << me << "Frame" << dir << "- PID: SPS_ACK " << to << "," << from; break;
    case GET_VERSION:     qDebug() << time << "Node" << me << "Frame" << dir << "- PID: GET_VERSION " << to << "," << from; break;
    case VERSION_NO:     qDebug() << time << "Node" << me << "Frame" << dir << "- PID: VERSION_NO " << to << "," << from; break;
    case TALK_IN:     qDebug() << time << "Node" << me << "Frame" << dir << "- PID: TALK_IN " << to << "," << from; break;
    case TALK_OUT:     qDebug() << time << "Node" << me << "Frame" << dir << "- PID: TALK_OUT " << to << "," << from; break;
    case EVAL:     qDebug() << time << "Node" << me << "Frame" << dir << "- PID: EVAL " << to << "," << from; break;
    case CALL_CODE:     qDebug() << time << "Node" << me << "Frame" << dir << "- PID: CALL_CODE " << to << "," << from; break;
    case MEM_READ:     qDebug() << time << "Node" << me << "Frame" << dir << "- PID: MEM_READ " << to << "," << from; break;
    case MEM_DATA:     qDebug() << time << "Node" << me << "Frame" << dir << "- PID: MEM_DATA " << to << "," << from; break;
    case CHECK_MEM:     qDebug() << time << "Node" << me << "Frame" << dir << "- PID: CHECK_MEM " << to << "," << from; break;
    case MEM_CHECK:     qDebug() << time << "Node" << me << "Frame" << dir << "- PID: MEM_CHECK " << to << "," << from; break;
    case FILL_MEM:     qDebug() << time << "Node" << me << "Frame" << dir << "- PID: FILL_MEM " << to << "," << from; break;
    case RAM_WRITE:     qDebug() << time << "Node" << me << "Frame" << dir << "- PID: RAM_WRITE " << to << "," << from; break;
    case FLASH_WRITE:     qDebug() << time << "Node" << me << "Frame" << dir << "- PID: FLASH_WRITE " << to << "," << from; break;
    case WRITE_CONF:     qDebug() << time << "Node" << me << "Frame" << dir << "- PID: WRITE_CONF " << to << "," << from; break;
    case ERASE_MEM:     qDebug() << time << "Node" << me << "Frame" << dir << "- PID: ERASE_MEM " << to << "," << from; break;
    case ERASE_CONF:     qDebug() << time << "Node" << me << "Frame" << dir << "- PID: ERASE_CONF " << to << "," << from; break;
    case MAX_PIDS:     qDebug() << time << "Node" << me << "Frame" << dir << "- PID: MAX_PIDS " << to << "," << from; break;
    default: qDebug() << time << "Node" << me << "Frame" << dir << "- Unknown PID: " << pid; break;
    }
}

void frameOut(sfpFrame * frame)
{
    frameSay(frame, "out");
}

void frameIn(sfpFrame * frame)
{
    frameSay(frame, "in");
}

const char * spsState(spsState_t state)
{
    switch(state) {
    case ANY_SPS: return "ANY_SPS";
    case ONLY_SPS0: return "ONLY_SPS0";
    case ONLY_SPS1: return "ONLY_SPS1";
    case WAIT_ACK0: return "WAIT_ACK0";
    case WAIT_ACK1: return "WAIT_ACK1";
    default: return "unknown state";
    }
}

void txSpsState(sfpLink_t * link, spsState_t state)
{
    if (!verbose) return;

    qDebug() << "Node" << whoami() << link->name << "Tx SPS state changed from:" << spsState(link->txSps) << " to:" << spsState(state);
}

void runSm(Long n) // run the transmitter and receiver state machines n times
{
    while(n-- && !exitflag) {
        setTime(getTime() + 1);
        sfpTxSm(&alink);
        serviceTx(&alink);
        sfpRxSm(&alink);
        processFrames();
    }
}


}
