#include "comm_backend_internal.h"

#define LCI_SAFECALL(stmt)                                                    \
  do {                                                                        \
    int lci_errno = (stmt);                                                   \
    if (LCI_OK != lci_errno)                                                  \
      fprintf(stderr, "LCI call failed with %d \n", lci_errno);               \
  } while (0)

namespace comm_backend {

std::vector<CompHandler> g_handlers;

void local_callback(LCI_request_t request)
{
  auto handler = reinterpret_cast<CompHandler>(request.user_context);
  void *address;
  size_t size;
  if (request.type == LCI_MEDIUM) {
    address = request.data.mbuffer.address;
    size = request.data.mbuffer.length;
  } else {
    address = request.data.lbuffer.address;
    size = request.data.lbuffer.length;
  }
  handler({static_cast<char*>(address), size});
}

void remote_callback(LCI_request_t request)
{
  auto am_handler = static_cast<AmHandler>(request.tag);
  auto handler = g_handlers[am_handler];
  void *address;
  size_t size;
  if (request.type == LCI_MEDIUM) {
    address = request.data.mbuffer.address;
    size = request.data.mbuffer.length;
  } else {
    address = request.data.lbuffer.address;
    size = request.data.lbuffer.length;
  }
  handler({static_cast<char*>(address), size});
}

void CommBackendLCI1::init(int *argc, char ***argv, int *numNodes, int *myNodeID)
{
  int initialized = false;
  LCI_SAFECALL(LCI_initialized(&initialized));
  if (!initialized)
    LCI_SAFECALL(LCI_initialize());
}

void CommBackendLCI1::exit()
{
  LCI_SAFECALL(LCI_finalize());
}

AmHandler CommBackendLCI1::registerAmHandler(CompHandler handler)
{
  g_handlers.push_back(handler);
  return g_handlers.size() - 1;
}

void CommBackendLCI1::sendAm(int rank, char *msg, size_t size, CompHandler localComp, AmHandler remoteComp)
{
  // we use LCI tag to pass the remoteComp
  LCI_error_t ret;
  do {
    if (size <= LCI_MEDIUM_SIZE) {
      LCI_mbuffer_t buffer;
      buffer.address = msg;
      buffer.length = size;
      auto ret = LCI_putma(LCI_UR_ENDPOINT, buffer, rank, remoteComp, LCI_DEFAULT_COMP_REMOTE);
      if (ret == LCI_OK) {
        // eager put is immediately completed.
        localComp({msg, size});
      }
    } else {
      LCI_lbuffer_t buffer;
      buffer.address = msg;
      buffer.length = size;
      buffer.segment = LCI_SEGMENT_ALL;
      LCI_comp_t completion;
      auto ret = LCI_putla(LCI_UR_ENDPOINT, buffer, completion, rank, remoteComp, LCI_DEFAULT_COMP_REMOTE, reinterpret_cast<void*>(localComp));
    }
  } while (ret == LCI_ERR_RETRY);
}

bool CommBackendLCI1::progress(void)
{
  auto ret = LCI_progress(LCI_UR_DEVICE);
  return ret == LCI_OK;
}

void CommBackendLCI1::barrier(void)
{
  LCI_SAFECALL(LCI_barrier());
}


} // namespace comm_backend