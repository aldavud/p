#include <stdlib.h>
#include <stdio.h>
#include <pinocchio.h>

/* ========================================================================= */

DECLARE_CLASS(BlockContext);

/* ========================================================================= */

static BlockContext make_context(uns_int size)
{
    NEW_ARRAY_OBJECT(BlockContext, Optr[size]);
    return result;
}

BlockContext optain_context(uns_int size)
{
    return make_context(size);
}

BlockContext capture_current_env()
{
    BlockContext target;
    BlockContext context = current_env();
    if (!context->stacked) {
		return context;
    }
    uns_int size   = context->size;
    target         = optain_context(size);
    HEADER(target) = HEADER(context);
    size          += CONTEXT_SIZE;
    Optr * from    = (Optr*)context;
    Optr * to      = (Optr*)target;

    while(size) {
        size--;
        *to++ = *from++;
    }

    if (context->home_context == (MethodContext)context) {
        target->home_context = (MethodContext)target;
    }

    target->stacked = 0;
    set_env((Optr)target);
    return target;
}

/* ========================================================================= */

Optr BlockContext_lookup(BlockContext self, uns_int local_id, uns_int scope_id)
{
    while (scope_id != self->scope_id && (Optr)self->outer_scope != nil) {
        self = self->outer_scope;
    }
    assert1(scope_id == self->scope_id, "TODO jump to error handler");
    assert(local_id < self->size,
    	   printf("Lookup failed, index \"%lu\" out of range [0:%lu]", 
                 local_id, self->size));

    return self->locals[local_id];
}

void BlockContext_assign(BlockContext self, uns_int local_id,
                         uns_int scope_id, Optr value)
{
    while (scope_id != self->scope_id && (Optr)self->outer_scope != nil) {
        self = self->outer_scope;
    }
    assert1(scope_id == scope_id, "TODO jump to error handler");
    assert(local_id < self->size,
    	   printf("Lookup failed, index \"%lu\" out of range [0:%lu]", 
                  local_id, self->size));

    self->locals[local_id] = value;
}
