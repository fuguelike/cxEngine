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
    this->size = cxJsonToSize2f(value, this->size);
}
CX_SETTER_DEF(cxView, position)
{
    this->position = cxJsonToVec2f(value, this->position);
}
CX_SETTER_DEF(cxView, anchor)
{
    this->anchor = cxJsonToVec2f(value, this->anchor);
}
CX_SETTER_DEF(cxView, border)
{
    this->isShowBorder = cxJsonToBool(value, this->isShowBorder);
}
CX_SETTER_DEF(cxView, color)
{
    this->color = cxJsonToColor4f(value, this->color);
}
CX_SETTER_DEF(cxView, raxis)
{
    this->raxis = cxJsonToVec3f(value, this->raxis);
}
CX_SETTER_DEF(cxView, scale)
{
    if(cxJsonIsNumber(value)){
        cxFloat d = cxJsonToDouble(value, 1.0);
        this->scale = cxVec2fv(d, d);
    }else{
        this->scale = cxJsonToVec2f(value, this->scale);
    }
}
CX_SETTER_DEF(cxView, fixscale)
{
    cxEngine engine = cxEngineInstance();
    cxConstChars autofix = cxJsonToConstChars(value);
    if(cxConstCharsEqu(autofix, "width")){
        this->fixscale.x = engine->scale.x;
        this->fixscale.y = engine->scale.x;
    }else if(cxConstCharsEqu(autofix, "height")){
        this->fixscale.x = engine->scale.y;
        this->fixscale.y = engine->scale.y;
    }else{
        this->fixscale = cxJsonToVec2f(value,this->fixscale);
    }
}
CX_SETTER_DEF(cxView, visible)
{
    this->isVisible = cxJsonToBool(value, this->isVisible);
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
    this->hideTop = cxJsonToBool(value, this->hideTop);
}
CX_SETTER_DEF(cxView, cropping)
{
    this->isCropping = cxJsonToBool(value, this->isCropping);
}
CX_SETTER_DEF(cxView, autobox)
{
    this->autoBox = cxJsonToBox4f(value, this->autoBox);
}
CX_SETTER_DEF(cxView, resizing)
{
    cxConstChars mask = cxJsonToConstChars(value);
    if(mask != NULL){
        this->autoMask = cxViewAutoResizeNone;
    }
    if(cxConstCharsHas(mask,"left")){
        this->autoMask |= cxViewAutoResizeLeft;
    }
    if(cxConstCharsHas(mask, "right")){
        this->autoMask |= cxViewAutoResizeRight;
    }
    if(cxConstCharsHas(mask, "top")){
        this->autoMask |= cxViewAutoResizeTop;
    }
    if(cxConstCharsHas(mask, "bottom")){
        this->autoMask |= cxViewAutoResizeBottom;
    }
    if(cxConstCharsHas(mask, "width")){
        this->autoMask |= cxViewAutoResizeWidth;
    }
    if(cxConstCharsHas(mask, "height")){
        this->autoMask |= cxViewAutoResizeHeight;
    }
    if(cxConstCharsHas(mask, "outside")){
        this->autoMask |= cxViewAutoOutside;
    }
    if(cxConstCharsHas(mask, "fill")){
        this->autoMask = cxViewAutoResizeFill;
    }
}
CX_SETTER_DEF(cxView, subviews)
{
    cxJson subviews = cxJsonToArray(value);
    CX_JSON_ARRAY_EACH_BEG(subviews, item)
    {
        cxAny object = cxObjectCreateWithJson(item);
        CX_ASSERT(CX_INSTANCE_OF(object, cxView), "subview must is cxView type");
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
        CX_ASSERT(CX_INSTANCE_OF(object, cxAction), "actions must is cxAction type");
        cxViewAppendAction(this, object);
    }
    CX_JSON_ARRAY_EACH_END(actions, item)
}
CX_SETTER_DEF(cxView, tag)
{
    this->tag = cxJsonToInt(value, this->tag);
}
CX_SETTER_DEF(cxView, bordercolor)
{
    this->borderColor = cxJsonToColor3f(value, this->borderColor);
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
    this->onlyTransform = false;
    this->hideTop       = true;
    this->isShowBorder  = false;
    this->isVisible     = true;
    this->isDirty       = true;
    this->isTouch       = true;
    this->color         = cxColor4fv(1.0f, 1.0f, 1.0f, 1.0f);
    this->size          = cxSize2fv(0.0f, 0.0f);
    this->anchor        = cxVec2fv(0.0f, 0.0f);
    this->raxis         = cxVec3fv(0.0f, 0.0f, 1.0f);
    this->scale         = cxVec2fv(1.0f, 1.0f);
    this->fixscale      = cxVec2fv(1.0f, 1.0f);
    this->borderColor   = cxRED;
    this->subViews      = CX_ALLOC(cxList);
    this->actions       = CX_ALLOC(cxHash);
    this->removes       = CX_ALLOC(cxArray);
    this->binded        = CX_ALLOC(cxHash);
    this->bindes        = CX_ALLOC(cxHash);
}
CX_OBJECT_FREE(cxView, cxObject)
{
    //unbind binds
    cxViewUnBindAll(this);
    CX_RELEASE(this->removes);
    CX_RELEASE(this->subViews);
    CX_RELEASE(this->actions);
    CX_RELEASE(this->bindes);
    CX_RELEASE(this->binded);
    //
    CX_EVENT_RELEASE(this->onTransform);
    CX_EVENT_RELEASE(this->onEnter);
    CX_EVENT_RELEASE(this->onExit);
    CX_EVENT_RELEASE(this->onUpdate);
    CX_EVENT_RELEASE(this->onResize);
    CX_EVENT_RELEASE(this->onLayout);

    CX_SIGNAL_RELEASE(this->onDraw);
}
CX_OBJECT_TERM(cxView, cxObject)

