//
//  cxView.h
//  cxEngine
//
//  Created by xuhua on 10/2/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxView_h
#define cxEngine_cxView_h

#include <libgen.h>
#include <cxcore/cxBase.h>
#include "cxInput.h"
#include "cxActionMgr.h"

CX_C_BEGIN

typedef enum {
    cxViewAutoResizeNone        = 0,
    cxViewAutoResizeLeft        = 1 << 0,
    cxViewAutoResizeRight       = 1 << 1,
    cxViewAutoResizeTop         = 1 << 2,
    cxViewAutoResizeBottom      = 1 << 3,
    cxViewAutoResizeWidth       = cxViewAutoResizeLeft | cxViewAutoResizeRight,
    cxViewAutoResizeHeight      = cxViewAutoResizeTop | cxViewAutoResizeBottom,
    cxViewAutoResizeLeftTop     = cxViewAutoResizeLeft | cxViewAutoResizeTop,
    cxViewAutoResizeLeftBottom  = cxViewAutoResizeBottom | cxViewAutoResizeLeft,
    cxViewAutoResizeRightTop    = cxViewAutoResizeTop | cxViewAutoResizeRight,
    cxViewAutoResizeRightBottom = cxViewAutoResizeBottom | cxViewAutoResizeRight,
    cxViewAutoResizeFill        = cxViewAutoResizeWidth | cxViewAutoResizeHeight,
    cxViewAutoOutside           = 1 << 16
}cxViewAutoResizeMask;

#define CX_HASH_KEY_TO_ANY(_e_) (*(cxAny *)(_e_)->key)

CX_OBJECT_DEF(cxView, cxObject)
    cxHash bindes;//bind's views
    cxHash binded;//binded's views
    cxInt tag;
    cxViewAutoResizeMask autoMask;
    cxBox4f  autoBox;
    cxHash actions;
    cxArray removes;
    cxArray appends;
    cxInt zorder;
    cxListElement *subElement;
    cxList subViews;
    cxView parentView;      //parent view
    cxBool isPrepend;
    cxBool isFront;    //
    cxBool isRunning;
    cxBool isDirty;
    cxBool isVisible;
    cxBool onlyTransform;   //only process transform
    cxBool isShowBorder;    //if draw border
    cxBool hideTop;         //=true hide prev view when use cxWindowPush
    cxBool isSort;
    cxBool isCropping;
    cxBool isTouch;     //enable touch
    cxBool isRemoved;   //if remove
    cxSize2f size;
    cxVec2f position;
    cxVec2f scale;
    cxVec2f fixscale;
    cxVec2f anchor;
    cxVec3f raxis;
    cxFloat angle;
    cxRect4f scissor;
    cxMatrix4f normalMatrix;
    cxMatrix4f anchorMatrix;
    cxColor4f color;
    cxColor3f borderColor;

    CX_METHOD_DEF(cxBool, Touch, cxAny, cxTouchItems *);
    CX_METHOD_DEF(cxBool, Key, cxAny, cxKey *);
    CX_METHOD_DEF(void, Draw, cxAny);
    CX_METHOD_DEF(void, After, cxAny);
    CX_METHOD_DEF(void, Before, cxAny);
    //method event
    CX_METHOD_DEF(void, onAppend,cxAny,cxAny);
    CX_METHOD_DEF(void, onRemove,cxAny,cxAny);
    CX_METHOD_DEF(void, onSort,cxAny);

    CX_SIGNAL_ALLOC(onDraw);        //(cxAny pview)

    CX_EVENT_ALLOC(onEnter);    //when view appended
    CX_EVENT_ALLOC(onExit);     //when view removed
    CX_EVENT_ALLOC(onUpdate);
    CX_EVENT_ALLOC(onResize);
    CX_EVENT_ALLOC(onLayout);
    CX_EVENT_ALLOC(onTransform);
CX_OBJECT_END(cxView, cxObject)

void cxViewEnableTouch(cxAny pview,cxBool enable);

cxBool cxViewIsRunning(cxAny pview);

cxMatrix4f *cxViewNormalMatrix(cxAny pview);

cxMatrix4f *cxViewAnchorMatrix(cxAny pview);

void cxViewUnBindAll(cxAny pview);

cxAny cxViewBindesFirst(cxAny pview);

cxHash cxViewBindes(cxAny pview);

cxHash cxViewBinded(cxAny pview);

//bd pview and bview bind data
void cxViewBind(cxAny pview,cxAny bview,cxAny bd);

void cxViewUnBind(cxAny pview,cxAny bview);

