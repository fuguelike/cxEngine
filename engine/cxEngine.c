//
//  cxEngine.c
//  cxEngine
//
//  Created by xuhua on 9/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxEngine.h"
#include "cxValue.h"

#include <textures/cxTextureFactory.h>
#include <textures/cxTextureJPG.h>
#include <textures/cxTexturePKM.h>
#include <textures/cxTexturePNG.h>
#include <textures/cxTexturePVR.h>
#include <textures/cxTextureTXT.h>
#include <textures/cxTextureJSON.h>

#include <shaders/cxShaderAlpha.h>
#include <shaders/cxShaderDefault.h>
#include <shaders/cxShaderColor.h>
#include <shaders/cxShaderClipping.h>
#include <shaders/cxShaderMultiple.h>
#include <shaders/cxShaderTTF.h>

#include <streams/cxAssetsStream.h>
#include <streams/cxFileStream.h>
#include <streams/cxMemStream.h>
#include <streams/cxMMapStream.h>

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
#include <views/cxSpine.h>
#include <views/cxParticle.h>

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
#include <actions/cxSkeleton.h>

#include <socket/cxUDP.h>
#include <socket/cxTCP.h>
#include <socket/cxHttp.h>
#include <socket/cxHttpConn.h>
#include <socket/cxLooper.h>

#include <algorithm/cxAStar.h>

cxEngine engineInstance = NULL;

void cxEngineRecvJson(cxString json)
{
    CX_RETURN(engineInstance == NULL);
    cxEngine engine = cxEngineInstance();
    CX_SIGNAL_FIRE(engine->onRecvJson, CX_SIGNAL_TYPE(cxString),json);
    CX_LOGGER("cxengine recv:%s",cxStringBody(json));
}

void cxEngineExit()
{
    CX_RETURN(engineInstance == NULL);
    cxEnginePause();
    cxEngineDestroy();
    cxEngineTerminate();
}

void cxEnginePause()
{
    CX_RETURN(engineInstance == NULL);
    cxEngine engine = cxEngineInstance();
    engine->isPause = true;
    CX_SIGNAL_FIRE(engine->onPause, CX_SIGNAL_TYPE());
    if(engine->isInit){
        cxPauseMusic();
    }
}

static void cxEngineTypes()
{
    //register core
    CX_TYPE_REG(cxTexture);
    CX_TYPE_REG(cxView);
    CX_TYPE_REG(cxAction);
    CX_TYPE_REG(cxEngine);
    CX_TYPE_REG(cxJson);
    CX_TYPE_REG(cxPlayer);
    CX_TYPE_REG(cxTouchItem);
    CX_TYPE_REG(cxAStar);
    CX_TYPE_REG(cxGroup);
    CX_TYPE_REG(cxShader);
    CX_TYPE_REG(cxValue);
    
    //register shader
    CX_TYPE_REG(cxShaderAlpha);
    CX_TYPE_REG(cxShaderClipping);
    CX_TYPE_REG(cxShaderDefault);
    CX_TYPE_REG(cxShaderColor);
    CX_TYPE_REG(cxShaderMultiple);
    CX_TYPE_REG(cxShaderTTF);
    
    //register streams
    CX_TYPE_REG(cxAssetsStream);
    CX_TYPE_REG(cxFileStream);
    CX_TYPE_REG(cxMemStream);
    CX_TYPE_REG(cxMMapStream);
    
    //register textures
    CX_TYPE_REG(cxTextureJPG);
    CX_TYPE_REG(cxTexturePKM);
    CX_TYPE_REG(cxTexturePNG);
    CX_TYPE_REG(cxTextureTXT);
    CX_TYPE_REG(cxTexturePVR);
    CX_TYPE_REG(cxTextureJSON);
    
    //register views
    CX_TYPE_REG(cxWindow);
    CX_TYPE_REG(cxSprite);
    CX_TYPE_REG(cxScroll);
    CX_TYPE_REG(cxTable);
    CX_TYPE_REG(cxWindow);
    CX_TYPE_REG(cxClipping);
    CX_TYPE_REG(cxLoading);
    CX_TYPE_REG(cxAsync);
    CX_TYPE_REG(cxPolygon);
    CX_TYPE_REG(cxAtlas);
    CX_TYPE_REG(cxButton);
    CX_TYPE_REG(cxLabelTTF);
    CX_TYPE_REG(cxLabelBMP);
    CX_TYPE_REG(cxAlert);
    CX_TYPE_REG(cxSpine);
    
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
    CX_TYPE_REG(cxSkeleton);
    //scoket
    CX_TYPE_REG(cxUDP);
    CX_TYPE_REG(cxTCP);
    CX_TYPE_REG(cxLooper);
    CX_TYPE_REG(cxHttpConn);
    CX_TYPE_REG(cxHttp);
}