void cxViewSetOnlyTransform(cxAny pview,cxBool v)
{
    CX_ASSERT_THIS(pview, cxView);
    this->onlyTransform = v;
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
    CX_HASH_FOREACH(this->bindes, ele1, tmp1){
        cxView view = cxHashElementKeyToAny(ele1);
        cxHashDel(view->binded, cxHashAnyKey(this));
    }
    cxHashClean(this->bindes);
    //clean binded view
    CX_HASH_FOREACH(this->binded, ele2, tmp2){
        cxView view = cxHashElementKeyToAny(ele2);
        cxHashDel(view->bindes, cxHashAnyKey(this));
    }
    cxHashClean(this->binded);
}

cxHash cxViewBindes(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    return this->bindes;
}

cxHash cxViewBinded(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    return this->binded;
}

//解除bind关系
void cxViewUnBind(cxAny pview,cxAny bview)
{
    CX_ASSERT_THIS(pview, cxView);
    cxView view = CX_TYPE_CAST(cxView, bview);
    cxHashDel(this->bindes, cxHashAnyKey(bview));
    cxHashDel(view->binded, cxHashAnyKey(pview));
}

void cxViewBind(cxAny pview,cxAny bview,cxAny bd)
{
    CX_ASSERT(pview != bview, "self can't bind self");
    CX_ASSERT_THIS(pview, cxView);
    if(bd == NULL){
        bd = cxStringCreate("%p bind %p",pview,bview);
    }
    cxView bind = CX_TYPE_CAST(cxView, bview);
    //bind new view
    cxHashSet(this->bindes, cxHashAnyKey(bind), bd);
    //this binded bind
    cxHashSet(bind->binded, cxHashAnyKey(this), bd);
}

static inline void cxViewAppendImp(cxAny pview,cxAny newview,cxBool prepend)
{
    CX_ASSERT_THIS(pview, cxView);
    CX_ASSERT_TYPE(newview, cxView);
    cxView new = newview;
    CX_RETURN(new->parentView == pview);
    CX_ASSERT(newview != NULL && new->subElement == NULL, "newview null or add to view");
    new->subElement = prepend ? cxListPrepend(this->subViews, new) : cxListAppend(this->subViews, new);
    new->parentView = this;
    if(this->isRunning){
        cxViewEnter(new);
        cxViewLayout(new);
    }
    CX_METHOD_RUN(this->onAppend, this, newview);
}

void cxViewPrepend(cxAny pview,cxAny newview)
{
    cxViewAppendImp(pview, newview, true);
}

void cxViewAppend(cxAny pview,cxAny newview)
{
    cxViewAppendImp(pview, newview, false);
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
    cxView parent = this->parentView;
    CX_RETURN(parent == NULL);
    cxListMoveToTail(parent->subViews, this->subElement);
    this->isFront = false;
}

