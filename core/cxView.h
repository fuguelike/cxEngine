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
#include <chipmunk/chipmunk.h>
#include <libxml/xmlreader.h>
#include "cxBase.h"
#include "cxList.h"
#include "cxInput.h"
#include "cxHash.h"
#include "cxString.h"
#include "cxXMLScript.h"

CX_C_BEGIN

typedef enum {
    cxChipmunkShapeCircle = 1,
    cxChipmunkShapeBox,
}cxChipmunkShape;

typedef struct {
    cxChipmunkShape shape;
    cxVec2f cp;         //when shape == cxChipmunkShapeCircle
    cxBool isStatic;
    cxFloat m;
    cxFloat e;
    cxFloat u;
    cpGroup group;
    cpLayers layer;
    cpCollisionType ctype;
}cxChipmunkAttr;

// L200,R100,T100,B100

typedef enum {
    cxViewAutoResizeNone        = 0,
    cxViewAutoResizeLeft        = 1 << 0,
    cxViewAutoResizeRight       = 1 << 1,
    cxViewAutoResizeTop         = 1 << 2,
    cxViewAutoResizeBottom      = 1 << 3,
    cxViewAutoResizeWidth       = cxViewAutoResizeLeft|cxViewAutoResizeRight,
    cxViewAutoResizeHeight      = cxViewAutoResizeTop|cxViewAutoResizeBottom,
    cxViewAutoResizeFill        = cxViewAutoResizeWidth|cxViewAutoResizeHeight,
}cxViewAutoResizeMask;

typedef enum{
    cxViewIsTouchTypeNone       = 0,
    cxViewIsTouchTypeSelf       = 1 << 0,   //only touch self
    cxViewIsTouchTypeSubview    = 1 << 1,   //touch subviews
}cxViewIsTouchType;

typedef cxBox4f cxViewAutoResizeBox;

CX_OBJECT_DEF(cxView, cxObject)
    cxChipmunkAttr *cAttr;
    cxAny args;
    cxViewAutoResizeMask autoMask;
    cxViewAutoResizeBox  autoBox;
    cxHash caches;
    cxHash actions;
    cxArray removes;
    cxInt zorder;
    cxListElement *subElement;
    cxList subViews;
    cxView parentView;      //parent view
    cxBool isRunning;
    cxBool isDirty;
    cxBool isVisible;
    cxBool isBorder;        //if draw border
    cxBool hideTop;         //=true hide prev view
    cxBool isSort;
    cxBool isCropping;
    cxSize2f size;
    cxVec2f position;
    cxVec2f scale;
    cxVec2f fixscale;       //use fit scree size
    cxVec2f anchor;
    cxVec3f raxis;
    cxFloat angle;
    cxRect4f scissor;
    cxMatrix4f normalMatrix;
    cxMatrix4f anchorMatrix;
    cxColor4f color;
    CX_METHOD_ALLOC(cxViewIsTouchType,IsTouch,cxAny,cxTouch *);
    CX_METHOD_ALLOC(cxBool, Touch, cxAny, cxTouch *);
    CX_METHOD_ALLOC(cxViewIsTouchType, IsOnKey, cxAny, cxKey *);
    CX_METHOD_ALLOC(cxBool, OnKey, cxAny, cxKey *);
    CX_METHOD_ALLOC(void, Draw, cxAny);
    CX_METHOD_ALLOC(void, DrawAfter, cxAny);
    CX_METHOD_ALLOC(void, DrawBefore, cxAny);
    CX_SIGNAL_ALLOC(EmmitDraw);
    CX_EVENT_ALLOC(onEnter);
    CX_EVENT_ALLOC(onExit);
    CX_EVENT_ALLOC(onUpdate);
    CX_EVENT_ALLOC(onResize);
    CX_EVENT_ALLOC(onLayout);
    CX_EVENT_ALLOC(onDirty);
CX_OBJECT_END(cxView)

cxChipmunkAttr *cxViewSupportChipmunk(cxAny pview);

