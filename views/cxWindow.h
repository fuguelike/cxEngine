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

cxAny cxWindowPushXML(cxConstChars xml);

cxAny cxWindowReplaceXML(cxConstChars xml);

cxAny cxWindowTopView();

void cxWindowPopXML();

void cxWindowPushView(cxAny pview,cxAny args);

void cxWindowPopView(cxAny args);

void cxWindowReplaceView(cxAny pview,cxAny args);

CX_C_END

#endif
