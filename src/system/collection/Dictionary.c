#include <stdlib.h>
#include <stdio.h>
#include <system/collection/Dictionary.h>

/* ========================================================================= */

DECLARE_CLASS(Dictionary);

/* ========================================================================= */

Dictionary new_Dictionary()
{
    NEW_OBJECT(Dictionary);
    result->size      = 0;
    result->ratio     = 500;
    result->maxLinear = 20;
    result->data      = new_Array_withAll(DICTIONARY_SIZE, nil);
    return result;
}

/* ========================================================================= */

static long get_hash(Dictionary self, Optr key)
{
    long hash;
    Optr tag = GETTAG(key);
    if (TAG_IS_LAYOUT(tag, Words)) {
        hash = Symbol_hash((Symbol)key)->value;
    } else if (TAG_IS_LAYOUT(tag, Int)) { 
        hash = ((SmallInt)key)->value;
    } else {
        assert1(NULL, "Quick dictionaries only support ints and strings!");
        // make the compiler happy :)
        return 0;
    }
    hash %= self->data->size;
    return hash;
}

void push_hash(Optr key)
{
    SmallInt hash;
    Optr tag = GETTAG(key);
    if (TAG_IS_LAYOUT(tag, Words)) {
        hash = Symbol_hash((Symbol)key);
    } else if (TAG_IS_LAYOUT(tag, Int)) { 
        hash = (SmallInt)key;
    } else {
        Class_direct_dispatch(key, HEADER(key), (Optr)SMB_hash, 0);
        return;
    }
    PUSH_EXP(hash);
}

long unwrap_hash(Dictionary self, Optr w_hash)
{
    return unwrap_int(w_hash) % self->data->size;
}


static DictBucket * get_bucketp(Dictionary dictionary, long hash)
{
    return (DictBucket *)&dictionary->data->values[hash];
}

/* ========================================================================= *
 * Quick dictionary functions. Only for bootstrapping.                       *
 * ========================================================================= */

static long Dictionary_grow_check(Dictionary self)
{
    uns_int amount = self->size + 1;
    self->size     = amount;
    uns_int size   = self->data->size;
    return (100 * amount) / size > self->ratio;
}

static void Dictionary_quick_check_grow(Dictionary self)
{
    if (!Dictionary_grow_check(self)) { return; }

    Array old = self->data;
    self->data     = new_Array_withAll(old->size << 1, (Optr)nil);
    self->size     = 0;
    long i;
    for (i = 0; i < old->size; i++) {
        DictBucket bucket = (DictBucket)old->values[i];
        if (bucket == (DictBucket)nil) { continue; }
        long j;
        uns_int tally = bucket->tally;
        for (j = 0; j < tally; j=j+2) {
            Optr key = bucket->values[j];
            Dictionary_quick_store(self, key, bucket->values[j+1]);
        }
        
    }
}

void Dictionary_quick_store(Dictionary self,
                                 Optr key, Optr value)
{
    assert0(self != (Dictionary)nil);
    long hash = get_hash(self, key);
    DictBucket * bucketp = get_bucketp(self, hash);
    if (*bucketp == (DictBucket)nil) {
        *bucketp                     = new_bucket();
        DictBucket bucket = *bucketp;
        bucket->values[0]            = key;
        bucket->values[1]            = value;
        bucket->tally                = 2;
        return Dictionary_quick_check_grow(self);
    }
    if (Bucket_quick_store(bucketp, key, value)) {
        Dictionary_quick_check_grow(self);
    }
}

Optr Dictionary_quick_lookup(Dictionary self, Optr key)
{
    long hash = get_hash(self, key);
    DictBucket * bucketp = get_bucketp(self, hash);
    DictBucket bucket = *bucketp;
    if (bucket == (DictBucket)nil) {
        return NULL;
    }
    long i;
    uns_int tally = bucket->tally;
    for (i = 0; i < tally; i=i+2) {
        switch (Bucket_quick_compare_key(key, bucket->values[i]))
        {
            case -1: assert1(NULL, "Invalid key for quickstore!\n");
            case 1:
                return bucket->values[i+1];
        }
    }
    return NULL;
}

