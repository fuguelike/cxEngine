//
//  cxEngine.h
//  cxEngine
//  引擎主要定义
//  Created by xuhua on 9/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxEngine_h
#define cxEngine_cxEngine_h

#include <views/cxWindow.h>
#include <actions/cxCurve.h>
#include "cxBase.h"
#include "cxStack.h"
#include "cxTypes.h"
#include "cxInput.h"
#include "cxXMLScript.h"
#include "cxEventItem.h"
#include "cxEventArg.h"
#include "cxUtil.h"

CX_C_BEGIN

#define GL_ASSERT() CX_ASSERT(glGetError() == GL_NO_ERROR,"OpenGL error")

CX_OBJECT_DEF(cxEngine, cxObject)
    cxHash events;
    cxHash scripts;
    cxHash actions;
    cxHash datasets;
    cxCurve curve;
    cxStack autoStack;
    cxFloat frameInterval;
    cxSize2f winsize;
    cxBool isInit;
    cxBool isShowBorder;
    cxWindow window;
    cxDouble lastTime;
    cxFloat frameDelta;
    cxFloat contentScaleFactor;
    CX_SIGNAL_ALLOC(onPause);
    CX_SIGNAL_ALLOC(onResume);
    CX_SIGNAL_ALLOC(onMemory);
    CX_SIGNAL_ALLOC(onUpdate);
    cxTouch touch;
    CX_EVENT_ALLOC(onFree);
    cxString lang;
CX_OBJECT_END(cxEngine)

cxEngine cxEngineInstance();

cxStack cxEngineAutoStack();

void cxEngineRegisteEvent(cxConstChars name,cxEventFunc func);

cxEventItem cxEngineGetEvent(cxConstChars name);

void cxEngineRemoveScript(cxConstChars file);

void cxEngineTimeReset();

cxXMLScript cxEngineGetXMLScript(cxConstChars file);

cxCurveItem cxEngineGetCurve(cxConstChars name);

cxAny cxEngineLoadActionXML(cxConstChars file);

cxTypes cxEngineDataSet(cxConstChars url);

cxString cxEngineLangText(cxConstChars xml,cxConstChars key);

void cxEngineRegisteSystemEvent();

//
extern cxBool cxEngineInit(cxEngine engine);
extern void cxEngineMain(cxEngine engine);
extern void cxEngineFree(cxEngine engine);
//

cxBool cxEngineFireTouch(cxTouchType type,cxVec2f pos);

void cxEngineLookAt(cxMatrix4f *matrix,const cxVec2f point);

void cxEngineExit();

void cxEnginePause();

void cxEngineResume();

void cxEngineMemory();

void cxEngineDraw();

void cxEngineLayout(cxInt width,cxInt height);

void cxEngineDestroy();

CX_C_END

#endif
