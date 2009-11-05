#ifndef SYMBOL_H
#define SYMBOL_H

#include <pinocchio.h>

/* ========================================================================= */

struct Type_Symbol_t { 
    Type_SmallInt hash;
    uns_int  size;
    wchar_t       value[];
};

CREATE_INITIALIZERS(Type_Symbol)
extern Type_Symbol new_Type_Symbol(const wchar_t* name);
extern void install_symbol_methods(Type_Class class);

/* ========================================================================= */

extern void initialize_Symbol();
extern uns_int Type_Symbol_hash(Type_Symbol symbol_string);

/* ========================================================================= */

#include <system/type/SymbolDefinition.hi>

/* ========================================================================= */

#endif // SYMBOL_H
