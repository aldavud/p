#include <stdlib.h>
#include <stdio.h>
#include <system/type/Boolean.h>

/* ========================================================================= */

DECLARE_CLASS(Type_Boolean);
DECLARE_CLASS(True);
DECLARE_CLASS(False);

AST_Constant False_Const;
Type_Boolean False;

AST_Constant True_Const;
Type_Boolean True;

/* ========================================================================= */

void pre_init_Type_False() {
    False_Class = new_Class_named(Type_Boolean_Class,
                                  L"False",
                                  create_layout(0, OBJECT));
    REFER_TO(False);
    False         = NEW_t(Type_Boolean);
    False->hash   = Type_Object_hash(False);
    HEADER(False) = False_Class;
    False_Const   = new_AST_Constant((Object) False);
}

void pre_init_Type_True()
{
    True_Class = new_Class_named(Type_Boolean_Class,
                                 L"True",
                                 create_layout(0, OBJECT));
    REFER_TO(True);
    True         = NEW_t(Type_Boolean);
    True->hash   = Type_Object_hash(True);
    HEADER(True) = True_Class;
    True_Const   = new_AST_Constant((Object) True);
}

void pre_init_Type_Boolean()
{
    Type_Boolean_Class = new_Class_named(Type_Object_Class,
                                         L"Boolean",
                                         create_layout(0, OBJECT));
    REFER_TO(Type_Boolean);
    pre_init_Type_True();
    pre_init_Type_False();
}

/* ========================================================================= */

AST_Constant get_bool_const(bool value)
{
    return value ? True_Const : False_Const;
}

Type_Boolean get_bool(bool value)
{
    return value ? True : False;
}

/* ========================================================================= */

void post_init_Type_True()
{
}

/* ========================================================================= */

void post_init_Type_False()
{
}

/* ========================================================================= */

NATIVE0(Type_Boolean_hash)
    RETURN_FROM_NATIVE(((Type_Boolean)self)->hash)
}

void post_init_Type_Boolean()
{
    post_init_Type_True();
    post_init_Type_False();
    Collection_Dictionary natives = add_plugin(L"Type.Boolean");
    store_native(natives, SMB_hash,       NM_Type_Boolean_hash);
}

