#include "lunamiga.h"

struct IORequest *AmiCreateIORequest(struct MsgPort* port, ULONG size) {
	struct IORequest* ioreq;
	
	ioreq = (struct IORequest*)AllocMem(size, MEMF_PUBLIC | MEMF_CLEAR);
	ioreq->io_Message.mn_Node.ln_Type = NT_REPLYMSG;
	ioreq->io_Message.mn_ReplyPort = port;
	ioreq->io_Message.mn_Length = size;
	
	return ioreq;
}

void AmiDeleteIORequest(struct IORequest* ioreq) {
	ioreq->io_Unit->unit_MsgPort.mp_Node.ln_Succ = (struct Node*)-1;
	ioreq->io_Device = (struct Device*)-1;
	FreeMem(ioreq, ioreq->io_Message.mn_Length);
}

struct MsgPort* AmiCreateMsgPort(void) {
	struct MsgPort* port;
	int signum;
	struct ExecBase* execptr;

	execptr = (*(struct ExecBase **) 4);
	port = (struct MsgPort*)AllocMem(sizeof(struct MsgPort), MEMF_PUBLIC | MEMF_CLEAR);
	signum = AllocSignal(-1);
	port->mp_Node.ln_Type = NT_MSGPORT;
	port->mp_Flags = PA_SIGNAL;
	port->mp_SigBit = signum;
	port->mp_SigTask = execptr->ThisTask;
	port->mp_MsgList.lh_TailPred = (struct Node*)&port->mp_MsgList;
	port->mp_MsgList.lh_Tail = 0;
	port->mp_MsgList.lh_Head = (struct Node*)&port->mp_MsgList.lh_Tail;

	return port;
}

void AmiDeleteMsgPort(struct MsgPort* port) {
	FreeSignal(port->mp_SigBit);
	port->mp_MsgList.lh_Head = (struct Node*)-1;
	port->mp_Node.ln_Succ = (struct Node*)-1;
	FreeMem(port, sizeof(struct MsgPort));
}

