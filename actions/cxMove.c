//
//  cxMove.c
//  cxEngine
//
//  Created by xuhua on 10/10/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxActionRoot.h>
#include "cxMove.h"

const luaL_Reg cxMoveInstanceMethods[] = {
    
    CX_LUA_SUPER(cxAction)
};

static cxInt cxMoveLuaMake(lua_State *L)
{
    CX_LUA_NEW_THIS(cxMove);
    this->super.duration = luaL_checknumber(L, 1);
    this->endPos = cxLuaGetVec2fv(L, 2, cxVec2fv(0, 0));
    CX_LUA_RET_THIS(cxMove);
}

const luaL_Reg cxMoveTypeMethods[] = {
    {"make",cxMoveLuaMake},//make(time,{x=100,y=200})
    CX_LUA_TYPE(cxMove)
};

void cxMoveTypeInit()
{
    CX_LUA_LOAD_TYPE(cxMove);
}

static void cxMoveInit(cxAny pav)
{
    cxMove this = pav;
    CX_ASSERT(this->super.view != NULL, "view not set");
    this->prevPos = this->begPos = this->super.view->position;
    kmVec2Subtract(&this->posDelta, &this->endPos, &this->begPos);
}

void cxMoveSetPos(cxAny pav,cxVec2f pos)
{
    cxMove this = pav;
    this->endPos = pos;
}

static void cxMoveStep(cxAny pav,cxFloat dt,cxFloat time)
{
    cxMove this = pav;
    cxVec2f npos;
    cxVec2f diff;
    kmVec2Subtract(&diff, &this->super.view->position, &this->prevPos);
    kmVec2Add(&this->begPos, &this->begPos, &diff);
    kmVec2Scale(&npos, &this->posDelta, time);
    kmVec2Add(&npos, &this->begPos, &npos);
    this->prevPos = npos;
    cxViewSetPos(this->super.view, npos);
}

static void cxMoveReadAttr(cxAny rootAction,cxAny mAction, xmlTextReaderPtr reader)
{
    cxActionRoot root = rootAction;
    cxActionReadAttr(rootAction, mAction, reader);
    cxMove this = mAction;
    this->endPos.x = cxXMLReadFloatAttr(reader, root->functions, "cxMove.x", this->endPos.x);
    this->endPos.y = cxXMLReadFloatAttr(reader, root->functions, "cxMove.y", this->endPos.y);
}

CX_OBJECT_INIT(cxMove, cxAction)
{
    cxObjectSetReadAttrFunc(this, cxMoveReadAttr);
    CX_METHOD_SET(this->super.Init, cxMoveInit);
    CX_METHOD_SET(this->super.Step, cxMoveStep);
}
CX_OBJECT_FREE(cxMove, cxAction)
{
    
}
CX_OBJECT_TERM(cxMove, cxAction)

cxMove cxMoveCreate(cxFloat dutation, cxVec2f endpos)
{
    cxMove this = CX_CREATE(cxMove);
    this->super.duration = dutation;
    this->endPos = endpos;
    return this;
}
