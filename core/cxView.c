//
//  cxView.c
//  cxEngine
//
//  Created by xuhua on 10/2/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <actions/cxTimer.h>
#include <kazmath/matrix.h>
#include "cxView.h"
#include "cxEngine.h"
#include "cxOpenGL.h"
#include "cxAction.h"
#include "cxViewRoot.h"

static cxInt cxViewLuaAppendView(lua_State *L)
{
    CX_LUA_DEF_THIS(cxView);
    cxViewAppend(this, CX_LUA_GET_PTR(2));
    return 0;
}

static cxInt cxViewLuaAppendEvent(lua_State *L)
{
    CX_LUA_EVENT_BEG(cxView);
    CX_LUA_EVENT_APPEND(onEnter);
    CX_LUA_EVENT_APPEND(onExit);
    CX_LUA_EVENT_APPEND(onResize);
    CX_LUA_EVENT_APPEND(onLayout);
    CX_LUA_EVENT_APPEND(onDirty);
    CX_LUA_EVENT_APPEND(onUpdate);
    CX_LUA_EVENT_END(cxView);
}

static cxInt cxViewLuaAppendAction(lua_State *L)
{
    CX_LUA_DEF_THIS(cxView);
    cxAction action = CX_LUA_GET_PTR(2);
    cxUInt id = cxViewAppendAction(this, action);
    lua_pushnumber(L, id);
    return 1;
}

static cxInt cxViewLuaCreateTimer(lua_State *L)
{
    CX_LUA_DEF_THIS(cxView);
    cxFloat freq = luaL_checknumber(L, 2);
    cxInt repeat = luaL_checkinteger(L, 3);
    cxAny timer = cxViewAppendTimer(this, freq, repeat);
    CX_LUA_PUSH_OBJECT(timer);
    return 1;
}

static cxInt cxViewLuaSetColor(lua_State *L)
{
    CX_LUA_DEF_THIS(cxView);
    cxColor4f color = cxLuaColor4fValue(L, 2, this->color);
    cxViewSetAlpha(this, color.a);
    cxViewSetColor(this, cxColor3fv(color.r, color.g, color.b));
    lua_pushvalue(L, 1);
    return 1;
}

static cxInt cxViewLuaGetColor(lua_State *L)
{
    CX_LUA_DEF_THIS(cxView);
    cxLuaPushColor4f(L, this->color);
    return 1;
}

static cxInt cxViewLuaGetPosition(lua_State *L)
{
    CX_LUA_DEF_THIS(cxView);
    cxLuaPushVec2fv(L, this->position);
    return 1;
}

static cxInt cxViewLuaGetSize(lua_State *L)
{
    CX_LUA_DEF_THIS(cxView);
    cxLuaPushSize2fv(L, this->size);
    return 1;
}

static cxInt cxViewLuaGetScale(lua_State *L)
{
    CX_LUA_DEF_THIS(cxView);
    cxLuaPushVec2fv(L, this->scale);
    return 1;
}

static cxInt cxViewLuaGetFixScale(lua_State *L)
{
    CX_LUA_DEF_THIS(cxView);
    cxLuaPushVec2fv(L, this->fixscale);
    return 1;
}

static cxInt cxViewLuaSetSize(lua_State *L)
{
    cxInt top = lua_gettop(L);
    CX_LUA_DEF_THIS(cxView);
    cxSize2f size = this->size;
    if(lua_istable(L, 2)){
        size = cxLuaSize2fValue(L, 2, this->size);
    }else if(top == 3){
        size.w = luaL_checknumber(L, 2);
        size.h = luaL_checknumber(L, 3);
    }else{
        luaL_error(L, "args error");
    }
    cxViewSetSize(this, size);
    lua_pushvalue(L, 1);
    return 1;
}

static cxInt cxViewLuaSetPosition(lua_State *L)
{
    cxInt top = lua_gettop(L);
    CX_LUA_DEF_THIS(cxView);
    cxVec2f pos = this->position;
    if(lua_istable(L, 2)){
        pos = cxLuaVec2fValue(L, 2, this->position);
    }else if(top == 3){
        pos.x = luaL_checknumber(L, 2);
        pos.y = luaL_checknumber(L, 3);
    }else{
        luaL_error(L, "args error");
    }
    cxViewSetPos(this, pos);
    lua_pushvalue(L, 1);
    return 1;
}