static void remove_from_bucket(uns_int idx, DictBucket bucket)
{
    uns_int tally = bucket->tally;
    bucket->values[idx]     = bucket->values[tally-2];
    bucket->values[idx+1]   = bucket->values[tally-1];
    bucket->values[tally-2] = nil;
    bucket->values[tally-1] = nil;
    bucket->tally = tally-2;
}

static void add_to_bucket(DictBucket * bucketp, Optr key, Optr value)
{
    if ((Optr)*bucketp == nil) {
        *bucketp = new_bucket();
    } else if ((*bucketp)->tally == (*bucketp)->size) {
        Bucket_grow(bucketp);
    }

    DictBucket b       = *bucketp;
    uns_int tally      = b->tally;
    b->values[tally]   = key;
    b->values[tally+1] = value;
    b->tally = tally+2;
}

threaded* tpush_hash(Optr key)
{
    SmallInt hash;
    Optr tag = GETTAG(key);
    if (TAG_IS_LAYOUT(tag, Words)) {
        hash = Symbol_hash((Symbol)key);
    } else if (TAG_IS_LAYOUT(tag, Int)) { 
        hash = (SmallInt)key;
    } else {
        return Class_direct_dispatch(key, HEADER(key), (Optr)SMB_hash, 0);
    }
    PUSH_EXP(hash);
    return NULL;
}

THREADED(bucket_rehash)
    Dictionary dict   = (Dictionary)PEEK_EXP(3);
    uns_int idx       = (uns_int)PEEK_EXP(1);
    DictBucket bucket = (DictBucket)PEEK_EXP(2);
    Optr key          = bucket->values[idx];
    threaded* next_pc = NULL;

    while(!next_pc) {
        Optr w_hash = pop_EXP();
        long hash   = unwrap_hash(dict, w_hash);
        DictBucket * bucketp = get_bucketp(dict, hash);
        if (*bucketp != bucket) {
            Optr value = bucket->values[idx + 1];
            remove_from_bucket(idx, bucket);
            add_to_bucket(bucketp, key, value);
        }
        idx += 2;
        if (idx >= bucket->tally) {
            return pc - 1;
        }
        POKE_EXP(0, idx);
        next_pc = tpush_hash(bucket->values[idx]);
    }

    return next_pc;
}

static THREADED(dict_grow)
    uns_int idx     = (uns_int)PEEK_EXP(3);
    Array old       = (Array)PEEK_EXP(4);

    if (idx == old->size) {
        ZAPN_EXP(5);
        return t_return(pc);
    }

    DictBucket bucket = (DictBucket)old->values[idx];
    POKE_EXP(3, idx + 1);

    if ((Optr)bucket == nil || bucket->tally == 0) {
        return pc;
    }

    POKE_EXP(1, bucket);
    POKE_EXP(0, 0);

    inc_pc(pc);

    threaded* next_pc = tpush_hash(bucket->values[0]);
    if (next_pc) {
        return next_pc;
    }
    return t_bucket_rehash(pc + 1);
}

NNATIVE(Dictionary_grow, 2,
    t_dict_grow,
    t_bucket_rehash)

static void Dictionary_grow(Dictionary self)
{
    Array old  = self->data;
    if (old->size >= 32) {
        self->data = new_Array_withAll(old->size << 1, (Optr)nil);
    } else {
        self->data = new_Array_withAll(old->size << 32, (Optr)nil);
    }
    self->size = 0;

    uns_int i = 0;
    for (i = 0; i < old->size; i++) {
        self->data->values[i] = old->values[i];
    }
    
    CLAIM_EXP(5);
    POKE_EXP(4, old);
    POKE_EXP(3, 0);
    POKE_EXP(2, self);
    push_code(T_Dictionary_grow);
}

/* ========================================================================= */

