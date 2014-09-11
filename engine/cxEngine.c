//
//  cxEngine.c
//  cxEngine
//
//  Created by xuhua on 9/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxEngine.h"
#include "cxIconv.h"

#include <textures/cxTextureFactory.h>
#include <textures/cxTextureJPG.h>
#include <textures/cxTexturePKM.h>
#include <textures/cxTexturePNG.h>
#include <textures/cxTexturePVR.h>
#include <textures/cxTextureTXT.h>
#include <textures/cxTextureJSON.h>

#include <shaders/cxShaderAlpha.h>
#include <shaders/cxShaderDefault.h>
#include <shaders/cxShaderPositionColor.h>
#include <shaders/cxShaderClipping.h>

#include <streams/cxAssetsStream.h>
#include <streams/cxFileStream.h>
#include <streams/cxMemStream.h>

#include <views/cxButton.h>
#include <views/cxLoading.h>
#include <views/cxLabelTTF.h>
#include <views/cxWindow.h>
#include <views/cxScroll.h>
#include <views/cxTable.h>
#include <views/cxClipping.h>
#include <views/cxLabelBMP.h>
#include <views/cxPolygon.h>
#include <views/cxAlert.h>
#include <views/cxLayer.h>

#include <actions/cxParticle.h>
#include <actions/cxMultiple.h>
#include <actions/cxMove.h>
#include <actions/cxFade.h>
#include <actions/cxJump.h>
#include <actions/cxRotate.h>
#include <actions/cxTimer.h>
#include <actions/cxTint.h>
#include <actions/cxFollow.h>
#include <actions/cxAnimate.h>
#include <actions/cxParabola.h>
#include <actions/cxRunner.h>
#include <actions/cxScale.h>
#include <actions/cxSpline.h>
#include <actions/cxTimeLine.h>
#include <actions/cxBezier.h>
#include <actions/cxSpeed.h>

#include <algorithm/cxAStar.h>

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

static void cxEngineTypes()
{
    //register core
    CX_TYPE_REG(cxStream);
    CX_TYPE_REG(cxTexture);
    CX_TYPE_REG(cxView);
    CX_TYPE_REG(cxAction);
    CX_TYPE_REG(cxEngine);
    CX_TYPE_REG(cxJson);
    CX_TYPE_REG(cxPlayer);
    CX_TYPE_REG(cxTouchItem);
    CX_TYPE_REG(cxAStar);
    CX_TYPE_REG(cxActionMgr);
    CX_TYPE_REG(cxShader);
    
    //register shader
    CX_TYPE_REG(cxShaderAlpha);
    CX_TYPE_REG(cxShaderClipping);
    CX_TYPE_REG(cxShaderDefault);
    CX_TYPE_REG(cxShaderPositionColor);
    
    //register streams
    CX_TYPE_REG(cxAssetsStream);
    CX_TYPE_REG(cxFileStream);
    CX_TYPE_REG(cxMemStream);
    
    //register textures
    CX_TYPE_REG(cxTextureJPG);
    CX_TYPE_REG(cxTexturePKM);
    CX_TYPE_REG(cxTexturePNG);
    CX_TYPE_REG(cxTextureTXT);
    CX_TYPE_REG(cxTexturePVR);
    CX_TYPE_REG(cxTextureJSON);
    
    //register views
    CX_TYPE_REG(cxSprite);
    CX_TYPE_REG(cxScroll);
    CX_TYPE_REG(cxTable);
    CX_TYPE_REG(cxWindow);
    CX_TYPE_REG(cxClipping);
    CX_TYPE_REG(cxLoading);
    CX_TYPE_REG(cxPolygon);
    CX_TYPE_REG(cxAtlas);
    CX_TYPE_REG(cxButton);
    CX_TYPE_REG(cxLabelTTF);
    CX_TYPE_REG(cxLabelBMP);
    CX_TYPE_REG(cxAlert);
    CX_TYPE_REG(cxLayer);
    CX_TYPE_REG(cxElement);
    
    //register actions
    CX_TYPE_REG(cxMultiple);
    CX_TYPE_REG(cxAnimateItem);
    CX_TYPE_REG(cxAnimate);
    CX_TYPE_REG(cxFade);
    CX_TYPE_REG(cxFollow);
    CX_TYPE_REG(cxJump);
    CX_TYPE_REG(cxMove);
    CX_TYPE_REG(cxParabola);
    CX_TYPE_REG(cxParticle);
    CX_TYPE_REG(cxRotate);
    CX_TYPE_REG(cxRunner);
    CX_TYPE_REG(cxScale);
    CX_TYPE_REG(cxSpline);
    CX_TYPE_REG(cxTimer);
    CX_TYPE_REG(cxTint);
    CX_TYPE_REG(cxTimeLine);
    CX_TYPE_REG(cxBezier);
    CX_TYPE_REG(cxSpeed);
}