static cxInt cxViewLuaSetScale(lua_State *L)
{
    cxInt top = lua_gettop(L);
    CX_LUA_DEF_THIS(cxView);
    cxVec2f scale = this->scale;
    if(lua_istable(L, 2)){
        scale = cxLuaVec2fValue(L, 2, this->position);
    }else if(top == 3){
        scale.x = luaL_checknumber(L, 2);
        scale.y = luaL_checknumber(L, 3);
    }else{
        luaL_error(L, "args error");
    }
    cxViewSetScale(this, scale);
    lua_pushvalue(L, 1);
    return 1;
}

static cxInt cxViewLuaSetFixScale(lua_State *L)
{
    cxInt top = lua_gettop(L);
    CX_LUA_DEF_THIS(cxView);
    cxVec2f fixscale = this->fixscale;
    if(lua_istable(L, 2)){
        fixscale = cxLuaVec2fValue(L, 2, this->position);
    }else if(top == 3){
        fixscale.x = luaL_checknumber(L, 2);
        fixscale.y = luaL_checknumber(L, 3);
    }else{
        luaL_error(L, "args error");
    }
    cxViewSetFixScale(this, fixscale);
    lua_pushvalue(L, 1);
    return 1;
}

static cxInt cxViewLuaMake(lua_State *L)
{
    CX_LUA_CREATE_THIS(cxView);
    if(!lua_istable(L, 1)){
        CX_LUA_PUSH_THIS(cxView);
    }
    cxVec2f pos = this->position;
    lua_getfield(L, 1, "x");
    if(lua_isnumber(L, -1)){
        pos.x = lua_tonumber(L, -1);
    }
    lua_pop(L, 1);
    lua_getfield(L, 1, "y");
    if(lua_isnumber(L, -1)){
        pos.y = lua_tonumber(L, -1);
    }
    lua_pop(L, 1);
    cxViewSetPos(this, pos);
    
    cxSize2f size = this->size;
    lua_getfield(L, 1, "w");
    if(lua_isnumber(L, -1)){
        size.w = lua_tonumber(L, -1);
    }
    lua_pop(L, 1);
    lua_getfield(L, 1, "h");
    if(lua_isnumber(L, -1)){
        size.h = lua_tonumber(L, -1);
    }
    lua_pop(L, 1);
    cxViewSetSize(this, size);

    CX_LUA_PUSH_THIS(cxView);
}

CX_LUA_METHOD_BEG(cxView)
    {"AppendView",cxViewLuaAppendView},
    {"AppendAction",cxViewLuaAppendAction},
    {"CreateTimer",cxViewLuaCreateTimer},
    {"Make",cxViewLuaMake},
    CX_LUA_EVENT(cxView),
    CX_LUA_PROPERTY(cxView,Color),
    CX_LUA_PROPERTY(cxView,Position),
    CX_LUA_PROPERTY(cxView,Size),
    CX_LUA_PROPERTY(cxView,Scale),
    CX_LUA_PROPERTY(cxView,FixScale),
CX_LUA_METHOD_END(cxView)

void __cxViewTypeInit()
{
    CX_LUA_LOAD_TYPE(cxView);
}

//al at ar ab
static void cxViewRootReadAutoResize(cxReaderAttrInfo *info)
{
    cxView this = info->object;
    cxFloat v = 0;
    v = cxXMLReadFloatAttr(info, "cxView.left", -1) * this->fixscale.x;
    if(v >= 0){
        this->autoMask |= cxViewAutoResizeLeft;
        this->autoBox.l = v;
    }
    v = cxXMLReadFloatAttr(info, "cxView.right", -1) * this->fixscale.x;
    if(v >= 0){
        this->autoMask |= cxViewAutoResizeRight;
        this->autoBox.r = v;
    }
    v = cxXMLReadFloatAttr(info, "cxView.top", -1) * this->fixscale.x;
    if(v >= 0){
        this->autoMask |= cxViewAutoResizeTop;
        this->autoBox.t = v;
    }
    v = cxXMLReadFloatAttr(info, "cxView.bottom", -1) * this->fixscale.x;
    if(v >= 0){
        this->autoMask |= cxViewAutoResizeBottom;
        this->autoBox.b = v;
    }
    if(cxXMLHasAttr(info->reader, "cxView.fill")){
        this->autoMask = cxViewAutoResizeFill;
        this->autoBox = cxXMLReadBox4fAttr(info, "cxView.fill", this->autoBox);
    }
}

