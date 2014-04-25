//
//  cxWindow.h
//  cxEngine
//
//  Created by xuhua on 10/2/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxWindow_h
#define cxEngine_cxWindow_h

#include <core/cxView.h>
#include <core/cxStack.h>

CX_C_BEGIN

CX_OBJECT_DEF(cxWindow, cxView)
    cxStack views;
CX_OBJECT_END(cxWindow)

cxAny cxWindowTopView();

void cxWindowPushView(cxAny pview);

void cxWindowPopView();

void cxWindowReplaceView(cxAny pview);

CX_C_END

#endif
