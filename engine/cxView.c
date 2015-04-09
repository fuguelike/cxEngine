//
//  cxView.c
//  cxEngine
//
//  Created by xuhua on 10/2/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <actions/cxTimer.h>
#include <kazmath/GL/matrix.h>
#include <cxcore/cxBase.h>
#include "cxView.h"
#include "cxEngine.h"
#include "cxOpenGL.h"
#include "cxAction.h"
#include <engine/cxAsync.h>

CX_TYPE(cxUIMap, cxObject)
{
    
}
CX_INIT(cxUIMap, cxObject)
{
    
}
CX_FREE(cxUIMap, cxObject)
{
    CX_RELEASE(this->jsonkey);
    CX_RELEASE(this->property);
    CX_RELEASE(this->select);
}
CX_TERM(cxUIMap, cxObject)

cxUIMap cxUIMapCreate(cxStr s,cxStr p,cxStr k)
{
    cxUIMap this = CX_CREATE(cxUIMap);
    CX_RETAIN_SET(this->select, s);
    CX_RETAIN_SET(this->property, p);
    CX_RETAIN_SET(this->jsonkey, k);
    return this;
}

CX_SETTER_DEF(cxView, size)
{
    cxSize2f size = cxJsonToSize2f(value, this->Size);
    cxViewSetSize(this, size);
}
CX_SETTER_DEF(cxView, position)
{
    cxVec2f position = cxJsonToVec2f(value, this->Position);
    cxViewSetPosition(this, position);
}
CX_SETTER_DEF(cxView, anchor)
{
     cxVec2f anchor = cxJsonToVec2f(value, this->Anchor);
    cxViewSetAnchor(this, anchor);
}
CX_SETTER_DEF(cxView, border)
{
    this->isShowBorder = cxJsonToBool(value, this->isShowBorder);
}
CX_SETTER_DEF(cxView, color)
{
    cxColor4f color = cxJsonToColor4f(value, this->Color);
    cxViewSetColor(this, cxColor3fv(color.r, color.g, color.b));
    cxViewSetAlpha(this, color.a);
}
CX_SETTER_DEF(cxView, raxis)
{
    cxVec3f raxis = cxJsonToVec3f(value, this->Raxis);
    cxViewSetRaxis(this, raxis);
}
CX_SETTER_DEF(cxView, scale)
{
    cxVec2f scale = this->Scale;
    if(cxJsonIsNumber(value)){
        cxFloat d = cxJsonToDouble(value, 1.0);
        scale = cxVec2fv(d, d);
    }else{
        scale = cxJsonToVec2f(value, scale);
    }
    cxViewSetScale(this, scale);
}
CX_SETTER_DEF(cxView, fixscale)
{
    cxVec2f fixScale = this->FixScale;
    cxVec2f scale = cxEngineGetScale();
    if(cxJsonIsObject(value)){
        fixScale = cxJsonToVec2f(value,fixScale);
        cxViewSetFixScale(this, fixScale);
        return;
    }
    if(!cxJsonIsStr(value)){
        return;
    }
    cxConstChars autofix = cxJsonToConstChars(value);
    if(cxConstCharsEqu(autofix, "width")){
        fixScale.x = scale.x;
        fixScale.y = scale.x;
    }else if(cxConstCharsEqu(autofix, "height")){
        fixScale.x = scale.y;
        fixScale.y = scale.y;
    }
    cxViewSetFixScale(this, fixScale);
}
CX_SETTER_DEF(cxView, visible)
{
    cxBool isVisible = cxJsonToBool(value, this->IsVisible);
    cxViewSetIsVisible(this, isVisible);
}
CX_SETTER_DEF(cxView, degrees)
{
    cxFloat degrees = cxJsonToDouble(value, INFINITY);
    if(!isinf(degrees)){
        cxViewSetDegrees(this, degrees);
    }
}
CX_SETTER_DEF(cxView, sleeptop)
{
    cxBool sleepTop = cxJsonToBool(value, this->SleepTop);
    cxViewSetSleepTop(this, sleepTop);
}
CX_SETTER_DEF(cxView, cropping)
{
    cxBool isCropping = cxJsonToBool(value, this->IsCropping);
    cxViewSetIsCropping(this, isCropping);
}
CX_SETTER_DEF(cxView, autobox)
{
    cxBox4f box = cxJsonToBox4f(value, this->AutoBox);
    cxViewSetAutoBox(this,box);
}
CX_SETTER_DEF(cxView, resizing)
{
    cxConstChars mask = cxJsonToConstChars(value);
    if(mask != NULL){
        this->AutoMask = cxViewAutoResizeNone;
    }
    if(cxConstCharsHas(mask,"left")){
        this->AutoMask |= cxViewAutoResizeLeft;
    }
    if(cxConstCharsHas(mask, "right")){
        this->AutoMask |= cxViewAutoResizeRight;
    }
    if(cxConstCharsHas(mask, "top")){
        this->AutoMask |= cxViewAutoResizeTop;
    }
    if(cxConstCharsHas(mask, "bottom")){
        this->AutoMask |= cxViewAutoResizeBottom;
    }
    if(cxConstCharsHas(mask, "width")){
        this->AutoMask |= cxViewAutoResizeWidth;
    }
    if(cxConstCharsHas(mask, "height")){
        this->AutoMask |= cxViewAutoResizeHeight;
    }
    if(cxConstCharsHas(mask, "leftside")){
        this->AutoMask |= cxViewAutoResizeLeftSide;
    }
    if(cxConstCharsHas(mask, "rightside")){
        this->AutoMask |= cxViewAutoResizeRightSide;
    }
    if(cxConstCharsHas(mask, "topside")){
        this->AutoMask |= cxViewAutoResizeTopSide;
    }
    if(cxConstCharsHas(mask, "bottomside")){
        this->AutoMask |= cxViewAutoResizeBottomSide;
    }
    if(cxConstCharsHas(mask, "allside")){
        this->AutoMask |= cxViewAutoResizeAllSide;
    }
    if(cxConstCharsHas(mask, "fill")){
        this->AutoMask = cxViewAutoResizeFill;
    }
}
CX_SETTER_DEF(cxView, subviews)
{
    cxInt from = cxHashLength(this->UIViews);
    if(cxJsonIsArray(value)){
        cxJson subviews = cxJsonToArray(value);
        CX_JSON_ARRAY_EACH_BEG(subviews, item)
        //int key -> string key
        cxChar itemKey[16]={0};
        snprintf(itemKey, 16, "%d",itemIndex + from);
        cxView nview = cxJsonMakeObject(item);
        CX_ASSERT_TYPE(nview, cxView);
        cxViewAppend(this, nview);
        cxViewAppendUIView(this, nview, itemKey);
        CX_JSON_ARRAY_EACH_END(subviews, item)
    }else if(cxJsonIsObject(value)){
        cxJson subviews = cxJsonToObject(value);
        CX_JSON_OBJECT_EACH_BEG(subviews, item)
        cxView nview = cxJsonMakeObject(item);
        CX_ASSERT_TYPE(nview, cxView);
        CX_ASSERT(!isdigit(itemKey[0]), "json key digit head error");
        cxViewAppend(this, nview);
        cxViewAppendUIView(this, nview, itemKey);
        CX_JSON_OBJECT_EACH_END(subviews, item)
    }
}
CX_SETTER_DEF(cxView, actions)
{
    cxJson actions = cxJsonToArray(value);
    CX_JSON_ARRAY_EACH_BEG(actions, item)
    {
        cxAny object = cxJsonMakeObject(item);
        CX_ASSERT_TYPE(object, cxAction);
        cxViewAppendAction(this, object);
    }
    CX_JSON_ARRAY_EACH_END(actions, item)
}
CX_SETTER_DEF(cxView, tag)
{
    cxUInt tag = cxJsonToInt(value, this->Tag);
    cxViewSetTag(this, tag);
}
CX_SETTER_DEF(cxView, bordercolor)
{
    cxColor3f borderColor = cxJsonToColor3f(value, this->BorderColor);
    cxViewSetBorderColor(this, borderColor);
}
CX_SETTER_DEF(cxView, values)
{
    CX_ASSERT(cxJsonIsObject(value), "values must object");
    cxViewSetUIValues(this, value);
}
CX_SETTER_DEF(cxView, maps)
{
    CX_ASSERT(cxJsonIsArray(value), "maps must array");
    cxJson maps = cxJsonToArray(value);
    CX_JSON_ARRAY_EACH_BEG(maps, item)
    cxStr m = cxJsonToStr(item);
    //select->property=key
    cxRegex regex = cxRegexCreate("^(.+?)->(.+?)=(.+?)$", m, 0);
    if(!cxRegexNext(regex) || cxRegexLength(regex) != 4){
        CX_WARN("%s ui map error",cxStrBody(m));
        continue;
    }
    //object key use to select
    cxStr s = cxRegexMatch(regex, 1);
    if(!cxStrOK(s)){
        continue;
    }
    cxStr p = cxRegexMatch(regex, 2);
    if(!cxStrOK(p)){
        continue;
    }
    cxStr k = cxRegexMatch(regex, 3);
    if(!cxStrOK(k)){
        continue;
    }
    cxUIMap map = cxUIMapCreate(s,p,k);
    cxArrayAppend(this->UIMaps, map);
    CX_JSON_ARRAY_EACH_END(maps, item)
}
CX_SETTER_DEF(cxView, uikey)
{
    cxConstChars uikey = cxJsonToConstChars(value);
    if(!cxConstCharsOK(uikey)){
        return;
    }
    if(this->UIKey != NULL){
        allocator->free(this->UIKey);
    }
    this->UIKey = allocator->strdup(uikey);
}
CX_SETTER_DEF(cxView, shader)
{
    cxConstChars name = cxJsonToConstChars(value);
    cxShader shader = cxOpenGLShaderByName(name);
    cxViewSetShader(this, shader);
}
void cxViewTransform(cxAny pview);
void cxViewClearRemoves(cxView this);
void cxViewUpdateActions(cxView pview);
void cxViewDrawBorder(cxAny pview);

