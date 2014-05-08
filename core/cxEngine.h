//
//  cxEngine.h
//  Created by xuhua on 9/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxEngine_h
#define cxEngine_cxEngine_h

#include <views/cxWindow.h>
#include <actions/cxCurve.h>
#include "cxBase.h"
#include "cxStack.h"
#include "cxInput.h"
#include "cxEventArg.h"
#include "cxUtil.h"
#include "cxMessage.h"
#include "cxBMPFont.h"
#include "cxUrlPath.h"
#include "cxType.h"
#include "cxController.h"

CX_C_BEGIN

#define GL_ASSERT() CX_ASSERT(glGetError() == GL_NO_ERROR,"OpenGL error")

CX_OBJECT_DEF(cxEngine, cxObject)
    cxHash bmpfonts;
    cxHash jsons;
    cxHash dbenvs;
    cxFloat frameInterval;
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

cxEngine cxEngineInstance();

cxAny cxEngineCreateObject(cxConstChars name);

void cxEngineTimeReset();

cxAny cxEngineDB(cxConstChars url);

cxJson cxEngineLoadJsonFile(cxConstChars file);

cxBMPFont cxEngineLoadBMPFont(cxConstChars file);

void cxEngineSystemInit(cxEngine engine);

//extern
void cxEngineInit(cxEngine engine);

void cxEngineMain(cxEngine engine);

void cxEngineFree(cxEngine engine);
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

cxInt cxEnginePlatform();

void cxEngineExit();

void cxEnginePause();

void cxEngineResume();

void cxEngineMemory();

void cxEngineDraw();

void cxEngineLayout(cxInt width,cxInt height);

void cxEngineDestroy();

CX_C_END

#endif