void cxViewSetCache(cxAny pview,cxConstChars key,cxAny object);

cxAny cxViewGetCache(cxAny pview,cxConstChars key);

void cxViewSetCropping(cxAny pview,cxBool cropping);

cxVec2f cxViewPosition(cxAny pview);

void cxViewSetArgs(cxAny pview,cxAny args);

cxAny cxViewArgs(cxAny pview);

cxVec2f cxViewScale(cxAny pview);

cxSize2f cxViewSize(cxAny pview);

cxColor4f cxViewColor(cxAny pview);

cxBox4f cxViewBox(cxAny pview);

cxBool cxViewContainsGLBox(cxAny pview);

cxRect4f cxViewGLRect(cxAny pview);

void cxViewSetTop(cxAny pview,cxBool top);

void cxViewSetDirty(cxAny pview,cxBool dirty);

void cxViewOnUpdate(cxAny pview,cxEventFunc func);

void cxViewSetBorder(cxAny pview,cxBool border);

cxBool cxViewZeroSize(cxAny pview);

void cxViewReadAttr(cxReaderAttrInfo *info);

cxAny cxViewGetParentView(cxAny pview);

void cxViewSetColor(cxAny pview,cxColor3f color);

void cxViewSetAlpha(cxAny pview,cxFloat alpha);

void cxViewStopAction(cxAny pview,cxUInt actionId);

cxBool cxViewHasAction(cxAny pview,cxUInt actionId);

cxAny cxViewAppendTimer(cxAny pview,cxFloat time,cxInt repeat);

cxUInt cxViewAppendAction(cxAny pview,cxAny pav);

void cxViewSetOrder(cxAny pview,cxInt order);

cxBool cxViewOnKey(cxAny pview,cxKey *key);

cxUInt cxViewIsOnKey(cxAny pview,cxKey *key);

cxBool cxViewTouch(cxAny pview,cxTouch *touch);

cxUInt cxViewIsTouch(cxAny pview,cxTouch *touch);

void cxViewAppend(cxAny pview,cxAny newview);

void cxViewLayout(cxAny pview);

void cxViewAutoResizing(cxAny pview);

cxBool cxViewHitTest(cxAny pview,cxVec2f wPoint,cxVec2f *vPoint);

cxVec2f cxWindowPointToGLPoint(cxVec2f wPoint);

cxVec2f cxGLPointToWindowPoint(cxVec2f glPoint);

cxVec2f cxViewPointToGLPoint(cxAny pview,cxVec2f pos);

cxVec2f cxGLPointToViewPoint(cxAny pview,cxVec2f pos);

cxVec2f cxViewPointToWindowPoint(cxAny pview,cxVec2f vPoint);

cxVec2f cxWindowPointToViewPoint(cxAny pview,cxVec2f wPoint);

void cxViewSetAutoResizeBox(cxAny pview,cxBox4f box);

void cxViewSetAutoResizeMask(cxAny pview,cxViewAutoResizeMask mask);

void cxViewSetSize(cxAny pview,cxSize2f size);

void cxViewSort(cxAny pview);

void cxViewSetVisible(cxAny pview,cxBool visible);

void cxViewSetPosImp(cxAny pview,cxVec2f pos);

void cxViewSetPos(cxAny pview,cxVec2f pos);

void cxViewSetAnchor(cxAny pview,cxVec2f anchor);

void cxViewSetScale(cxAny pview,cxVec2f scale);

void cxViewSetFixScale(cxAny pview,cxVec2f scale);

void cxViewSetRaxis(cxAny pview,cxVec3f raxis);

cxFloat cxViewAngle(cxAny pview);

void cxViewSetAngle(cxAny pview,cxFloat angle);

void cxViewSetDegrees(cxAny pview,cxFloat degrees);

void cxViewDraw(cxAny pview);

void cxViewEnter(cxAny pview);

void cxViewExit(cxAny pview);

void cxViewRemoved(cxAny pview);

void cxViewTransform(cxAny pview);

CX_C_END

#endif
