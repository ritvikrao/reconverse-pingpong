#ifndef CPV_MACROS_H
#define CPV_MACROS_H

#include "convcore.h"

#define CMK_TAG(x, y) x##y##_

#define CpvDeclare(t, v) t *CMK_TAG(Cpv_, v)
#define CpvStaticDeclare(t, v) static t *CMK_TAG(Cpv_, v)

#define CpvInitialize(t, v)                            \
    do                                                 \
    {                                                  \
        if (CmiMyPE())                                 \
        {                                              \
            CmiNodeBarrier();                          \
        }                                              \
        else                                           \
        {                                              \
            CMK_TAG(Cpv_, v) = new t[CmiMyNodeSize()]; \
            CmiNodeBarrier();                          \
        }                                              \
    } while (0)
;

#define CpvAccess(v) CMK_TAG(Cpv_, v)[CmiMyPE()]

#endif