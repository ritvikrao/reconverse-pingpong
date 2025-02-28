#ifndef COMM_BACKEND_INTERNAL_H
#define COMM_BACKEND_INTERNAL_H

#include <cstdio>
#include <vector>

#include "converse_config.h"
#include "comm_backend/comm_backend.h"

namespace comm_backend {

class CommBackendBase 
{
 public:
  virtual void init(int *argc, char ***argv) = 0;
  virtual void exit() = 0;
  virtual int getMyNodeId() = 0;
  virtual int getNumNodes() = 0;
  virtual AmHandler registerAmHandler(CompHandler handler) = 0;
  virtual void sendAm(int rank, void *msg, size_t size, CompHandler localComp, AmHandler remoteComp) = 0;
  // return true if there is more work to do
  virtual bool progress(void) = 0;
  virtual void barrier(void) = 0;
};

} // namespace comm_backend

#ifdef RECONVERSE_ENABLE_COMM_LCI1
#include "comm_backend/lci1/comm_backend_lci1.h"
#endif

#endif // COMM_BACKEND_INTERNAL_H