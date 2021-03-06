#ifndef ORGANIZATION_CLASSREFERENCE_H
#define ORGANIZATION_CLASSREFERENCE_H

#include <pinocchio.h>

/* ========================================================================= */

struct ClassReference_t {
    Optr package;
    Optr class;
};

extern ClassReference new_ClassReference(Class class);
extern ClassReference new_ClassReference_empty();


/* ========================================================================= */

extern void ClassReference_eval();

/* ========================================================================= */

#endif // ORGANIZATION_CLASSREFERENCE_H