static void cxViewRootReadRectToView(cxReaderAttrInfo *info)
{
    cxView this = info->object;
    cxVec2f pos = cxXMLReadVec2fAttr(info, "cxView.position", this->position);
    pos.x = cxXMLReadFloatAttr(info, "cxView.x", pos.x);
    pos.y= cxXMLReadFloatAttr(info, "cxView.y", pos.y);
    cxViewSetPos(this, pos);
    cxViewSetOrder(this, cxXMLReadIntAttr(info, "cxView.z", this->zorder));
    cxSize2f size = cxXMLReadSize2fAttr(info, "cxView.size", this->size);
    size.w = cxXMLReadFloatAttr(info, "cxView.w", size.w);
    size.h = cxXMLReadFloatAttr(info, "cxView.h", size.h);
    cxViewSetSize(this, size);
}

cxBool cxViewZeroSize(cxAny pview)
{
    cxView this = pview;
    return cxSize2Zero(this->size);
}

void cxViewReadAttr(cxReaderAttrInfo *info)
{
    cxObjectReadAttr(info);
    cxView this = info->object;
    //fixscale
    cxViewSetFixScale(this, cxXMLReadVec2fAttr(info, "cxView.fixScale", this->fixscale));
    //rect
    cxViewRootReadRectToView(info);
    //resize
    cxViewRootReadAutoResize(info);
    //cropping
    cxViewSetCropping(this,cxXMLReadBoolAttr(info, "cxView.cropping", this->isCropping));
    //top
    cxViewSetTop(this, cxXMLReadBoolAttr(info, "cxView.hideTop", this->hideTop));
    //anchor
    cxViewSetAnchor(this, cxXMLReadVec2fAttr(info, "cxView.anchor", this->anchor));
    //scale
    cxViewSetScale(this, cxXMLReadVec2fAttr(info, "cxView.scale", this->scale));
    //color
    cxColor4f color = cxXMLReadColor4fAttr(info, "cxView.color", this->color);
    cxViewSetColor(this, cxColor3fv(color.r, color.g, color.b));
    cxViewSetAlpha(this, cxXMLReadFloatAttr(info, "cxView.alpha", color.a));
    //visible
    cxViewSetVisible(this, cxXMLReadBoolAttr(info, "cxView.visible", this->isVisible));
    //debug border
    cxViewSetBorder(this, cxXMLReadBoolAttr(info, "cxView.border", this->isBorder));
    //rotate raxis
    cxViewSetRaxis(this, cxXMLReadVec3fAttr(info, "cxView.raxis", this->raxis));
    //rotate angle
    cxViewSetAngle(this, cxXMLReadFloatAttr(info, "cxView.angle", this->angle));
    //rotate degress
    cxViewSetDegrees(this, cxXMLReadFloatAttr(info, "cxView.degrees", kmRadiansToDegrees(this->angle)));
    //cxAtlas support
    this->supportAtlasSet = cxXMLReadBoolAttr(info, "cxAtlasSet.support", false);
    //view event
    cxXMLAppendEvent(info, this, cxView, onEnter);
    cxXMLAppendEvent(info, this, cxView, onExit);
    cxXMLAppendEvent(info, this, cxView, onUpdate);
    cxXMLAppendEvent(info, this, cxView, onResize);
    cxXMLAppendEvent(info, this, cxView, onLayout);
    cxXMLAppendEvent(info, this, cxView, onDirty);
}

void cxViewSetCropping(cxAny pview,cxBool cropping)
{
    cxView this = pview;
    this->isCropping = cropping;
}