THREADED(push_hash)
    SmallInt hash;
    Optr key = PEEK_EXP(0);
    Optr tag = GETTAG(key);
    if (TAG_IS_LAYOUT(tag, Words)) {
        hash = Symbol_hash((Symbol)key);
    } else if (TAG_IS_LAYOUT(tag, Int)) { 
        hash = (SmallInt)key;
    } else {
        set_pc(pc + 1);
        return Class_direct_dispatch(key, HEADER(key), (Optr)SMB_hash, 0);
    }
    PUSH_EXP(hash);
    return pc + 1;
}

static threaded* Bucket_compare_key(Optr inkey, Optr dictkey)
{
    long result = Bucket_quick_compare_key(inkey, dictkey);

    if (result == -1) {
        return Class_direct_dispatch(inkey, HEADER(inkey),
                                     (Optr)SMB__equal, 1, dictkey);
    }
    PUSH_EXP(get_bool(result));
    return NULL;
}

THREADED(dictionary_bucket)
    Optr w_hash     = PEEK_EXP(0);
    Optr key        = PEEK_EXP(1);
    Dictionary self = (Dictionary)PEEK_EXP(2);
    long hash       = unwrap_hash(self, w_hash);

    DictBucket bucket = *get_bucketp(self, hash);

    if (bucket == (DictBucket)nil || bucket->tally == 0) {
        ZAPN_EXP(3);
        return pc + 3;
    }

    POKE_EXP(0, bucket);
    PUSH_EXP(0);
    set_pc(pc + 1);
    threaded * next_pc = Bucket_compare_key(key, bucket->values[0]);
    if (next_pc) {
        return next_pc;
    }
    return pc + 1;
}

THREADED(bucket_lookup)
    uns_int idx       = (uns_int)PEEK_EXP(1);
    DictBucket bucket = (DictBucket)PEEK_EXP(2);
    Optr key = PEEK_EXP(3);
    
    threaded* next_pc = NULL;
    while (!next_pc) {
        Optr boolean = PEEK_EXP(0);
        if (boolean == (Optr)true) {
            ZAPN_EXP(4);
            Optr result = bucket->values[idx + 1];
            POKE_EXP(0, result);
            return pc + 1;
        }    

        idx += 2;

        uns_int tally = bucket->tally;
        if (idx >= tally) {
            ZAPN_EXP(5);
            return pc + 2;
        }

        // zap boolean
        ZAP_EXP();

        POKE_EXP(0, idx);
        next_pc = Bucket_compare_key(key, bucket->values[idx]);
    }
    return next_pc;
}

THREADED(return_null)
    PUSH_EXP(NULL);
    return t_return(pc);
}

NNATIVE(iDictionary_at_, 5,
    t_push_hash,
    t_dictionary_bucket,
    t_bucket_lookup,
    t_return,
    t_return_null)

threaded* Dictionary_lookup_push(Dictionary dict, Optr msg)
{
    CLAIM_EXP(2);
    POKE_EXP(1, dict);
    POKE_EXP(0, msg);
    return push_code(T_iDictionary_at_);
}

/* ========================================================================= */

NNATIVE(Dictionary_at_, 5,
    t_push_hash,
    t_dictionary_bucket,
    t_bucket_lookup,
    t_return,
    t_return_nil)

NATIVE1(Dictionary_at_)
    push_code(T_Dictionary_at_);
}

THREADED(dictionary_ifAbsent_)
    t_return(pc);
    Optr block = PEEK_EXP(0);
    return apply(block, 0);
}

THREADED(pop_return)
    Optr result = pop_EXP();
    POKE_EXP(0, result);
    return t_return(pc);
}

NNATIVE(Dictionary_at_ifAbsent_, 5,
    t_push_hash,
    t_dictionary_bucket,
    t_bucket_lookup,
    t_pop_return,
    t_dictionary_ifAbsent_)

