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
#include "cxViewLoader.h"

CX_C_BEGIN

#define GL_ASSERT() CX_ASSERT(glGetError() == GL_NO_ERROR,"OpenGL error")

CX_OBJECT_DEF(cxEngine, cxObject)
    cxStack stack;
    cxHash files;
    cxFloat interval;
    cxSize2f winsize;   //screen size
    cxSize2f dessize;   //design size
    cxFloat  desRate;   //design h/w
    cxVec2f scale;
    cxBool isInit;
    cxBool isShowBorder;
    cxBool isTouch;
    cxBool isPause;
    cxWindow window;
    cxDouble lastTime;
    cxFloat frameDelta;
    CX_SIGNAL_ALLOC(onPause);
    CX_SIGNAL_ALLOC(onResume);
    CX_SIGNAL_ALLOC(onMemory);
    CX_SIGNAL_ALLOC(onUpdate);
    CX_SIGNAL_ALLOC(onTouch);
    CX_SIGNAL_ALLOC(onRecvJson);
    CX_EVENT_ALLOC(onExit);
    cxTouch touch;
    cxKey key;
    cxString lang;
CX_OBJECT_END(cxEngine)

void cxEnginePush(cxAny object);

void cxEnginePop();

cxAny cxEngineTop();

void cxTypeRunObjectSetter(cxObject object,cxJson json);

cxAny cxEngineTypeCreate(cxJson json);

cxAny cxObjectLoadWithFile(cxConstChars file);

cxAny cxObjectLoadWithJson(cxJson json);

cxEngine cxEngineInstance();

void cxEngineSetDesignSize(cxSize2f dessize);

void cxEngineTimeReset();

cxJson cxEngineLoadJson(cxConstChars file);

cxBMPFont cxEngineLoadBMPFont(cxConstChars file);

cxTimer cxEngineTimer(cxFloat freq,cxInt repeat);

//engine init
CX_EXTERN void cxEngineInit(cxEngine engine);

//engine start draw
CX_EXTERN void cxEngineMain(cxEngine engine);

//engine free
CX_EXTERN void cxEngineFree(cxEngine engine);
//
cxVec2f cxEngineTouchToWindow(cxVec2f pos);

cxVec2f cxEngineWindowToTouch(cxVec2f pos);

void cxEngineEnableTouch(cxBool enable);

cxBool cxEngineFireTouch(cxTouchType type,cxVec2f pos);

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