CX_OBJECT_INIT(cxView, cxObject)
{
    this->super.cxBase = cxBaseTypeView;
    this->hideTop = true;
    this->isBorder = false;
    this->isVisible = true;
    this->isDirty = true;
    this->color = cxColor4fv(1.0f, 1.0f, 1.0f, 1.0f);
    this->size = cxSize2fv(0.0f, 0.0f);
    this->anchor = cxVec2fv(0.5f, 0.5f);
    this->raxis = cxVec3fv(0.0f, 0.0f, 1.0f);
    this->scale = cxVec2fv(1.0f, 1.0f);
    this->fixscale = cxVec2fv(1.0f, 1.0f);
    this->subViews = CX_ALLOC(cxList);
    
    CX_METHOD_OVERRIDE(this->IsTouch, cxViewIsTouch);
    CX_METHOD_OVERRIDE(this->IsOnKey, cxViewIsOnKey);
    cxObjectSetReadAttrFunc(this, cxViewReadAttr);
    
    this->actions = CX_ALLOC(cxHash);
    this->caches = CX_ALLOC(cxHash);
    this->removes = CX_ALLOC(cxArray);
}
CX_OBJECT_FREE(cxView, cxObject)
{
    CX_RELEASE(this->removes);
    CX_RELEASE(this->subViews);
    CX_RELEASE(this->actions);
    CX_RELEASE(this->caches);
    CX_RELEASE(this->args);
    
    CX_EVENT_RELEASE(this->onDirty);
    CX_EVENT_RELEASE(this->onEnter);
    CX_EVENT_RELEASE(this->onExit);
    CX_EVENT_RELEASE(this->onUpdate);
    CX_EVENT_RELEASE(this->onResize);
    CX_EVENT_RELEASE(this->onLayout);
    
    CX_SIGNAL_RELEASE(this->onDraw);
    
    CX_METHOD_RELEASE(this->IsTouch);
    CX_METHOD_RELEASE(this->Touch);
    CX_METHOD_RELEASE(this->IsOnKey);
    CX_METHOD_RELEASE(this->OnKey);
    CX_METHOD_RELEASE(this->Draw);
    CX_METHOD_RELEASE(this->After);
    CX_METHOD_RELEASE(this->Before);
}
CX_OBJECT_TERM(cxView, cxObject)

void cxViewSetCache(cxAny pview,cxConstChars key,cxAny object)
{
    cxView this = pview;
    CX_ASSERT(object != NULL, "args error");
    cxHashSet(this->caches, cxHashStrKey(key), object);
}

cxAny cxViewGetCache(cxAny pview,cxConstChars key)
{
    cxView this = pview;
    return cxHashGet(this->caches, cxHashStrKey(key));
}

void cxViewSetArgs(cxAny pview,cxAny args)
{
    cxView this = pview;
    CX_RETAIN_SWAP(this->args, args);
}

cxVec2f cxViewPosition(cxAny pview)
{
    cxView this = pview;
    return this->position;
}

cxBool cxViewSupportAtlasSet(cxAny pview)
{
    cxView this = pview;
    return this->supportAtlasSet;
}

cxList cxViewSubViews(cxAny pview)
{
    cxView this = pview;
    return this->subViews;
}

cxSize2f cxViewSize(cxAny pview)
{
    cxView this = pview;
    return this->size;
}

cxBox4f cxViewBox(cxAny pview)
{
    cxView this = pview;
    cxFloat wh = this->size.w/2.0f;
    cxFloat hh = this->size.h/2.0f;
    return cxBox4fv(-wh, wh, hh, -hh);
}

cxAny cxViewArgs(cxAny pview)
{
    cxView this = pview;
    return this->args;
}

cxColor4f cxViewColor(cxAny pview)
{
    cxView this = pview;
    return this->color;
}

cxBool cxViewContainsGLBox(cxAny pview)
{
    cxEngine engine = cxEngineInstance();
    cxRect4f vr = cxViewGLRect(pview);
    cxRect4f gr = cxRect4fv(0, 0, engine->winsize.w, engine->winsize.h);
    return cxRect4fContainsRect4f(gr,vr);
}

cxRect4f cxViewGLRect(cxAny pview)
{
    cxView this = pview;
    cxFloat wh = this->size.w/2.0f;
    cxFloat hh = this->size.h/2.0f;
    cxVec2f p1 = cxVec2fv(-wh, -hh);
    cxVec2f p2 = cxVec2fv(wh, hh);
    p1 = cxViewPointToGLPoint(pview, p1);
    p2 = cxViewPointToGLPoint(pview, p2);
    return cxRect4fv(p1.x, p1.y, p2.x - p1.x, p2.y - p1.y);
}

void cxViewSetTop(cxAny pview,cxBool top)
{
    cxView this = pview;
    this->hideTop = top;
}

void cxViewSetAutoResizeBox(cxAny pview,cxBox4f box)
{
    cxView this = pview;
    this->autoBox = box;
}

void cxViewSetAutoResizeMask(cxAny pview,cxViewAutoResizeMask mask)
{
    cxView this = pview;
    this->autoMask = mask;
}

void cxViewSetDirty(cxAny pview,cxBool dirty)
{
    cxView this = pview;
    this->isDirty = dirty;
}

void cxViewSetBorder(cxAny pview,cxBool border)
{
    cxView this = pview;
    this->isBorder = border;
}

cxVec2f cxWindowPointToGLPoint(cxVec2f wPoint)
{
    cxEngine engine = cxEngineInstance();
    cxFloat x = wPoint.x + engine->winsize.w/2.0f;
    cxFloat y = wPoint.y + engine->winsize.h/2.0f;
    return cxVec2fv(x, y);
}

