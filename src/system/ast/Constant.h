#ifndef CONSTANT_H
#define CONSTANT_H

#include <pinocchio.h>

/* ========================================================================= */

struct Constant_t {
    Optr          constant;
};

extern Constant new_Constant(Optr constant);

/* ========================================================================= */

#endif // CONSTANT_H
