//
//  cxEngine.h
//  Created by xuhua on 9/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxEngine_h
#define cxEngine_cxEngine_h

#include <views/cxWindow.h>
#include <actions/cxCurve.h>
#include <actions/cxTimer.h>
#include <cxcore/cxBase.h>
#include "cxInput.h"
#include "cxUtil.h"
#include "cxBMPFont.h"
#include "cxUrlPath.h"
#include "cxActionMgr.h"

CX_C_BEGIN

#define GL_ASSERT() CX_ASSERT(glGetError() == GL_NO_ERROR,"OpenGL error")

CX_OBJECT_DEF(cxEngine, cxObject)
    cxHash files;
    cxFloat interval;
    cxSize2f winsize;   //screen size
    cxSize2f dessize;   //design size
    cxFloat  desRate;   //design h/w
    cxVec2f scale;
    cxBool isInit;
    cxBool isShowBorder;
    cxBool isTouch;
    cxBool isGesture;
    cxBool isPause;
    cxWindow window;
    cxDouble lastTime;
    cxFloat frameDelta;
    CX_SIGNAL_ALLOC(onPause);
    CX_SIGNAL_ALLOC(onResume);
    CX_SIGNAL_ALLOC(onMemory);
    CX_SIGNAL_ALLOC(onUpdate);
    CX_SIGNAL_ALLOC(onRecvJson);
    CX_EVENT_ALLOC(onExit);

    cxHash items;           //touch items
    cxTouchItems points;    //all touch points
    cxKey key;
    cxString lang;
CX_OBJECT_END(cxEngine, cxObject)

//type init
CX_EXTERN void cxEngineType(cxEngine engine);

//engine init
CX_EXTERN void cxEngineInit(cxEngine engine);

//engine start draw
CX_EXTERN void cxEngineMain(cxEngine engine);

//engine free
CX_EXTERN void cxEngineFree(cxEngine engine);

cxEngine cxEngineInstance();

cxJson cxEngineJsonReader(cxConstChars src);

cxJson cxEngineLoadJson(cxConstChars file);

void cxEngineSetDesignSize(cxSize2f dessize);

void cxEngineTimeReset();

cxBMPFont cxEngineLoadBMPFont(cxConstChars file);

cxTimer cxEngineTimer(cxFloat freq,cxInt repeat);

cxVec2f cxEngineTouchToWindow(cxVec2f pos);

cxVec2f cxEngineWindowToTouch(cxVec2f pos);

void cxEngineEnableTouch(cxBool enable);

cxBool cxEngineFireTouch(cxTouchType type,cxInt num,cxTouchPoint *points);

cxBool cxEngineFireKey(cxKeyType type,cxInt code);

void cxEngineSendJson(cxString json);

void cxEngineRecvJson(cxString json);

void cxEngineSetLocalized(cxString lang);

void cxEngineBegin();

void cxEngineTerminate();

void cxEngineExit();

void cxEnginePause();

void cxEngineResume();

void cxEngineMemory();

void cxEngineDraw();

void cxEngineLayout(cxInt width,cxInt height);

void cxEngineDestroy();

CX_C_END

#endif