cxVec2f cxGLPointToWindowPoint(cxVec2f glPoint)
{
    cxEngine engine = cxEngineInstance();
    cxFloat x = glPoint.x - engine->winsize.w/2.0f;
    cxFloat y = glPoint.y - engine->winsize.h/2.0f;
    return cxVec2fv(x, y);
}

cxVec2f cxViewPointToGLPoint(cxAny pview,cxVec2f pos)
{
    cxVec2f ret = cxViewPointToWindowPoint(pview, pos);
    return cxWindowPointToGLPoint(ret);
}

cxVec2f cxGLPointToViewPoint(cxAny pview,cxVec2f pos)
{
    cxVec2f ret = cxGLPointToWindowPoint(pos);
    return cxWindowPointToViewPoint(pview,ret);
}

cxVec2f cxViewPointToWindowPoint(cxAny pview,cxVec2f vPoint)
{
    cxView this = pview;
    cxView pv = this;
    cxVec3f out;
    kmVec3Fill(&out, vPoint.x, vPoint.y, 0);
    while (pv != NULL && pv->parentView != NULL) {
        kmVec3Transform(&out, &out, &pv->anchorMatrix);
        kmVec3Transform(&out, &out, &pv->normalMatrix);
        pv = pv->parentView;
    }
    return cxVec2fv(out.x, out.y);
}

cxVec2f cxWindowPointToViewPoint(cxAny pview,cxVec2f wPoint)
{
    cxView this = pview;
    cxView pv = this;
    cxVec3f out;
    cxMatrix4f matrix;
    kmVec3Fill(&out, wPoint.x, wPoint.y, 0);
    cxArray list = CX_ALLOC(cxArray);
    while (pv != NULL && pv->parentView != NULL) {
        cxArrayAppend(list, pv);
        pv = pv->parentView;
    }
    CX_ARRAY_REVERSE(list, ele){
        pv = cxArrayObject(ele);
        kmMat4Inverse(&matrix, &pv->normalMatrix);
        kmVec3Transform(&out, &out, &matrix);
        kmMat4Inverse(&matrix, &pv->anchorMatrix);
        kmVec3Transform(&out, &out, &matrix);
    }
    CX_RELEASE(list);
    return cxVec2fv(out.x, out.y);
}

void cxViewSetSize(cxAny pview,cxSize2f size)
{
    cxView this = pview;
    CX_RETURN(cxSize2fEqu(this->size, size));
    this->size = size;
    this->isDirty = true;
    CX_EVENT_FIRE(this, onResize);
}

static int cxViewSortByZOrder(cxListElement *lp,cxListElement *rp)
{
    cxView v1 = (cxView)lp->any;
    cxView v2 = (cxView)rp->any;
    return v1->zorder - v2->zorder;
}

void cxViewSort(cxAny pview)
{
    cxView this = pview;
    cxListSort(this->subViews, cxViewSortByZOrder);
    this->isSort = false;
}

void cxViewSetVisible(cxAny pview,cxBool visible)
{
    cxView this = pview;
    this->isVisible = visible;
}

cxAny cxViewGetParentView(cxAny pview)
{
    CX_ASSERT(pview != NULL, "args error");
    cxView this = pview;
    return this->parentView;
}

void cxViewSetColor(cxAny pview,cxColor3f color)
{
    cxView this = pview;
    if(!kmAlmostEqual(this->color.r, color.r)){
        this->color.r = color.r;
        this->isDirty = true;
    }
    if(!kmAlmostEqual(this->color.g, color.g)){
        this->color.g = color.g;
        this->isDirty = true;
    }
    if(!kmAlmostEqual(this->color.b, color.b)){
        this->color.b = color.b;
        this->isDirty = true;
    }
}

void cxViewSetAlpha(cxAny pview,cxFloat alpha)
{
    cxView this = pview;
    CX_RETURN(kmAlmostEqual(this->color.a, alpha));
    this->color.a = alpha;
    this->isDirty = true;
}

void cxViewSetOrder(cxAny pview,cxInt order)
{
    cxView this = pview;
    CX_RETURN(this->zorder == order);
    this->zorder = order;
    if(this->parentView != NULL){
        this->parentView->isSort = true;
    }
}

void cxViewSetPos(cxAny pview,cxVec2f pos)
{
    cxView this = pview;
    CX_RETURN(cxVec2fEqu(this->position, pos));
    this->position = pos;
    this->isDirty = true;
}

