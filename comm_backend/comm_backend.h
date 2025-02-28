#ifndef RECONVERSE_COMM_BACKEND_H
#define RECONVERSE_COMM_BACKEND_H

#include <cstddef>

namespace comm_backend {

struct Status {
    void* msg;
    size_t size;
};
using CompHandler = void (*)(Status status);
using AmHandler = int;

/**
 * @brief Initialize the communication backend. Not thread-safe.
 */
void init(int *argc, char ***argv);
/**
 * @brief Finalize the communication backend. Not thread-safe.
 */
void exit();
/**
 * @brief Get the node ID of the current process. Thread-safe.
 */
int getMyNodeId();
/**
 * @brief Get the number of nodes in the system. Thread-safe.
 */
int getNumNodes();
/**
 * @brief Register an active message handler. Not thread-safe.
 */
AmHandler registerAmHandler(CompHandler handler);
/**
 * @brief Send an active message. Thread-safe.
 */
void sendAm(int rank, void *msg, size_t size, CompHandler localComp, AmHandler remoteComp);
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