void cxViewUpdateView(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    if(this->isRemoved || this->IsSleep){
        goto finished;
    }
    //update in not visible
    CX_CALL(this, OnUpdate, CX_M(void));
    if(!this->IsVisible){
        goto finished;
    }
    cxViewUpdateActions(this);
    cxViewTransform(this);
    cxViewCheckSort(this);
    cxViewCheckFront(this);
    if(!this->IsDraw || !this->IsVisible || this->IsSleep || this->isRemoved){
        goto finished;
    }
    CX_VIEW_FOREACH_SUBVIEWS(this, ele){
        cxViewUpdateView(ele->any);
    }
finished:
    //process remove view
    cxViewClearRemoves(this);
}

void cxViewDrawView(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    if(!this->IsDraw || !this->IsVisible || this->IsSleep){
        return;
    }
    //
    cxBool cropping = cxViewGetIsCropping(this);
    //draw start
    kmGLPushMatrix();
    CX_CALL(this, OnGround, CX_M(void));
    kmGLMultMatrix(&this->normalMatrix);
    kmGLMultMatrix(&this->anchorMatrix);
    if(cropping){
        cxOpenGLEnableScissor(this->scissor);
    }
    CX_CALL(this, OnBefore, CX_M(void));
    CX_CALL(this, OnDraw, CX_M(void));
    CX_VIEW_FOREACH_SUBVIEWS(this, ele){
        cxViewDrawView(ele->any);
    }
    CX_CALL(this, OnAfter, CX_M(void));
    if(cropping){
        cxOpenGLDisableScissor();
    }
    if(cxEngineGetIsDebug() || this->isShowBorder){
        cxViewDrawBorder(this);
    }
    kmGLPopMatrix();
}