void cxViewSetAnchor(cxAny pview,cxVec2f anchor)
{
    cxView this = pview;
    CX_RETURN(cxVec2fEqu(this->anchor, anchor));
    this->anchor = anchor;
    this->isDirty = true;
}

void cxViewSetFixScale(cxAny pview,cxVec2f scale)
{
    cxView this = pview;
    CX_RETURN(cxVec2fEqu(this->fixscale, scale));
    this->fixscale = scale;
    this->isDirty = true;
}

void cxViewSetScale(cxAny pview,cxVec2f scale)
{
    cxView this = pview;
    CX_RETURN(cxVec2fEqu(this->scale, scale));
    this->scale = scale;
    this->isDirty = true;
}

void cxViewSetDegrees(cxAny pview,cxFloat degrees)
{
    cxViewSetAngle(pview,kmDegreesToRadians(degrees));
}

void cxViewSetRaxis(cxAny pview,cxVec3f raxis)
{
    cxView this = pview;
    CX_RETURN(kmVec3AreEqual(&this->raxis, &raxis));
    this->raxis = raxis;
    this->isDirty = true;
}

cxFloat cxViewAngle(cxAny pview)
{
    cxView this = pview;
    return this->angle;
}

void cxViewSetAngle(cxAny pview,cxFloat angle)
{
    cxView this = pview;
    CX_RETURN(cxFloatEqu(this->angle,angle));
    this->angle = angle;
    this->isDirty = true;
}

cxInt cxViewSubviewCount(cxAny pview)
{
    cxView this = pview;
    return cxListLength(this->subViews);
}

cxVec2f cxViewScale(cxAny pview)
{
    cxView this = pview;
    return cxVec2fv(this->fixscale.x * this->scale.x, this->fixscale.y * this->scale.y);
}

void cxViewTransform(cxAny pview)
{
    cxView this = pview;
    CX_RETURN(!this->isDirty);
    cxMatrix4f transMatrix;
    kmMat4Translation(&transMatrix, this->position.x, this->position.y, 0.0f);
    
    cxMatrix4f rotateMatrix;
    kmMat4RotationAxisAngle(&rotateMatrix, &this->raxis, this->angle);
    
    cxVec2f scale = cxViewScale(this);
    cxMatrix4f scaleMatrix;
    kmMat4Scaling(&scaleMatrix, scale.x, scale.y, 1.0f);
    
    kmMat4Multiply(&this->normalMatrix, &transMatrix, &rotateMatrix);
    kmMat4Multiply(&this->normalMatrix, &this->normalMatrix, &scaleMatrix);
    
    cxFloat x = this->size.w * (0.5f - this->anchor.x);
    cxFloat y = this->size.h * (0.5f - this->anchor.y);
    kmMat4Translation(&this->anchorMatrix, x, y, 0);
    
    CX_EVENT_FIRE(this, onDirty);
    //
    if(this->isCropping){
        this->scissor = cxViewGLRect(pview);
    }
    this->isDirty = false;
}

static void cxViewDrawBorder(cxAny pview)
{
    cxView this = pview;
    cxBox4f b = cxViewBox(this);
    cxBoxVec2f box = cxBoxVec2fFromBox4f(b);
    cxDrawLineBox(&box, cxRED);
}

void cxViewEnter(cxAny pview)
{
    cxView this = pview;
    this->isRunning = true;
    CX_EVENT_FIRE(this, onEnter);
    CX_LIST_FOREACH(this->subViews, ele){
        cxView view = ele->any;
        if(view->isRunning){
            continue;
        }
        cxViewEnter(view);
    }
}

void cxViewExit(cxAny pview)
{
    cxView this = pview;
    CX_LIST_FOREACH(this->subViews, ele){
        cxView view = ele->any;
        if(!view->isRunning){
            continue;
        }
        cxViewExit(view);
    }
    CX_EVENT_FIRE(this, onExit);
    this->isRunning = false;
}

void cxViewAppend(cxAny pview,cxAny newview)
{
    CX_ASSERT(pview != NULL && newview != NULL, "parent view or new view null");
    cxView this = pview;
    cxView new = newview;
    CX_RETURN(new->parentView == pview);
    //
    CX_ASSERT(newview != NULL && new->subElement == NULL, "newview null or add to view");
    new->subElement = cxListAppend(this->subViews, new);
    new->parentView = this;
    //
    if(this->isRunning){
        cxViewEnter(new);
        cxViewLayout(new);
    }
}