NATIVE2(Dictionary_at_ifAbsent_)
    Optr w_index = NATIVE_ARG(0);
    Optr w_block = NATIVE_ARG(1);
    POKE_EXP(2, w_block);
    POKE_EXP(1, self);
    POKE_EXP(0, w_index);
    push_code(T_Dictionary_at_ifAbsent_);
}

static void Bucket_store(DictBucket bucket, Optr key, Optr value, uns_int idx)
{
    bucket->values[idx]   = key;
    bucket->values[idx+1] = value;
}

THREADED(dictionary_store)
    t_return(pc);
    Optr w_hash     = PEEK_EXP(0);
    Dictionary self = (Dictionary)PEEK_EXP(4);
    long hash       = unwrap_hash(self, w_hash);
    Optr key        = PEEK_EXP(3);
    Optr value      = PEEK_EXP(2);

    DictBucket * bucketp = get_bucketp(self, hash);
    if (*bucketp == (DictBucket)nil || (*bucketp)->tally == 0) { 
        add_to_bucket(bucketp, key, value);
        ZAPN_EXP(3);
        POKE_EXP(0, value);
        return pc + 2;
    }

    set_pc(pc + 1);
    POKE_EXP(0, 0);
    POKE_EXP(1, bucketp);
    threaded* next_pc = Bucket_compare_key(key, (*bucketp)->values[0]);
    if (next_pc) {
        return next_pc;
    }
    return pc + 1;
}

THREADED(bucket_store)
    uns_int idx          = PEEK_EXP(1);
    DictBucket * bucketp = PEEK_EXP(2);
    Optr key             = PEEK_EXP(4);
    threaded* next_pc    = NULL;
    DictBucket bucket    = *bucketp;

    while (!next_pc) {
        Optr bool = PEEK_EXP(0);
        if (bool == true) {
            Optr value = PEEK_EXP(3);
            ZAPN_EXP(5);
            POKE_EXP(0, value);
            Bucket_store(bucket, key, value, idx);
            return t_return(pc);
        }
        
        idx += 2;

        if (idx >= bucket->tally) {
            Optr value = PEEK_EXP(3);
            add_to_bucket(bucketp, key, value);
            ZAPN_EXP(4);
            POKE_EXP(0, value);
            return pc + 1;
        }
        ZAP_EXP();
        POKE_EXP(0, idx);
        next_pc = Bucket_compare_key(key, bucket->values[idx]);
    }
    return next_pc;
}

THREADED(dictionary_check_grow)
    Optr value      = PEEK_EXP(0);
    Dictionary self = (Dictionary)PEEK_EXP(1);
    POKE_EXP(1, value);
    if (!Dictionary_grow_check(self)) {
        ZAP_EXP();
        return t_return(pc);
    }
    POKE_EXP(0, self);
    assert0(NULL);
}

NNATIVE(Dictionary_at_put_, 5,
    t_peek1,
    t_push_hash,
    t_dictionary_store,
    t_bucket_store,
    t_dictionary_check_grow);

NATIVE2(Dictionary_at_put_)
    push_code(T_Dictionary_at_put_);
}

NATIVE0(Dictionary_grow)
    Dictionary_grow((Dictionary)self);
}

void post_init_Dictionary()
{
    change_slot_type(Dictionary_Class, UIntSlot_Class, 3, 0,1,2);

    INIT_NATIVE(Dictionary_at_);
    INIT_NATIVE(Dictionary_at_put_);
    INIT_NATIVE(Dictionary_at_ifAbsent_);
    INIT_NATIVE(iDictionary_at_);
    INIT_NATIVE(Dictionary_grow);

    Dictionary natives = add_plugin(L"Collection.Dictionary");
    store_native(natives, SMB_at_put_,      NM_Dictionary_at_put_);
    store_native(natives, SMB_at_,          NM_Dictionary_at_);
    store_native(natives, SMB_at_ifAbsent_, NM_Dictionary_at_ifAbsent_);
    store_native(natives, SMB_grow,         NM_Dictionary_grow);
}
