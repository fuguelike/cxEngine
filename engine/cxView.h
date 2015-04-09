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
#include <engine/cxShader.h>
#include "cxInput.h"

CX_C_BEGIN

//map json value
CX_DEF(cxUIMap, cxObject)
    cxStr select;
    cxStr property;
    cxStr jsonkey;
CX_END(cxUIMap, cxObject)

#define cxUIMapS(_p_) cxStrBody((_p_)->select)

#define cxUIMapP(_p_) cxStrBody((_p_)->property)

#define cxUIMapK(_p_) cxStrBody((_p_)->jsonkey)

cxUIMap cxUIMapCreate(cxStr s,cxStr p,cxStr k);

void cxViewUpdateValues(cxAny pview,cxAny values);

void cxViewFireUILoaded(cxAny pview,cxAny values);

cxAny cxViewLoaderFile(cxConstChars path,cxAny values);

#define cxViewCreateFile(p) cxViewLoaderFile(p,NULL)

cxAny cxViewLoaderWithType(cxConstType type,cxAny values);

//with bind value load ui
#define cxViewLoaderUI(_t_,_v_)     cxViewLoaderWithType(_t_##TypeName, _v_)

//without bind value create ui
#define cxViewCreateUI(_t_)         cxViewLoaderWithType(_t_##TypeName, NULL)

cxAny cxViewLoaderJson(cxJson json,cxAny values);

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
    cxViewAutoResizeTopSide     = 1 << 16,
    cxViewAutoResizeBottomSide  = 1 << 17,
    cxViewAutoResizeLeftSide    = 1 << 18,
    cxViewAutoResizeRightSide   = 1 << 19,
    cxViewAutoResizeAllSide     = cxViewAutoResizeTopSide|cxViewAutoResizeBottomSide|cxViewAutoResizeLeftSide|cxViewAutoResizeRightSide,
}cxViewAutoResizeMask;

typedef struct {
    cxVec2f position;
    cxBool hited;
}cxHitInfo;

typedef enum {
    cxViewDirtyNone         = 0,
    cxViewDirtySize         = 1 << 0,   //size changed
    cxViewDirtyScale        = 1 << 1,   //scale changed
    cxViewDirtyPosition     = 1 << 2,   //position changed
    cxViewDirtyColor        = 1 << 3,   //color changed
    cxViewDirtyAnchor       = 1 << 4,   //anchor changed
    cxViewDirtyFixScale     = 1 << 5,   //fix scale changed
    cxViewDirtyRaxis        = 1 << 6,   //rorate raxis changed
    cxViewDirtyAngle        = 1 << 7,   //angle change
    cxViewDirtyTexture      = 1 << 8,   //texture or texture coord change
    cxViewDirtyForce        = 1 << 9,   //force dirty
    cxViewDirtyNormal       = cxViewDirtyPosition|cxViewDirtyAngle|cxViewDirtyRaxis|cxViewDirtyScale|cxViewDirtyFixScale,
}cxViewDirty;

CX_DEF(cxView, cxObject)
    //cxView info
    cxArray removes;            //use delay remove view
    cxBool isRunning;
    cxBool isShowBorder;        //if draw border
    cxBool isSort;              //if do sort view
    cxBool isRemoved;           //if remove
    cxRect4f scissor;
    cxMatrix4f normalMatrix;
    cxMatrix4f anchorMatrix;
    cxArray frontViews;                 //brind front views flags
    cxBool isTestBound;                 //run none cxViewSetIsVisible(view, cxViewContainsGLBox(view));
    CX_FIELD_DEF(cxInt Index);          //draw index
    CX_FIELD_DEF(cxFloat Z);            //Z value
    CX_FIELD_DEF(cxList SubViews);
    CX_FIELD_DEF(cxListElement *Element);
    CX_FIELD_DEF(cxHash Bindes);
    CX_FIELD_DEF(cxHash Binded);
    CX_FIELD_DEF(cxUInt Tag);
    CX_FIELD_DEF(cxViewAutoResizeMask AutoMask);
    CX_FIELD_DEF(cxBox4f  AutoBox);
    CX_FIELD_DEF(cxViewDirty Dirty);
    CX_FIELD_DEF(cxBool SleepTop);
    CX_FIELD_DEF(cxSize2f Size);
    CX_FIELD_DEF(cxVec2f Position);
    CX_FIELD_DEF(cxVec2f Scale);
    CX_FIELD_DEF(cxVec2f FixScale);
    CX_FIELD_DEF(cxVec2f Anchor);       //-0.5f <--> 0.5f
    CX_FIELD_DEF(cxVec3f Raxis);
    CX_FIELD_DEF(cxFloat Angle);
    CX_FIELD_DEF(cxColor4f Color);
    CX_FIELD_DEF(cxColor3f BorderColor);
    CX_FIELD_DEF(cxView ParentView);
    CX_FIELD_DEF(cxBool IsVisible);
    CX_FIELD_DEF(cxBool IsCropping);
    CX_FIELD_DEF(cxHash Actions);
    CX_FIELD_DEF(cxInt Order);
    CX_FIELD_DEF(cxBool IsSleep);
    CX_FIELD_DEF(cxBool IsDraw);
    CX_FIELD_DEF(cxInt Assist);
    CX_FIELD_DEF(cxVec2f Offset);       //postion offset
    //shader
    CX_FIELD_DEF(cxAny Shader);
    //ui info
    CX_FIELD_DEF(cxHash UIViews);       //subviews save to hash
    CX_FIELD_DEF(cxChars UIKey);        //hashkey
    CX_FIELD_DEF(cxJson UIValues);      //bind values
    CX_FIELD_DEF(cxArray UIMaps);       //ui maps
