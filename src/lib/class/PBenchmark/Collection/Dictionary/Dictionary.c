#include <lib/class/PBenchmark/Collection/Dictionary/Dictionary.h>


Optr layout_PBenchmark_Collection_Dictionary_Dictionary_Class_class;
Optr layout_PBenchmark_Collection_Dictionary_Dictionary;


static void init_SMB_setUp() {
    Symbol SMB_setUp = new_Symbol(L"setUp");
    Super PSuper25056 = new_Super(SMB_setUp, 0);
    Symbol SMB_new_ = new_Symbol(L"new:");
    Symbol SMB_dictSize = new_Symbol(L"dictSize");
    // dictSize. 
    Send PSend25059 = new_Send((Optr)self, SMB_dictSize, 0);
    // new:. 
    Send PSend25058 = new_Send((Optr)Dictionary_classReference, SMB_new_, 1, (Optr)PSend25059);
    Assign PAssign25057 = new_Assign((Optr)slot_PBenchmark_Collection_Dictionary_AbstractDictionary_dict, (Optr)PSend25058);
    // dictSize. 
    Send PSend25060 = new_Send((Optr)self, SMB_dictSize, 0);
    Variable VAR_i_1_0 = new_Variable_named(L"i", 1);
    Array PArray25062 = new_Array_with(1, (Optr)VAR_i_1_0);
    Symbol SMB_key_ = new_Symbol(L"key:");
    // key:. 
    Send PSend25064 = new_Send((Optr)self, SMB_key_, 1, (Optr)VAR_i_1_0);
    Symbol SMB_value_ = new_Symbol(L"value:");
    // value:. 
    Send PSend25065 = new_Send((Optr)self, SMB_value_, 1, (Optr)VAR_i_1_0);
    Symbol SMB_at_put_ = new_Symbol(L"at:put:");
    // at:put:. 
    Send PSend25066 = new_Send((Optr)slot_PBenchmark_Collection_Dictionary_AbstractDictionary_dict, SMB_at_put_, 2, (Optr)PSend25064, (Optr)PSend25065);
    Array PThreadedCode25063 = instantiate_Array_with(ThreadedCode_Class, 0, 15, (Optr)&t_push_slot, (Optr)slot_PBenchmark_Collection_Dictionary_AbstractDictionary_dict, (Optr)&t_push_self, (Optr)&t_push_variable, (Optr)VAR_i_1_0, (Optr)&t_send1, (Optr)PSend25064, (Optr)&t_push_self, (Optr)&t_push_variable, (Optr)VAR_i_1_0, (Optr)&t_send1, (Optr)PSend25065, (Optr)&t_send2, (Optr)PSend25066, (Optr)&t_method_return);
    Block PBlock25061 = new_Block_with(PArray25062, empty_Array, PThreadedCode25063, 1, PSend25066);
    SmallInt int_1 = new_SmallInt(1);
    Constant int_1_Const = new_Constant((Optr)int_1);
    Symbol SMB_to_do_ = new_Symbol(L"to:do:");
    // to:do:. 
    Send PSend25067 = new_Send((Optr)int_1_Const, SMB_to_do_, 2, (Optr)PSend25060, (Optr)PBlock25061);
    Array PThreadedCode25055 = instantiate_Array_with(ThreadedCode_Class, 0, 26, (Optr)&t_push_self, (Optr)&t_super0, (Optr)PSuper25056, (Optr)&t_zap, (Optr)&t_push1, (Optr)PAssign25057, (Optr)&t_push_class_reference, (Optr)Dictionary_classReference, (Optr)&t_push_self, (Optr)&t_send0, (Optr)PSend25059, (Optr)&t_send1, (Optr)PSend25058, (Optr)&t_assign, (Optr)&t_zap, (Optr)&t_push_1, (Optr)&t_push_self, (Optr)&t_send0, (Optr)PSend25060, (Optr)&t_push_closure, (Optr)PBlock25061, (Optr)&t_send_to_do_, (Optr)&t_continue_to_do_, (Optr)PSend25067, (Optr)&t_zap, (Optr)&t_method_return_self);
    Method PMethod25054 = new_Method_with(empty_Array, empty_Array, empty_Array, PThreadedCode25055, 4, PSuper25056, PAssign25057, PSend25067, self);
    
    MethodClosure MC_SMB_setUp = new_MethodClosure((Method)PMethod25054, PBenchmark_Collection_Dictionary_Dictionary_Class);
    store_method(PBenchmark_Collection_Dictionary_Dictionary_Class, SMB_setUp, MC_SMB_setUp);
}

void init_PBenchmark_Collection_Dictionary_PBSTDictionary_layout() {
    layout_PBenchmark_Collection_Dictionary_Dictionary_Class_class = (Optr)create_layout_with_vars(ObjectLayout_Class, 5);
    ((Array)layout_PBenchmark_Collection_Dictionary_Dictionary_Class_class)->values[0] = slot_Kernel_Object_Object_Class_class_layout; // layout 
    ((Array)layout_PBenchmark_Collection_Dictionary_Dictionary_Class_class)->values[1] = slot_Kernel_Object_Object_Class_class_superclass; // superclass 
    ((Array)layout_PBenchmark_Collection_Dictionary_Dictionary_Class_class)->values[2] = slot_Kernel_Object_Object_Class_class_methods; // methods 
    ((Array)layout_PBenchmark_Collection_Dictionary_Dictionary_Class_class)->values[3] = slot_Kernel_Object_Object_Class_class_name; // name 
    ((Array)layout_PBenchmark_Collection_Dictionary_Dictionary_Class_class)->values[4] = slot_Kernel_Object_Object_Class_class_package; // package 
    
    Symbol  SMB_Dictionary = new_Symbol(L"Dictionary");
    layout_PBenchmark_Collection_Dictionary_Dictionary = (Optr)create_layout_with_vars(ObjectLayout_Class, 6);
    ((Array)layout_PBenchmark_Collection_Dictionary_Dictionary)->values[0] = slot_PBenchmark_AbstractBenchmark_probeCount; // probeCount 
    ((Array)layout_PBenchmark_Collection_Dictionary_Dictionary)->values[1] = slot_PBenchmark_BenchmarkSuite_runs; // runs 
    ((Array)layout_PBenchmark_Collection_Dictionary_Dictionary)->values[2] = slot_PBenchmark_Collection_Dictionary_AbstractDictionary_dict; // dict 
    ((Array)layout_PBenchmark_Collection_Dictionary_Dictionary)->values[3] = slot_PBenchmark_Collection_Dictionary_AbstractDictionary_dictSize; // dictSize 
    ((Array)layout_PBenchmark_Collection_Dictionary_Dictionary)->values[4] = slot_PBenchmark_Collection_Dictionary_AbstractDictionary_keyBlock; // keyBlock 
    ((Array)layout_PBenchmark_Collection_Dictionary_Dictionary)->values[5] = slot_PBenchmark_Collection_Dictionary_AbstractDictionary_valueBlock; // valueBlock 
    PBenchmark_Collection_Dictionary_Dictionary_Class = (Class)new_Class(PBenchmark_Collection_Dictionary_AbstractDictionary_Class, layout_PBenchmark_Collection_Dictionary_Dictionary_Class_class);
    PBenchmark_Collection_Dictionary_Dictionary_Class->layout = layout_PBenchmark_Collection_Dictionary_Dictionary;
    PBenchmark_Collection_Dictionary_Dictionary_Class->name = SMB_Dictionary;
    
}

void init_PBenchmark_Collection_Dictionary_PBSTDictionary_methods() {
    init_SMB_setUp();
    
}