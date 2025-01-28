
struct CmiMessageHeader
{
    int handlerId;
    int messageId;
    int messageSize;
};

#define CmiMessageHeaderSize sizeof(CmiMessageHeader)