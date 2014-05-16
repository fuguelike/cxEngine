//
//  cxHash.h
//  cxCore
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxCore_cxHash_h
#define cxCore_cxHash_h

#include "uthash.h"
#include "cxCore.h"

CX_C_BEGIN

#define CX_HASH_MAX_KEY_LENGTH  128

#define CX_HASH_FOREACH(_hash_,_ele_,_tmp_)         \
cxHashElement *_ele_ = NULL,*_tmp_=NULL;            \
HASH_ITER(hh, _hash_->hashPtr, _ele_, _tmp_)

typedef struct {
    cxAny any;
    cxChar key[CX_HASH_MAX_KEY_LENGTH+1];
    UT_hash_handle hh;
}cxHashElement;

typedef struct {
    cxInt length;
    cxChar *data;
}cxHashKey;

typedef int (*cxHashCmpFunc)(cxHashElement *lp,cxHashElement *rp);

#define cxHashIntKey(_v_)       (cxHashKey){.length=sizeof(cxInt),.data=(cxChar *)&(cxInt){_v_}}

#define cxHashPtrKey(_p_)       (cxHashKey){.length=sizeof(cxAny),.data=(cxChar *)&(cxLong){(cxLong)(_p_)}}

#define cxHashStrKey(_s_)       (cxHashKey){.length=(cxInt)strlen(_s_),.data=(cxChar *)(_s_)}

#define cxHashMemKey(_d_,_l_)   (cxHashKey){.length=(_l_),.data=(cxChar *)(_d_)}

CX_OBJECT_DEF(cxHash, cxObject)
    cxHashElement *hashPtr;
CX_OBJECT_END(cxHash)

#define cxHashSort(_hash_,_cmp_)    HASH_SORT(_hash_,_cmp_)

void    cxHashClean(cxHash hash);

cxInt   cxHashLength(cxHash hash);

void    cxHashDelElement(cxHash hash,cxHashElement *element);

cxHashElement *cxHashGetElement(cxHash hash,cxHashKey key);

cxBool  cxHashHas(cxHash hash,cxHashKey key);

void    cxHashDel(cxHash hash,cxHashKey key);

void    cxHashSet(cxHash hash,cxHashKey key,cxAny any);

cxAny   cxHashGet(cxHash hash,cxHashKey key);

cxAny   cxHashFirst(cxHash hash);

CX_C_END

#endif























