
#include <stdlib.h>
#include <stdio.h>
#include <system/type/Object.h>

/* ========================================================================= */

Type_Class Type_Object_Class;

/* ========================================================================= */

void pre_init_Type_Object() 
{
    // explicitely use new_Class not new_Class_named! to avoid early use
    // of symbols.
    // do manually instanciate since we cannot use dict yet
    Type_Class Object_mclass  = (Type_Class)basic_instantiate_Object(Metaclass, METACLASS_SIZE);
    Type_Object_Class         = (Type_Class)basic_instantiate_Object(Object_mclass, CLASS_SIZE);
    Type_Object_Class->super  = Nil;

    /*
    Type_Class_Class          = NEW_t(Type_Class);
    Type_Class_Class->type    = create_type(4, OBJECT);
    
    HEADER(Type_Class_Class)  = (Object)Type_Class_Class;
    Type_Class_Class->super   = (Object)Type_Object_Class;
    Type_Object_Class->type   = create_type(0, OBJECT);
    
    HEADER(Type_Object_Class) = (Object) Type_Class_Class;
    Type_Object_Class->super  = Nil;
    */
}

void inter_init_Type_Object() 
{
    ((Type_Class)HEADER(Type_Object_Class))->type = create_type(CLASS_SIZE, OBJECT, CLASS_VARS);
    Type_Object_Class->type   = create_type(OBJECT_SIZE, OBJECT); 
}

/* ========================================================================= */

NATIVE0(Type_Object_basicNew)
    assert_class(self);
    Object result = instantiate((Type_Class)self);
    RETURN_FROM_NATIVE(result);
}

NATIVE1(Type_Object_equals)
    Object arg = NATIVE_ARG(0);
    RETURN_FROM_NATIVE(get_bool(self == arg));
}

NATIVE0(Type_Object_asString)
    // TODO
    assert1(NULL, "NYI");
}

NATIVE(Type_Object_isNil)
    RETURN_FROM_NATIVE(False);
}

Object raw_Type_Object_at(Type_Object o, Object tag, uns_int index)
{
    ASSERT_TAG_SIZE(tag, index);
    return o->ivals[index];
}

NATIVE1(Type_Object_instVarAt_)
    Object w_index = NATIVE_ARG(0);
    int index = unwrap_int(w_index);
    Object tag = GETTAG(self);
    ASSERT_TAG_TYPE(tag, Object);
    RETURN_FROM_NATIVE(raw_Type_Object_at((Type_Object)self, tag, index));
}

void raw_Type_Object_at_put(Type_Object o, Object tag,
                            uns_int index, Object value)
{
    ASSERT_TAG_SIZE(tag, index);
    o->ivals[index] = value;
}

NATIVE2(Type_Object_instVarAt_put_)
    Object w_index = NATIVE_ARG(0);
    int index = unwrap_int(w_index);
    Object tag = GETTAG(self);
    ASSERT_TAG_TYPE(tag, Object);
    raw_Type_Object_at_put((Type_Object)self, tag, index, NATIVE_ARG(1));
    RETURN_FROM_NATIVE(self);
}

/* ========================================================================= */

void post_init_Type_Object()
{
    // put the names here, now after the Symbols_Class is initialized
    /* 
    Type_Class_Class->name   = new_Type_String(L"Type_Class");
    Type_Class_Class->methods = new_Type_Dictionary();
    */
    ((Type_Class)HEADER(Type_Object_Class))->methods = new_Type_Dictionary();
    Type_Object_Class->name = new_Type_String(L"Object");
    Type_Object_Class->methods = new_Type_Dictionary();
    
    store_native_method((Type_Class)HEADER(Type_Object_Class), SMB_basicNew, NM_Type_Object_basicNew);
    store_native_method((Type_Class)Type_Object_Class, SMB__pequal,         NM_Type_Object_equals);
    store_native_method((Type_Class)Type_Object_Class, SMB__equal,         NM_Type_Object_equals);
    store_native_method((Type_Class)Type_Object_Class, SMB__equal,          NM_Type_Object_equals);
    store_native_method((Type_Class)Type_Object_Class, SMB_isNil,           NM_Type_Object_isNil);
    store_native_method((Type_Class)Type_Object_Class, SMB_instVarAt_,      NM_Type_Object_instVarAt_);
    store_native_method((Type_Class)Type_Object_Class, SMB_instVarAt_put_,  NM_Type_Object_instVarAt_put_);
}