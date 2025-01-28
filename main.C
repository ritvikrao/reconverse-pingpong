//+pe <N> threads, each running a scheduler
#include "reconverse.h"

void CmiDeleteArgs(char **argv,int k)
{
	int i=0;
	while ((argv[i]=argv[i+k])!=NULL)
		i++;
}

static void CmiAddCLA(const char *arg,const char *param,const char *desc) {
	int i;
	//if (CmiMyPe()!=0) return; /*Don't bother if we're not PE 0*/
	if (desc==NULL) return; /*It's an internal argument*/
	if (usageChecked) { /* Printf should work now */
		if (printUsage)
			CmiPrintf(CLAformatString,arg,param,desc);
	}
	else { /* Printf doesn't work yet-- just add to the list.
		This assumes the const char *'s are static references,
		which is probably reasonable. */
                CLA *temp;
		i=CLAlistLen++;
		if (CLAlistLen>CLAlistMax) { /*Grow the CLA list */
			CLAlistMax=16+2*CLAlistLen;
			temp = (CLA *)realloc(CLAlist,sizeof(CLA)*CLAlistMax);
                        if(temp != NULL) {
			  CLAlist=temp;
                        } else {
                          free(CLAlist);
                          //CmiAbort("Reallocation failed for CLAlist\n");
                        }
		}
		CLAlist[i].arg=arg;
		CLAlist[i].param=param;
		CLAlist[i].desc=desc;
	}
}

int CmiGetArgIntDesc(char **argv,const char *arg,int *optDest,const char *desc)
{
	int i;
	int argLen=strlen(arg);
	CmiAddCLA(arg,"integer",desc);
	for (i=0;argv[i]!=NULL;i++)
		if (0==strncmp(argv[i],arg,argLen))
		{/*We *may* have found the argument*/
			const char *opt=NULL;
			int nDel=0;
			switch(argv[i][argLen]) {
			case 0: /* like "-p","27" */
				opt=argv[i+1]; nDel=2; break;
			case '=': /* like "-p=27" */
				opt=&argv[i][argLen+1]; nDel=1; break;
			case '-':case '+':
			case '0':case '1':case '2':case '3':case '4':
			case '5':case '6':case '7':case '8':case '9':
				/* like "-p27" */
				opt=&argv[i][argLen]; nDel=1; break;
			default:
				continue; /*False alarm-- skip it*/
			}
			if (opt==NULL) {
				fprintf(stderr, "Command-line flag '%s' expects a numerical argument, "
				                "but none was provided\n", arg);
				//CmiAbort("Bad command-line argument\n");
                exit(0);
                        }
			if (sscanf(opt,"%i",optDest)<1) {
			/*Bad command line argument-- die*/
				fprintf(stderr,"Cannot parse %s option '%s' "
					"as an integer.\n",arg,opt);
				//CmiAbort("Bad command-line argument\n");
                exit(0);
			}
			CmiDeleteArgs(&argv[i],nDel);
			return 1;
		}
	return 0;/*Didn't find the argument-- dest is unchanged*/
}
int CmiGetArgInt(char **argv,const char *arg,int *optDest) {
	return CmiGetArgIntDesc(argv,arg,optDest,"");
}

static void CmiStartThreads(char **argv){
    for(int i=0; i<Cmi_npes; i++)
    {
        
    }

}

void ConverseInit(int argc, char **argv, CmiStartFn fn){

    Cmi_npes = 1;
    int plusPSet = CmiGetArgInt(argv,"+pe",&Cmi_npes);

    // NOTE: calling CmiNumPes() here it sometimes returns zero
    printf("Charm++> Running in SMP mode: %d processes\n", Cmi_npes);

    Cmi_argvcopy = CmiCopyArgs(argv);
    Cmi_argv = argv;
    Cmi_startfn = fn;

    CmiStartThreads(argv);
    ConverseRunPE(initret);
}