void cxViewAutoResizing(cxAny pview)
{
    cxView this = pview;
    CX_RETURN(this->parentView == NULL || this->autoMask == cxViewAutoResizeNone);
    cxView parent = this->parentView;
    cxSize2f size = this->size;
    cxVec2f pos = this->position;
    cxViewAutoResizeMask mask = this->autoMask;
    cxVec2f scale = cxViewScale(this);
    //left right
    if((mask & cxViewAutoResizeLeft) && (mask & cxViewAutoResizeRight)){
        size.w = parent->size.w - this->autoBox.l - this->autoBox.r;
        this->scale.x = 1.0f;
        this->fixscale.x = 1.0f;
        pos.x = (this->autoBox.l - this->autoBox.r)/2.0f;
    }else if(mask & cxViewAutoResizeLeft){
        pos.x  = -parent->size.w/2.0f;
        pos.x += size.w * this->anchor.x * scale.x;
        pos.x += this->autoBox.l;
    }else if(mask & cxViewAutoResizeRight){
        pos.x = parent->size.w/2.0f;
        pos.x -= size.w * (1.0f - this->anchor.x) * scale.x;
        pos.x -= this->autoBox.r;
    }
    //top bottom
    if((mask & cxViewAutoResizeTop) && (mask & cxViewAutoResizeBottom)){
        size.h = parent->size.h - this->autoBox.t - this->autoBox.b;
        this->scale.y = 1.0f;
        this->fixscale.y = 1.0f;
        pos.y = (this->autoBox.b - this->autoBox.t)/2.0f;
    }else if(mask & cxViewAutoResizeTop){
        pos.y = parent->size.h/2.0f;
        pos.y -= size.h * (1.0f - this->anchor.y) * scale.y;
        pos.y -= this->autoBox.t;
    }else if(mask & cxViewAutoResizeBottom){
        pos.y  = -parent->size.h/2.0f;
        pos.y += size.h * this->anchor.y * scale.y;
        pos.y += this->autoBox.b;
    }
    cxViewSetPos(this, pos);
    cxViewSetSize(this, size);
}

void cxViewLayout(cxAny pview)
{
    cxView this = pview;
    cxViewAutoResizing(this);
    CX_LIST_FOREACH(this->subViews, ele){
        cxView view = ele->any;
        cxViewLayout(view);
    }
    CX_EVENT_FIRE(this, onLayout);
}

//remove from parent
void cxViewRemoved(cxAny pview)
{
    cxView this = pview;
    CX_RETURN(this->parentView == NULL);
    if(this->isRunning){
        cxViewExit(this);
    }
    cxView parent = this->parentView;
    //join to remove list
    cxArrayAppend(parent->removes, this);
    cxListRemove(parent->subViews, this->subElement);
    this->subElement = NULL;
    this->parentView = NULL;
}

cxBool cxViewHitTest(cxAny pview,cxVec2f wPoint,cxVec2f *vPoint)
{
    cxVec2f pos = cxWindowPointToViewPoint(pview, wPoint);
    cxBox4f box = cxViewBox(pview);
    if(vPoint != NULL){
        *vPoint = pos;
    }
    return cxBox2fContainPoint(box, pos);
}

static cxBool cxViewTouchSubViews(cxAny pview,cxTouch *touch)
{
    cxView this = pview;
    cxListElement *head = cxListFirst(this->subViews);
    for(cxListElement *ele = cxListLast(this->subViews);ele != NULL && head != NULL;ele = ele->prev){
        cxView view = ele->any;
        if(cxViewTouch(view, touch)){
            return true;
        }
        if(ele == head){
            break;
        }
    }
    return false;
}

cxUInt cxViewIsTouch(cxAny pview,cxTouch *touch)
{
    cxView this = pview;
    if(!this->isVisible){
        return cxViewIsTouchTypeNone;
    }
    return cxViewIsTouchTypeSelf | cxViewIsTouchTypeSubview;
}

cxBool cxViewTouch(cxAny pview,cxTouch *touch)
{
    cxView this = pview;
    cxUInt type = CX_METHOD_GET(cxViewIsTouchTypeNone, this->IsTouch,this,touch);
    if(type == cxViewIsTouchTypeNone){
        return false;
    }
    if((type & cxViewIsTouchTypeSubview) && cxViewTouchSubViews(pview,touch)){
        return true;
    }
    if(type & cxViewIsTouchTypeSelf){
        return CX_METHOD_GET(false, this->Touch,this,touch);
    }
    return false;
}

