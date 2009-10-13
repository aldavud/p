
#include <stdlib.h>
#include <stdio.h>
#include <pinocchio.h>
#include <system/ast/Continue.h>
#include <system/ast/Callec.h>

/* ========================================================================= */

Type_Class AST_Callec_Class;

/* ========================================================================= */

AST_Callec new_AST_Callec()
{
    NEW_OBJECT(AST_Callec);
    result->cont      = new_AST_Continue((Object)result);
    return result;
}

void pre_init_AST_Callec()
{
    AST_Callec_Class = new_Named_Class((Object)Type_Object_Class,
                                       L"AST_Callec",
                                       create_type(3, OBJECT));
}

/* ========================================================================= */

void AST_Callec_eval(AST_Callec self)
{
    LOGFUN;
    self->cont->EXP = _EXP_;
    self->cont->CNT = _CNT_;
    push_EXP(self->target);
    push_CNT(send_Eval);
}

/* ========================================================================= */

void post_init_AST_Callec()
{
}
