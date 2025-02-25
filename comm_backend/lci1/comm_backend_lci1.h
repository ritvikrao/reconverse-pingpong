#ifndef RECONVERSE_COMM_BACKEND_LCI1_H
#define RECONVERSE_COMM_BACKEND_LCI1_H

#include "lci.h"

namespace comm_backend
{

class CommBackendLCI1 : public CommBackendBase
{
 public:
  void init(int *argc, char ***argv, int *numNodes, int *myNodeID) override;
  void exit() override;
  AmHandler registerAmHandler(CompHandler handler) override;
  void sendAm(int rank, char *msg, size_t size, CompHandler localComp, AmHandler remoteComp) override;
  bool progress(void) override;
  void barrier(void) override;
};

} // namespace comm_backend

#endif // RECONVERSE_COMM_BACKEND_LCI1_H