CX_METHOD_DEF(cxView,OnDraw,void)
{
    
}
//transform after draw,onDraw before
CX_METHOD_DEF(cxView,OnBefore,void)
{
    
}
//transform before draw
CX_METHOD_DEF(cxView,OnGround, void)
{
    
}
//transform after draw,onDraw after
CX_METHOD_DEF(cxView,OnAfter,void)
{
    
}
CX_METHOD_DEF(cxView,OnTouch,cxBool,const cxTouchItems *points)
{
    return false;
}
CX_METHOD_DEF(cxView,OnKey,cxBool,const cxKey *key)
{
    return false;
}
CX_METHOD_DEF(cxView, OnDirty, void)
{
}
CX_METHOD_DEF(cxView, OnEnter, void)
{
    //when view appended
}
CX_METHOD_DEF(cxView, OnExit, void)
{
    //when view removed
}
CX_METHOD_DEF(cxView, OnUpdate, void)
{
    //when view update
}
CX_METHOD_DEF(cxView, OnLayout, void)
{
    //when view onLayout
}
CX_METHOD_DEF(cxView, OnTransform, void)
{
    CX_RETURN(!(this->Dirty & cxViewDirtyNormal));
    cxFloat x = this->Position.x + this->Offset.x;
    cxFloat y = this->Position.y + this->Offset.y;
    cxMatrix4f transMatrix;
    kmMat4Translation(&transMatrix, x, y, this->Z);
    cxMatrix4f rotateMatrix;
    kmMat4RotationAxisAngle(&rotateMatrix, &this->Raxis, this->Angle);
    cxVec2f scale = cxViewTransformScale(this);
    cxMatrix4f scaleMatrix;
    kmMat4Scaling(&scaleMatrix, scale.x, scale.y, 1.0f);
    kmMat4Multiply(&this->normalMatrix, &transMatrix, &rotateMatrix);
    kmMat4Multiply(&this->normalMatrix, &this->normalMatrix, &scaleMatrix);
}
CX_METHOD_DEF(cxView, UIMap, void)
{
    cxJson values = cxViewGetUIValues(this);
    if(values == NULL){
        return;
    }
    CX_ARRAY_FOREACH(this->UIMaps, e){
        cxUIMap map = cxArrayObject(e);
        cxConstChars key = cxUIMapK(map);
        cxJson value = cxJsonSelect(values, key);
        cxViewSelectUpdate(this, cxUIMapS(map), cxUIMapP(map), value);
    }
}
//when bring view
CX_METHOD_DEF(cxView, OnFront, void)
{
    
}
//when from json ui loaded
CX_METHOD_DEF(cxView, UILoaded, void, cxAny root)
{
    CX_CALL(this, UIMap, CX_M(void));
    CX_HASH_FOREACH(this->UIViews, ele, tmp){
        CX_CALL(ele->any, UILoaded, CX_M(void,cxAny), root);
    }
}
CX_TYPE(cxView, cxObject)
{
    //
    CX_SETTER(cxView, shader);
    //use ui map value
    CX_SETTER(cxView, values);
    CX_SETTER(cxView, maps);
    CX_SETTER(cxView, uikey);       //cxViewSelect uikey
    //view
    CX_SETTER(cxView, size);
    CX_SETTER(cxView, position);
    CX_SETTER(cxView, anchor);
    CX_SETTER(cxView, border);
    CX_SETTER(cxView, color);
    CX_SETTER(cxView, raxis);
    CX_SETTER(cxView, scale);
    CX_SETTER(cxView, fixscale);
    CX_SETTER(cxView, visible);
    CX_SETTER(cxView, degrees);
    CX_SETTER(cxView, sleeptop);
    CX_SETTER(cxView, cropping);
    CX_SETTER(cxView, autobox);
    CX_SETTER(cxView, resizing);
    CX_SETTER(cxView, subviews);
    CX_SETTER(cxView, actions);
    CX_SETTER(cxView, tag);
    CX_SETTER(cxView, bordercolor);
    
    CX_METHOD(cxView, UIMap);
    CX_METHOD(cxView, UILoaded);
    
    CX_METHOD(cxView, OnFront);
    CX_METHOD(cxView, OnDraw);
    CX_METHOD(cxView, OnTransform);
    CX_METHOD(cxView, OnGround);
    CX_METHOD(cxView, OnBefore);
    CX_METHOD(cxView, OnAfter);
    CX_METHOD(cxView, OnTouch);
    CX_METHOD(cxView, OnKey);
    CX_METHOD(cxView, OnDirty);
    CX_METHOD(cxView, OnEnter);
    CX_METHOD(cxView, OnExit);
    CX_METHOD(cxView, OnUpdate);
    CX_METHOD(cxView, OnLayout);
}
CX_INIT(cxView, cxObject)
{
    this->IsDraw        = true;
    this->SleepTop      = true;
    this->IsVisible     = true;
    this->Dirty         = 0xFFFFFFFF;
    this->Color         = cxColor4fv(1.0f, 1.0f, 1.0f, 1.0f);
    this->Size          = cxSize2fv(0.0f, 0.0f);
    this->Anchor        = cxVec2fv(0.0f, 0.0f);
    this->Raxis         = cxVec3fv(0.0f, 0.0f, 1.0f);
    this->Scale         = cxVec2fv(1.0f, 1.0f);
    this->FixScale      = cxVec2fv(1.0f, 1.0f);
    this->BorderColor   = cxRED;
    this->frontViews    = CX_ALLOC(cxArray);
    this->SubViews      = CX_ALLOC(cxList);
    this->Actions       = CX_ALLOC(cxHash);
    this->removes       = CX_ALLOC(cxArray);
    this->Binded        = CX_ALLOC(cxHash);
    this->Bindes        = CX_ALLOC(cxHash);
    this->UIViews       = CX_ALLOC(cxHash);
    this->UIMaps        = CX_ALLOC(cxArray);
}
CX_FREE(cxView, cxObject)
{
    //unbind binds
    cxViewUnBindAll(this);
    CX_RELEASE(this->frontViews);
    CX_RELEASE(this->removes);
    CX_RELEASE(this->SubViews);
    CX_RELEASE(this->Actions);
    CX_RELEASE(this->Bindes);
    CX_RELEASE(this->Binded);
    //
    CX_RELEASE(this->UIViews);
    CX_RELEASE(this->UIValues);
    CX_RELEASE(this->UIMaps);
    allocator->free(this->UIKey);
    //
    CX_RELEASE(this->Shader);
}
CX_TERM(cxView, cxObject)

