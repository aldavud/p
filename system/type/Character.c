
#include <stdlib.h>
#include <stdio.h>
#include <system/type/Character.h>

/* ========================================================================= */

Type_Class Type_Character_Class;

/* ========================================================================= */

Type_Character new_Type_Character_raw(wchar_t value)
{
    NEW_OBJECT(Type_Character);
    result->value         = value;
    return result;
}


Type_Character new_Type_Character(wchar_t value)
{
    if (value < CHARACTER_TABLE_SIZE) {
        return Type_Character_table->chars[value];
    }
    return new_Type_Character_raw(value);
}

Type_Character new_Type_Character_fromInt(int value)
{
    wchar_t casted = (wchar_t)value;
    assert(((int)casted == value), 
        printf("Unicode Character \"%lc\" %i out of Range [%"F_I"i Byte]", 
                casted, value, sizeof(wchar_t)));
    
    return new_Type_Character(casted);
}
    

void pre_init_Type_Character()
{
    Type_Character_Class = new_Class_named((Object)Type_Object_Class,
                                           L"Type_Character",
                                           create_type(0, CHAR));
}

/* ========================================================================= */

NATIVE1(Type_Character_equals_)
    NYI;
}

NATIVE0(Type_Character_asString)
    const wchar_t str[2] = { ((Type_Character)self)->value, '\0' };
    RETURN_FROM_NATIVE(new_Type_String(str));
}

NATIVE0(Type_Character_asSymbol)
    const wchar_t str[2] = { ((Type_Character)self)->value, '\0' };
    RETURN_FROM_NATIVE(new_Type_Symbol(str));
}

NATIVE0(Type_Character_asSmallInt)
    RETURN_FROM_NATIVE(new_Type_SmallInt(((Type_Character)self)->value));
}

/* ========================================================================= */


void post_init_Type_Character()
{ 
    store_native_method(Type_Character_Class, SMB_asString,   NM_Type_Character_asString);
    store_native_method(Type_Character_Class, SMB_asSymbol,   NM_Type_Character_asSymbol);
    store_native_method(Type_Character_Class, SMB_asSmallInt, NM_Type_Character_asSmallInt);
}