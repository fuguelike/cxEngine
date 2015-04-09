//
//  cxEngine.h
//  Created by xuhua on 9/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxEngine_h
#define cxEngine_cxEngine_h

#include <views/cxWindow.h>
#include <cxcore/cxBase.h>
#include <textures/cxTextureCache.h>
#include "cxInput.h"
#include "cxUtil.h"
#include "cxBMPFont.h"
#include "cxGroup.h"
#include "cxPlayer.h"
#include "cxLooper.h"

CX_C_BEGIN

#define GL_ASSERT() CX_ASSERT(glGetError() == GL_NO_ERROR,"OpenGL error")

CX_STRING_KEY_DEF(cxEngineNoticPause);
CX_STRING_KEY_DEF(cxEngineNoticResume);
CX_STRING_KEY_DEF(cxEngineNoticMemory);
CX_STRING_KEY_DEF(cxEngineNoticUpdate);     //update opt
CX_STRING_KEY_DEF(cxEngineNoticRecvJson);   //recv json

CX_DEF(cxEngine, cxObject)
    CX_FIELD_DEF(cxSize2f WinSize);     //screen size
    CX_FIELD_DEF(cxSize2f DesSize);     //design size
    CX_FIELD_DEF(cxVec2f Scale);        //dessize / winsize
    CX_FIELD_DEF(cxBool IsDebug);       //debug border
    CX_FIELD_DEF(cxFloat FixDelta);     //time per frame fix time = 1.0f/60.0f
    CX_FIELD_DEF(cxFloat TimeDelta);    //real pre frame time
    CX_FIELD_DEF(cxStr Lang);        //zh en
    CX_FIELD_DEF(cxStr Country);     //CN TW
    CX_FIELD_DEF(cxStr Localized);   //current lang file path
    CX_FIELD_DEF(cxFloat Interval);     //default 1.0f/60.0f
    CX_FIELD_DEF(cxWindow Window);      //main window
    CX_FIELD_DEF(cxJson Manifest);      //Manifest.json
    CX_FIELD_DEF(cxUInt Version);       //version number
    CX_FIELD_DEF(cxFloat ScaleFactor);  //ios screen content ScaleFactor
    CX_FIELD_DEF(cxUInt64 TotalFrames); //total frames
    CX_FIELD_DEF(cxBool IsInit);
    CX_FIELD_DEF(cxBool IsTouch);
    CX_FIELD_DEF(cxBool IsPause);
    CX_FIELD_DEF(cxInt Accel);           //update step default:1
    cxDouble prevTime;
    cxHash files;
    cxHash assets;
    cxHash items;                       //touch items
    cxTouchItems points;                //all touch points
    cxKey key;
    cxHash groups;                      //action groups
    cxCurve curve;                      //cxCurve instance
    cxOpenGL opengl;                    //cxOpenGL instance
    cxPlayer player;                    //cxPlayer instance
    cxTextureCache textures;            //cxTextureCache instance
    cxLooper looper;
CX_END(cxEngine, cxObject)

CX_FIELD_IMP(cxEngine, cxBool, IsInit);
CX_FIELD_IMP(cxEngine, cxBool, IsTouch);
CX_FIELD_IMP(cxEngine, cxBool, IsPause);

extern cxEngine engine;

CX_INLINE cxEngine cxEngineInstance()
{
    return engine;
}

CX_INLINE void cxEngineSetAccel(cxInt accel)
{
    engine->Accel = accel;
}

CX_INLINE void cxEngineSetAccelTime(cxFloat time)
{
    cxInt accel = time / engine->FixDelta;
    accel = CX_MAX(accel, 1);
    cxEngineSetAccel(accel);
}

CX_INLINE cxInt64 cxEngineTotalFrames()
{
    return engine->TotalFrames;
}

CX_INLINE cxFloat cxEngineTime()
{
    return engine->TotalFrames * engine->Interval;
}

CX_INLINE cxFloat cxEngineScaleFactor()
{
    return engine->ScaleFactor;
}

CX_INLINE cxLooper cxEngineLooper()
{
    return engine->looper;
}

#define cxAppManifest  cxEngineGetManifest()