void cxEngineStartup()
{
#if !defined(NDEBUG)
    CX_LOGGER("cxEngine Version: %d,Run Mode:DEBUG",CX_ENGINE_VERSION);
#else
    CX_LOGGER("cxEngine Version: %d,Run Mode:RELEASE",CX_ENGINE_VERSION);
#endif
    CX_ASSERT(engineInstance == NULL, "repeat init engine");
    cxGlobalInit();
    engineInstance = CX_ALLOC(cxEngine);
    //registe all type
    cxEngineTypes();
    //set localized lang
    cxEngineSetLang(cxLocalizedLang());
    //init engine
    //set cxjsonReader
    cxJsonSetReader(cxEngineJsonReader);
    //init model
    engineInstance->Window      = CX_ALLOC(cxWindow);
    engineInstance->curve       = CX_ALLOC(cxCurve);
    engineInstance->opengl      = CX_ALLOC(cxOpenGL);
    engineInstance->iconv       = CX_ALLOC(cxIconv);
    engineInstance->player      = CX_ALLOC(cxPlayer);
    engineInstance->textures    = CX_ALLOC(cxTextureFactory);
    engineInstance->looper      = CX_ALLOC(cxLooper);
    //registe other type
    cxEngineType(engineInstance);
    //use init engine
    cxEngineInit(engineInstance);
}

void cxEngineClear()
{
    CX_RETURN(engineInstance == NULL);
    cxEngine engine = cxEngineInstance();
    cxTextureFactoryClear();
    cxHashClear(engine->files);
    cxHashClear(engine->assets);
}

void cxEngineResume()
{
    CX_RETURN(engineInstance == NULL);
    cxEngine engine = cxEngineInstance();
    engine->isPause = false;
    CX_SIGNAL_FIRE(engine->onResume, CX_SIGNAL_TYPE());
    if(engine->isInit){
        cxResumeMusic();
    }
}

void cxEngineMemory()
{
    CX_RETURN(engineInstance == NULL);
    cxEngine engine = cxEngineInstance();
    CX_SIGNAL_FIRE(engine->onMemory, CX_SIGNAL_TYPE());
}