static cxBool cxViewOnKeySubViews(cxAny pview,cxKey *key)
{
    cxView this = pview;
    cxListElement *head = cxListFirst(this->subViews);
    for(cxListElement *ele = cxListLast(this->subViews);ele != NULL && head != NULL;ele = ele->prev){
        cxView view = ele->any;
        if(cxViewOnKey(view, key)){
            return true;
        }
        if(ele == head){
            break;
        }
    }
    return false;
}

cxUInt cxViewIsOnKey(cxAny pview,cxKey *key)
{
    cxView this = pview;
    if(!this->isVisible){
        return cxViewIsTouchTypeNone;
    }
    return cxViewIsTouchTypeSelf | cxViewIsTouchTypeSubview;
}

cxBool cxViewOnKey(cxAny pview,cxKey *key)
{
    cxView this = pview;
    cxUInt type = CX_METHOD_GET(cxViewIsTouchTypeNone, this->IsOnKey,this,key);
    if(type == cxViewIsTouchTypeNone){
        return false;
    }
    if((type & cxViewIsTouchTypeSubview) && cxViewOnKeySubViews(pview,key)){
        return true;
    }
    if(type & cxViewIsTouchTypeSelf){
        return CX_METHOD_GET(false, this->OnKey,this , key);
    }
    return false;
}

cxBool cxViewHasAction(cxAny pview,cxUInt actionId)
{
    cxView this = pview;
    cxHashKey key = cxHashIntKey(actionId);
    return cxHashHas(this->actions, key);
}

cxAny cxViewGetAction(cxAny pview,cxUInt actionId)
{
    cxView this = pview;
    cxHashKey key = cxHashIntKey(actionId);
    return cxHashGet(this->actions, key);
}

void cxViewStopAction(cxAny pview,cxUInt actionId)
{
    cxView this = pview;
    cxHashKey key = cxHashIntKey(actionId);
    cxAny ptr = cxHashGet(this->actions, key);
    if(ptr != NULL){
        cxActionStop(ptr);
    }
}

cxAny cxViewAppendTimer(cxAny pview,cxFloat freq,cxInt repeat)
{
    cxTimer timer = cxTimerCreate(freq, repeat);
    cxViewAppendAction(pview, timer);
    return timer;
}

cxUInt cxViewAppendAction(cxAny pview,cxAny pav)
{
    CX_ASSERT(pav != NULL && pview != NULL, "view or action null");
    cxView this = pview;
    cxAction action = pav;
    cxActionSetView(action, pview);
    cxUInt actionId = cxActionGetId(action);
    cxHashKey key = cxHashIntKey(actionId);
    cxAny ptr = cxHashGet(this->actions, key);
    if(ptr != NULL){
        cxActionStop(ptr);
    }
    cxHashSet(this->actions, key, action);
    return actionId;
}

static void cxViewUpdateActions(cxView this)
{
    cxEngine engine = cxEngineInstance();
    CX_HASH_FOREACH(this->actions, ele, tmp){
        cxAction action = ele->any;
        if(!cxActionUpdate(action, engine->frameDelta)){
            continue;
        }
        cxHashDelElement(this->actions, ele);
    }
}

void cxViewDraw(cxAny pview)
{
    cxView this = pview;
    if(!this->isVisible){
        goto finished;
    }
    //update action and update
    CX_EVENT_FIRE(this, onUpdate);
    cxViewUpdateActions(this);
    cxViewTransform(this);
    
    kmGLPushMatrix();
    kmGLMultMatrix(&this->normalMatrix);
    kmGLMultMatrix(&this->anchorMatrix);
    //check cropping
    if(this->isCropping){
        cxOpenGLEnableScissor(this->scissor);
    }
    //check sort
    if(this->isSort){
        cxViewSort(this);
    }
    CX_METHOD_RUN(this->Before, this);
    CX_METHOD_RUN(this->Draw, this);
    CX_LIST_FOREACH_SAFE(this->subViews, ele, tmp){
        cxView view = ele->any;
        cxViewDraw(view);
    }
    CX_SIGNAL_FIRE(this->onDraw, CX_FUNC_TYPE(cxAny),CX_SLOT_OBJECT);
    CX_METHOD_RUN(this->After,this);
    //
    if(this->isCropping){
        cxOpenGLDisableScissor();
    }
    if(cxEngineInstance()->isShowBorder || this->isBorder){
        cxViewDrawBorder(this);
    }
    kmGLPopMatrix();
finished:
    //remove subview
    cxArrayClean(this->removes);
}









