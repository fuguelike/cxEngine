//
//  cxHash.h
//  cxCore
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxCore_cxHash_h
#define cxCore_cxHash_h

#include "cxCore.h"
#include <uthash.h>

CX_C_BEGIN

#define CX_HASH_MAX_KEY  64

#define CX_HASH_FOREACH(_hash_,_ele_,_tmp_)         \
cxHashElement *_ele_ = NULL,*_tmp_=NULL;            \
HASH_ITER(hh, _hash_->hashPtr, _ele_, _tmp_)

typedef struct {
    cxAny any;
    cxChar key[CX_HASH_MAX_KEY+1];
    UT_hash_handle hh;
}cxHashElement;

#define cxHashElementKeyToLong(_e_)     *(cxLong *)((_e_)->key)

#define cxHashElementKeyToAny(_e_)      (cxAny)cxHashElementKeyToLong(_e_)

#define cxHashElementKeyToInt(_e_)      *(cxInt *)((_e_)->key)

#define cxHashElementKeyToStr(_e_)      (_e_)->key

typedef struct {
    cxInt length;
    cxChars data;
}cxHashKey;

typedef cxInt (*cxHashCmpFunc)(cxHashElement *lp,cxHashElement *rp);

#define cxHashIntKey(_v_)       (cxHashKey){.length=sizeof(cxInt),.data=(cxChars)&(cxInt){_v_}}

#define cxHashLongKey(_v_)      (cxHashKey){.length=sizeof(cxLong),.data=(cxChars)&(cxLong){_v_}}

#define cxHashAnyKey(_p_)       (cxHashKey){.length=sizeof(cxAny),.data=(cxChars)&(cxLong){(cxLong)(_p_)}}

#define cxHashStrKey(_s_)       (cxHashKey){.length=(cxInt)strlen(_s_),.data=(cxChars)(_s_)}

CX_OBJECT_DEF(cxHash, cxObject)
    cxHashElement *hashPtr;
CX_OBJECT_END(cxHash, cxObject)

#define cxHashSort(_hash_,_cmp_)    HASH_SORT(_hash_,_cmp_)

void    cxHashClean(cxHash hash);

cxInt   cxHashLength(cxHash hash);

void    cxHashDelElement(cxHash hash,cxHashElement *element);

cxHashElement *cxHashGetElement(cxHash hash,cxHashKey key);

cxBool  cxHashHas(cxHash hash,cxHashKey key);

cxBool  cxHashDel(cxHash hash,cxHashKey key);

cxBool  cxHashSet(cxHash hash,cxHashKey key,cxAny any);

cxAny   cxHashGet(cxHash hash,cxHashKey key);

cxAny   cxHashFirst(cxHash hash);

CX_C_END

#endif























