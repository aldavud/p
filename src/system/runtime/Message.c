
#include <stdlib.h>
#include <stdio.h>
#include <system/runtime/Message.h>

/* ========================================================================= */

Message new_Message(Optr selector, uns_int size)
{
    NEW_ARRAY_OBJECT(Message, Optr[size]);
    result->size     = size;
    result->selector = (Symbol)selector;
    return result;
}