void cxViewReset(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    cxViewClearActions(this);
    cxViewUnBindAll(this);
}

void cxViewSetShaders(cxAny pview,cxShader shader)
{
    CX_ASSERT_THIS(pview, cxView);
    cxViewSetShader(this, shader);
    CX_VIEW_FOREACH_SUBVIEWS(this, e){
        cxViewSetShaders(e->any, shader);
    }
}

void cxViewSetUIValues(cxAny pthis,const cxJson values)
{
    CX_ASSERT_THIS(pthis, cxView);
    CX_ASSERT(values != NULL, "values args error");
    //set value
    if(this->UIValues == NULL){
        CX_RETAIN_SWAP(this->UIValues, values);
        return;
    }
    //merge value
    CX_JSON_OBJECT_EACH_BEG(values, item)
    cxJsonSetJson(this->UIValues, itemKey, item);
    CX_JSON_OBJECT_EACH_END(vs, item)
}

void cxViewAppendUIView(cxAny pview,cxAny pnview,cxConstChars uikey)
{
    CX_ASSERT_THIS(pview, cxView);
    CX_ASSERT_VALUE(pnview, cxView, nview);
    CX_ASSERT(cxConstCharsOK(uikey), "uikey args error");
    if(nview->UIKey == NULL){
        nview->UIKey = allocator->strdup(uikey);
    }
    cxHashSet(this->UIViews, cxHashStrKey(uikey), nview);
}

void cxViewUpdateValues(cxAny pview,cxAny values)
{
    CX_ASSERT_THIS(pview, cxView);
    if(values == NULL){
        return;
    }
    if(cxJsonObjectLength(values) == 0){
        return;
    }
    cxViewSetUIValues(this, values);
    CX_CALL(this, UIMap, CX_M(void));
}

void cxViewFireUILoaded(cxAny pview,cxAny values)
{
    CX_ASSERT_THIS(pview, cxView);
    if(values != NULL && cxJsonObjectLength(values) > 0){
        cxViewSetUIValues(this, values);
    }
    CX_CALL(this, UILoaded, CX_M(void,cxAny),this);
}

cxAny cxViewLoaderJson(cxJson json,cxAny values)
{
    cxLoader this = cxLoaderJson(json);
    cxAny root = this->Root;
    CX_ASSERT_TYPE(root, cxView);
    cxViewFireUILoaded(root, values);
    return root;
}

cxAny cxViewLoaderWithType(cxConstType type,cxAny values)
{
    CX_ASSERT(cxConstCharsOK(type), "type error");
    cxType ptype = cxTypesGetType(type);
    CX_ASSERT(ptype != NULL, "%s type not found",type);
    CX_ASSERT(ptype->UI != NULL, "%s type not bind ui",type);
    return cxViewLoaderFile(ptype->UI, values);
}

cxAny cxViewLoaderFile(cxConstChars path,cxAny values)
{
    CX_ASSERT(cxConstCharsOK(path), "path error");
    cxJson json = cxJsonRead(path);
    CX_ASSERT(json != NULL, "read json failed");
    return cxViewLoaderJson(json, values);
}

void cxViewUnBindAll(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    //clean bind's view
    CX_HASH_FOREACH(this->Bindes, bs, t1){
        cxView view = cxHashKeyToAny(bs);
        cxHashDel(view->Binded, cxHashAnyKey(this));
    }
    cxHashClear(this->Bindes);
    //clean binded view
    CX_HASH_FOREACH(this->Binded, bd, t2){
        cxView view = cxHashKeyToAny(bd);
        cxHashDel(view->Bindes, cxHashAnyKey(this));
    }
    cxHashClear(this->Binded);
}

cxAny cxViewBindesFirst(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    return this->Bindes->hptr != NULL?cxHashKeyToAny(this->Bindes->hptr):NULL;
}

//解除bind关系
void cxViewUnBind(cxAny pview,cxAny bview)
{
    CX_ASSERT_THIS(pview, cxView);
    CX_ASSERT_VALUE(bview, cxView, view);
    cxHashDel(this->Bindes, cxHashAnyKey(bview));
    cxHashDel(view->Binded, cxHashAnyKey(pview));
}

cxBool cxViewHasBind(cxAny pview,cxAny bview)
{
    CX_ASSERT_THIS(pview, cxView);
    CX_ASSERT_TYPE(bview, cxView);
    return cxHashHas(this->Bindes, cxHashAnyKey(bview));
}

void cxViewBind(cxAny pview,cxAny bview,cxAny bd)
{
    CX_ASSERT(pview != bview, "self can't bind self");
    CX_ASSERT_THIS(pview, cxView);
    CX_ASSERT_VALUE(bview, cxView, bind);
    bd = (bd == NULL)?cxStrCreate("%s->%s",CX_NAME_OF(this),CX_NAME_OF(bind)):bd;
    //bind new view
    cxHashSet(this->Bindes, cxHashAnyKey(bind), bd);
    //this binded bind
    cxHashSet(bind->Binded, cxHashAnyKey(this), bd);
}

void cxViewPrepend(cxAny pview,cxAny newview)
{
    CX_ASSERT_THIS(pview, cxView);
    CX_ASSERT_VALUE(newview, cxView, nview);
    nview->isRemoved = false;
    nview->ParentView = pview;
    nview->Element = cxListPrepend(this->SubViews, nview);
    if(this->isRunning){
        cxViewEnter(nview);
        cxViewLayout(nview);
    }
}

cxAny cxViewAppendTypeImp(cxAny pview,cxConstType type)
{
    cxAny nview = cxTypesCreateObject(type);
    CX_ASSERT_TYPE(nview, cxView);
    cxViewAppend(pview, nview);
    return nview;
}

void cxViewAppend(cxAny pview,cxAny newview)
{
    CX_ASSERT_THIS(pview, cxView);
    CX_ASSERT_VALUE(newview, cxView, nview);
    nview->isRemoved = false;
    nview->ParentView = pview;
    nview->Element = cxListAppend(this->SubViews, nview);
    if(this->isRunning){
        cxViewEnter(nview);
        cxViewLayout(nview);
    }
}