void cxViewSetCropping(cxAny pview,cxBool cropping)
{
    CX_ASSERT_THIS(pview, cxView);
    this->isCropping = cropping;
}

cxBool cxViewZeroSize(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    return cxSize2fZero(this->size);
}

cxVec2f cxViewTouchDelta(cxAny pview,cxTouchItem item)
{
    CX_ASSERT_THIS(pview, cxView);
    cxVec2f delta = item->delta;
    cxView parent = cxViewParent(this);
    CX_RETURN(parent == NULL,delta);
    delta.x /= parent->scale.x;
    delta.y /= parent->scale.y;
    return delta;
}

cxAny cxViewParent(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    return this->parentView;
}

cxVec2f cxViewPosition(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    return this->position;
}

cxList cxViewSubViews(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    return this->subViews;
}

cxInt cxViewTag(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    return this->tag;
}

void cxViewSetTag(cxAny pview,cxInt tag)
{
    CX_ASSERT_THIS(pview, cxView);
    this->tag = tag;
}

cxSize2f cxViewSize(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    return this->size;
}

cxSize2f cxViewContentSize(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    cxVec2f scale = cxViewScale(pview);
    return cxSize2fv(this->size.w * scale.x, this->size.h * scale.y);
}

cxBox4f cxViewBox(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    cxFloat wh = this->size.w/2.0f;
    cxFloat hh = this->size.h/2.0f;
    return cxBox4fv(-wh, wh, hh, -hh);
}

cxColor4f cxViewColor(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    return this->color;
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
    cxFloat wh = this->size.w/2.0f;
    cxFloat hh = this->size.h/2.0f;
    cxVec2f p1 = cxVec2fv(-wh, -hh);
    cxVec2f p2 = cxVec2fv(wh, hh);
    p1 = cxViewPointToGLPoint(pview, p1);
    p2 = cxViewPointToGLPoint(pview, p2);
    return cxRect4fv(p1.x, p1.y, p2.x - p1.x, p2.y - p1.y);
}

void cxViewSetHideTop(cxAny pview,cxBool hideTop)
{
    CX_ASSERT_THIS(pview, cxView);
    this->hideTop = hideTop;
}

void cxViewSetAutoResizeBox(cxAny pview,cxBox4f box)
{
    CX_ASSERT_THIS(pview, cxView);
    this->autoBox = box;
}

void cxViewSetAutoResizeMask(cxAny pview,cxViewAutoResizeMask mask)
{
    CX_ASSERT_THIS(pview, cxView);
    this->autoMask = mask;
}

