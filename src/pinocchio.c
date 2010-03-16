#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>
#include <wchar.h>
#include <pinocchio.h>
#include <debug.h>
#include <locale.h>

void printf0(const char * string, ...) {
}

/* ========================================================================= */

pthread_key_t Double_Stack;
pthread_key_t _EXP_;
cont   * _CNT_;

void init_pthread_keys()
{
    tkey(Double_Stack,  NULL);
    tkey(_EXP_,         NULL);
    // tkey(_CNT_, NULL);
}

// Object * Double_Stack;
// Object * _EXP_;
// cont   * _CNT_;

/* ========================================================================= */

Collection_Dictionary _NATIVES_;

/* ========================================================================= */

jmp_buf Eval_Exit;
jmp_buf Eval_Continue;
jmp_buf Eval_Abort;

/* 
 * Avoid longjmps as much as possible since they impose a large
 * performance penalty!
 */
void CNT_continue_eval()
{
    longjmp(Eval_Continue, 1);
}

void CNT_abort_eval()
{
    longjmp(Eval_Abort, 1);
}

void CNT_exit_eval()
{
    longjmp(Eval_Exit, 1);
}

void initialize_Natives()
{
    _NATIVES_ = new_Collection_Dictionary();
}

/* ========================================================================= */

#define EVAL_IF(name) \
    if (class == name##_Class) { \
        return name##_eval((name)exp); \
    }


void CNT_send_Eval()
{
    zap_CNT();
    Object exp = peek_EXP(0);

    Type_Class class = HEADER(exp);
    EVAL_IF(AST_Constant)
    EVAL_IF(AST_Variable)
    EVAL_IF(AST_Assign)
    EVAL_IF(AST_Send)
    EVAL_IF(AST_Super)
    EVAL_IF(AST_Self)
    EVAL_IF(AST_Block)
    EVAL_IF(AST_InstVariable)
    EVAL_IF(Organization_ClassReference)
    
    /* TODO fallback by actually sending the eval message */
    print_Class(exp);
    assert(NULL,
		   printf("\"%ls\" has no native eval function. Maybe you wanted wrap it in a AST_Constant?\n", 
				  ((Type_Class)class)->name->value));
}

/* ========================================================================= */

bool isInstance(Object object, Object class) 
{
    // TODO check for MetaClass stuff
    assert_class(class);
    Type_Class type = HEADER(object);
    while ((Object)type != Nil) {
        if ((Object)type == class) {
            return 1;
        }
        type = type->super;
    }
    return 0;
}

/* ========================================================================= */

/**
 * setjmp and longjmp have an overhead but they allow us to avoid testing if
 * we are at the end of the stack. It's only expensive for starting new
 * threads, and the boosts performance for longer living threads.
 */
int IN_EVAL = 0;

void start_eval()
{
    if (IN_EVAL) {
        assert(NULL, printf("Re-entering evaluation thread!\n"));
    }

    IN_EVAL = 1;

    #ifndef NOJMP
    push_CNT(exit_eval);
    #endif
}

Object finish_eval()
{
    #ifndef NOJMP

    if (!setjmp(Eval_Exit)) {
        setjmp(Eval_Continue);
        for (;;) {
            peek_CNT()();
        }
    }

    zap_CNT();

    #else // NOJMP

    while (!empty_CNT()) {
        peek_CNT()();
    }

    #endif // NOJMP
    
    Object result = pop_EXP();
    IN_EVAL = 0;
    return result;
}

Object Eval(Object code)
{
    start_eval();
    
    push_EXP(code);
    push_CNT(send_Eval);

    return finish_eval();
}

Object Eval_Send0(Object self, Type_Symbol symbol)
{
    start_eval();
    Type_Class_direct_dispatch(self, HEADER(self), (Object)symbol, 0);
    return finish_eval();
}

Object Eval_Send1(Object self, Type_Symbol symbol, Object arg)
{
    start_eval();
    Type_Class_direct_dispatch(self, HEADER(self), (Object)symbol, 1, arg);
    return finish_eval();
}

Object Eval_Send2(Object self, Type_Symbol symbol, Object arg1,  Object arg2)
{
    start_eval();
    Type_Class_direct_dispatch(self, HEADER(self), (Object)symbol, 2, arg1, arg2);
    return finish_eval();
}

/* ========================================================================= */

#include <pinocchioHelper.ci>

int main(int argc, const char ** argv)
{
    setlocale(LC_ALL, "");
    #include <pinocchioPreInit.ci>
    initialize_Natives();
    #include <pinocchioPostInit.ci>

    init_pthread_keys();

    pinocchio_main(argc, argv);
    return EXIT_SUCCESS;
}