void cxEngineBegin()
{
    cxEngine engine = cxEngineInstance();
    //registe all type
    cxEngineTypes();
    //registe other type
    cxEngineType(engine);
    //set localized lang
    cxEngineSetLocalized(cxLocalizedLang());
    //open player
    cxPlayerOpen();
    //init engine
    //set cxjsonReader
    cxJsonSetReader(cxEngineJsonReader);
    //use init engine
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
    CX_RETURN(isExit);
    cxEngine engine = cxEngineInstance();
    cxDouble now = cxTimestamp();
    engine->frameDelta = now - engine->lastTime;
    engine->frameDelta = CX_MAX(0, engine->frameDelta);
    if(!engine->isInit){
        goto completed;
    }
    if(engine->isPause){
        goto completed;
    }
    cxMemPoolBegin();
    cxOpenGLClear();
    CX_SIGNAL_FIRE(engine->onUpdate, CX_FUNC_TYPE(cxAny,cxFloat),CX_SLOT_OBJECT,engine->frameDelta);
    kmGLPushMatrix();
    cxViewDraw(engine->window);
    kmGLPopMatrix();
    cxMemPoolClean();
completed:
    engine->lastTime = now;
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
    //$WinSize.w $WinSize.h
    cxJsonRegisterSize2f("WinSize", engine->winsize);
    cxViewSetSize(engine->window, engine->winsize);
    //
    if(!cxSize2fZero(engine->dessize)){
        engine->scale.x = engine->winsize.w/engine->dessize.w;
        engine->scale.y = engine->winsize.h/engine->dessize.h;
        cxJsonRegisterVec2f("Scale", engine->scale);
    }else{
        engine->scale.x = 1.0f;
        engine->scale.y = 1.0f;
        cxJsonRegisterVec2f("Scale", engine->scale);
    }
    //
    if(!engine->isInit){
        cxOpenGLCheckFeature();
        cxEngineMain(engine);
        cxDumpGlobalJson();
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
    cxOpenGLSetDepthTest(false);
    cxOpenGLViewport(cxBox4fv(0, engine->winsize.w, 0, engine->winsize.h));
    //
    cxMatrix4f matrix;
    cxEngineLookAt(&matrix,zeye,cxVec2fv(0, 0));
    kmGLMultMatrix(&matrix);
    //
    if(!engine->isInit){
        cxViewEnter(engine->window);
    }
    cxViewLayout(engine->window);
    engine->isInit = true;
    engine->lastTime = cxTimestamp();
}

cxTimer cxEngineTimer(cxFloat freq,cxInt repeat)
{
    return cxViewAppendTimer(instance->window, freq, repeat);
}

CX_OBJECT_TYPE(cxEngine, cxObject)
{}
CX_OBJECT_INIT(cxEngine, cxObject)
{
    this->interval = 1.0f/60.0f;
    this->isShowBorder = true;
    this->isTouch = true;
    this->isGesture = true;
    this->scale    = cxVec2fv(1.0f, 1.0f);
    this->window   = CX_ALLOC(cxWindow);
    this->files    = CX_ALLOC(cxHash);
    this->items    = CX_ALLOC(cxHash);
}
CX_OBJECT_FREE(cxEngine, cxObject)
{
    CX_RELEASE(this->items);
    CX_RELEASE(this->files);
    CX_RELEASE(this->lang);
    CX_RELEASE(this->window);
    CX_EVENT_RELEASE(this->onExit);
    CX_SIGNAL_RELEASE(this->onRecvJson);
    CX_SIGNAL_RELEASE(this->onUpdate);
    CX_SIGNAL_RELEASE(this->onPause);
    CX_SIGNAL_RELEASE(this->onResume);
    CX_SIGNAL_RELEASE(this->onMemory);
    cxCurveDestroy();
    cxOpenGLDestroy();
    cxIconvDestroy();
    cxPlayerDestroy();
    cxMessageDestroy();
    kmGLFreeAll();
}
CX_OBJECT_TERM(cxEngine, cxObject)

//cxTypeRunObjectSetter(object, json);
cxJson cxEngineJsonReader(cxConstChars src)
{
    cxUrlPath path = cxUrlPathParse(src);
    cxJson json = cxEngineLoadJson(path->path);
    CX_RETURN(json == NULL, NULL);
    //get file.json?key
    if(path->count == 2){
        json = cxJsonObject(json, path->key);
    }
    return json;
}

cxJson cxEngineLoadJson(cxConstChars file)
{
    cxEngine this = cxEngineInstance();
    cxJson json = cxHashGet(this->files, cxHashStrKey(file));
    if(json != NULL){
        goto completed;
    }
    cxString data = cxAssetsData(file);
    CX_RETURN(data == NULL, NULL);
    json = cxJsonCreate(data);
    CX_RETURN(json == NULL, NULL);
    cxHashSet(this->files, cxHashStrKey(file), json);
completed:
    CX_ASSERT(CX_INSTANCE_OF(json, cxJson), "%s type error",file);
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
    this->frameDelta = 0;
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
    return cxViewKey(this->window, &this->key);
}

//get touch direction
static cxTouchDirection cxTouchGetDirection(cxVec2f delta)
{
    cxTouchDirection ret = cxTouchDirectionNone;
    if(delta.x < 0){
        ret |= cxTouchDirectionLeft;
    }else if(delta.x > 0){
        ret |= cxTouchDirectionRight;
    }
    if(delta.y < 0){
        ret |= cxTouchDirectionDown;
    }else if(delta.y > 0){
        ret |= cxTouchDirectionUp;
    }
    return ret;
}

static void cxEngineComputeTouchItem(cxDouble now,cxTouchItem item,cxVec2f cpos)
{
    kmVec2Subtract(&item->delta, &cpos, &item->previous);
    item->previous = cpos;
    item->direction = cxTouchGetDirection(item->delta);
    //get move speed
    cxDouble dt = now - item->prevTime;
    if(dt > 0){
        kmVec2Scale(&item->speed, &item->delta, 1.0f / dt);
    }
    item->prevTime = now;
}

static void cxEngineInitTouchItem(cxDouble now,cxTouchItem item,cxVec2f cpos)
{
    item->delta = cxVec2fv(0, 0);
    item->previous = cpos;
    item->direction = cxTouchDirectionNone;
    item->prevTime = now;
    item->speed = cxVec2fv(0, 0);
}

cxBool cxEngineFireTouch(cxTouchType type,cxInt num,cxTouchPoint *points)
{
    cxEngine this = cxEngineInstance();
    //current fires point
    this->points.number = 0;
    cxDouble now = cxTimestamp();
    cxTouchItems delItems = {.number = 0};
    cxTouchItem item = NULL;
    //save point
    for(cxInt i=0; i < num; i++){
        cxTouchPoint p = points[i];
        cxVec2f cpos = cxEngineTouchToWindow(p.xy);
        cxHashKey key = cxHashLongKey(p.id);
        if(type == cxTouchTypeDown){
            item  = CX_ALLOC(cxTouchItem);
            cxHashSet(this->items, key, item);
            CX_RELEASE(item);
            cxEngineInitTouchItem(now, item, cpos);
        }else if(type == cxTouchTypeMove){
            item = cxHashGet(this->items, key);
            CX_CONTINUE(item == NULL);
            cxEngineComputeTouchItem(now, item, cpos);
        }else if(type == cxTouchTypeUp || type == cxTouchTypeCancel){
            item = cxHashGet(this->items, key);
            CX_CONTINUE(item == NULL);
            delItems.items[delItems.number++] = item;
        }
        CX_CONTINUE(item == NULL);
        item->position = cpos;
        item->type = type;
        item->key = p.id;
    }
    //get all points
    CX_HASH_FOREACH(this->items, ele, tmp){
        item = ele->any;
        this->points.items[this->points.number++] = item;
    }
    cxBool ret = cxViewTouch(this->window, &this->points);
    //remove up cancel point
    for(cxInt i=0; i < delItems.number; i++){
        item = delItems.items[i];
        cxHashKey key = cxHashLongKey(item->key);
        cxHashDel(this->items, key);
    }
    return ret;
}











