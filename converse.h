#ifndef CONVERSE_H
#define CONVERSE_H

#include "CpvMacros.h" // for backward compatibility

typedef void (*CmiStartFn)(int argc, char **argv);
void ConverseInit(int argc, char **argv, CmiStartFn fn, int usched = 0, int initret = 0);

static CmiStartFn Cmi_startfn;

// handler tools
typedef void (*CmiHandler)(void *msg);
typedef void (*CmiHandlerEx)(void *msg, void *userPtr);

int CmiRegisterHandler(CmiHandler h);

// state getters
int CmiMyPe();
int CmiMyNode();
int CmiMyNodeSize();
int CmiMyRank();
int CmiNumPes();
int CmiStopFlag();

void CmiSetHandler(void *msg, int handlerId);
void CmiNodeBarrier();
void CmiNodeAllBarrier();

void CsdExitScheduler();

void CmiAbort(const char *format, ...);
void CmiInitCPUTopology(char **argv);
void CmiInitCPUAffinity(char **argv);

#endif