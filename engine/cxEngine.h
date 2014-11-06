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
#include <textures/cxTextureCache.h>
#include <socket/cxLooper.h>
#include "cxInput.h"
#include "cxUtil.h"
#include "cxBMPFont.h"
#include "cxGroup.h"
#include "cxIconv.h"
#include "cxPlayer.h"

CX_C_BEGIN

#define GL_ASSERT() CX_ASSERT(glGetError() == GL_NO_ERROR,"OpenGL error")

CX_DEF(cxEngine, cxObject)
    CX_FIELD_DEF(cxSize2f WinSize);     //screen size
    CX_FIELD_DEF(cxSize2f DesSize);     //design size
    CX_FIELD_DEF(cxVec2f Scale);        //dessize / winsize
    CX_FIELD_DEF(cxBool IsShowBorder);  //show debug border
    CX_FIELD_DEF(cxFloat FrameDelta);   //time per frame
    CX_FIELD_DEF(cxString Lang);        //zh en
    CX_FIELD_DEF(cxString Country);     //CN TW
    CX_FIELD_DEF(cxString Localized);   //current lang file path
    CX_FIELD_DEF(cxFloat Interval);     //
    CX_FIELD_DEF(cxWindow Window);      //main window
    CX_METHOD_DEF(cxString, JsonFilter, cxString);
    CX_FIELD_DEF(cxJson Config);
    CX_FIELD_DEF(cxUInt Version);       //version number
    cxBool isInit;
    cxBool isTouch;
    cxBool isPause;
    cxHash files;
    cxHash assets;
    CX_SIGNAL_ALLOC(onPause);
    CX_SIGNAL_ALLOC(onResume);
    CX_SIGNAL_ALLOC(onMemory);
    CX_SIGNAL_ALLOC(onUpdate);
    CX_SIGNAL_ALLOC(onRecvJson);
    cxHash items;           //touch items
    cxTouchItems points;    //all touch points
    cxKey key;
    cxHash groups;          //action groups
    //

    cxCurve curve;              //cxCurve instance
    cxOpenGL opengl;            //cxOpenGL instance
    cxIconv iconv;              //cxIconv instance
    cxPlayer player;            //cxPlayer instance
    cxTextureCache textures;    //cxTextureCache instance
    cxLooper looper;            //socket looper
CX_END(cxEngine, cxObject)

extern cxEngine engineInstance;

CX_INLINE cxEngine cxEngineInstance()
{
    return engineInstance;
}

CX_INLINE cxJson cxEngineGetConfig()
{
    return engineInstance->Config;
}

CX_INLINE cxUInt cxEngineVersion()
{
    return engineInstance->Version;
}

CX_INLINE cxTextureCache cxTextureCacheInstance()
{
    return engineInstance->textures;
}

CX_INLINE cxLooper cxLooperInstance()
{
    return engineInstance->looper;
}

CX_INLINE cxPlayer cxPlayerInstance()
{
    return engineInstance->player;
}

CX_INLINE cxIconv cxIconvInstance()
{
    return engineInstance->iconv;
}

CX_INLINE cxWindow cxEngineGetWindow()
{
    return engineInstance->Window;
}

CX_INLINE cxOpenGL cxOpenGLInstance()
{
    return engineInstance->opengl;
}

CX_INLINE cxCurve cxCurveInstance()
{
    return engineInstance->curve;
}

CX_INLINE void cxEngineSetInterval(cxFloat interval)
{
    engineInstance->Interval = interval;
}

CX_INLINE cxString cxEngineGetLocalized()
{
    return engineInstance->Localized;
}

CX_INLINE cxString cxEngineGetCountry()
{
    return engineInstance->Country;
}

CX_INLINE void cxEngineSetCountry(cxString country)
{
    CX_RETAIN_SWAP(engineInstance->Country, country);
}

CX_INLINE cxString cxEngineGetLang()
{
    return engineInstance->Lang;
}

CX_INLINE void cxEngineSetLang(cxString lng)
{
    CX_RETAIN_SWAP(engineInstance->Lang, lng);
}

CX_INLINE cxFloat cxEngineGetFrameDelta()
{
    return engineInstance->FrameDelta;
}

CX_INLINE void cxEngineSetScale(cxVec2f v)
{
    engineInstance->Scale = v;
}

CX_INLINE cxVec2f cxEngineGetScale()
{
    return engineInstance->Scale;
}

CX_INLINE cxBool cxEngineGetIsShowBorder()
{
    return engineInstance->IsShowBorder;
}

CX_INLINE void cxEngineSetIsShowBorder(cxBool v)
{
#ifdef NDEBUG
    engineInstance->IsShowBorder = false;
#else
    engineInstance->IsShowBorder = v;
#endif
}

//use init method
CX_INLINE void cxEngineSetDesSize(cxSize2f size)
{
    engineInstance->DesSize = size;
}

CX_INLINE cxSize2f cxEngineGetWinSize()
{
    return engineInstance->WinSize;
}

void cxEngineClear();

//type init
CX_EXTERN void cxEngineType(cxEngine engine);

//engine init
CX_EXTERN void cxEngineInit(cxEngine engine);

//engine start draw
CX_EXTERN void cxEngineMain(cxEngine engine);

//engine free
CX_EXTERN void cxEngineFree(cxEngine engine);

cxString cxLocalizedString(cxConstChars key);

cxJson cxEngineJsonReader(cxConstChars src);

cxAny cxEngineCreateObject(cxConstChars src);

cxString cxEngineAssetsData(cxConstChars file);

cxJson cxEngineLoadJson(cxConstChars file);

cxBMPFont cxEngineLoadBMPFont(cxConstChars file);

cxTimer cxEngineTimer(cxFloat freq,cxInt repeat);

cxVec2f cxEngineTouchToWindow(cxVec2f pos);

cxVec2f cxEngineWindowToTouch(cxVec2f pos);

void cxEngineEnableTouch(cxBool enable);

cxBool cxEngineFireTouch(cxTouchType type,cxInt num,cxTouchPoint *points);

cxBool cxEngineFireKey(cxKeyType type,cxInt code);

void cxEngineSendJson(cxString json);

void cxEngineRecvJson(cxString json);

//初始化数据
void cxEngineStartup();

void cxEngineTerminate();

void cxEngineExit();

void cxEnginePause();

void cxEngineResume();

void cxEngineMemory();

void cxEngineDraw(cxFloat dt);

void cxEngineLayout(cxInt width,cxInt height);

void cxEngineDestroy();

CX_C_END

#endif
