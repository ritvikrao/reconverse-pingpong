#ifndef CONVERSE_H
#define CONVERSE_H

#include "CpvMacros.h" // for backward compatibility


typedef struct
{
  int num_pus;
  int num_cores;
  int num_sockets;

  int total_num_pus;
} CmiHwlocTopology;

extern CmiHwlocTopology CmiHwlocTopologyLocal;

extern void CmiInitHwlocTopology(void);
extern int CmiSetCPUAffinityLogical(int mycore);

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
#endif /* CONVERSE_H */
