//
//  cxWindow.h
//  cxEngine
//
//  Created by xuhua on 10/2/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxWindow_h
#define cxEngine_cxWindow_h

#include <engine/cxView.h>
#include <engine/cxAction.h>

CX_C_BEGIN

CX_DEF(cxWindow, cxView)
    cxStack views;
CX_END(cxWindow, cxView)

cxAny cxWindowHeadView();

cxAny cxWindowTailView();

cxAny cxWindowTopView();

void cxWindowClear();

void cxWindowPushView(cxAny pview);

void cxWindowPopView();

void cxWindowReplaceView(cxAny pview);

void cxWindowAppend(cxAny pview);

void cxWindowAppendTop(cxAny pview);

CX_C_END

#endif
