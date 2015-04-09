//
//  cxFramework.c
//  cxFramework
//
//  Created by xuhua on 4/23/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <sys/time.h>
#include "utlist.h"
#include "cxBase.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if CX_TARGET_PLATFORM == CX_PLATFORM_IOS
#include <libkern/OSAtomic.h>
cxUInt32 cxAtomicAddInt32(cxInt32 *p, cxInt32 x)
{
    return OSAtomicAdd32((int32_t)x, (int32_t *)p);
}
cxUInt32 cxAtomicSubInt32(cxInt32 *p, cxInt32 x)
{
    return OSAtomicAdd32(-((int32_t)x), (int32_t *)p);
}
#elif CX_TARGET_PLATFORM == CX_PLATFORM_ANDROID
#include <sys/atomics.h>
cxUInt32 cxAtomicAddInt32(cxInt32 *p, cxInt32 x)
{
    return __sync_fetch_and_add(p,x);
}
cxUInt32 cxAtomicSubInt32(cxInt32 *p, cxInt32 x)
{
    return __sync_fetch_and_sub(p,x);
}
//mac
#elif CX_TARGET_PLATFORM == CX_PLATFORM_MAC
#include <libkern/OSAtomic.h>
cxUInt32 cxAtomicAddInt32(cxInt32 *p, cxInt32 x)
{
    return OSAtomicAdd32((int32_t)x, (int32_t *)p);
}
cxUInt32 cxAtomicSubInt32(cxInt32 *p, cxInt32 x)
{
    return OSAtomicAdd32(-((int32_t)x), (int32_t *)p);
}
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static cxAny cxMalloc(cxSize size)
{
    return calloc(1,size);
}

static cxAny cxRealloc(cxAny ptr,cxSize size)
{
    return realloc(ptr, size);
}

static cxAny cxCalloc(cxInt num,cxSize size)
{
    return calloc(num, size);
}

static void cxFree(cxAny ptr)
{
    free(ptr);
}

static cxChars cxStrdup(cxConstChars s)
{
    cxInt len = (cxInt)strlen(s);
    cxChars ret = cxMalloc(len + 1);
    memcpy(ret, s, len);
    ret[len]='\0';
    return ret;
}

