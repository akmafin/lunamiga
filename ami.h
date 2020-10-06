#ifndef ami_h
#define ami_h

struct IORequest *AmiCreateIORequest(struct MsgPort* port, ULONG size);
void AmiDeleteIORequest(struct IORequest* ioreq);
struct MsgPort* AmiCreateMsgPort(void);
void AmiDeleteMsgPort(struct MsgPort* port);

#endif
