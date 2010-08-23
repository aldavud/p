#include <stdlib.h>
#include <stdio.h>
#include <pinocchio.h>

/* ========================================================================= */

Optr true;
Optr false;
Constant false_Const;
Constant true_Const;

/* ========================================================================= */

Optr get_bool(bool value)
{
    return value ? true : false;
}

/* ========================================================================= */

void post_init_Boolean()
{
    true         = (Optr)instantiate(True_Class);
    true_Const   = new_Constant((Optr)true);

    false        = (Optr)instantiate(False_Class);
    false_Const  = new_Constant((Optr)false);
}