CX_END(cxView, cxObject)

CX_FIELD_IMP(cxView, cxFloat, Z);
CX_FIELD_IMP(cxView, cxInt, Index);
CX_FIELD_GET(cxView, cxVec2f, Offset);
CX_FIELD_IMO(cxView, cxAny, Shader);
void cxViewSetShaders(cxAny pview,cxShader shader);//set all subviews

CX_FIELD_GET(cxView, cxArray, UIMaps);
CX_FIELD_GET(cxView, cxJson, UIValues);
void cxViewSetUIValues(cxAny pthis,const cxJson values);
CX_FIELD_GET(cxView, cxChars, UIKey);
CX_FIELD_GET(cxView, cxHash, UIViews);

CX_FIELD_GET(cxView, cxListElement *, Element);
CX_INLINE const cxMatrix4f *cxViewGetNormalMatrix(cxAny pthis)
{
    CX_ASSERT_THIS(pthis, cxView);
    return &this->normalMatrix;
}
CX_INLINE const cxMatrix4f *cxViewGetAnchorMatrix(cxAny pthis)
{
    CX_ASSERT_THIS(pthis, cxView);
    return &this->anchorMatrix;
}
#define CX_VIEW_FOREACH_SUBVIEWS(_v_,_e_)           \
cxList _e_##subViews = cxViewGetSubViews(_v_);      \
CX_LIST_FOREACH(_e_##subViews, _e_)

CX_FIELD_IMP(cxView, cxInt, Assist);
CX_FIELD_IMP(cxView, cxBool, IsDraw);
CX_FIELD_IMP(cxView, cxBool, IsSleep);
CX_FIELD_IMP(cxView, cxBool, IsVisible);
CX_FIELD_GET(cxView, cxView, ParentView);
CX_FIELD_GET(cxView, cxList, SubViews);
CX_FIELD_IMP(cxView, cxBox4f, AutoBox);
CX_FIELD_IMP(cxView, cxViewAutoResizeMask, AutoMask);
CX_FIELD_IMP(cxView, cxUInt, Tag);
CX_FIELD_GET(cxView, cxHash, Bindes);
CX_FIELD_GET(cxView, cxHash, Binded);
CX_FIELD_IMP(cxView, cxBool, SleepTop);
CX_FIELD_GET(cxView, cxSize2f, Size);
CX_FIELD_GET(cxView, cxVec2f, Position);
CX_FIELD_GET(cxView, cxVec2f, Scale);
CX_FIELD_GET(cxView, cxVec2f, FixScale);
CX_INLINE cxVec2f cxViewTransformScale(cxAny pthis)
{
    CX_ASSERT_THIS(pthis, cxView);
    return cxVec2fv(this->Scale.x * this->FixScale.x, this->Scale.y * this->FixScale.y);
}
CX_FIELD_GET(cxView, cxVec2f, Anchor);
CX_FIELD_GET(cxView, cxVec3f, Raxis);
CX_FIELD_GET(cxView, cxFloat, Angle);
CX_FIELD_GET(cxView, cxColor4f, Color);
CX_FIELD_IMP(cxView, cxColor3f, BorderColor);
CX_FIELD_IMP(cxView, cxBool, IsCropping);

CX_FIELD_GET(cxView, cxViewDirty, Dirty);
CX_INLINE void cxViewSetDirty(cxAny pthis,cxViewDirty dirty)
{
    CX_ASSERT_THIS(pthis, cxView);
    this->Dirty |= dirty;
}
CX_INLINE void cxViewClearDirty(cxAny pthis)
{
    CX_ASSERT_THIS(pthis, cxView);
    this->Dirty = cxViewDirtyNone;
}
CX_INLINE cxBool cxViewIsDirty(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxView);
    return this->Dirty != cxViewDirtyNone;
}

void cxViewReset(cxAny pview);

void cxViewUnBindAll(cxAny pview);

