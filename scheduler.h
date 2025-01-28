struct CmiMessageHeader
{
    int handlerId;
    int messageId;
    int messageSize;
};

#define CmiMessageHeaderSize sizeof(CmiMessageHeader)

struct CmiMessage
{
    CmiMessageHeader header;
    char data[];
};

struct CmiState
{
    int pe;
    int rank = 0;
};

// state relevant functionality
CmiState CmiGetState(void);
void CmiInitState();

// state getters
int CmiMyPE();
int CmiMyNode();

// message sending
void CmiPushPE(int destPE, int messageSize, void *msg);
void CmiSyncSendAndFree(int destNode, int destPE, int messageSize, void *msg);