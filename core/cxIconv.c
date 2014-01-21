//
//  cxIconv.c
//  cxEngine
//
//  Created by xuhua on 10/20/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxTypes.h"
#include "cxIconv.h"

static cxIconv instance = NULL;

static cxInt cxIconvLuaIconv(lua_State *L)
{
    cxString s = cxLuaObjectValue(L, 1);
    if(!cxObjectIsType(s, cxStringTypeName)){
        luaL_error(L, "args error");
        return 0;
    }
    cxConstChars from = luaL_checkstring(L, 2);
    cxConstChars to = luaL_checkstring(L, 3);
    cxString d = cxIconvConvert(s, from, to);
    CX_LUA_PUSH_OBJECT(d);
    return 1;
}

CX_LUA_METHOD_BEG(cxIconv)
    {"Iconv",cxIconvLuaIconv},
CX_LUA_METHOD_END(cxIconv)

void __cxIconvTypeInit()
{
    CX_LUA_LOAD_TYPE(cxIconv);
}

CX_OBJECT_INIT(cxIconvItem, cxObject)
{
    
}
CX_OBJECT_FREE(cxIconvItem, cxObject)
{
    iconv_close(this->iconvptr);
    CX_RELEASE(this->from);
    CX_RELEASE(this->to);
}
CX_OBJECT_TERM(cxIconvItem, cxObject)

static cxIconvItem cxIconvItemCreate(cxConstChars from,cxConstChars to)
{
    cxIconvItem this = CX_CREATE(cxIconvItem);
    this->from = cxStringAllocChars(from);
    this->to = cxStringAllocChars(to);
    this->iconvptr = iconv_open(to,from);
    CX_ASSERT(this->iconvptr != NULL, "from(%s) -> to(%s) failed",from,to);
    return this;
}

static cxString cxIconvItemConvert(cxIconvItem this,const cxString string)
{
    CX_RETURN(string == NULL, NULL);
    cxSize insiz = cxStringLength(string);
    cxSize allocsize = insiz * 4 + 1;
    cxSize outsiz = allocsize;
    cxSize outlen = 0;
    cxChar *inptr = (cxChar *)cxStringBody(string);
    cxChar *outptr = allocator->malloc(outsiz);
    cxChar *buffer = outptr;
    int ret = iconv(this->iconvptr, &inptr, (size_t *)&insiz, &outptr, (size_t *)&outsiz);
    if(ret == 0){
        outlen = allocsize - outsiz;
        outptr[outlen] = '\0';
        return cxStringAttach(buffer, outlen);
    }
    allocator->free(buffer);
    return NULL;
}

void cxIconvDestroy()
{
    CX_RELEASE(instance);
    instance = NULL;
}

cxIconv cxIconvInstance()
{
    if(instance == NULL){
        instance = CX_ALLOC(cxIconv);
    }
    return instance;
}

CX_OBJECT_INIT(cxIconv, cxObject)
{
    this->caches = CX_ALLOC(cxHash);
}
CX_OBJECT_FREE(cxIconv, cxObject)
{
    CX_RELEASE(this->caches);
}
CX_OBJECT_TERM(cxIconv, cxObject)

cxString cxIconvConvertUTF8ToUTF16(const cxString string)
{
    return cxIconvConvert(string, "UTF-8", "UTF-16LE");
}

cxString cxIconvConvert(const cxString string,cxConstChars from,cxConstChars to)
{
    cxIconv this = cxIconvInstance();
    cxConstChars key = CX_CONST_STRING("%s->%s",from,to);
    cxIconvItem item = cxHashGet(this->caches, cxHashStrKey(key));
    if(item == NULL){
        item = cxIconvItemCreate(from,to);
        cxHashSet(this->caches, cxHashStrKey(key), item);
    }
    CX_RETURN(item == NULL, NULL);
    return cxIconvItemConvert(item,string);
}



