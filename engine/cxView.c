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
    cxEngine engine = cxEngineInstance();
    cxConstChars autofix = cxJsonToConstChars(value);
    cxVec2f fixScale = this->FixScale;
    if(cxConstCharsEqu(autofix, "width")){
        fixScale.x = engine->scale.x;
        fixScale.y = engine->scale.x;
    }else if(cxConstCharsEqu(autofix, "height")){
        fixScale.x = engine->scale.y;
        fixScale.y = engine->scale.y;
    }else{
        fixScale = cxJsonToVec2f(value,fixScale);
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
CX_SETTER_DEF(cxView, hidetop)
{
    cxBool hideTop = cxJsonToBool(value, this->HideTop);
    cxViewSetHideTop(this, hideTop);
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
    if(cxConstCharsHas(mask, "outside")){
        this->AutoMask |= cxViewAutoOutside;
    }
    if(cxConstCharsHas(mask, "fill")){
        this->AutoMask = cxViewAutoResizeFill;
    }
}
CX_SETTER_DEF(cxView, subviews)
{
    cxJson subviews = cxJsonToArray(value);
    CX_JSON_ARRAY_EACH_BEG(subviews, item)
    {
        cxAny object = cxObjectCreateWithJson(item);
        CX_ASSERT_TYPE(object, cxView);
        cxViewAppend(this, object);
    }
    CX_JSON_ARRAY_EACH_END(subviews, item)
}
CX_SETTER_DEF(cxView, actions)
{
    cxJson actions = cxJsonToArray(value);
    CX_JSON_ARRAY_EACH_BEG(actions, item)
    {
        cxAny object = cxObjectCreateWithJson(item);
        CX_ASSERT_TYPE(object, cxAction);
        cxViewAppendAction(this, object);
    }
    CX_JSON_ARRAY_EACH_END(actions, item)
}
CX_SETTER_DEF(cxView, tag)
{
    cxLong tag = cxJsonToLong(value, this->Tag);
    cxViewSetTag(this, tag);
}
CX_SETTER_DEF(cxView, bordercolor)
{
    cxColor3f borderColor = cxJsonToColor3f(value, this->BorderColor);
    cxViewSetBorderColor(this, borderColor);
}

CX_OBJECT_TYPE(cxView, cxObject)
{
    CX_PROPERTY_SETTER(cxView, size);
    CX_PROPERTY_SETTER(cxView, position);
    CX_PROPERTY_SETTER(cxView, anchor);
    CX_PROPERTY_SETTER(cxView, border);
    CX_PROPERTY_SETTER(cxView, color);
    CX_PROPERTY_SETTER(cxView, raxis);
    CX_PROPERTY_SETTER(cxView, scale);
    CX_PROPERTY_SETTER(cxView, fixscale);
    CX_PROPERTY_SETTER(cxView, visible);
    CX_PROPERTY_SETTER(cxView, degrees);
    CX_PROPERTY_SETTER(cxView, hidetop);
    CX_PROPERTY_SETTER(cxView, cropping);
    CX_PROPERTY_SETTER(cxView, autobox);
    CX_PROPERTY_SETTER(cxView, resizing);
    CX_PROPERTY_SETTER(cxView, subviews);
    CX_PROPERTY_SETTER(cxView, actions);
    CX_PROPERTY_SETTER(cxView, tag);
    CX_PROPERTY_SETTER(cxView, bordercolor);
}
CX_OBJECT_INIT(cxView, cxObject)
{
    this->HideTop       = true;
    this->isShowBorder  = false;
    this->IsVisible     = true;
    this->Dirty         = cxViewDirtyNone;
    this->TouchFlags    = cxViewTouchFlagsSubviews | cxViewTouchFlagsSelf;
    this->Color         = cxColor4fv(1.0f, 1.0f, 1.0f, 1.0f);
    this->Size          = cxSize2fv(0.0f, 0.0f);
    this->Anchor        = cxVec2fv(0.0f, 0.0f);
    this->Raxis         = cxVec3fv(0.0f, 0.0f, 1.0f);
    this->Scale         = cxVec2fv(1.0f, 1.0f);
    this->FixScale      = cxVec2fv(1.0f, 1.0f);
    this->BorderColor   = cxRED;
    this->SubViews      = CX_ALLOC(cxList);
    this->actions       = CX_ALLOC(cxHash);
    this->removes       = CX_ALLOC(cxArray);
    this->appends       = CX_ALLOC(cxArray);
    this->Binded        = CX_ALLOC(cxHash);
    this->Bindes        = CX_ALLOC(cxHash);
}
CX_OBJECT_FREE(cxView, cxObject)
{
    //unbind binds
    cxViewUnBindAll(this);
    CX_RELEASE(this->appends);
    CX_RELEASE(this->removes);
    CX_RELEASE(this->SubViews);
    CX_RELEASE(this->actions);
    CX_RELEASE(this->Bindes);
    CX_RELEASE(this->Binded);
    //
    CX_EVENT_RELEASE(this->onDirty);
    CX_EVENT_RELEASE(this->onEnter);
    CX_EVENT_RELEASE(this->onExit);
    CX_EVENT_RELEASE(this->onUpdate);
    CX_EVENT_RELEASE(this->onResize);
    CX_EVENT_RELEASE(this->onLayout);

    CX_SIGNAL_RELEASE(this->onDraw);
}
CX_OBJECT_TERM(cxView, cxObject)

cxBool cxViewIsRunning(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    return this->isRunning;
}

cxMatrix4f *cxViewNormalMatrix(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    return &this->normalMatrix;
}

cxMatrix4f *cxViewAnchorMatrix(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    return &this->anchorMatrix;
}

void cxViewUnBindAll(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    //clean bind's view
    CX_HASH_FOREACH(this->Bindes, ele1, tmp1){
        cxView view = cxHashKeyToAny(ele1);
        cxHashDel(view->Binded, cxHashAnyKey(this));
    }
    cxHashClean(this->Bindes);
    //clean binded view
    CX_HASH_FOREACH(this->Binded, ele2, tmp2){
        cxView view = cxHashKeyToAny(ele2);
        cxHashDel(view->Bindes, cxHashAnyKey(this));
    }
    cxHashClean(this->Binded);
}

cxAny cxViewBindesFirst(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    if(this->Bindes->hashPtr == NULL){
        return NULL;
    }
    return cxHashKeyToAny(this->Bindes->hashPtr);
}

//解除bind关系
void cxViewUnBind(cxAny pview,cxAny bview)
{
    CX_ASSERT_THIS(pview, cxView);
    CX_ASSERT_VALUE(bview, cxView, view);
    cxHashDel(this->Bindes, cxHashAnyKey(bview));
    cxHashDel(view->Binded, cxHashAnyKey(pview));
}

void cxViewBind(cxAny pview,cxAny bview,cxAny bd)
{
    CX_ASSERT(pview != bview, "self can't bind self");
    CX_ASSERT_THIS(pview, cxView);
    CX_ASSERT_VALUE(bview, cxView, bind);
    bd = (bd == NULL?cxStringCreate("%s bind %s",TYPE(this),TYPE(bind)): bd);
    //bind new view
    cxHashSet(this->Bindes, cxHashAnyKey(bind), bd);
    //this binded bind
    cxHashSet(bind->Binded, cxHashAnyKey(this), bd);
}

void cxViewPrepend(cxAny pview,cxAny newview)
{
    CX_ASSERT_THIS(pview, cxView);
    CX_ASSERT_VALUE(newview, cxView, nview);
    nview->isPrepend = true;
    nview->isRemoved = false;
    cxArrayAppend(this->appends, newview);
    nview->ParentView = pview;
}

cxAny cxViewAppendTypeImp(cxAny pview,cxConstType type)
{
    cxAny nview = cxObjectCreateWithType(type);
    CX_ASSERT_TYPE(nview, cxView);
    cxViewAppend(pview, nview);
    return nview;
}

void cxViewAppend(cxAny pview,cxAny newview)
{
    CX_ASSERT_THIS(pview, cxView);
    CX_ASSERT_VALUE(newview, cxView, nview);
    nview->isPrepend = false;
    nview->isRemoved = false;
    cxArrayAppend(this->appends, newview);
    nview->ParentView = pview;
}

void cxViewRemove(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    if(this->isRemoved || this->ParentView == NULL){
        return;
    }
    cxView parent = this->ParentView;
    cxArrayAppend(parent->removes, this);
    this->isRemoved = true;
}

void cxViewBringFront(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    this->isFront = true;
}

void cxViewCheckFront(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    CX_RETURN(!this->isFront);
    this->isFront = false;
    cxView parent = this->ParentView;
    CX_RETURN(parent == NULL);
    cxListMoveToTail(parent->SubViews, this->subElement);
}

cxBool cxViewZeroSize(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    return cxSize2fZero(this->Size);
}

cxVec2f cxViewTouchDelta(cxAny pview,cxTouchItem item)
{
    CX_ASSERT_THIS(pview, cxView);
    cxVec2f delta = item->delta;
    cxView parent = cxViewGetParentView(this);
    CX_RETURN(parent == NULL,delta);
    cxVec2f scale = cxViewGetScale(parent);
    delta.x /= scale.x;
    delta.y /= scale.y;
    return delta;
}

cxSize2f cxViewContentSize(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    cxVec2f scale = cxViewGetScale(this);
    cxSize2f size = cxViewGetSize(this);
    return cxSize2fv(size.w * scale.x, size.h * scale.y);
}

cxBox4f cxViewBox(cxAny pview)
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
    cxEngine engine = cxEngineInstance();
    cxRect4f vr = cxViewGLRect(this);
    cxRect4f gr = cxRect4fv(0, 0, engine->winsize.w, engine->winsize.h);
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

cxVec2f cxWindowPointToViewPoint(cxAny pview,cxVec2f wPoint)
{
    CX_ASSERT_THIS(pview, cxView);
    cxView pv = this;
    cxVec3f out;
    kmVec3Fill(&out, wPoint.x, wPoint.y, 0);
    //
    cxView vs[64];
    cxInt num = 0;
    while (pv != NULL && pv->ParentView != NULL) {
        vs[num++] = pv;
        pv = pv->ParentView;
        CX_ASSERT(num < 64, "vs too small");
    }
    //
    cxMatrix4f matrix;
    for(cxInt i= num; i > 0; i--){
        pv = vs[i - 1];
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
    cxEngine engine = cxEngineInstance();
    if(size.w > 0 && size.w < 1.0f){
        size.w = engine->winsize.w * size.w;
    }
    if(size.h > 0 && size.h < 1.0f){
        size.h = engine->winsize.h * size.h;
    }
    if(size.w < 1.0f || size.h < 1.0f){
        return;
    }
    cxSize2f vsize = cxViewGetSize(pview);
    CX_RETURN(cxSize2fEqu(vsize, size));
    this->Size = size;
    this->Dirty |= cxViewDirtySize;
    CX_EVENT_FIRE(this, onResize);
}

static cxInt cxViewSortByZOrder(cxListElement *lp,cxListElement *rp)
{
    cxView v1 = (cxView)lp->any;
    cxView v2 = (cxView)rp->any;
    return v1->zorder - v2->zorder;
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
    CX_METHOD_RUN(this->onSort,this);
}

void cxViewSetColor(cxAny pview,cxColor3f color)
{
    CX_ASSERT_THIS(pview, cxView);
    cxBool dirty = false;
    cxColor4f vcolor = cxViewGetColor(this);
    if(!kmAlmostEqual(vcolor.r, color.r)){
        this->Color.r = color.r;
        dirty = true;
    }
    if(!kmAlmostEqual(vcolor.g, color.g)){
        this->Color.g = color.g;
        dirty = true;
    }
    if(!kmAlmostEqual(vcolor.b, color.b)){
        this->Color.b = color.b;
        dirty = true;
    }
    this->Dirty |= cxViewDirtyColor;
}

void cxViewSetAlpha(cxAny pview,cxFloat alpha)
{
    CX_ASSERT_THIS(pview, cxView);
    cxColor4f vcolor = cxViewGetColor(this);
    CX_RETURN(kmAlmostEqual(vcolor.a, alpha));
    this->Color.a = alpha;
    this->Dirty |= cxViewDirtyColor;
}

void cxViewSetOrder(cxAny pview,cxInt order)
{
    CX_ASSERT_THIS(pview, cxView);
    CX_RETURN(this->zorder == order);
    this->zorder = order;
    CX_RETURN(this->ParentView == NULL);
    this->ParentView->isSort = true;
}

void cxViewSetPosition(cxAny pview,cxVec2f pos)
{
    CX_ASSERT_THIS(pview, cxView);
    cxVec2f vpos = cxViewGetPosition(this);
    CX_RETURN(cxVec2fEqu(vpos, pos));
    this->Position = pos;
    this->Dirty |= cxViewDirtyPosition;
}

cxVec2f cxViewSetAnchor(cxAny pview,cxVec2f anchor)
{
    CX_ASSERT_THIS(pview, cxView);
    cxSize2f size = cxViewGetSize(this);
    if((anchor.x > 0.5f || anchor.x < -0.5f) && !kmAlmostEqual(size.w, 0)){
        anchor.x = (anchor.x / (size.w/2.0f)) / 2.0f;
    }
    if((anchor.y > 0.5f || anchor.y < -0.5f) && !kmAlmostEqual(size.h, 0)){
        anchor.y = (anchor.y / (size.h/2.0f)) / 2.0f;
    }
    cxVec2f opos = cxViewGetPosition(this);
    CX_RETURN(cxVec2fEqu(this->Anchor, anchor),opos);
    if(!cxViewZeroSize(this)){
        cxSize2f size = cxViewContentSize(this);
        cxVec2f delta;
        kmVec2Subtract(&delta, &anchor, &this->Anchor);
        opos.x += size.w * delta.x;
        opos.y += size.h * delta.y;
    }
    this->Anchor = anchor;
    this->Dirty |= cxViewDirtyAnchor;
    return opos;
}

void cxViewSetFixScale(cxAny pview,cxVec2f scale)
{
    CX_ASSERT_THIS(pview, cxView);
    cxVec2f vfixScale = cxViewGetFixScale(this);
    CX_RETURN(cxVec2fEqu(vfixScale, scale));
    this->FixScale = scale;
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
    CX_RETURN(kmVec3AreEqual(&vraxis, &raxis));
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

cxInt cxViewSubviewCount(cxAny pview)
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
    cxMatrix4f transMatrix;
    kmMat4Translation(&transMatrix, this->Position.x, this->Position.y, 0.0f);
    
    cxMatrix4f rotateMatrix;
    kmMat4RotationAxisAngle(&rotateMatrix, &this->Raxis, this->Angle);
    
    cxVec2f scale = cxViewGetScale(this);
    cxMatrix4f scaleMatrix;
    kmMat4Scaling(&scaleMatrix, scale.x, scale.y, 1.0f);
    
    cxFloat x = -this->Size.w * this->Anchor.x;
    cxFloat y = -this->Size.h * this->Anchor.y;
    kmMat4Translation(&this->anchorMatrix, x, y, 0);
    
    //translate,rotate,scale
    kmMat4Multiply(&this->normalMatrix, &transMatrix, &rotateMatrix);
    kmMat4Multiply(&this->normalMatrix, &this->normalMatrix, &scaleMatrix);
    
    CX_EVENT_FIRE(this, onDirty);
    //
    if(cxViewGetIsCropping(pview)){
        this->scissor = cxViewGLRect(pview);
    }
    cxViewSetDirty(this, cxViewDirtyNone);
}

static void cxViewDrawBorder(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    cxBox4f b = cxViewBox(this);
    cxBoxVec2f box = cxBoxVec2fFromBox4f(b);
    cxDrawLineBox(&box, this->BorderColor);
}

void cxViewEnter(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    this->isRunning = true;
    CX_EVENT_FIRE(this, onEnter);
    CX_VIEW_FOREACH_SUBVIEWS(this, ele){
        cxView view = ele->any;
        if(view->isRunning){
            continue;
        }
        cxViewEnter(view);
    }
}

void cxViewExit(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    CX_VIEW_FOREACH_SUBVIEWS(this, ele){
        cxView view = ele->any;
        if(!view->isRunning){
            continue;
        }
        cxViewExit(view);
    }
    CX_EVENT_FIRE(this, onExit);
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
    cxVec2f scale = cxViewGetScale(this);
    //left right
    if((mask & cxViewAutoResizeLeft) && (mask & cxViewAutoResizeRight)){
        size.w = parent->Size.w - this->AutoBox.l - this->AutoBox.r;
        this->Scale.x = 1.0f;
        this->FixScale.x = 1.0f;
        pos.x = (this->AutoBox.l - this->AutoBox.r)/2.0f;
    }else if(mask & cxViewAutoResizeLeft){
        pos.x  = -parent->Size.w/2.0f;
        pos.x += size.w * (this->Anchor.x + 0.5f) * scale.x;
        if(mask & cxViewAutoOutside){
            pos.x -= (this->AutoBox.l + size.w);
        }else{
            pos.x += this->AutoBox.l;
        }
    }else if(mask & cxViewAutoResizeRight){
        pos.x = parent->Size.w/2.0f;
        pos.x -= size.w * (0.5f - this->Anchor.x) * scale.x;
        if(mask & cxViewAutoOutside){
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
        if(mask & cxViewAutoOutside){
            pos.y += (this->AutoBox.t + size.h);
        }else{
            pos.y -= this->AutoBox.t;
        }
    }else if(mask & cxViewAutoResizeBottom){
        pos.y  = -parent->Size.h/2.0f;
        pos.y += size.h * (this->Anchor.y + 0.5) * scale.y;
        if(mask & cxViewAutoOutside){
            pos.y -= (this->AutoBox.b + size.h);
        }else{
            pos.y += this->AutoBox.b;
        }
    }
    //update pos and size
    cxViewSetPosition(this, pos);
    cxViewSetSize(this, size);
}

void cxViewLayout(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    cxViewAutoResizing(this);
    CX_VIEW_FOREACH_SUBVIEWS(this, ele){
        cxView view = ele->any;
        cxViewLayout(view);
    }
    CX_EVENT_FIRE(this, onLayout);
}

cxBool cxViewHitTest(cxAny pview,cxVec2f wPoint,cxVec2f *vPoint)
{
    CX_ASSERT_THIS(pview, cxView);
    cxVec2f pos = cxWindowPointToViewPoint(this, wPoint);
    cxBox4f box = cxViewBox(this);
    if(vPoint != NULL){
        *vPoint = pos;
    }
    return cxBox2fContainPoint(box, pos);
}

cxBool cxViewTouch(cxAny pview,cxTouchItems *points)
{
    CX_ASSERT_THIS(pview, cxView);
    if(!this->IsVisible){
        return false;
    }
    if(this->TouchFlags == cxViewTouchFlagsNone){
        return false;
    }
    if(!(this->TouchFlags & cxViewTouchFlagsSubviews)){
        goto completed;
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
        if(cxViewTouch(ele->any,points)){
            return true;
        }
        if(ele == head){
            break;
        }
    }
completed:
    return CX_METHOD_GET(false, this->Touch, this, points);
}

cxBool cxViewKey(cxAny pview,cxKey *key)
{
    CX_ASSERT_THIS(pview, cxView);
    if(!this->IsVisible){
        return false;
    }
    if(this->TouchFlags == cxViewTouchFlagsNone){
        return false;
    }
    if(!(this->TouchFlags & cxViewTouchFlagsSubviews)){
        goto completed;
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
        if(cxViewKey(ele->any, key)){
            return true;
        }
        if(ele == head){
            break;
        }
    }
completed:
    return CX_METHOD_GET(false, this->Key,this , key);
}

void cxViewCleanActions(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    cxHashClean(this->actions);
}

cxBool cxViewHasAction(cxAny pview,cxUInt actionId)
{
    CX_ASSERT_THIS(pview, cxView);
    cxHashKey key = cxHashLongKey(actionId);
    return cxHashHas(this->actions, key);
}

cxAny cxViewGetAction(cxAny pview,cxUInt actionId)
{
    CX_ASSERT_THIS(pview, cxView);
    cxHashKey key = cxHashLongKey(actionId);
    return cxHashGet(this->actions, key);
}

void cxViewStopAction(cxAny pview,cxUInt actionId)
{
    CX_ASSERT_THIS(pview, cxView);
    cxHashKey key = cxHashLongKey(actionId);
    cxAny ptr = cxHashGet(this->actions, key);
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

cxUInt cxViewAppendAction(cxAny pview,cxAny pav)
{
    CX_ASSERT_THIS(pview, cxView);
    CX_ASSERT_VALUE(pav, cxAction, action);
    cxActionSetView(action, pview);
    cxUInt actionId = cxActionGetActionId(action);
    cxHashKey key = cxHashLongKey(actionId);
    cxAny ptr = cxHashGet(this->actions, key);
    if(ptr != NULL){
        cxActionStop(ptr);
    }
    cxHashSet(this->actions, key, action);
    return actionId;
}

static void cxViewUpdateActions(cxView pview)
{
    CX_ASSERT_THIS(pview, cxView);
    cxEngine engine = cxEngineInstance();
    CX_HASH_FOREACH(this->actions, ele, tmp){
        cxAction action = ele->any;
        if(!cxActionUpdate(action, engine->frameDelta)){
            continue;
        }
        cxHashDelElement(this->actions, ele);
    }
}

static void cxViewCleanRemoves(cxView this)
{
    CX_ARRAY_FOREACH(this->removes, ele){
        cxView view = cxArrayObject(ele);
        CX_ASSERT_TYPE(view, cxView);
        if(view->isRunning){
            cxViewExit(view);
        }
        cxView parent = view->ParentView;
        CX_METHOD_RUN(parent->onRemove, parent, view);
        cxListRemove(this->SubViews, view->subElement);
        view->subElement = NULL;
        view->ParentView = NULL;
    }
    cxArrayClean(this->removes);
}

static void cxViewCleanAppends(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    CX_ARRAY_FOREACH(this->appends, ele){
        cxView nview = cxArrayObject(ele);
        CX_ASSERT(nview->ParentView != NULL, "append set");
        if(nview->subElement != NULL){
            continue;
        }
        if(nview->isPrepend){
            //append head
            nview->subElement = cxListPrepend(this->SubViews, nview);
        }else{
            //append tail
            nview->subElement = cxListAppend(this->SubViews, nview);
        }
        if(this->isRunning){
            cxViewEnter(nview);
            cxViewLayout(nview);
        }
        CX_METHOD_RUN(this->onAppend, this , nview);
    }
    cxArrayClean(this->appends);
}

void cxViewDraw(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    //process append view
    cxViewCleanAppends(this);
    if(this->isRemoved){
        goto finished;
    }
    CX_EVENT_FIRE(this, onUpdate);
    if(!this->IsVisible){
        goto finished;
    }
    cxViewUpdateActions(this);
    cxViewTransform(this);
    cxViewCheckSort(this);
    cxViewCheckFront(this);
    if(!this->IsVisible || this->isRemoved){
        goto finished;
    }
    cxBool isCropping = cxViewGetIsCropping(this);
    kmGLPushMatrix();
    kmGLMultMatrix(&this->normalMatrix);
    kmGLMultMatrix(&this->anchorMatrix);
    if(isCropping){
        cxOpenGLEnableScissor(this->scissor);
    }
    CX_METHOD_RUN(this->Before, this);
    CX_METHOD_RUN(this->Draw, this);
    CX_VIEW_FOREACH_SUBVIEWS(this, ele){
        cxView view = ele->any;
        cxViewDraw(view);
    }
    CX_SIGNAL_FIRE(this->onDraw, CX_FUNC_TYPE(cxAny),CX_SLOT_OBJECT);
    CX_METHOD_RUN(this->After,this);
    if(isCropping){
        cxOpenGLDisableScissor();
    }
    if(cxEngineInstance()->isShowBorder || this->isShowBorder){
        cxViewDrawBorder(this);
    }
    kmGLPopMatrix();
finished:
    //process remove view
    cxViewCleanRemoves(this);
}









