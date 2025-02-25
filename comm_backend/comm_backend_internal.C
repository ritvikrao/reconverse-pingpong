#include "comm_backend/comm_backend_internal.h"

namespace comm_backend {

CommBackendBase *commBackend = nullptr;

void init(int *argc, char ***argv, int *numNodes, int *myNodeID) 
{
#ifdef RECONVERSE_ENABLE_COMM_LCI1
  commBackend = new CommBackendLCI1();
#endif
  if (commBackend == nullptr) {
    *numNodes = 1;
    *myNodeID = 0;
    return;
  }
  
  commBackend->init(argc, argv, numNodes, myNodeID);
}

void exit()
{
  if (commBackend == nullptr) {
    fprintf(stderr, "Error: commBackend is null\n");
    return;
  }
  commBackend->exit();
  delete commBackend;
}

AmHandler registerAmHandlerr(CompHandler handler)
{
  if (commBackend == nullptr) {
    fprintf(stderr, "Error: commBackend is null\n");
    return -1;
  }
  return commBackend->registerAmHandler(handler);
}

void sendAm(int rank, char *msg, size_t size, CompHandler localComp, AmHandler remoteComp)
{
  if (commBackend == nullptr) {
    fprintf(stderr, "Error: commBackend is null\n");
    return;
  }
  commBackend->sendAm(rank, msg, size, localComp, remoteComp);
}

bool progress(void)
{
  if (commBackend == nullptr) {
    fprintf(stderr, "Error: commBackend is null\n");
    return false;
  }
  return commBackend->progress();
}

void barrier(void)
{
  if (commBackend == nullptr) {
    fprintf(stderr, "Error: commBackend is null\n");
    return;
  }
  commBackend->barrier();
}

} // namespace comm_backend