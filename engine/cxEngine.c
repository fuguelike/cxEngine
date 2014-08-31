//
//  cxEngine.c
//  cxEngine
//
//  Created by xuhua on 9/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxEngine.h"
#include "cxMath.h"
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
    CX_TYPE_DEF(cxStream);
    CX_TYPE_DEF(cxTexture);
    CX_TYPE_DEF(cxView);
    CX_TYPE_DEF(cxAction);
    CX_TYPE_DEF(cxViewLoader);
    CX_TYPE_DEF(cxEngine);
    CX_TYPE_DEF(cxJson);
    CX_TYPE_DEF(cxPlayer);
    
    //register streams
    CX_TYPE_DEF(cxAssetsStream);
    CX_TYPE_DEF(cxFileStream);
    CX_TYPE_DEF(cxMemStream);
    
    //register textures
    CX_TYPE_DEF(cxTextureJPG);
    CX_TYPE_DEF(cxTexturePKM);
    CX_TYPE_DEF(cxTexturePNG);
    CX_TYPE_DEF(cxTextureTXT);
    CX_TYPE_DEF(cxTexturePVR);
    CX_TYPE_DEF(cxTextureJSON);
    
    //register views
    CX_TYPE_DEF(cxSprite);
    CX_TYPE_DEF(cxScroll);
    CX_TYPE_DEF(cxTable);
    CX_TYPE_DEF(cxWindow);
    CX_TYPE_DEF(cxClipping);
    CX_TYPE_DEF(cxLoading);
    CX_TYPE_DEF(cxPolygon);
    CX_TYPE_DEF(cxAtlas);
    CX_TYPE_DEF(cxButton);
    CX_TYPE_DEF(cxLabelTTF);
    CX_TYPE_DEF(cxLabelBMP);
    CX_TYPE_DEF(cxAlert);
    
    //register actions
    CX_TYPE_DEF(cxMultiple);
    CX_TYPE_DEF(cxAnimate);
    CX_TYPE_DEF(cxFade);
    CX_TYPE_DEF(cxFollow);
    CX_TYPE_DEF(cxJump);
    CX_TYPE_DEF(cxMove);
    CX_TYPE_DEF(cxParabola);
    CX_TYPE_DEF(cxParticle);
    CX_TYPE_DEF(cxRotate);
    CX_TYPE_DEF(cxRunner);
    CX_TYPE_DEF(cxScale);
    CX_TYPE_DEF(cxSpline);
    CX_TYPE_DEF(cxTimer);
    CX_TYPE_DEF(cxTint);
    CX_TYPE_DEF(cxTimeLine);
    CX_TYPE_DEF(cxBezier);
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
    cxMemPoolBegin();
    cxDouble now = cxTimestamp();
    engine->frameDelta = now - engine->lastTime;
    CX_SIGNAL_FIRE(engine->onUpdate, CX_FUNC_TYPE(cxAny,cxFloat),CX_SLOT_OBJECT,engine->frameDelta);
    
    kmGLPushMatrix();
    cxViewDraw(engine->window);
    kmGLPopMatrix();
    
    engine->lastTime = now;
    cxMemPoolClean();
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
    cxJsonRegisterDouble("WinW", width);
    cxJsonRegisterDouble("WinH", height);
    cxViewSetSize(engine->window, engine->winsize);
    if(!cxSize2Zero(engine->dessize)){
        engine->scale.x = engine->winsize.w/engine->dessize.w;
        engine->scale.y = engine->winsize.h/engine->dessize.h;
        cxJsonRegisterDouble("ScaleX", engine->scale.x);
        cxJsonRegisterDouble("ScaleY", engine->scale.y);
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

cxTimer cxEngineTimer(cxFloat freq,cxInt repeat)
{
    return cxViewAppendTimer(instance->window, freq, repeat);
}

void cxEnginePush(cxAny object)
{
    CX_ASSERT(object != NULL && instance != NULL, "args error");
    cxStackPush(instance->stack, object);
}

void cxEnginePop()
{
    CX_ASSERT(instance != NULL, "cxEngine instance null");
    cxStackPop(instance->stack);
}

cxAny cxEngineTop()
{
    CX_ASSERT(instance != NULL, "cxEngine instance null");
    return cxStackTop(instance->stack);
}

CX_OBJECT_TYPE(cxEngine, cxObject)
{}
CX_OBJECT_INIT(cxEngine, cxObject)
{
    kmGLInitialize();
    this->interval = 1.0f/60.0f;
    this->isShowBorder = true;
    this->isTouch = true;
    this->scale    = cxVec2fv(1.0f, 1.0f);
    this->window   = CX_ALLOC(cxWindow);
    this->files    = CX_ALLOC(cxHash);
    this->stack    = CX_ALLOC(cxStack);
}
CX_OBJECT_FREE(cxEngine, cxObject)
{
    CX_RELEASE(this->stack);
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

void cxTypeRunObjectSetter(cxObject object,cxJson json)
{
    CX_ASSERT(object != NULL, "object args error");
    CX_JSON_OBJECT_EACH_BEG(json, item)
    cxObjectSetter(object, key, item);
    CX_JSON_OBJECT_EACH_END(json, item)
}

cxAny cxEngineTypeCreate(cxJson json)
{
    CX_ASSERT(json != NULL, "json error");
    cxConstChars type = cxJsonConstChars(json, "type");
    CX_ASSERT(type != NULL, "type field null");
    cxObject object = cxObjectCreateWithType(type);
    CX_ASSERT(object != NULL, "type(%s) create object failed",type);
    cxTypeRunObjectSetter(object, json);
    return object;
}

//parse type id subview property
cxAny cxObjectLoadWithJson(cxJson json)
{
    CX_ASSERT(json != NULL, "json args error");
    cxConstChars src = NULL;
    cxObject object = NULL;
    if(cxJsonIsString(json)){
        src = cxJsonToConstChars(json);
    }else if(cxJsonIsObject(json)){
        src = cxJsonConstChars(json, "src");
    }
    if(src != NULL){
        object = cxObjectLoadWithFile(src);
        cxTypeRunObjectSetter(object, json);
    }else {
        object = cxEngineTypeCreate(json);
    }
    if(object == NULL){
        CX_ERROR("create object failed");
        return NULL;
    }
    cxConstChars id = cxJsonConstChars(json, "id");
    cxViewLoader curr = cxEngineTop();
    if(id != NULL && curr != NULL){
        cxHashSet(curr->objects, cxHashStrKey(id), object);
    }
    return object;
}

cxAny cxObjectLoadWithFile(cxConstChars file)
{
    cxUrlPath path = cxUrlPathParse(file);
    cxJson json = cxEngineLoadJson(path->path);
    CX_RETURN(json == NULL, NULL);
    //get file.json?key
    if(path->count == 2){
        json = cxJsonObject(json, path->key);
    }
    CX_RETURN(json == NULL, NULL);
    return cxObjectLoadWithJson(json);
}

cxJson cxEngineLoadJson(cxConstChars file)
{
    cxEngine this = cxEngineInstance();
    cxJson json = cxHashGet(this->files, cxHashStrKey(file));
    if(json != NULL){
        CX_ASSERT(CX_INSTANCE_OF(json, cxJson), "%s type error",file);
        return json;
    }
    cxString data = cxAssetsData(file);
    CX_RETURN(data == NULL, NULL);
    json = cxJsonCreate(data);
    CX_RETURN(json == NULL, NULL);
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
        this->touch.direction = cxTouchDirectionNone;
    }else if(type == cxTouchTypeMove){
        kmVec2Subtract(&this->touch.delta, &cpos, &this->touch.previous);
        this->touch.previous = cpos;
        this->touch.movement.x += this->touch.delta.x;
        this->touch.movement.y += this->touch.delta.y;
        this->touch.direction = cxTouchGetDirection(this->touch.delta);
    }else{
        this->touch.utime = time;
    }
    this->touch.current = cpos;
    this->touch.type = type;
    return cxViewTouch(this->window, &this->touch);
}