void cxEngineDraw(cxFloat dt)
{
    CX_RETURN(engineInstance == NULL);
    cxEngine engine = cxEngineInstance();
    if(!engine->isInit || engine->isPause){
        return;
    }
    cxMemPoolBegin();
    cxOpenGLClear();
    cxLooperUpdate(engine->looper);
    CX_SIGNAL_FIRE(engine->onUpdate, CX_SIGNAL_TYPE(cxFloat),engine->FrameDelta);
    kmGLPushMatrix();
    cxViewDraw(engine->Window);
    kmGLPopMatrix();
    cxMemPoolClear();
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

void cxEngineLayout(cxInt width,cxInt height)
{
    CX_LOGGER("openGL layout width=%d height=%d",width,height);
    cxEngine engine = cxEngineInstance();
    engine->WinSize = cxSize2fv(width, height);
    //$WinSize.w $WinSize.h
    cxJsonRegisterSize2f("WinSize", engine->WinSize);
    cxJsonRegisterSize2f("DesSize", engine->DesSize);
    cxViewSetSize(engine->Window, engine->WinSize);
    //
    if(!cxSize2fZero(engine->DesSize)){
        engine->Scale.x = engine->WinSize.w/engine->DesSize.w;
        engine->Scale.y = engine->WinSize.h/engine->DesSize.h;
        cxJsonRegisterVec2f("Scale", engine->Scale);
    }else{
        engine->Scale.x = 1.0f;
        engine->Scale.y = 1.0f;
        cxJsonRegisterVec2f("Scale", engine->Scale);
    }
    //
    if(!engine->isInit){
        cxOpenGLCheckFeature();
        cxEngineMain(engine);
        cxDumpGlobalJson();
    }
    //
    cxFloat zeye = engine->WinSize.h / 1.1566f;
    kmMat4 perspective={0};
    kmGLMatrixMode(KM_GL_PROJECTION);
    kmGLLoadIdentity();
    //
    kmMat4PerspectiveProjection(&perspective, 60.0f, engine->WinSize.w / engine->WinSize.h, 0.0f, zeye * 2);
    kmGLMultMatrix(&perspective);
    kmGLMatrixMode(KM_GL_MODELVIEW);
    kmGLLoadIdentity();
    //
    cxOpenGLSetDepthTest(false);
    cxOpenGLViewport(cxBox4fv(0, engine->WinSize.w, 0, engine->WinSize.h));
    //
    cxMatrix4f matrix;
    cxEngineLookAt(&matrix,zeye,cxVec2fv(0, 0));
    kmGLMultMatrix(&matrix);
    //
    if(!engine->isInit){
        cxViewEnter(engine->Window);
    }
    cxViewLayout(engine->Window);
    engine->isInit = true;
}

cxTimer cxEngineTimer(cxFloat freq,cxInt repeat)
{
    cxEngine engine = cxEngineInstance();
    return cxViewAppendTimer(engine->Window, freq, repeat);
}

CX_TYPE(cxEngine, cxObject)
{
    
}
CX_INIT(cxEngine, cxObject)
{
    this->Interval = 1.0f/60.0f;
    this->FrameDelta = 1.0f/60.0f;
    this->IsShowBorder = false;
    this->isTouch = true;
    this->Scale = cxVec2fv(1.0f, 1.0f);
    this->files = CX_ALLOC(cxHash);
    this->items = CX_ALLOC(cxHash);
    this->groups = CX_ALLOC(cxHash);
    this->assets = CX_ALLOC(cxHash);
}
CX_FREE(cxEngine, cxObject)
{
    CX_RELEASE(this->groups);
    CX_RELEASE(this->items);
    CX_RELEASE(this->files);
    CX_RELEASE(this->assets);
    CX_RELEASE(this->Lang);
    CX_RELEASE(this->Window);
    CX_SIGNAL_RELEASE(this->onRecvJson);
    CX_SIGNAL_RELEASE(this->onUpdate);
    CX_SIGNAL_RELEASE(this->onPause);
    CX_SIGNAL_RELEASE(this->onResume);
    CX_SIGNAL_RELEASE(this->onMemory);
    CX_RELEASE(this->curve);
    CX_RELEASE(this->opengl);
    CX_RELEASE(this->iconv);
    CX_RELEASE(this->player);
    CX_RELEASE(this->textures);
    CX_RELEASE(this->looper);
    kmGLFreeAll();
}
CX_TERM(cxEngine, cxObject)

//a.json?key
cxJson cxEngineJsonReader(cxConstChars src)
{
    cxPath path = cxPathParse(src);
    cxJson json = cxEngineLoadJson(path->path);
    CX_RETURN(json == NULL, NULL);
    //get file.json?key
    if(path->count == 2){
        json = cxJsonObject(json, path->key);
    }
    return json;
}

cxAny cxEngineCreateObject(cxConstChars src)
{
    cxJson json = cxEngineJsonReader(src);
    CX_ASSERT(json != NULL, "read json error");
    return cxObjectCreateUseJson(json);
}

cxString cxEngineAssetsData(cxConstChars file)
{
    cxHashKey key = cxHashStrKey(file);
    cxEngine this = cxEngineInstance();
    cxString data = cxHashGet(this->assets, key);
    if(data == NULL){
        data = cxAssetsData(file);
        CX_ASSERT(data != NULL, "load assets data %s failed",file);
        cxHashSet(this->assets, key, data);
    }
    return data;
}

cxJson cxEngineLoadJson(cxConstChars file)
{
    cxHashKey key = cxHashStrKey(file);
    cxEngine this = cxEngineInstance();
    cxJson json = cxHashGet(this->files, key);
    if(json == NULL){
        cxString data = cxAssetsData(file);
        CX_ASSERT(data != NULL, "load json file %s failed");
        json = cxJsonCreate(data);
        cxHashSet(this->files, key, json);
    }
    return json;
}

cxBMPFont cxEngineLoadBMPFont(cxConstChars file)
{
    cxEngine this = cxEngineInstance();
    cxBMPFont font = cxHashGet(this->files, cxHashStrKey(file));
    if(font == NULL){
        font = cxBMPFontCreate(file);
        CX_ASSERT(font != NULL, "create bmp %s font failed",file);
        cxHashSet(this->files, cxHashStrKey(file), font);
    }
    CX_ASSERT_TYPE(font, cxBMPFont);
    return font;
}

void cxEngineDestroy()
{
    cxEngineFree(engineInstance);
    CX_RELEASE(engineInstance);
    engineInstance = NULL;
    cxGlobalFree();
}

cxVec2f cxEngineTouchToWindow(cxVec2f pos)
{
    cxEngine this = cxEngineInstance();
    cxVec2f rv;
    rv.x = pos.x - this->WinSize.w / 2.0f;
    rv.y = this->WinSize.h / 2.0f - pos.y;
    return rv;
}

cxVec2f cxEngineWindowToTouch(cxVec2f pos)
{
    cxEngine this = cxEngineInstance();
    cxVec2f rv;
    rv.x = this->WinSize.w / 2.0f - pos.x;
    rv.y = pos.y - this->WinSize.h / 2.0f;
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
    return cxViewKey(this->Window, &this->key);
}

static void cxEngineComputeItem(cxDouble now,cxTouchItem item,cxVec2f cpos)
{
    item->delta = cxVec2fSub(cpos, item->previous);
    //get move speed
    cxFloat dt = now - item->startTime;
    if(cxFloatEqu(dt, 0)){
        item->speed = cxVec2fv(0, 0);
    }else{
        item->speed.x = (cpos.x - item->startpos.x) / dt;
        item->speed.y = (cpos.y - item->startpos.y) / dt;
    }
    //get movement
    item->movement += cxVec2fLength(item->delta);
    item->isTap = dt < TAP_TIME && item->movement < TAP_MOVEMENT;
    item->previous = cpos;
}

static void cxEngineInitItem(cxDouble now,cxTouchItem item,cxVec2f cpos)
{
    item->delta = cxVec2fv(0, 0);
    item->previous = cpos;
    item->startTime = now;
    item->speed = cxVec2fv(0, 0);
    item->movement = 0;
    item->startpos = cpos;
}

cxBool cxEngineFireTouch(cxTouchType type,cxInt num,cxTouchPoint *points)
{
    cxEngine this = cxEngineInstance();
    //current fires point
    cxDouble now = cxTimestamp();
    cxTouchItems delItems = {0};
    cxTouchItem item = NULL;
    //save point
    for(cxInt i=0; i < num; i++){
        cxTouchPoint p = points[i];
        cxVec2f cpos = cxEngineTouchToWindow(p.xy);
        cxHashKey key = cxHashLongKey(p.id);
        item = NULL;
        if(type == cxTouchTypeDown){
            item  = CX_ALLOC(cxTouchItem);
            cxHashSet(this->items, key, item);
            CX_RELEASE(item);
            cxEngineInitItem(now, item, cpos);
            goto setTouchAttr;
        }
        if(type == cxTouchTypeMove){
            item = cxHashGet(this->items, key);
            CX_ASSERT(item != NULL, "item error");
            cxEngineComputeItem(now, item, cpos);
            goto setTouchAttr;
        }
        if(type == cxTouchTypeUp){
            item = cxHashGet(this->items, key);
            CX_ASSERT(item != NULL, "item error");
            cxEngineComputeItem(now, item, cpos);
            cxTouchItemsAppend(delItems, item);
            goto setTouchAttr;
        }
        if(type == cxTouchTypeCancel){
            item = cxHashGet(this->items, key);
            CX_ASSERT(item != NULL, "item error");
            cxEngineComputeItem(now, item, cpos);
            cxTouchItemsAppend(delItems, item);
            goto setTouchAttr;
        }
        setTouchAttr:
        if(item == NULL){
            continue;
        }
        item->position = cpos;
        item->type = type;
        item->key = p.id;
    }
    //get all points
    this->points.number = 0;
    CX_HASH_FOREACH(this->items, ele, tmp){
        cxTouchItemsAppend(this->points, ele->any);
    }
    cxBool ret = cxViewTouch(this->Window, &this->points);
    //remove up cancel point
    for(cxInt i=0; i < delItems.number; i++){
        item = delItems.items[i];
        cxHashDel(this->items, cxHashLongKey(item->key));
    }
    return ret;
}











