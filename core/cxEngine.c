//
//  cxEngine.c
//  cxEngine
//
//  Created by xuhua on 9/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <kazmath/matrix.h>
#include <streams/cxAssetsStream.h>
#include <socket/cxEventBase.h>
#include "cxType.h"
#include "cxEngine.h"
#include "cxAutoPool.h"
#include "cxOpenGL.h"
#include "cxUtil.h"
#include "cxIconv.h"

static cxEngine instance = NULL;

static cxBool isExit = false;

void cxEngineRecvJson(cxString json)
{
    CX_RETURN(isExit);
    cxEngine engine = cxEngineInstance();
    CX_SIGNAL_FIRE(engine->onRecvJson, CX_FUNC_TYPE(cxAny,cxString), CX_SLOT_OBJECT, json);
    CX_LOGGER("cxengine recv:%s",cxStringBody(json));
}

void cxEngineExit()
{
    CX_RETURN(isExit);
    cxEngine engine = cxEngineInstance();
    CX_EVENT_FIRE(engine, onExit);
    cxEnginePause();
    cxEngineDestroy();
    cxEngineTerminate();
    isExit = true;
}

void cxEnginePause()
{
    CX_RETURN(isExit);
    cxEngine engine = cxEngineInstance();
    engine->isPause = true;
    CX_SIGNAL_FIRE(engine->onPause, CX_FUNC_TYPE(cxAny),CX_SLOT_OBJECT);
    if(engine->isInit){
        cxPauseMusic();
    }
}

void cxEngineBegin()
{
    cxEngine engine = cxEngineInstance();
    cxEngineSetLocalized(cxLocalizedLang());
    cxPlayerOpen();
    cxEngineInit(engine);
}

void cxEngineResume()
{
    CX_RETURN(isExit);
    cxEngine engine = cxEngineInstance();
    engine->isPause = false;
    CX_SIGNAL_FIRE(engine->onResume, CX_FUNC_TYPE(cxAny),CX_SLOT_OBJECT);
    if(engine->isInit){
        cxResumeMusic();
    }
}

void cxEngineMemory()
{
    CX_RETURN(isExit);
    cxEngine engine = cxEngineInstance();
    CX_SIGNAL_FIRE(engine->onMemory, CX_FUNC_TYPE(cxAny),CX_SLOT_OBJECT);
}

void cxEngineDraw()
{
    cxEngine engine = cxEngineInstance();
    CX_RETURN(isExit || !engine->isInit || engine->isPause);
    cxOpenGLClear();
    cxAutoPoolBegin();
    cxDouble now = cxTimestamp();
    engine->frameDelta = now - engine->lastTime;
    CX_SIGNAL_FIRE(engine->onUpdate, CX_FUNC_TYPE(cxAny,cxFloat),CX_SLOT_OBJECT,engine->frameDelta);
    
    kmGLPushMatrix();
    cxViewDraw(engine->window);
    kmGLPopMatrix();
    
    engine->lastTime = now;
    cxAutoPoolClean();
}

static void cxEngineLookAt(cxMatrix4f *matrix,cxFloat zeye,const cxVec2f point)
{
    cxVec3f eye;
    cxVec3f center;
    cxVec3f up;
    kmVec3Fill(&eye, point.x, point.y, zeye);
    kmVec3Fill(&center, point.x, point.y, 0.0f);
    kmVec3Fill(&up, 0.0f, 1.0f, 0.0f);
    kmMat4Identity(matrix);
    kmMat4LookAt(matrix, &eye, &center, &up);
}

void cxEngineSetDesignSize(cxSize2f dessize)
{
    cxEngine engine = cxEngineInstance();
    engine->dessize = dessize;
}

void cxEngineLayout(cxInt width,cxInt height)
{
    CX_LOGGER("openGL layout width=%d height=%d",width,height);
    cxEngine engine = cxEngineInstance();
    engine->winsize = cxSize2fv(width, height);
    cxViewSetSize(engine->window, engine->winsize);
    if(!cxSize2Zero(engine->dessize)){
        engine->scale.x = engine->winsize.w/engine->dessize.w;
        engine->scale.y = engine->winsize.h/engine->dessize.h;
    }
    //
    if(!engine->isInit){
        cxOpenGLCheckFeature();
        cxEngineMain(engine);
    }
    //
    cxFloat zeye = engine->winsize.h / 1.1566f;
    kmMat4 perspective={0};
    kmGLMatrixMode(KM_GL_PROJECTION);
    kmGLLoadIdentity();
    //
    kmMat4PerspectiveProjection(&perspective, 60.0f, engine->winsize.w / engine->winsize.h, 0.0f, zeye * 2);
    kmGLMultMatrix(&perspective);
    kmGLMatrixMode(KM_GL_MODELVIEW);
    kmGLLoadIdentity();
    //
    cxOpenGLViewport(cxBox4fv(0, engine->winsize.w, 0, engine->winsize.h));
    //
    cxMatrix4f matrix;
    cxEngineLookAt(&matrix,zeye,cxVec2fv(0, 0));
    kmGLMultMatrix(&matrix);
    //
    engine->lastTime = cxTimestamp();
    if(!engine->isInit){
        cxViewEnter(engine->window);
    }
    cxViewLayout(engine->window);
    engine->isInit = true;
}

