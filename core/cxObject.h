//
//  cxObject.h
//  cxEngine
//
//  Created by xuhua on 12/27/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxObject_h
#define cxEngine_cxObject_h

#include "cxConfig.h"
#include "cxAllocator.h"

CX_C_BEGIN

typedef enum {
    cxBaseTypeObject,
    cxBaseTypeView,
    cxBaseTypeAction,
    cxBaseTypeStream,
    cxBaseTypeTexture,
    cxBaseTypeShader,
}cxBaseType;

typedef void (*cxObjectFunc)(cxPointer this);

typedef cxAny (*cxAnyFunc)(cxAny object);

typedef struct {
    xmlTextReaderPtr reader;
    cxAny root;
    cxAny object;
}cxReaderAttrInfo;

#define cxReaderAttrInfoMake(reader,root,object)    &(cxReaderAttrInfo){reader,root,object}

typedef void (*cxReadAttrFunc)(cxReaderAttrInfo *info);

cxAny cxObjectAlloc(cxConstType type,int size,cxObjectFunc initFunc,cxObjectFunc freeFunc);

cxAny cxObjectCreate(cxConstType type,int size,cxObjectFunc initFunc,cxObjectFunc freeFunc);

void cxObjectRetain(cxAny ptr);

void cxObjectRelease(cxAny ptr);

cxAny cxObjectAutoRelease(cxAny ptr);

void cxLuaLoad(cxConstType name, const luaL_Reg *methods);

cxBool cxObjectIsBaseType(cxAny pobj,cxBaseType type);

cxBool cxObjectIsType(cxAny pobj,cxConstType type);

cxConstType cxObjectType(cxAny pobj);

cxInt cxObjectBind(cxAny obj);

void cxObjectReadAttr(cxReaderAttrInfo *info);

void cxObjectSetReadAttrFunc(cxAny obj,cxReadAttrFunc func);

void cxObjectReadAttrRun(cxReaderAttrInfo *info);

cxAny cxObjectRoot(cxAny obj);

void cxObjectSetRoot(cxAny obj,cxAny root);

void cxObjectSetTag(cxAny obj,cxInt tag);

cxInt cxObjectGetTag(cxAny obj);

#define CX_RETURN(cond,...)         if(cond)return __VA_ARGS__

#define CX_BREAK(cond)              if(cond)break

#define CX_CONTINUE(cond)           if(cond)continue

#define CX_OBJECT_DEF(_t_,_b_)      CX_OBJECT_BEG(_t_) struct _b_ super;

#define CX_OBJECT_INIT(_t_,_b_)     void __##_t_##AutoInit(_t_ this){__##_b_##AutoInit((_b_)this);{

#define CX_OBJECT_FREE(_t_,_b_)     }};void __##_t_##AutoFree(_t_ this){{

#define CX_OBJECT_TERM(_t_,_b_)     }__##_b_##AutoFree((_b_)this);}

#define CX_ALLOC(_t_)               cxObjectAlloc(_t_##TypeName,sizeof(struct _t_),(cxObjectFunc)__##_t_##AutoInit,(cxObjectFunc)__##_t_##AutoFree)

#define CX_CREATE(_t_)              cxObjectCreate(_t_##TypeName,sizeof(struct _t_),(cxObjectFunc)__##_t_##AutoInit,(cxObjectFunc)__##_t_##AutoFree)

#define CX_RETAIN(_o_)              cxObjectRetain(_o_)

#define CX_RELEASE(_o_)             cxObjectRelease(_o_)

#define CX_AUTOFREE(_o_)            cxObjectAutoRelease(_o_)

#define CX_RETAIN_SWAP(_s_,_d_)     {CX_RELEASE(_s_);(_s_)=(cxAny)(_d_);CX_RETAIN(_s_);}

#define CX_LUA_GET_PTR(i)           (lua_isuserdata(gL, i)?lua_touserdata(gL, i):NULL)

#define CX_LUA_PROPERTY(t,n)        {"Set"#n,t##LuaSet##n},{"Get"#n,t##LuaGet##n}

#define CX_LUA_NUMBER_IS_INT(n)     (((lua_Number)((cxInt)(n))) == (n))

#define CX_LUA_LOAD_TYPE(t)         cxLuaLoad(t##TypeName,t##LuaMethods)

#define CX_LUA_DEF_THIS(t)          t this = CX_LUA_GET_PTR(1);CX_ASSERT(this != NULL,"get this error")

#define CX_LUA_CREATE_THIS(t)       t this = CX_CREATE(t)

#define CX_LUA_ALLOC_THIS(t)        t this = CX_ALLOC(t)

#define CX_LUA_PUSH_THIS(t)         lua_pushlightuserdata(gL,this);return 1

#define CX_LUA_METHOD_BEG(_t_)      const luaL_Reg _t_##LuaMethods[] = {

#define CX_LUA_METHOD_END(_t_)      {"Alloc",__##_t_##LuaAlloc},{"Create",__##_t_##LuaCreate},{NULL,NULL}};

#define CX_LUA_PUSH_OBJECT(o)       ((o) != NULL) ? lua_pushlightuserdata(gL,o) : lua_pushnil(gL)

//object

#define CX_OBJECT_BEG(_t_)                                          \
static CX_UNUSED_ATTRIBUTE cxConstType _t_##TypeName = #_t_;        \
typedef struct _t_ *_t_;                                            \
void __##_t_##AutoInit(_t_ this);                                   \
void __##_t_##AutoFree(_t_ this);                                   \
void __##_t_##TypeInit();                                           \
extern const luaL_Reg _t_##LuaMethods[];                            \
struct _t_ {

#define CX_OBJECT_END(_t_) };                                       \
static inline cxInt __##_t_##LuaAlloc(lua_State *L)                 \
{                                                                   \
    CX_LUA_ALLOC_THIS(_t_);                                         \
    CX_LUA_PUSH_THIS(_t_);                                          \
}                                                                   \
static inline cxInt __##_t_##LuaCreate(lua_State *L)                \
{                                                                   \
    CX_LUA_CREATE_THIS(_t_);                                        \
    CX_LUA_PUSH_THIS(_t_);                                          \
}

extern lua_State *gL;

//method

#define CX_METHOD_ALLOC(_r_,_n_,...)    _r_ (*_n_)(__VA_ARGS__)

#define CX_METHOD_RELEASE(_m_)          _m_ = NULL

#define CX_METHOD_FIRE(_d_,_m_,...)     ((_m_) != NULL)?((_m_)(__VA_ARGS__)):(_d_)

#define CX_METHOD_OVERRIDE(_m_,_f_)     _m_ = _f_

//base type define
CX_OBJECT_BEG(cxObject)
    cxConstType cxType;
    cxBaseType cxBase;
    cxUInt cxRefcount;
    cxObjectFunc cxFree;
    cxInt cxTag;
    cxInt cxBind;           //bind to lua table
    cxAny cxRoot;
    CX_METHOD_ALLOC(void, ReadAttr, cxReaderAttrInfo *);
CX_OBJECT_END(cxObject)

CX_OBJECT_DEF(cxMemory, cxObject)
    cxAny pointer;
CX_OBJECT_END(cxMemory)

cxAny cxMemoryCreate(cxInt size);

CX_C_END

#endif
