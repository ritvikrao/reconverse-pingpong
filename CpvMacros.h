//copy CpvMacros from converse with array indexed by myPe implementation

#define CMK_TAG(x,y) x##y##_
#define CpvAccess(v) CMK_TAG(Cpv_,v)[_Cmi_myrank]
#define CpvDeclare(t,v) t CMK_TAG(Cpv_,v)[2]
#define CpvStaticDeclare(t,v) static t CMK_TAG(Cpv_,v)[2]

//define variables (do we use cpv or cth here?)
CpvDeclare(CmiHandlerInfo*, CmiHandlerTable);
CpvStaticDeclare(int, CmiHandlerCount);
CpvStaticDeclare(int, CmiHandlerLocal);
CpvStaticDeclare(int, CmiHandlerGlobal);
CpvDeclare(int, CmiHandlerMax);

static void CmiExtendHandlerTable(int atLeastLen) {
    int max = CpvAccess(CmiHandlerMax);
    int newmax = (atLeastLen+(atLeastLen>>2)+32);
    int bytes = max*sizeof(CmiHandlerInfo);
    int newbytes = newmax*sizeof(CmiHandlerInfo);
    CmiHandlerInfo *nu = (CmiHandlerInfo*)malloc(newbytes);
    CmiHandlerInfo *tab = CpvAccess(CmiHandlerTable);
    _MEMCHECK(nu);
    if (tab) {
      memcpy(nu, tab, bytes);
    }
    memset(((char *)nu)+bytes, 0, (newbytes-bytes));
    free(tab); tab=nu;
    CpvAccess(CmiHandlerTable) = tab;
    CpvAccess(CmiHandlerMax) = newmax;
}

void CmiNumberHandler(int n, CmiHandler h)
{
  CmiHandlerInfo *tab;
  if (n >= CpvAccess(CmiHandlerMax)) CmiExtendHandlerTable(n);
  tab = CpvAccess(CmiHandlerTable);
  tab[n].hdlr = (CmiHandlerEx)h; /* LIE!  This assumes extra pointer will be ignored!*/
  tab[n].userPtr = 0;
}

int CmiRegisterHandler(CmiHandler h)
{
  int Count = CpvAccess(CmiHandlerCount);
  CmiNumberHandler(Count, h);
  CpvAccess(CmiHandlerCount) = Count+DIST_BETWEEN_HANDLERS;
  return Count;
}