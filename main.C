//+pe <N> threads, each running a scheduler
#include "CpvMacros.h"
#include "scheduler.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *converseRunPe(void *args){
    //call cmi start function, pass args to it
    Cmi_startfn(Cmi_argc, Cmi_argv);
    //call scheduler
    CsdScheduler();
    return NULL;
}

static void CmiStartThreads(){
	pthread_t threadId[Cmi_npes];
    for(int i=0; i<Cmi_npes; i++)
    {
        pthread_create(&threadId[i], NULL, converseRunPe, NULL);
    }
	for(int i=0; i<Cmi_npes; i++)
	{
		pthread_join(threadId[i], NULL);
	}
}

//argument form: ./prog +pe <N>
void ConverseInit(int argc, char **argv, CmiStartFn fn){

    Cmi_npes = atoi(argv[2]);
    //int plusPSet = CmiGetArgInt(argv,"+pe",&Cmi_npes);

    // NOTE: calling CmiNumPes() here it sometimes returns zero
    printf("Charm++> Running in SMP mode: %d processes\n", Cmi_npes);

    Cmi_argc = argc;
    Cmi_argv=(char **)malloc(sizeof(char *)*(argc+1));
	int i;
	for (i=0;i<=argc;i++)
		Cmi_argv[i]=argv[i];
    Cmi_startfn = fn;

    CmiStartThreads();
    free(Cmi_argv);

}