cxAny cxViewBindesFirst(cxAny pview);

cxBool cxViewHasBind(cxAny pview,cxAny bview);

//bd pview and bview bind data
void cxViewBind(cxAny pview,cxAny bview,cxAny bd);

void cxViewUnBind(cxAny pview,cxAny bview);

void cxViewBringFront(cxAny pview);

void cxViewCheckFront(cxAny pview);

cxSize2f cxViewContentSize(cxAny pview);

cxInt cxViewSubviewCount(cxAny pview);

cxBox4f cxViewGetBox(cxAny pview);

cxBool cxViewContainsGLBox(cxAny pview);

cxRect4f cxViewGLRect(cxAny pview);

void cxViewSetShowBorder(cxAny pview,cxBool isShowBorder);

void cxViewSetBorderColor(cxAny pview,cxColor3f color);

cxBool cxViewZeroSize(cxAny pview);

void cxViewSetColor(cxAny pview,cxColor3f color);

void cxViewSetAlpha(cxAny pview,cxFloat alpha);

void cxViewStopAction(cxAny pview,cxUInt actionId);

cxBool cxViewHasAction(cxAny pview,cxUInt actionId);

void cxViewClearActions(cxAny pview);

cxAny cxViewGetAction(cxAny pview,cxUInt actionId);

cxAny cxViewAppendTimer(cxAny pview,cxFloat freq,cxInt repeat);

void cxViewRunAsync(cxAny pview,cxAny async);

#define cxViewAppendAsync(_o_,_t_)  cxViewAppendAction(_o_,cxDriverCreate(_t_))

cxUInt cxViewAppendAction(cxAny pview,cxAny pav);

void cxViewSetOrder(cxAny pview,cxInt order);

cxBool cxViewFireKey(cxAny pview,cxKey *key);

//touch count,current touches
cxBool cxViewFireTouch(cxAny pview,const cxTouchItems *points);

cxAny cxViewAppendTypeImp(cxAny pview,cxConstType type);

#define cxViewAppendType(_p_,_t_)   cxViewAppendTypeImp(_p_,_t_##TypeName)

void cxViewAppendUIView(cxAny pview,cxAny pnview,cxConstChars uikey);

void cxViewAppend(cxAny pview,cxAny newview);

void cxViewPrepend(cxAny pview,cxAny newview);

void cxViewLayout(cxAny pview);

void cxViewParentLayout(cxAny pview);

void cxViewAutoResizing(cxAny pview);

cxHitInfo cxViewHitTest(cxAny pview,cxVec2f wPoint);

cxVec2f cxWindowPointToGLPoint(cxVec2f wPoint);

cxVec2f cxGLPointToWindowPoint(cxVec2f glPoint);

cxVec2f cxViewPointToGLPoint(cxAny pview,cxVec2f pos);

void cxViewSortWithFunc(cxAny pview,cxCmpFunc func);

cxVec2f cxGLPointToViewPoint(cxAny pview,cxVec2f pos);

cxVec2f cxViewPointToWindowPoint(cxAny pview,cxVec2f vPoint);

cxVec2f cxViewPointToParent(cxAny pview,cxVec2f vPoint);

cxVec2f cxWindowPointToViewPoint(cxAny pview,cxVec2f wPoint);

void cxViewSetSize(cxAny pview,cxSize2f size);

void cxViewCheckSort(cxAny pview);

void cxViewSetPosition(cxAny pview,cxVec2f pos);

void cxViewSetOffset(cxAny pview,cxVec2f off);

cxAny cxViewSelect(cxAny pview,cxConstChars key);

#define CX_VIEW_SELECT(_o_,_t_,_n_,_s_) CX_ASSERT_VALUE(cxViewSelect(_o_, _s_), _t_, _n_)

void cxViewSelectUpdate(cxAny pview,cxConstChars select,cxConstChars property,cxJson value);

cxStr cxViewSelectPath(cxAny pview);

void cxViewDelayRun(cxAny pview,cxFloat time,cxAny func);

// -0.5 <-> +0.5
// -width/2 <-> width/2
//return fix pos,for keep view state
cxVec2f cxViewSetAnchor(cxAny pview,cxVec2f anchor);

void cxViewSetScale(cxAny pview,cxVec2f scale);

void cxViewSetFixScale(cxAny pview,cxVec2f fixscale);

void cxViewSetRaxis(cxAny pview,cxVec3f raxis);

void cxViewSetAngle(cxAny pview,cxFloat angle);

void cxViewSetDegrees(cxAny pview,cxFloat degrees);

void cxViewUpdateView(cxAny pview);

void cxViewDrawView(cxAny pview);

void cxViewEnter(cxAny pview);

void cxViewExit(cxAny pview);

void cxViewClear(cxAny pview);

void cxViewRemove(cxAny pview);


CX_C_END

#endif
