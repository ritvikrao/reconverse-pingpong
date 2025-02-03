#include "CpvMacros.h" // for backward compatibility

typedef void (*CmiStartFn)(int argc, char **argv);
void ConverseInit(int argc, char **argv, CmiStartFn fn);

#define CmiMessageHeaderSize sizeof(CmiMessageHeader)

// handler tools
typedef void (*CmiHandler)(void *msg);
typedef void (*CmiHandlerEx)(void *msg, void *userPtr);

int CmiRegisterHandler(CmiHandler h);

// state getters
int CmiMyPE();
int CmiMyNode();
int CmiMyNodeSize();
int CmiMyRank();
int CmiStopFlag();

void CsdExitScheduler();