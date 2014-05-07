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

//parse type id subview property
static cxAny cxViewLoadByJson(cxAny pview, cxJson json, cxHash hash)
{
    CX_ASSERT(json != NULL, "json args error");
    cxEngine engine = cxEngineInstance();
    //get view type
    cxConstChars type = cxJsonConstChars(json, "type");
    if(type == NULL){
        CX_ERROR("json must has type field");
        return NULL;
    }
    //create view
    cxView view = CX_METHOD_GET(NULL, engine->CreateView,type);
    if(view == NULL){
        CX_ERROR("create view %s failed");
        return NULL;
    }
    //save view to cache
    cxConstChars id = cxJsonConstChars(json, "id");
    if(id != NULL && hash != NULL && cxHashSet(hash, cxHashStrKey(id), view)){
        CX_WARN("view id %s,exists hash");
    }
    //init view
    CX_METHOD_RUN(view->InitView,view,json);
    //load subview
    cxJson subviews = cxJsonArray(json, "subviews");
    CX_JSON_ARRAY_EACH_BEG(subviews, item)
    {
        cxView subview = cxViewLoadByJson(view, item, hash);
        if(subview != NULL){
            cxViewAppend(view, subview);
        }
    }
    CX_JSON_ARRAY_EACH_END(subviews, item)
    return view;
}
//save to hash with id
cxAny cxViewLoad(cxConstChars file,cxHash hash)
{
    cxString data = cxAssetsData(file);
    if(data == NULL){
        return NULL;
    }
    cxJson json = cxJsonCreate(data);
    if(json == NULL){
        return NULL;
    }
    return cxViewLoadByJson(NULL,json, hash);
}

void cxViewSetCropping(cxAny pview,cxBool cropping)
{
    cxView this = pview;
    this->isCropping = cropping;
}

cxBool cxViewZeroSize(cxAny pview)
{
    cxView this = pview;
    return cxSize2Zero(this->size);
}

void cxViewInitView(cxAny pview,cxJson json)
{
    cxView this = pview;
    this->position.x = cxJsonDouble(json, "position.x", this->position.x);
    this->position.y = cxJsonDouble(json, "position.y", this->position.y);
    this->size.w = cxJsonDouble(json, "size.w", this->size.w);
    this->size.h = cxJsonDouble(json, "size.h", this->size.h);
    this->anchor.x = cxJsonDouble(json, "anchor.x", this->anchor.x);
    this->anchor.y = cxJsonDouble(json, "anchor.y", this->anchor.y);
    this->isShowBorder = cxJsonBool(json, "border", this->isShowBorder);
    this->color.a = cxJsonDouble(json, "color.a", this->color.a);
    this->color.r = cxJsonDouble(json, "color.r", this->color.r);
    this->color.g = cxJsonDouble(json, "color.g", this->color.g);
    this->color.b = cxJsonDouble(json, "color.b", this->color.b);
    this->raxis.x = cxJsonDouble(json, "raxis.x", this->raxis.x);
    this->raxis.y = cxJsonDouble(json, "raxis.y", this->raxis.y);
    this->raxis.z = cxJsonDouble(json, "raxis.z", this->raxis.z);
    this->scale.x = cxJsonDouble(json, "scale.x", this->scale.x);
    this->scale.y = cxJsonDouble(json, "scale.y", this->scale.y);
    this->fixscale.x = cxJsonDouble(json, "fixscale.x", this->fixscale.x);
    this->fixscale.x = cxJsonDouble(json, "fixscale.y", this->fixscale.y);
    this->isVisible = cxJsonBool(json, "visible", true);
    cxFloat degrees = cxJsonDouble(json, "degrees", INFINITY);
    if(!isinf(degrees)){
        cxViewSetDegrees(this, degrees);
    }
    this->hideTop = cxJsonBool(json, "hidetop", this->hideTop);
    this->isCropping = cxJsonBool(json, "cropping", this->isCropping);
    this->zorder = cxJsonInt(json, "zorder", this->zorder);
    this->isDirty = true;
}

CX_OBJECT_INIT(cxView, cxObject)
{
    this->hideTop = true;
    this->isShowBorder = false;
    this->isVisible = true;
    this->isDirty = true;
    this->color = cxColor4fv(1.0f, 1.0f, 1.0f, 1.0f);
    this->size = cxSize2fv(0.0f, 0.0f);
    this->anchor = cxVec2fv(0.5f, 0.5f);
    this->raxis = cxVec3fv(0.0f, 0.0f, 1.0f);
    this->scale = cxVec2fv(1.0f, 1.0f);
    this->fixscale = cxVec2fv(1.0f, 1.0f);

    CX_METHOD_OVERRIDE(this->IsTouch, cxViewIsTouch);
    CX_METHOD_OVERRIDE(this->IsOnKey, cxViewIsOnKey);
    CX_METHOD_OVERRIDE(this->InitView, cxViewInitView);
    
    this->subViews = CX_ALLOC(cxList);
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
    
    CX_EVENT_RELEASE(this->onDirty);
    CX_EVENT_RELEASE(this->onEnter);
    CX_EVENT_RELEASE(this->onExit);
    CX_EVENT_RELEASE(this->onUpdate);
    CX_EVENT_RELEASE(this->onResize);
    CX_EVENT_RELEASE(this->onLayout);
    
    CX_SIGNAL_RELEASE(this->onDraw);
    
    CX_METHOD_RELEASE(this->InitView);
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

void cxViewSetShowBorder(cxAny pview,cxBool isShowBorder)
{
    cxView this = pview;
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
    CX_ASSERT(newview != NULL && new->subElement == NULL, "newview null or add to view");
    new->subElement = cxListAppend(this->subViews, new);
    new->parentView = this;
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
    //update pos and size
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
    return cxViewIsTouchTypeThis | cxViewIsTouchTypeSubview;
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
    if(type & cxViewIsTouchTypeThis){
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
    return cxViewIsTouchTypeThis | cxViewIsTouchTypeSubview;
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
    if(type & cxViewIsTouchTypeThis){
        return CX_METHOD_GET(false, this->OnKey,this , key);
    }
    return false;
}

void cxViewCleanActions(cxAny pview)
{
    cxView this = pview;
    cxHashClean(this->actions);
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
    if(cxEngineInstance()->isShowBorder || this->isShowBorder){
        cxViewDrawBorder(this);
    }
    kmGLPopMatrix();
finished:
    //remove subview
    cxArrayClean(this->removes);
}