void cxViewBringFront(cxAny pview);

void cxViewCheckFront(cxAny pview);

void cxViewSetCropping(cxAny pview,cxBool cropping);

cxAny cxViewParent(cxAny pview);

cxVec2f cxViewTouchDelta(cxAny pview,cxTouchItem item);

cxVec2f cxViewPosition(cxAny pview);

cxList cxViewSubViews(cxAny pview);

cxVec2f cxViewScale(cxAny pview);

cxSize2f cxViewContentSize(cxAny pview);

cxSize2f cxViewSize(cxAny pview);

cxInt cxViewTag(cxAny pview);

void cxViewSetTag(cxAny pview,cxInt tag);

cxInt cxViewSubviewCount(cxAny pview);

cxColor4f cxViewColor(cxAny pview);

cxBox4f cxViewBox(cxAny pview);

cxBool cxViewContainsGLBox(cxAny pview);

cxRect4f cxViewGLRect(cxAny pview);

void cxViewSetHideTop(cxAny pview,cxBool hideTop);

void cxViewSetDirty(cxAny pview,cxBool dirty);

void cxViewSetShowBorder(cxAny pview,cxBool isShowBorder);

void cxViewSetBorderColor(cxAny pview,cxColor3f color);

cxBool cxViewZeroSize(cxAny pview);

cxAny cxViewGetParentView(cxAny pview);

void cxViewSetColor(cxAny pview,cxColor3f color);

void cxViewSetAlpha(cxAny pview,cxFloat alpha);

void cxViewStopAction(cxAny pview,cxUInt actionId);

cxBool cxViewHasAction(cxAny pview,cxUInt actionId);

void cxViewCleanActions(cxAny pview);

cxAny cxViewGetAction(cxAny pview,cxUInt actionId);

cxAny cxViewAppendTimer(cxAny pview,cxFloat freq,cxInt repeat);

cxUInt cxViewAppendAction(cxAny pview,cxAny pav);

void cxViewSetOrder(cxAny pview,cxInt order);

cxBool cxViewKey(cxAny pview,cxKey *key);

//touch count,current touches
cxBool cxViewTouch(cxAny pview,cxTouchItems *points);

void cxViewAppend(cxAny pview,cxAny newview);

void cxViewPrepend(cxAny pview,cxAny newview);

void cxViewLayout(cxAny pview);

void cxViewAutoResizing(cxAny pview);

cxBool cxViewHitTest(cxAny pview,cxVec2f wPoint,cxVec2f *vPoint);

cxVec2f cxWindowPointToGLPoint(cxVec2f wPoint);

cxVec2f cxGLPointToWindowPoint(cxVec2f glPoint);

cxVec2f cxViewPointToGLPoint(cxAny pview,cxVec2f pos);

void cxViewSortWithFunc(cxAny pview,cxCmpFunc func);

cxVec2f cxGLPointToViewPoint(cxAny pview,cxVec2f pos);

cxVec2f cxViewPointToWindowPoint(cxAny pview,cxVec2f vPoint);

cxVec2f cxWindowPointToViewPoint(cxAny pview,cxVec2f wPoint);

void cxViewSetAutoResizeBox(cxAny pview,cxBox4f box);

void cxViewSetAutoResizeMask(cxAny pview,cxViewAutoResizeMask mask);

void cxViewSetSize(cxAny pview,cxSize2f size);

void cxViewSort(cxAny pview);

void cxViewSetVisible(cxAny pview,cxBool visible);

void cxViewSetPos(cxAny pview,cxVec2f pos);

void cxViewSetOnlyTransform(cxAny pview,cxBool v);

// -0.5 <-> +0.5
// -width/2 <-> width/2
//return fix pos,for keep view state
cxVec2f cxViewSetAnchor(cxAny pview,cxVec2f anchor);

void cxViewSetScale(cxAny pview,cxVec2f scale);

void cxViewSetFixScale(cxAny pview,cxVec2f scale);

void cxViewSetRaxis(cxAny pview,cxVec3f raxis);

cxVec2f cxViewAnchor(cxAny pview);

cxFloat cxViewAngle(cxAny pview);

void cxViewSetAngle(cxAny pview,cxFloat angle);

void cxViewSetDegrees(cxAny pview,cxFloat degrees);

void cxViewDraw(cxAny pview);

void cxViewEnter(cxAny pview);

void cxViewExit(cxAny pview);

void cxViewRemove(cxAny pview);

void cxViewTransform(cxAny pview);

CX_C_END

#endif
