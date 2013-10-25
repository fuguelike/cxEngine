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
#include <libxml/xmlreader.h>
#include "cxTypes.h"
#include "cxBase.h"
#include "cxList.h"
#include "cxInput.h"
#include "cxHash.h"
#include "cxString.h"
#include "cxXMLScript.h"

CX_C_BEGIN

// L200,R100,T100,B100

typedef enum {
    cxViewAutoResizeNone        = 0,
    cxViewAutoResizeLeft        = 1 << 0,
    cxViewAutoResizeRight       = 1 << 1,
    cxViewAutoResizeTop         = 1 << 2,
    cxViewAutoResizeBottom      = 1 << 3,
    cxViewAutoResizeFill        = cxViewAutoResizeLeft|cxViewAutoResizeRight|cxViewAutoResizeTop|cxViewAutoResizeBottom,
}cxViewAutoResizeMask;

typedef struct {
    cxViewAutoResizeMask mask;
    cxBox4f box;
}cxViewAutoResizeVar;

typedef void (*cxViewFunc)(cxAny pview);

typedef cxBool (*cxViewTouchFunc)(cxAny pview,const cxTouch *touch);

CX_OBJECT_DEF(cxView, cxObject)
    cxAny args;
    cxViewAutoResizeVar resizeVar;
    cxHash actions;
    cxInt zorder;
    cxListElement *subElement;
    cxList subViews;
    cxView parentView;      //parent view
    cxBool isRunning;
    cxBool isDirty;
    cxBool isVisible;
    cxBool isBorder;        //if draw border
    cxBool isTop;           //=true hide prev view
    cxBool isSort;
    cxBool isCropping;
    cxSize2f size;
    cxVec2f position;
    cxVec2f scale;
    cxVec2f anchor;
    cxVec3f raxis;
    cxFloat radians;
    cxRect4f scissor;
    cxMatrix4f normalMatrix;
    cxMatrix4f anchorMatrix;
    cxColor4f color;
    CX_METHOD_DEF(cxViewTouchFunc, IsTouch);
    CX_METHOD_DEF(cxViewTouchFunc, Touch);
    CX_METHOD_DEF(cxViewFunc, Draw);
    CX_METHOD_DEF(cxViewFunc, Transform);
    CX_METHOD_DEF(cxViewFunc, DrawAfter);
    CX_METHOD_DEF(cxViewFunc, DrawBefore);
    CX_EVENT_ALLOC(onEnter);
    CX_EVENT_ALLOC(onExit);
    CX_EVENT_ALLOC(onUpdate);
    CX_EVENT_ALLOC(onResize);
    CX_EVENT_ALLOC(onLayout);
    CX_EVENT_ALLOC(onChanged);//subviews changed
CX_OBJECT_END(cxView)

void cxViewSetCropping(cxAny pview,cxBool cropping);

cxVec2f cxViewPosition(cxAny pview);

void cxViewSetArgs(cxAny pview,cxAny args);

cxAny cxViewArgs(cxAny pview);

cxSize2f cxViewSize(cxAny pview);

cxColor4f cxViewColor(cxAny pview);

cxRect4f cxViewGLRect(cxAny pview);

void cxViewSetTop(cxAny pview,cxBool top);

void cxViewSetDirty(cxAny pview,cxBool dirty);

void cxViewOnUpdate(cxAny pview,cxEventFunc func,cxAny args);

void cxViewSetBorder(cxAny pview,cxBool border);

cxBool cxViewZeroSize(cxAny pview);

void cxViewXMLReadAttr(cxAny pxml,cxAny view, xmlTextReaderPtr reader);

cxAny cxViewGetParentView(cxAny pview);

void cxViewSetColor(cxAny pview,cxColor3f color);

void cxViewSetAlpha(cxAny pview,cxFloat alpha);

cxUInt cxViewAppendAction(cxAny pview,cxAny pav);

void cxViewSetOrder(cxAny pview,cxInt order);

cxBool cxViewTouch(cxAny pview,const cxTouch *touch);

cxBool cxViewIsTouch(cxAny pview,const cxTouch *touch);

void cxViewAppend(cxAny pview,cxAny newview);

void cxViewLayout(cxAny pview);

void cxViewAutoResizing(cxAny pview);

cxBool cxViewHitTest(cxAny pview,const cxTouch *touch,cxVec2f *pos);

cxVec2f cxWindowPointToGLPoint(const cxVec2f wPoint);

cxVec2f cxGLPointToWindowPoint(const cxVec2f glPoint);

cxVec2f cxViewPointToGLPoint(cxAny pview,const cxVec2f pos);

cxVec2f cxGLPointToViewPoint(cxAny pview,const cxVec2f pos);

cxVec2f cxViewPointToWindowPoint(cxAny pview,const cxVec2f vPoint);

cxVec2f cxWindowPointToViewPoint(cxAny pview,const cxVec2f glPoint);

void cxViewSetBox(cxAny pview, const cxBoxVec2f box);

void cxViewSetAutoResizeBox(cxAny pview,const cxBox4f box);

void cxViewSetAutoResizeMask(cxAny pview,cxViewAutoResizeMask mask);

void cxViewSetSize(cxAny pview,const cxSize2f size);

void cxViewSort(cxAny pview);

void cxViewSetVisible(cxAny pview,cxBool visible);

void cxViewSetPosition(cxAny pview,const cxVec2f position);

void cxViewSetAnchor(cxAny pview,const cxVec2f anchor);

void cxViewSetScale(cxAny pview,const cxVec2f scale);

void cxViewSetRaxis(cxAny pview,cxVec3f raxis);

void cxViewSetRadians(cxAny pview,const cxFloat radians);

void cxViewSetDegrees(cxAny pview,const cxFloat degrees);

void cxViewDraw(cxAny pview);

void cxViewEnter(cxAny pview);

void cxViewExit(cxAny pview);

void cxViewRemoved(cxAny pview);

cxBoxVec2f cxViewLocationBox(cxAny pview);

void cxViewTransform(cxAny pview);

CX_C_END

#endif
