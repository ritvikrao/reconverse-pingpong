#include "reconverse.h"

int Cmi_argc;
static char     **Cmi_argv;
static CmiStartFn Cmi_startfn;
int Cmi_npes;

static void CmiStartThreads(char **argv);