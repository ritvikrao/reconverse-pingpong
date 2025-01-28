typedef void (*CmiStartFn)(int argc, char **argv);
static char     **Cmi_argv;
char            **Cmi_argvcopy;
static CmiStartFn Cmi_startfn;   /* The start function */
int Cmi_npes;