const cxAllocator *allocator = &(cxAllocator){
    .malloc         = cxMalloc,
    .realloc        = cxRealloc,
    .calloc         = cxCalloc,
    .free           = cxFree,
    .strdup         = cxStrdup,
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void cxUtilInfo(cxConstChars file,cxInt line,cxConstChars format, ...)
{
    va_list ap;
    va_start(ap, format);
    cxUtilPrint("INFO", file, line, format, ap);
    va_end(ap);
}

void cxUtilError(cxConstChars file,cxInt line,cxConstChars format, ...)
{
    va_list ap;
    va_start(ap, format);
    cxUtilPrint("ERROR", file, line, format, ap);
    va_end(ap);
}

void cxUtilWarn(cxConstChars file,cxInt line,cxConstChars format, ...)
{
    va_list ap;
    va_start(ap, format);
    cxUtilPrint("WARN", file, line, format, ap);
    va_end(ap);
}

void cxUtilAssert(cxConstChars file,cxInt line,cxConstChars format, ...)
{
    va_list ap;
    va_start(ap, format);
    cxUtilPrint("ASSERT", file, line, format, ap);
    va_end(ap);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define CX_TYPE_NAME_LEN(s) cxInt nameLen = (cxInt)strlen(s)

static cxType types = NULL;

static void cxSignatureClear(cxSignature signatures)
{
    cxSignature ele,tmp;
    HASH_ITER(hh, signatures, ele, tmp){
        allocator->free(ele);
    }
    HASH_CLEAR(hh, signatures);
}
static void cxSignatureCloneToType(cxType ptype,cxConstChars name)
{
    cxSignatureNew(ptype, name);
}
cxSignature cxSignatureNew(cxType ptype,cxConstChars name)
{
    CX_TYPE_NAME_LEN(name);
    cxSignature out = NULL;
    HASH_FIND(hh, ptype->signatures, name, nameLen, out);
    CX_RETURN(out != NULL,out);
    cxSignature this = allocator->malloc(sizeof(struct cxSignature));
    strcpy(this->name, name);
    HASH_ADD(hh, ptype->signatures, name, nameLen, this);
    return this;
}
cxBool cxSignatureHas(cxSignature this,cxConstChars name)
{
    CX_RETURN(this == NULL,false);
    cxSignature out = NULL;
    CX_TYPE_NAME_LEN(name);
    HASH_FIND(hh, this, name, nameLen, out);
    return out != NULL;
}
static void cxPropertyClear(cxProperty propertys)
{
    cxProperty ele,tmp;
    HASH_ITER(hh, propertys, ele, tmp){
        allocator->free(ele);
    }
    HASH_CLEAR(hh, propertys);
}
cxProperty cxObjectGetProperty(cxAny object,cxConstChars name)
{
    cxObject this = object;
    return cxTypeGetProperty(this->cxType, name);
}
cxBool cxPropertyRunSetter(cxAny object,cxConstChars key,cxAny value)
{
    CX_ASSERT(key != NULL && object != NULL, "args error");
    cxProperty p = cxObjectGetProperty(object, key);
    if(p != NULL){
        CX_ASSERT(p->cxSetter != NULL, "setter null");
        ((void(*)(cxAny,cxConstChars,cxAny))p->cxSetter)(object,key,value);
        return true;
    }
    return false;
}
cxBool cxPropertyRunGetter(cxAny object,cxConstChars key,cxAny *value)
{
    CX_ASSERT(key != NULL && object != NULL, "args error");
    cxProperty p = cxObjectGetProperty(object, key);
    if(p != NULL){
        CX_ASSERT(p->cxGetter != NULL, "getter null");
        *value = ((cxAny(*)(cxAny,cxConstChars))p->cxGetter)(object,key);
        return true;
    }
    return false;
}
static void cxPropertyCloneToType(cxType ptype,cxProperty pold)
{
    cxProperty pnew = cxPropertyNew(ptype, pold->name);
    pnew->cxGetter = pold->cxGetter;
    pnew->cxSetter = pold->cxSetter;
}
cxProperty cxPropertyNew(cxType ptype,cxConstChars name)
{
    CX_TYPE_NAME_LEN(name);
    cxProperty out = NULL;
    HASH_FIND(hh, ptype->propertys, name, nameLen, out);
    CX_RETURN(out != NULL,out);
    cxProperty this = allocator->malloc(sizeof(struct cxProperty));
    strcpy(this->name, name);
    HASH_ADD(hh, ptype->propertys, name, nameLen, this);
    return this;
}
static void MethodClear(cxMethod methods)
{
    cxMethod ele,tmp;
    HASH_ITER(hh, methods, ele, tmp){
        allocator->free(ele);
    }
    HASH_CLEAR(hh, methods);
}
//clone method to ptype
static void cxMethodCloneToType(cxType ptype,cxMethod pold)
{
    cxMethod pnew = cxMethodNew(ptype, pold->name);
    pnew->cxMethodFunc = pold->cxMethodFunc;
}
cxMethod cxMethodNew(cxType ptype,cxConstChars name)
{
    CX_TYPE_NAME_LEN(name);
    cxMethod out = NULL;
    HASH_FIND(hh, ptype->methods, name, nameLen, out);
    CX_RETURN(out != NULL,out);
    cxMethod this = allocator->malloc(sizeof(struct cxMethod));
    strcpy(this->name, name);
    HASH_ADD(hh, ptype->methods, name, nameLen, this);
    return this;
}
void cxTypesInit()
{
    types = NULL;
    CX_SET_TYPE(cxObject);
    CX_SET_TYPE(cxStr);
    CX_SET_TYPE(cxMemPool);
    CX_SET_TYPE(cxHash);
    CX_SET_TYPE(cxArray);
    CX_SET_TYPE(cxList);
    CX_SET_TYPE(cxStack);
    CX_SET_TYPE(cxNumber);
    CX_SET_TYPE(cxMessageItem);
    CX_SET_TYPE(cxMessage);
    CX_SET_TYPE(cxLoader);
    CX_SET_TYPE(cxPath);
    CX_SET_TYPE(cxStream);
    CX_SET_TYPE(cxAnyArray);
    CX_SET_TYPE(cxRegex);
    CX_SET_TYPE(cxConvert);
    CX_SET_TYPE(cxJson);
}
void cxTypesFree()
{
    cxType ele,tmp;
    HASH_ITER(hh, types, ele, tmp){
        cxSignatureClear(ele->signatures);
        cxPropertyClear(ele->propertys);
        MethodClear(ele->methods);
        allocator->free(ele->UI);
        allocator->free(ele);
    }
    HASH_CLEAR(hh, types);
}
cxProperty cxTypeGetProperty(cxType this,cxConstChars name)
{
    CX_ASSERT(this != NULL, "args error");
    cxProperty out = NULL;
    CX_TYPE_NAME_LEN(name);
    HASH_FIND(hh, this->propertys, name, nameLen, out);
    return out;
}
cxMethod cxTypeGetMethod(cxType this,cxConstChars name)
{
    CX_ASSERT(this != NULL, "args error");
    cxMethod out = NULL;
    CX_TYPE_NAME_LEN(name);
    HASH_FIND(hh, this->methods, name, nameLen, out);
    return out;
}
cxAny cxTypesCreateObject(cxConstType name)
{
    cxType this = cxTypesGetType(name);
    return this != NULL ? this->Create() : NULL;
}
cxAny cxTypesAllocObject(cxConstType name)
{
    cxType this = cxTypesGetType(name);
    return this != NULL ? this->Alloc() : NULL;
}
cxType cxTypesGetType(cxConstType name)
{
    cxType out = NULL;
    CX_TYPE_NAME_LEN(name);
    HASH_FIND(hh, types, name, nameLen, out);
    return out;
}
static void cxTypeCopyFromSuper(cxType this,cxType super)
{
    CX_RETURN(super == NULL);
    //copy signatures
    cxSignature sele,stmp;
    HASH_ITER(hh, super->signatures, sele, stmp){
        cxSignatureCloneToType(this, sele->name);
    }
    //copy methods
    cxMethod mele,mtmp;
    HASH_ITER(hh, super->methods, mele, mtmp){
        cxMethodCloneToType(this, mele);
    }
    //copy propertys
    cxProperty pele,ptmp;
    HASH_ITER(hh, super->propertys, pele, ptmp){
        cxPropertyCloneToType(this, pele);
    }
}
void cxTypeBindUI(cxType this,cxConstChars path)
{
    CX_ASSERT(this->UI == NULL, "repeat bind type UI");
    CX_ASSERT(cxConstCharsOK(path), "path error");
    this->UI = allocator->strdup(path);
}
cxType cxTypeNew(cxConstType ct,cxConstType sb,cxAny (*create)(),cxAny (*alloc)(),void (*autoType)(cxType),cxInt typeSizeof)
{
    cxType this = allocator->malloc(sizeof(struct cxType));
    this->typeName = ct;
    this->typeSizeof = typeSizeof;
    cxType super = cxTypesGetType(sb);
    CX_ASSERT(cxConstCharsEqu(ct, cxObjectTypeName) || super != NULL, "super type %s not register",sb);
    strcpy(this->name, ct);
    this->Alloc = alloc;
    this->Create = create;
    this->superType = super;
    CX_TYPE_NAME_LEN(ct);
    HASH_ADD(hh, types, name, nameLen, this);
    cxTypeCopyFromSuper(this, super);
    cxSignatureNew(this, ct);
    autoType(this);
    cxTypeFinished(this);
    return this;
}
cxBool cxMethodExists(cxConstType type,cxConstChars name)
{
    CX_RETURN(type == NULL,false);
    cxType ptype = cxTypesGetType(type);
    CX_ASSERT(ptype != NULL, "type %s not regsiter",type);
    cxMethod pmethod = cxTypeGetMethod(ptype, name);
    return pmethod != NULL;
}
cxBool cxMethodHas(cxAny object,cxConstChars name)
{
    CX_RETURN(object == NULL,false);
    cxObject this = object;
    cxMethod pmethod = cxTypeGetMethod(this->cxType, name);
    return pmethod != NULL;
}
cxAny cxMethodGet(cxAny pobj,cxConstChars name)
{
    cxObject this = pobj;
    CX_ASSERT(this->cxType != NULL, "object type null ");
    cxMethod pmethod = cxTypeGetMethod(this->cxType, name);
    CX_ASSERT(pmethod != NULL, "%s.%s method get failed",CX_NAME_OF(this),name);
    return pmethod->cxMethodFunc;
}
cxAny cxMethodSuper(cxAny pobj,cxConstType type,cxConstChars name)
{
    CX_ASSERT(cxInstanceOf(pobj, type), "object not has, %s base type",type);
    cxType ptype = cxTypesGetType(type);
    CX_ASSERT(ptype != NULL, "%s type get failed",type);
    cxMethod pmethod = cxTypeGetMethod(ptype, name);
    CX_ASSERT(pmethod != NULL, "%s.%s method get failed",type,name);
    return pmethod->cxMethodFunc;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

cxBool cxValidPointer(cxAny pobj,cxAny pointer)
{
    CX_ASSERT_THIS(pobj, cxObject);
    return pointer >= pobj && pointer < (pobj + this->cxType->typeSizeof);
}

cxBool cxInstanceOf(cxAny pobj,cxConstType type)
{
    cxObject this = pobj;
    CX_RETURN(this == NULL && type == NULL,true);
    CX_RETURN(this == NULL || type == NULL, false);
    CX_ASSERT(this->cxType != NULL, "object type null,must is object %s",type);
    CX_RETURN(this->cxType->typeName == type, true);
    return cxSignatureHas(this->cxType->signatures, type);
}

//auto remove append message
static void cxMessageRemove(cxAny dst)
{
    cxMessage this = cxMessageInstance();
    CX_HASH_FOREACH(this->keys, keye, keyt){
        cxHash list = keye->any;
        CX_HASH_FOREACH(list, vale, valt){
            cxMessageItem item = vale->any;
            if(item->dst != dst){
                continue;
            }
            cxHashDelElement(list, vale);
        }
    }
}

void __cxObjectAutoInit(cxObject this)
{
    
}
void __cxObjectAutoFree(cxObject this)
{
    if(this->Attr & cxObjectAttrMessage){
        cxMessageRemove(this);
    }
}
//when object create and init finished
CX_METHOD_DEF(cxObject, cxInit, void)
{
    
}
//when object free finished
CX_METHOD_DEF(cxObject, cxFree, void)
{
    
}
//Serialize to json
CX_METHOD_DEF(cxObject, Serialize, cxJson)
{
    cxJson json = cxJsonCreateObject();
    cxJsonSetConstChars(json, "cxType", CX_NAME_OF(this));
    return json;
}
void __cxObjectAutoType(cxType this)
{
    CX_METHOD(cxObject, Serialize);
    CX_METHOD(cxObject, cxInit);
    CX_METHOD(cxObject, cxFree);
}
cxAny cxObjectSerialize(cxAny pobj)
{
    CX_ASSERT_THIS(pobj, cxObject);
    return CX_CALL(this, Serialize, CX_M(cxJson));
}
cxDouble cxTimestamp()
{
    struct timeval val = {0};
    gettimeofday(&val, NULL);
    return val.tv_sec + (cxDouble)val.tv_usec/(cxDouble)1000000.0;
}
void cxCoreInit()
{
    cxSetRandSeed();
    cxTypesInit();
    cxMemPoolInit();
    cxLoaderInit();
    cxMessageInstance();
}

void cxCoreClear()
{
    cxMessageClear();
    cxLoaderClear();
    cxMemPoolClear();
}

void cxCoreFree()
{
    cxMessageDestroy();
    cxLoaderFree();
    cxMemPoolFree();
    cxTypesFree();
}

void __cxObjectRetain(cxAny ptr)
{
    CX_RETURN(ptr == NULL);
    cxObject object = (cxObject)ptr;
    CX_ASSERT(object->cxRefcount > 0, "retain count must > 0");
    cxAtomicAddInt32(&object->cxRefcount, 1);
}
void __cxObjectRelease(cxAny ptr)
{
    CX_RETURN(ptr == NULL);
    cxObject this = (cxObject)ptr;
    CX_ASSERT(this->cxRefcount > 0, "error,retain count must > 0");
    cxAtomicSubInt32(&this->cxRefcount, 1);
    CX_RETURN(this->cxRefcount > 0);
    this->cxAutoFree(this);
    CX_CALL(this, cxFree, CX_M(void));
    allocator->free(this);
}
cxAny __cxObjectAutoRelease(cxAny ptr)
{
    return cxMemPoolAppend(ptr);
}





