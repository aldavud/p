#ifndef AST_METHOD_H
#define AST_METHOD_H

#include <pinocchio.h>

/* ========================================================================= */

#define AST_METHOD_SIZE 5

/* ========================================================================= */

struct AST_Method_t {
    Type_Array  params;
    Type_Array  locals;
    Type_Array  body;
    AST_Info    info;
    Object      package;
};

CREATE_INITIALIZERS(AST_Method)
extern AST_Method new_AST_Method(uns_int paramCount,
                               uns_int localCount,
                               Type_Array body);

/* ========================================================================= */

/* ========================================================================= */

#endif // AST_METHOD_H