//remove all subview
void cxViewClear(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    CX_VIEW_FOREACH_SUBVIEWS(this, e){
        cxViewClear(e->any);
        cxViewRemove(e->any);
    }
}

void cxViewRemove(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    if(this->isRemoved || this->ParentView == NULL){
        return;
    }
    CX_ASSERT_VALUE(this->ParentView, cxView, parent);
    if(this->UIKey != NULL){
        cxHashDel(parent->UIViews, cxHashStrKey(this->UIKey));
        allocator->free((cxAny)this->UIKey);
        this->UIKey = NULL;
    }
    cxArrayAppend(parent->removes, this);
    this->isRemoved = true;
}

void cxViewBringFront(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    cxView parent = this->ParentView;
    CX_RETURN(parent == NULL);
    CX_ASSERT(this->Element != NULL, "not add parent view");
    cxArrayAppend(parent->frontViews, pview);
}

void cxViewCheckFront(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    CX_RETURN(cxArrayLength(this->frontViews) == 0);
    CX_ARRAY_FOREACH(this->frontViews, e){
        cxView bv = cxArrayObject(e);
        cxListMoveToTail(this->SubViews, bv->Element);
        CX_CALL(bv, OnFront, CX_M(void));
    }
    cxArrayClear(this->frontViews);
}

cxBool cxViewZeroSize(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    return cxSize2fZero(this->Size);
}

cxSize2f cxViewContentSize(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    cxSize2f size = cxViewGetSize(this);
    cxVec2f scale = cxViewTransformScale(this);
    return cxSize2fv(size.w * scale.x, size.h * scale.y);
}

cxBox4f cxViewGetBox(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    cxSize2f size = cxViewGetSize(this);
    cxFloat wh = size.w/2.0f;
    cxFloat hh = size.h/2.0f;
    return cxBox4fv(-wh, wh, hh, -hh);
}

cxBool cxViewContainsGLBox(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    cxSize2f wsize = cxEngineGetWinSize();
    cxRect4f vr = cxViewGLRect(this);
    cxRect4f gr = cxRect4fv(0, 0, wsize.w, wsize.h);
    return cxRect4fContainsRect4f(gr,vr);
}

cxRect4f cxViewGLRect(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    cxSize2f size = cxViewGetSize(this);
    cxFloat wh = size.w/2.0f;
    cxFloat hh = size.h/2.0f;
    cxVec2f p1 = cxVec2fv(-wh, -hh);
    cxVec2f p2 = cxVec2fv(wh, hh);
    p1 = cxViewPointToGLPoint(this, p1);
    p2 = cxViewPointToGLPoint(this, p2);
    return cxRect4fv(p1.x, p1.y, p2.x - p1.x, p2.y - p1.y);
}

void cxViewSetShowBorder(cxAny pview,cxBool isShowBorder)
{
    CX_ASSERT_THIS(pview, cxView);
    this->isShowBorder = isShowBorder;
}

cxVec2f cxWindowPointToGLPoint(cxVec2f wPoint)
{
    cxSize2f wsize = cxEngineGetWinSize();
    cxFloat x = wPoint.x + wsize.w/2.0f;
    cxFloat y = wPoint.y + wsize.h/2.0f;
    return cxVec2fv(x, y);
}

cxVec2f cxGLPointToWindowPoint(cxVec2f glPoint)
{
    cxSize2f wsize = cxEngineGetWinSize();
    cxFloat x = glPoint.x - wsize.w/2.0f;
    cxFloat y = glPoint.y - wsize.h/2.0f;
    return cxVec2fv(x, y);
}

cxVec2f cxViewPointToGLPoint(cxAny pview,cxVec2f pos)
{
    CX_ASSERT_THIS(pview, cxView);
    cxVec2f ret = cxViewPointToWindowPoint(this, pos);
    return cxWindowPointToGLPoint(ret);
}

cxVec2f cxGLPointToViewPoint(cxAny pview,cxVec2f pos)
{
    CX_ASSERT_THIS(pview, cxView);
    cxVec2f ret = cxGLPointToWindowPoint(pos);
    return cxWindowPointToViewPoint(this,ret);
}

cxVec2f cxViewPointToWindowPoint(cxAny pview,cxVec2f vPoint)
{
    CX_ASSERT_THIS(pview, cxView);
    cxView pv = this;
    cxVec3f out;
    kmVec3Fill(&out, vPoint.x, vPoint.y, 0);
    while (pv != NULL && pv->ParentView != NULL) {
        kmVec3MultiplyMat4(&out, &out, &pv->anchorMatrix);
        kmVec3MultiplyMat4(&out, &out, &pv->normalMatrix);
        pv = pv->ParentView;
    }
    return cxVec2fv(out.x, out.y);
}

cxVec2f cxViewPointToParent(cxAny pview,cxVec2f vPoint)
{
    CX_ASSERT_THIS(pview, cxView);
    cxVec3f out = cxVec3fv(vPoint.x, vPoint.y, 0);
    kmVec3MultiplyMat4(&out, &out, &this->anchorMatrix);
    kmVec3MultiplyMat4(&out, &out, &this->normalMatrix);
    return cxVec2fv(out.x, out.y);
}

cxVec2f cxWindowPointToViewPoint(cxAny pview,cxVec2f wPoint)
{
    CX_ASSERT_THIS(pview, cxView);
    cxView pv = this;
    cxVec3f out;
    kmVec3Fill(&out, wPoint.x, wPoint.y, 0);
    cxView views[64];
    cxInt num = 0;
    while (pv != NULL && pv->ParentView != NULL) {
        views[num++] = pv;
        pv = pv->ParentView;
        CX_ASSERT(num < 64, "vs too small");
    }
    cxMatrix4f matrix;
    for(cxInt i= num - 1; i >= 0; i--){
        pv = views[i];
        kmMat4Inverse(&matrix, &pv->normalMatrix);
        kmVec3MultiplyMat4(&out, &out, &matrix);
        kmMat4Inverse(&matrix, &pv->anchorMatrix);
        kmVec3MultiplyMat4(&out, &out, &matrix);
    }
    return cxVec2fv(out.x, out.y);
}