CX_OBJECT_INIT(cxEngine, cxObject)
{
    kmGLInitialize();
    this->interval = 1.0f/60.0f;
    this->isShowBorder = true;
    this->isTouch = true;
    this->scale    = cxVec2fv(1.0f, 1.0f);
    this->window   = CX_ALLOC(cxWindow);
    this->files    = CX_ALLOC(cxHash);
}
CX_OBJECT_FREE(cxEngine, cxObject)
{
    CX_RELEASE(this->files);
    CX_RELEASE(this->lang);
    CX_RELEASE(this->window);
    CX_EVENT_RELEASE(this->onExit);
    CX_SIGNAL_RELEASE(this->onRecvJson);
    CX_SIGNAL_RELEASE(this->onTouch);
    CX_SIGNAL_RELEASE(this->onUpdate);
    CX_SIGNAL_RELEASE(this->onPause);
    CX_SIGNAL_RELEASE(this->onResume);
    CX_SIGNAL_RELEASE(this->onMemory);
    cxEventBaseDestroy();
    cxCurveDestroy();
    cxOpenGLDestroy();
    cxIconvDestroy();
    cxPlayerDestroy();
    cxMessageDestroy();
    kmGLFreeAll();
}
CX_OBJECT_TERM(cxEngine, cxObject)

cxJson cxEngineLoadJson(cxConstChars file)
{
    cxEngine this = cxEngineInstance();
    cxJson json = cxHashGet(this->files, cxHashStrKey(file));
    if(json != NULL){
        CX_ASSERT(CX_INSTANCE_OF(json, cxJson), "%s type error",file);
        return json;
    }
    cxString data = cxAssetsData(file);
    if(data == NULL){
        return NULL;
    }
    json = cxJsonCreate(data);
    if(json == NULL){
        return NULL;
    }
    cxHashSet(this->files, cxHashStrKey(file), json);
    return json;
}

cxBMPFont cxEngineLoadBMPFont(cxConstChars file)
{
    cxEngine this = cxEngineInstance();
    cxBMPFont font = cxHashGet(this->files, cxHashStrKey(file));
    if(font != NULL){
        CX_ASSERT(CX_INSTANCE_OF(font, cxBMPFont), "%s type error",file);
        return font;
    }
    font = cxBMPFontCreate(file);
    if(font == NULL){
        return NULL;
    }
    cxHashSet(this->files, cxHashStrKey(file), font);
    return font;
}

void cxEngineSetLocalized(cxString lang)
{
    cxEngine this = cxEngineInstance();
    CX_RETAIN_SWAP(this->lang, lang);
}

cxEngine cxEngineInstance()
{
    if(instance == NULL) {
        CX_LOGGER("cxEngine Version: %d",CX_ENGINE_VERSION);
        instance = CX_ALLOC(cxEngine);
    }
    return instance;
}

void cxEngineDestroy()
{
    if(instance != NULL) {
        cxEngineFree(instance);
        CX_RELEASE(instance);
        instance = NULL;
    }
}

void cxEngineTimeReset()
{
    cxEngine this = cxEngineInstance();
    this->lastTime = cxTimestamp();
}

cxVec2f cxEngineTouchToWindow(cxVec2f pos)
{
    cxEngine this = cxEngineInstance();
    cxVec2f rv;
    rv.x = pos.x - this->winsize.w / 2.0f;
    rv.y = this->winsize.h / 2.0f - pos.y;
    return rv;
}

cxVec2f cxEngineWindowToTouch(cxVec2f pos)
{
    cxEngine this = cxEngineInstance();
    cxVec2f rv;
    rv.x = this->winsize.w / 2.0f - pos.x;
    rv.y = pos.y - this->winsize.h / 2.0f;
    return rv;
}

void cxEngineEnableTouch(cxBool enable)
{
    cxEngine this = cxEngineInstance();
    this->isTouch = enable;
}

cxBool cxEngineFireKey(cxKeyType type,cxInt code)
{
    cxEngine this = cxEngineInstance();
    this->key.type = type;
    this->key.code = code;
    return cxViewOnKey(this->window, &this->key);
}

cxBool cxEngineFireTouch(cxTouchType type,cxVec2f pos)
{
    cxEngine this = cxEngineInstance();
    if(!this->isTouch){
        return false;
    }
    cxDouble time = cxTimestamp();
    cxVec2f cpos = cxEngineTouchToWindow(pos);
    if(type == cxTouchTypeDown){
        this->touch.movement = cxVec2fv(0, 0);
        this->touch.delta = cxVec2fv(0, 0);
        this->touch.previous = cpos;
        this->touch.dtime = time;
        this->touch.start = cpos;
    }else if(type == cxTouchTypeMove){
        this->touch.delta = cxVec2fv(cpos.x - this->touch.previous.x, cpos.y - this->touch.previous.y);
        this->touch.previous = cpos;
        this->touch.movement.x += this->touch.delta.x;
        this->touch.movement.y += this->touch.delta.y;
    }else{
        this->touch.utime = time;
    }
    this->touch.current = cpos;
    this->touch.type = type;
    cxBool ret = false;
    CX_SIGNAL_FIRE(this->onTouch, CX_FUNC_TYPE(cxAny, cxTouch *, cxBool *),CX_SLOT_OBJECT, &this->touch, &ret);
    return (ret || cxViewTouch(this->window, &this->touch));
}