CX_INLINE cxJson cxEngineGetManifest()
{
    return engine->Manifest;
}
CX_INLINE cxUInt cxEngineVersion()
{
    return engine->Version;
}
CX_INLINE cxTextureCache cxTextureCacheInstance()
{
    return engine->textures;
}
CX_INLINE cxPlayer cxPlayerInstance()
{
    return engine->player;
}
CX_INLINE cxWindow cxEngineGetWindow()
{
    return engine->Window;
}
CX_INLINE cxOpenGL cxOpenGLInstance()
{
    return engine->opengl;
}
CX_INLINE cxCurve cxCurveInstance()
{
    return engine->curve;
}
CX_INLINE void cxEngineSetInterval(cxFloat interval)
{
    engine->Interval = interval;
}
CX_INLINE cxStr cxEngineGetLocalized()
{
    return engine->Localized;
}
CX_INLINE cxStr cxEngineGetCountry()
{
    return engine->Country;
}
CX_INLINE void cxEngineSetCountry(cxStr country)
{
    CX_RETAIN_SWAP(engine->Country, country);
}
CX_INLINE cxStr cxEngineGetLang()
{
    return engine->Lang;
}
CX_INLINE void cxEngineSetLang(cxStr lng)
{
    CX_RETAIN_SWAP(engine->Lang, lng);
}
CX_INLINE cxFloat cxEngineGetFixDelta()
{
    return engine->FixDelta;
}
CX_INLINE cxFloat cxEngineGetTimeDelta()
{
    return engine->TimeDelta;
}
CX_INLINE void cxEngineSetScale(cxVec2f v)
{
    engine->Scale = v;
}
CX_INLINE cxVec2f cxEngineGetScale()
{
    return engine->Scale;
}
CX_INLINE cxBool cxEngineGetIsDebug()
{
    return engine->IsDebug;
}
CX_INLINE void cxEngineSetIsDebug(cxBool v)
{
#ifdef NDEBUG
    engine->IsDebug = false;
#else
    engine->IsDebug = v;
#endif
}

//use init method
CX_INLINE void cxEngineSetDesSize(cxSize2f size)
{
    engine->DesSize = size;
}
CX_INLINE cxSize2f cxEngineGetDesSize()
{
    return engine->DesSize;
}

CX_INLINE cxSize2f cxEngineGetWinSize()
{
    return engine->WinSize;
}

CX_INLINE cxFloat cxEngineTimeFrame(cxInt frame)
{
    return engine->FixDelta * frame;
}

void cxEngineClear();

//type init this = cxEngine type
CX_EXTERN void cxEngineType(cxType this);

//engine init
CX_EXTERN void cxEngineInit(cxEngine engine);

//engine start draw
CX_EXTERN void cxEngineMain(cxEngine engine);

//engine free
CX_EXTERN void cxEngineFree(cxEngine engine);

json_t *cxEngineLocalizeder(cxConstChars key);

//format string
cxStr cxLocalizedFormat(cxConstChars key,...);

//format key
cxStr cxLocalizedStr(cxConstChars key,...);
//
cxConstChars cxLocalizedConstChars(cxConstChars key,...);

cxConstChars cxLocalizedConstCharsVA(cxConstChars key,va_list ap);

cxJson cxEngineJsonReader(cxConstChars src);

cxAny cxEngineCreateObject(cxConstChars src);

cxStr cxEngineAssetsData(cxConstChars file);

cxJson cxEngineLoadJson(cxConstChars file);

cxBMPFont cxEngineLoadBMPFont(cxConstChars file);

cxAny cxEngineTimer(cxFloat freq,cxInt repeat);

cxVec2f cxEngineTouchToWindow(cxVec2f pos);

cxVec2f cxEngineWindowToTouch(cxVec2f pos);

void cxEngineEnableTouch(cxBool enable);

cxBool cxEngineFireTouch(cxTouchType type,cxInt num,cxTouchPoint *points);

cxBool cxEngineFireKey(cxKeyType type,cxInt code);

void cxEngineSendJson(cxStr txt);

void cxEngineRecvJson(cxStr txt);

//初始化数据
void cxEngineStartup(cxBool layout);

void cxEngineTerminate();

void cxEngineExit();

void cxEnginePause();

void cxEngineResume();

void cxEngineMemory();

void cxEngineSet2DProjection(cxVec3f offset);

void cxEngineSet3DProjection(cxVec3f offset);

void cxEngineDraw(cxFloat dt);

void cxEngineLookAt(cxMatrix4f *matrix,cxFloat zeye,const cxVec3f point);

void cxEngineLayout(cxInt width,cxInt height);

void cxEngineDestroy();

CX_C_END

#endif