void cxViewSetSize(cxAny pview,cxSize2f size)
{
    CX_ASSERT_THIS(pview, cxView);
    cxSize2f wsize = cxEngineGetWinSize();
    if(size.w > 0 && size.w < 1.0f){
        size.w = wsize.w * size.w;
    }
    if(size.h > 0 && size.h < 1.0f){
        size.h = wsize.h * size.h;
    }
    cxSize2f vsize = cxViewGetSize(pview);
    CX_RETURN(cxSize2fEqu(vsize, size));
    this->Size = size;
    this->Dirty |= cxViewDirtySize;
}

static cxInt cxViewSortByZOrder(cxListElement *lp,cxListElement *rp)
{
    cxView v1 = (cxView)lp->any;
    cxView v2 = (cxView)rp->any;
    return v1->Order - v2->Order;
}

void cxViewSortWithFunc(cxAny pview,cxCmpFunc func)
{
    CX_ASSERT_THIS(pview, cxView);
    cxListSort(this->SubViews, func);
}

void cxViewCheckSort(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    CX_RETURN(!this->isSort);
    this->isSort = false;
    cxListSort(this->SubViews, cxViewSortByZOrder);
}

void cxViewSetColor(cxAny pview,cxColor3f color)
{
    CX_ASSERT_THIS(pview, cxView);
    cxBool dirty = false;
    cxColor4f vcolor = cxViewGetColor(this);
    if(!cxFloatEqu(vcolor.r, color.r)){
        this->Color.r = color.r;
        dirty = true;
    }
    if(!cxFloatEqu(vcolor.g, color.g)){
        this->Color.g = color.g;
        dirty = true;
    }
    if(!cxFloatEqu(vcolor.b, color.b)){
        this->Color.b = color.b;
        dirty = true;
    }
    if(dirty){
        this->Dirty |= cxViewDirtyColor;
    }
}

void cxViewSetAlpha(cxAny pview,cxFloat alpha)
{
    CX_ASSERT_THIS(pview, cxView);
    cxColor4f vcolor = cxViewGetColor(this);
    CX_RETURN(cxFloatEqu(vcolor.a, alpha));
    this->Color.a = alpha;
    this->Dirty |= cxViewDirtyColor;
}

void cxViewSetOrder(cxAny pview,cxInt order)
{
    CX_ASSERT_THIS(pview, cxView);
    CX_RETURN(this->Order == order);
    this->Order = order;
    CX_RETURN(this->ParentView == NULL);
    this->ParentView->isSort = true;
}
//get select path
cxStr cxViewSelectPath(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    cxStr ret = CX_CREATE(cxStr);
    cxInt vc = 0;
    cxView vs[64]={0};
    cxView cp = this;
    while (cp != NULL) {
        vs[vc++] = cp;
        cp = cp->ParentView;
    }
    for(cxInt i = vc - 1; i >=0; i--){
        cp = vs[i];
        if(cp->UIKey == NULL)continue;
        cxStrFormat(ret, "%s.",cp->UIKey);
    }
    cxStrErase(ret, -1);
    return ret;
}
//support key.key key.0.filed, max 8 level
cxAny cxViewSelect(cxAny pview,cxConstChars key)
{
    CX_ASSERT_THIS(pview, cxView);
    CX_ASSERT(cxConstCharsOK(key), "id error");
    cxChars ckey = allocator->strdup(key);
    cxInt num = 0;
    cxChars ckeys[16];
    cxChars src = ckey;
    ckeys[num++] = src;
    while (*src++ != '\0') {
        if(*src != '.')continue;
        CX_ASSERT(num < 16, ". opt too more");
        ckeys[num++] = src + 1;
        *src++ = '\0';
    }
    cxView pv = this;
    cxView rv = NULL;
    for(cxInt i=0; i < num;i ++){
        cxConstChars ckey = ckeys[i];
        //self -> this
        if(cxConstCharsEqu(ckey, "self")){
            rv = pv;
        }else{
            rv = cxHashGet(pv->UIViews, cxHashStrKey(ckey));
        }
        if(rv == NULL){
            break;
        }
        pv = rv;
    }
    allocator->free(ckey);
    return rv;
}

void cxViewSelectUpdate(cxAny pview,cxConstChars select,cxConstChars property,cxJson value)
{
    CX_ASSERT_THIS(pview, cxView);
    CX_ASSERT(cxConstCharsOK(property), "property key error");
    CX_ASSERT(cxConstCharsOK(select), "select key error");
    if(value == NULL){
        return;
    }
    cxAny ps = cxViewSelect(this, select);
    if(ps == NULL){
        CX_WARN("view select %s->%s=%s failed",select,property,cxStrBody(cxJsonDump(value)));
        return;
    }
    if(!cxPropertyRunSetter(ps, property, value)){
        CX_ERROR("run setter error:%s->%s=%s",select,property,cxStrBody(cxJsonDump(value)));
    }
}

void cxViewSetPosition(cxAny pview,cxVec2f pos)
{
    CX_ASSERT_THIS(pview, cxView);
    cxVec2f vpos = cxViewGetPosition(this);
    CX_RETURN(cxVec2fEqu(vpos, pos));
    this->Position = pos;
    this->Dirty |= cxViewDirtyPosition;
}

void cxViewSetOffset(cxAny pview,cxVec2f off)
{
    CX_ASSERT_THIS(pview, cxView);
    cxVec2f voff = cxViewGetOffset(this);
    CX_RETURN(cxVec2fEqu(voff, off));
    this->Offset = off;
    this->Dirty |= cxViewDirtyPosition;
}

