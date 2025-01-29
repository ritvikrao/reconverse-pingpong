typedef void* (*CmiStartFn)(void *argv);
static char     **Cmi_argv;
static CmiStartFn Cmi_startfn;
int Cmi_npes;

static void CmiStartThreads(char **argv);