void cxViewSetDirty(cxAny pview,cxBool dirty)
{
    CX_ASSERT_THIS(pview, cxView);
    this->isDirty = dirty;
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
    while (pv != NULL && pv->parentView != NULL) {
        kmVec3MultiplyMat4(&out, &out, &pv->anchorMatrix);
        kmVec3MultiplyMat4(&out, &out, &pv->normalMatrix);
        pv = pv->parentView;
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
    while (pv != NULL && pv->parentView != NULL) {
        vs[num++] = pv;
        pv = pv->parentView;
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
    CX_RETURN(cxSize2fEqu(this->size, size));
    this->size = size;
    this->isDirty = true;
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
    cxListSort(this->subViews, func);
}

void cxViewSort(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    cxListSort(this->subViews, cxViewSortByZOrder);
    CX_METHOD_RUN(this->onSort,this);
    this->isSort = false;
}

void cxViewSetVisible(cxAny pview,cxBool visible)
{
    CX_ASSERT_THIS(pview, cxView);
    this->isVisible = visible;
}

cxAny cxViewGetParentView(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    return this->parentView;
}

void cxViewSetColor(cxAny pview,cxColor3f color)
{
    CX_ASSERT_THIS(pview, cxView);
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
    CX_ASSERT_THIS(pview, cxView);
    CX_RETURN(kmAlmostEqual(this->color.a, alpha));
    this->color.a = alpha;
    this->isDirty = true;
}

void cxViewSetOrder(cxAny pview,cxInt order)
{
    CX_ASSERT_THIS(pview, cxView);
    CX_RETURN(this->zorder == order);
    this->zorder = order;
    CX_RETURN(this->parentView == NULL);
    this->parentView->isSort = true;
}

void cxViewSetPos(cxAny pview,cxVec2f pos)
{
    CX_ASSERT_THIS(pview, cxView);
    CX_RETURN(cxVec2fEqu(this->position, pos));
    this->position = pos;
    this->isDirty = true;
}

cxVec2f cxViewSetAnchor(cxAny pview,cxVec2f anchor)
{
    CX_ASSERT_THIS(pview, cxView);
    if((anchor.x > 0.5f || anchor.x < -0.5f) && !kmAlmostEqual(this->size.w, 0)){
        anchor.x = (anchor.x / (this->size.w/2.0f)) / 2.0f;
    }
    if((anchor.y > 0.5f || anchor.y < -0.5f) && !kmAlmostEqual(this->size.h, 0)){
        anchor.y = (anchor.y / (this->size.h/2.0f)) / 2.0f;
    }
    CX_RETURN(cxVec2fEqu(this->anchor, anchor),this->position);
    cxVec2f opos = cxViewPosition(this);
    if(!cxViewZeroSize(this)){
        cxSize2f size = cxViewContentSize(this);
        cxVec2f delta;
        kmVec2Subtract(&delta, &anchor, &this->anchor);
        opos.x += size.w * delta.x;
        opos.y += size.h * delta.y;
    }
    this->anchor = anchor;
    this->isDirty = true;
    return opos;
}

void cxViewSetFixScale(cxAny pview,cxVec2f scale)
{
    CX_ASSERT_THIS(pview, cxView);
    CX_RETURN(cxVec2fEqu(this->fixscale, scale));
    this->fixscale = scale;
    this->isDirty = true;
}

void cxViewSetScale(cxAny pview,cxVec2f scale)
{
    CX_ASSERT_THIS(pview, cxView);
    CX_RETURN(cxVec2fEqu(this->scale, scale));
    this->scale = scale;
    this->isDirty = true;
}

void cxViewSetDegrees(cxAny pview,cxFloat degrees)
{
    CX_ASSERT_THIS(pview, cxView);
    cxViewSetAngle(this,kmDegreesToRadians(degrees));
}

void cxViewSetRaxis(cxAny pview,cxVec3f raxis)
{
    CX_ASSERT_THIS(pview, cxView);
    CX_RETURN(kmVec3AreEqual(&this->raxis, &raxis));
    this->raxis = raxis;
    this->isDirty = true;
}

cxVec2f cxViewAnchor(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    return this->anchor;
}

cxFloat cxViewAngle(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    return this->angle;
}

void cxViewSetAngle(cxAny pview,cxFloat angle)
{
    CX_ASSERT_THIS(pview, cxView);
    CX_RETURN(cxFloatEqu(this->angle,angle));
    this->angle = angle;
    this->isDirty = true;
}

cxInt cxViewSubviewCount(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    return cxListLength(this->subViews);
}

cxVec2f cxViewScale(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    return cxVec2fv(this->fixscale.x * this->scale.x, this->fixscale.y * this->scale.y);
}

void cxViewTransform(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    CX_RETURN(!this->isDirty);
    cxMatrix4f transMatrix;
    kmMat4Translation(&transMatrix, this->position.x, this->position.y, 0.0f);
    
    cxMatrix4f rotateMatrix;
    kmMat4RotationAxisAngle(&rotateMatrix, &this->raxis, this->angle);
    
    cxVec2f scale = cxViewScale(this);
    cxMatrix4f scaleMatrix;
    kmMat4Scaling(&scaleMatrix, scale.x, scale.y, 1.0f);
    
    cxFloat x = -this->size.w * this->anchor.x;
    cxFloat y = -this->size.h * this->anchor.y;
    kmMat4Translation(&this->anchorMatrix, x, y, 0);
    
    //translate,rotate,scale
    kmMat4Multiply(&this->normalMatrix, &transMatrix, &rotateMatrix);
    kmMat4Multiply(&this->normalMatrix, &this->normalMatrix, &scaleMatrix);
    
    CX_EVENT_FIRE(this, onTransform);
    //
    if(this->isCropping){
        this->scissor = cxViewGLRect(pview);
    }
    this->isDirty = false;
}

void cxViewSetBorderColor(cxAny pview,cxColor3f color)
{
    CX_ASSERT_THIS(pview, cxView);
    this->borderColor = color;
}

static void cxViewDrawBorder(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    cxBox4f b = cxViewBox(this);
    cxBoxVec2f box = cxBoxVec2fFromBox4f(b);
    cxDrawLineBox(&box, this->borderColor);
}

void cxViewEnter(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
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
    CX_ASSERT_THIS(pview, cxView);
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

void cxViewAutoResizing(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
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
        pos.x += size.w * (this->anchor.x + 0.5f) * scale.x;
        if(mask & cxViewAutoOutside){
            pos.x -= (this->autoBox.l + size.w);
        }else{
            pos.x += this->autoBox.l;
        }
    }else if(mask & cxViewAutoResizeRight){
        pos.x = parent->size.w/2.0f;
        pos.x -= size.w * (0.5f - this->anchor.x) * scale.x;
        if(mask & cxViewAutoOutside){
            pos.x += (this->autoBox.r + size.w);
        }else{
            pos.x -= this->autoBox.r;
        }
    }
    //top bottom
    if((mask & cxViewAutoResizeTop) && (mask & cxViewAutoResizeBottom)){
        size.h = parent->size.h - this->autoBox.t - this->autoBox.b;
        this->scale.y = 1.0f;
        this->fixscale.y = 1.0f;
        pos.y = (this->autoBox.b - this->autoBox.t)/2.0f;
    }else if(mask & cxViewAutoResizeTop){
        pos.y = parent->size.h/2.0f;
        pos.y -= size.h * (0.5f - this->anchor.y) * scale.y;
        if(mask & cxViewAutoOutside){
            pos.y += (this->autoBox.t + size.h);
        }else{
            pos.y -= this->autoBox.t;
        }
    }else if(mask & cxViewAutoResizeBottom){
        pos.y  = -parent->size.h/2.0f;
        pos.y += size.h * (this->anchor.y + 0.5) * scale.y;
        if(mask & cxViewAutoOutside){
            pos.y -= (this->autoBox.b + size.h);
        }else{
            pos.y += this->autoBox.b;
        }
    }
    //update pos and size
    cxViewSetPos(this, pos);
    cxViewSetSize(this, size);
}

void cxViewLayout(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    cxViewAutoResizing(this);
    CX_LIST_FOREACH(this->subViews, ele){
        cxView view = ele->any;
        cxViewLayout(view);
    }
    CX_EVENT_FIRE(this, onLayout);
}

//remove from parent
void cxViewRemove(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    CX_RETURN(this->parentView == NULL);
    if(this->isRunning){
        cxViewExit(this);
    }
    cxView parent = this->parentView;
    CX_METHOD_RUN(parent->onRemove,parent,pview);
    //join to remove list
    cxArrayAppend(parent->removes, this);
    //remove draw list
    cxListRemove(parent->subViews, this->subElement);
    this->subElement = NULL;
    this->parentView = NULL;
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
    if(!this->isVisible || !this->isTouch){
        return false;
    }
    cxListElement *head = cxListFirst(this->subViews);
    if(head == NULL){
        goto completed;
    }
    cxListElement *tail = cxListLast(this->subViews);
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
    if(!this->isVisible){
        return false;
    }
    cxListElement *head = cxListFirst(this->subViews);
    if(head == NULL){
        goto completed;
    }
    cxListElement *tail = cxListLast(this->subViews);
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
    cxAction action = CX_TYPE_CAST(cxAction, pav);
    cxActionSetView(action, pview);
    cxUInt actionId = cxActionGetId(action);
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

void cxViewDraw(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    if(!this->isVisible){
        goto finished;
    }
    //update all action
    cxViewUpdateActions(this);
    //do update event
    CX_EVENT_FIRE(this, onUpdate);
    cxViewTransform(this);
    if(this->isSort){
        cxViewSort(this);
    }
    if(this->isFront){
        cxViewCheckFront(this);
    }
    if(this->onlyTransform || !this->isVisible){
        goto finished;
    }
    kmGLPushMatrix();
    kmGLMultMatrix(&this->normalMatrix);
    kmGLMultMatrix(&this->anchorMatrix);
    //check cropping
    if(this->isCropping){
        cxOpenGLEnableScissor(this->scissor);
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
    if(cxEngineInstance()->isShowBorder || this->isShowBorder){
        cxViewDrawBorder(this);
    }
    kmGLPopMatrix();
finished:
    //remove subview
    cxArrayClean(this->removes);
}