cxVec2f cxViewSetAnchor(cxAny pview,cxVec2f anchor)
{
    CX_ASSERT_THIS(pview, cxView);
    cxVec2f opos = cxViewGetPosition(this);
    cxSize2f size = cxViewGetSize(this);
    if(!cxFloatEqu(size.w, 0) && fabsf(anchor.x) > 0.5f){
        anchor.x = (anchor.x / (size.w/2.0f)) / 2.0f;
    }
    if(!cxFloatEqu(size.h, 0) && fabsf(anchor.y) > 0.5f){
        anchor.y = (anchor.y / (size.h/2.0f)) / 2.0f;
    }
    CX_ASSERT(fabsf(anchor.x) <= 0.5f && fabsf(anchor.y) <= 0.5f, "anchor error");
    CX_RETURN(cxVec2fEqu(this->Anchor, anchor),opos);
    cxSize2f csize = cxViewContentSize(this);
    cxVec2f delta = cxVec2fSub(anchor, this->Anchor);
    opos.x += csize.w * delta.x;
    opos.y += csize.h * delta.y;
    this->Anchor = anchor;
    this->Dirty |= cxViewDirtyAnchor;
    return opos;
}

void cxViewSetFixScale(cxAny pview,cxVec2f fixscale)
{
    CX_ASSERT_THIS(pview, cxView);
    cxVec2f vfixScale = cxViewGetFixScale(this);
    CX_RETURN(cxVec2fEqu(vfixScale, fixscale));
    this->FixScale = fixscale;
    this->Dirty |= cxViewDirtyFixScale;
}

void cxViewSetScale(cxAny pview,cxVec2f scale)
{
    CX_ASSERT_THIS(pview, cxView);
    cxVec2f vscale = cxViewGetScale(this);
    CX_RETURN(cxVec2fEqu(vscale, scale));
    this->Scale = scale;
    this->Dirty |= cxViewDirtyScale;
}

void cxViewSetDegrees(cxAny pview,cxFloat degrees)
{
    CX_ASSERT_THIS(pview, cxView);
    cxViewSetAngle(this,kmDegreesToRadians(degrees));
}

void cxViewSetRaxis(cxAny pview,cxVec3f raxis)
{
    CX_ASSERT_THIS(pview, cxView);
    cxVec3f vraxis = cxViewGetRaxis(this);
    CX_RETURN(cxVec3fEqu(vraxis, raxis));
    this->Raxis = raxis;
    this->Dirty |= cxViewDirtyRaxis;
}

void cxViewSetAngle(cxAny pview,cxFloat angle)
{
    CX_ASSERT_THIS(pview, cxView);
    cxFloat vangle = cxViewGetAngle(this);
    CX_RETURN(cxFloatEqu(vangle,angle));
    this->Angle = angle;
    this->Dirty |= cxViewDirtyAngle;
}

cxInt cxViewSubviewsCount(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    return cxListLength(this->SubViews);
}

void cxViewTransform(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    if(!cxViewIsDirty(this)){
        return;
    }
    //anchor
    cxFloat x = -this->Size.w * this->Anchor.x;
    cxFloat y = -this->Size.h * this->Anchor.y;
    kmMat4Translation(&this->anchorMatrix, x, y, 0.0f);
    //transform
    CX_CALL(this, OnTransform, CX_M(void));
    CX_CALL(this, OnDirty, CX_M(void));
    cxViewClearDirty(this);
    //set cropping area
    if(cxViewGetIsCropping(pview)){
        this->scissor = cxViewGLRect(pview);
    }
}

void cxViewDrawBorder(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    cxBox4f b = cxViewGetBox(this);
    cxBoxVec2f box = cxBoxVec2fFromBox4f(b);
    cxDrawLineBox(&box, this->BorderColor);
}

void cxViewEnter(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    this->isRunning = true;
    CX_CALL(this, OnEnter, CX_M(void));
    CX_VIEW_FOREACH_SUBVIEWS(this, ele){
        cxView view = ele->any;
        if(view->isRunning)continue;
        cxViewEnter(view);
    }
}

void cxViewExit(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    CX_VIEW_FOREACH_SUBVIEWS(this, ele){
        cxView view = ele->any;
        if(!view->isRunning)continue;
        cxViewExit(view);
    }
    CX_CALL(this, OnExit, CX_M(void));
    this->isRunning = false;
}

void cxViewAutoResizing(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    CX_RETURN(this->ParentView == NULL || this->AutoMask == cxViewAutoResizeNone);
    cxView parent = this->ParentView;
    cxSize2f size = this->Size;
    cxVec2f pos = this->Position;
    cxViewAutoResizeMask mask = this->AutoMask;
    cxVec2f scale = cxViewTransformScale(this);
    //left right
    if((mask & cxViewAutoResizeLeft) && (mask & cxViewAutoResizeRight)){
        size.w = parent->Size.w - this->AutoBox.l - this->AutoBox.r;
        this->Scale.x = 1.0f;
        this->FixScale.x = 1.0f;
        pos.x = (this->AutoBox.l - this->AutoBox.r)/2.0f;
    }else if(mask & cxViewAutoResizeLeft){
        pos.x  = -parent->Size.w/2.0f;
        pos.x += size.w * (this->Anchor.x + 0.5f) * scale.x;
        if(mask & cxViewAutoResizeLeftSide){
            pos.x -= (this->AutoBox.l + size.w);
        }else{
            pos.x += this->AutoBox.l;
        }
    }else if(mask & cxViewAutoResizeRight){
        pos.x = parent->Size.w/2.0f;
        pos.x -= size.w * (0.5f - this->Anchor.x) * scale.x;
        if(mask & cxViewAutoResizeRightSide){
            pos.x += (this->AutoBox.r + size.w);
        }else{
            pos.x -= this->AutoBox.r;
        }
    }
    //top bottom
    if((mask & cxViewAutoResizeTop) && (mask & cxViewAutoResizeBottom)){
        size.h = parent->Size.h - this->AutoBox.t - this->AutoBox.b;
        this->Scale.y = 1.0f;
        this->FixScale.y = 1.0f;
        pos.y = (this->AutoBox.b - this->AutoBox.t)/2.0f;
    }else if(mask & cxViewAutoResizeTop){
        pos.y = parent->Size.h/2.0f;
        pos.y -= size.h * (0.5f - this->Anchor.y) * scale.y;
        if(mask & cxViewAutoResizeTopSide){
            pos.y += (this->AutoBox.t + size.h);
        }else{
            pos.y -= this->AutoBox.t;
        }
    }else if(mask & cxViewAutoResizeBottom){
        pos.y  = -parent->Size.h/2.0f;
        pos.y += size.h * (this->Anchor.y + 0.5) * scale.y;
        if(mask & cxViewAutoResizeBottomSide){
            pos.y -= (this->AutoBox.b + size.h);
        }else{
            pos.y += this->AutoBox.b;
        }
    }
    //update pos and size
    cxViewSetPosition(this, pos);
    cxViewSetSize(this, size);
}

