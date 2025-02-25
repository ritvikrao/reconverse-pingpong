#ifndef RECONVERSE_COMM_BACKEND_H
#define RECONVERSE_COMM_BACKEND_H

#include <cstddef>

namespace comm_backend {

struct Status {
    char *msg;
    size_t size;
};
using CompHandler = void (*)(Status status);
using AmHandler = int;

/**
 * @brief Initialize the communication backend. Not thread-safe.
 */
void init(int *argc, char ***argv, int *numNodes, int *myNodeID);
/**
 * @brief Finalize the communication backend. Not thread-safe.
 */
void exit();
/**
 * @brief Register an active message handler. Not thread-safe.
 */
AmHandler registerAmHandlerr(CompHandler handler);
/**
 * @brief Send an active message. Thread-safe.
 */
void sendAm(int rank, char *msg, size_t size, CompHandler localComp, AmHandler remoteComp);
/**
 * @brief Make progress on the communication backend. Thread-safe.
 */
bool progress(void);
/**
 * @brief Block until all nodes have reached this point. Thread-safe.
 */
void barrier(void);

} // namespace comm_backend

#endif // RECONVERSE_COMM_BACKEND_H