void cxViewParentLayout(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    CX_RETURN(this->ParentView == NULL);
    cxViewLayout(this->ParentView);
}

void cxViewLayout(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    cxViewAutoResizing(this);
    CX_VIEW_FOREACH_SUBVIEWS(this, ele){
        cxView view = ele->any;
        cxViewLayout(view);
    }
    CX_CALL(this, OnLayout, CX_M(void));
}

cxHitInfo cxViewHitTest(cxAny pview,cxVec2f wPoint)
{
    CX_ASSERT_THIS(pview, cxView);
    cxBox4f box = cxViewGetBox(this);
    cxHitInfo ret={0};
    ret.position = cxWindowPointToViewPoint(this, wPoint);
    ret.hited = cxBox4fContainPoint(box, ret.position);
    return ret;
}

cxBool cxViewFireTouch(cxAny pview,const cxTouchItems *points)
{
    CX_ASSERT_THIS(pview, cxView);
    if(!this->IsVisible || this->IsSleep){
        return false;
    }
    cxListElement *head = cxListFirst(this->SubViews);
    if(head == NULL){
        goto completed;
    }
    cxListElement *tail = cxListLast(this->SubViews);
    if(tail == NULL){
        goto completed;
    }
    for(cxListElement *ele = tail;ele != NULL;ele = ele->prev){
        if(cxViewFireTouch(ele->any,points))return true;
        if(ele == head)break;
    }
completed:
    return CX_CALL(this, OnTouch, CX_M(cxBool,const cxTouchItems *),points);
}

cxBool cxViewFireKey(cxAny pview,cxKey *key)
{
    CX_ASSERT_THIS(pview, cxView);
    if(!this->IsVisible || this->IsSleep){
        return false;
    }
    cxListElement *head = cxListFirst(this->SubViews);
    if(head == NULL){
        goto completed;
    }
    cxListElement *tail = cxListLast(this->SubViews);
    if(tail == NULL){
        goto completed;
    }
    for(cxListElement *ele = tail;ele != NULL;ele = ele->prev){
        if(cxViewFireKey(ele->any, key))return true;
        if(ele == head)break;
    }
completed:
    return CX_CALL(this, OnKey, CX_M(cxBool,const cxKey *),key);
}

void cxViewClearActions(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    cxHashClear(this->Actions);
}

cxBool cxViewHasAction(cxAny pview,cxUInt actionId)
{
    CX_ASSERT_THIS(pview, cxView);
    cxHashKey key = cxHashLongKey(actionId);
    return cxHashHas(this->Actions, key);
}

cxAny cxViewGetAction(cxAny pview,cxUInt actionId)
{
    CX_ASSERT_THIS(pview, cxView);
    cxHashKey key = cxHashLongKey(actionId);
    return cxHashGet(this->Actions, key);
}

void cxViewStopAction(cxAny pview,cxUInt actionId)
{
    CX_ASSERT_THIS(pview, cxView);
    cxHashKey key = cxHashLongKey(actionId);
    cxAny ptr = cxHashGet(this->Actions, key);
    if(ptr != NULL){
        cxActionStop(ptr);
    }
}

cxAny cxViewAppendTimer(cxAny pview,cxFloat freq,cxInt repeat)
{
    CX_ASSERT_THIS(pview, cxView);
    cxTimer timer = cxTimerCreate(freq, repeat);
    cxViewAppendAction(this, timer);
    return timer;
}

void cxViewRunAsync(cxAny pview,cxAny pasync)
{
    CX_ASSERT_THIS(pview, cxView);
    CX_ASSERT_VALUE(pasync, cxAsync, async);
    cxDriver drive = cxDriverCreateImp(async);
    cxAsyncSetView(drive->Async, pview);
    cxViewAppendAction(this, drive);
}

cxUInt cxViewAppendAction(cxAny pview,cxAny pav)
{
    CX_ASSERT_THIS(pview, cxView);
    CX_ASSERT_VALUE(pav, cxAction, action);
    cxActionSetView(action, pview);
    cxUInt actionId = cxActionGetId(action);
    cxHashKey key = cxHashLongKey(actionId);
    cxAny ptr = cxHashGet(this->Actions, key);
    if(ptr != NULL){
        cxActionStop(ptr);
    }
    cxHashSet(this->Actions, key, action);
    return actionId;
}

void cxViewUpdateActions(cxView pview)
{
    CX_ASSERT_THIS(pview, cxView);
    cxFloat dt = cxEngineGetFixDelta();
    CX_HASH_FOREACH(this->Actions, ele, tmp){
        CX_ASSERT_VALUE(ele->any, cxAction, action);
        if(!cxActionRun(action, dt)){
            continue;
        }
        cxHashDelElement(this->Actions, ele);
    }
}

static void cxDelayFunc(cxAny pav)
{
    CX_ASSERT_THIS(cxActionGetView(pav), cxView);
    cxAny func = cxObjectGetUserData(pav);
    ((void(*)(cxAny))func)(this);
}

void cxViewDelayRun(cxAny pview,cxFloat time,cxAny func)
{
    cxTimer timer = cxViewAppendTimer(pview, time, 1);
    cxObjectSetUserData(timer, func);
    CX_ADD(cxAction, timer, onExit, cxDelayFunc);
}

void cxViewClearRemoves(cxView this)
{
    CX_ARRAY_FOREACH(this->removes, ele){
        cxView view = cxArrayObject(ele);
        CX_ASSERT_TYPE(view, cxView);
        if(view->isRunning){
            cxViewExit(view);
        }
        cxArrayRemove(this->frontViews, view);
        //from subviews remove
        cxListRemove(this->SubViews, view->Element);
        view->Element = NULL;
        view->ParentView = NULL;
    }
    cxArrayClear(this->removes);
}










