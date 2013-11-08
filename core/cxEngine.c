//
//  cxEngine.c
//  cxEngine
//
//  Created by xuhua on 9/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <kazmath/matrix.h>
#include <streams/cxAssetsStream.h>
#include <core/cxActionXML.h>
#include <socket/cxEventBase.h>
#include "cxViewXML.h"
#include "cxEngine.h"
#include "cxAutoPool.h"
#include "cxOpenGL.h"
#include "cxUtil.h"
#include "cxHashXML.h"
#include "cxDB.h"

static cxEngine instance = NULL;

void cxEngineExit()
{
    cxEngine engine = cxEngineInstance();
    CX_EVENT_FIRE(engine, onFree);
    cxEnginePause();
    cxEngineDestroy();
    cxAllocatorFree();
    exit(0);
}

void cxEnginePause()
{
    cxEngine engine = cxEngineInstance();
    CX_SIGNAL_FIRE(engine->onPause, CX_FUNC_TYPE(cxAny),CX_SLOT_OBJECT);
}

void cxEngineResume()
{
    cxEngine engine = cxEngineInstance();
    CX_SIGNAL_FIRE(engine->onResume, CX_FUNC_TYPE(cxAny),CX_SLOT_OBJECT);
}

void cxEngineMemory()
{
    cxEngine engine = cxEngineInstance();
    CX_SIGNAL_FIRE(engine->onMemory, CX_FUNC_TYPE(cxAny),CX_SLOT_OBJECT);
}

void cxEngineDraw()
{
    cxEngine engine = cxEngineInstance();
    CX_RETURN(!engine->isInit);
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

void cxEngineLookAt(cxMatrix4f *matrix,const cxVec2f point)
{
    cxEngine engine = cxEngineInstance();
    cxFloat zeye = engine->winsize.h / 1.1566f;
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
    engine->winsize = cxSize2fv(width, height);
    cxViewSetSize(engine->window, engine->winsize);
    if(!cxSize2Zero(engine->dessize)){
        engine->scale = cxVec2fv(engine->winsize.w/engine->dessize.w, engine->winsize.h/engine->dessize.h);
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
    //init gl
    cxOpenGLViewport(cxBox4fv(0, engine->winsize.w, 0, engine->winsize.h));
    //
    cxMatrix4f matrix;
    cxEngineLookAt(&matrix,cxVec2fv(0, 0));
    kmGLMultMatrix(&matrix);
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
    this->autopool = CX_ALLOC(cxStack);
    this->frameInterval = 1.0f/60.0f;
    this->isShowBorder = true;
    this->contentScaleFactor = 1.0f;
    this->scale = cxVec2fv(1.0f, 1.0f);
    this->window = CX_ALLOC(cxWindow);
    this->scripts = CX_ALLOC(cxHash);
    this->events = CX_ALLOC(cxHash);
    this->datasets = CX_ALLOC(cxHash);
    this->actions = CX_ALLOC(cxHash);
}
CX_OBJECT_FREE(cxEngine, cxObject)
{
    CX_RELEASE(this->actions);
    CX_RELEASE(this->lang);
    CX_RELEASE(this->datasets);
    CX_RELEASE(this->events);
    CX_RELEASE(this->scripts);
    CX_SIGNAL_RELEASE(this->onTouch);
    CX_EVENT_RELEASE(this->onFree);
    CX_SIGNAL_RELEASE(this->onUpdate);
    CX_SIGNAL_RELEASE(this->onPause);
    CX_SIGNAL_RELEASE(this->onResume);
    CX_SIGNAL_RELEASE(this->onMemory);
    CX_RELEASE(this->window);
    cxEventBaseDestroy();
    cxCurveDestroy();
    cxOpenGLDestroy();
    cxMessageDestroy();
    kmGLFreeAll();
    cxDBEnvDestroy();
    CX_RELEASE(this->autopool);
}
CX_OBJECT_TERM(cxEngine, cxObject)

cxStack cxEngineAutoStack()
{
    return cxEngineInstance()->autopool;
}

void cxEngineSetLocalLang(cxString lang)
{
    cxEngine this = cxEngineInstance();
    CX_RETAIN_SWAP(this->lang, lang);
}

cxEngine cxEngineInstance()
{
    if(instance == NULL) {
        instance = CX_ALLOC(cxEngine);
        cxAllocatorInit();
        cxEngineSystemInit();
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

void cxEngineRegisteEvent(cxConstChars name,cxEventFunc func)
{
    cxEngine this = cxEngineInstance();
    if(func == NULL){
        cxHashDel(this->events, cxHashStrKey(name));
    }else{
        cxEventItem event = cxHashGet(this->events, cxHashStrKey(name));
        CX_ASSERT(event == NULL, "name %s event registered",name);
        event = CX_ALLOC(cxEventItem);
        event->func = func;
        cxHashSet(this->events, cxHashStrKey(name), event);
        CX_RELEASE(event);
    }
}

cxEventItem cxEngineGetEvent(cxConstChars name)
{
    cxEngine this = cxEngineInstance();
    return cxHashGet(this->events, cxHashStrKey(name));
}

void cxEngineSystemInit()
{
    //set locate lang
    cxEngineSetLocalLang(cxStringConstChars("en"));//cxLocaleLang());
    //register global event
    cxEngineRegisteEvent("cxActionRun", cxViewRunActionEvent);
    cxEngineRegisteEvent("cxPlay", cxPlaySoundEvent);
    cxEngineRegisteEvent("cxLogger", cxPrintMessageEvent);
    cxEngineRegisteEvent("cxPushView", cxViewPushViewEvent);
    cxEngineRegisteEvent("cxPopView", cxViewPopViewEvent);
    cxEngineRegisteEvent("cxReplaceView", cxViewReplaceViewEvent);
    cxEngineRegisteEvent("cxPost", cxMessagePostEvent);
}

cxXMLScript cxEngineGetXMLScript(cxConstChars file)
{
    cxEngine this = cxEngineInstance();
    CX_RETURN(file == NULL,NULL);
    cxXMLScript script = cxHashGet(this->scripts, cxHashStrKey(file));
    if(script != NULL){
        return script;
    }
    script = CX_ALLOC(cxXMLScript);
    cxStream stream = cxAssetsStreamCreate(file);
    if(stream != NULL){
        cxString data = stream->interface->AllBytes(stream);
        CX_RETAIN_SWAP(script->bytes, data);
    }
    if(script->bytes != NULL){
        cxHashSet(this->scripts, cxHashStrKey(file), script);
    }else{
        CX_ERROR("file %s can't load bytes",file);
    }
    CX_RELEASE(script);
    return script;
}

void cxEngineTimeReset()
{
    cxEngine this = cxEngineInstance();
    this->lastTime = cxTimestamp();
}

void cxEngineRemoveScript(cxConstChars file)
{
    cxEngine this = cxEngineInstance();
    CX_RETURN(file == NULL);
    cxHashDel(this->scripts, cxHashStrKey(file));
}

cxTypes cxEngineDataSet(cxConstChars url)
{
    CX_RETURN(url == NULL, NULL);
    cxEngine this = cxEngineInstance();
    cxChar path[128]={0};
    cxChar key[128]={0};
    cxInt ret = cxParseURL(url, path, key);
    CX_RETURN(ret == 0, NULL);
    cxHashXML sets = cxHashGet(this->datasets, cxHashStrKey(path));
    if(sets == NULL){
        sets = CX_CREATE(cxHashXML);
        if(!cxHashXMLLoad(sets, path)){
            return NULL;
        }
        cxHashSet(this->datasets, cxHashStrKey(path), sets);
    }
    if(ret == 2){
        return cxHashGet(sets->items, cxHashStrKey(key));
    }
    return NULL;
}

cxString cxEngineTypesText(cxConstChars xml,cxConstChars key)
{
    cxEngine this = cxEngineInstance();
    cxConstChars url = NULL;
    //from cxLangString
    CX_ASSERT(this->lang != NULL, "system not set locate lang");
    
    url = CX_CONST_STRING("%s?%s",xml,cxStringBody(this->lang));
    cxString ret = NULL;
    cxTypes types = cxEngineDataSet(url);
    if(cxTypesIsType(types,cxTypesLangString)){
        ret = cxTypesGet(types, key);
    }
    CX_RETURN(ret != NULL,ret);
    
    //from cxString
    url = CX_CONST_STRING("%s?%s",xml,key);
    types = cxEngineDataSet(url);
    if(cxTypesIsType(types,cxTypesString)){
        ret = types->assist;
    }
    CX_RETURN(ret != NULL,ret);
    
    //parse query?key=?
    cxChar qk[128]={0};
    cxChar qv[128]={0};
    if(cxParseQuery(key, qk, qv) != 2){
        return ret;
    }
    url = CX_CONST_STRING("%s?%s",xml,qk);
    //from cxHash
    types = cxEngineDataSet(url);
    if(cxTypesIsType(types,cxTypesHash)){
        cxAny value = cxTypesGet(types, qv);
        ret = cxObjectIsType(value, cxStringAutoType) ? value : NULL;
    }
    CX_RETURN(ret != NULL,ret);
    
    //from cxDB get String
    if(cxTypesIsType(types,cxTypesDB)){
        ret = cxDBGet(types->assist, cxStringStatic(qv));
    }
    return ret;
}

cxVec2f cxEngineTouchToWindow(cxVec2f pos)
{
    cxEngine this = cxEngineInstance();
    cxVec2f rv;
    rv.x = pos.x - this->winsize.w/2.0f;
    rv.y = this->winsize.h/2.0f - pos.y;
    return rv;
}

cxVec2f cxEngineWindowToTouch(cxVec2f pos)
{
    cxEngine this = cxEngineInstance();
    cxVec2f rv;
    rv.x = this->winsize.w/2.0f - pos.x;
    rv.y = pos.y - this->winsize.h/2.0f;
    return rv;
}

cxBool cxEngineFireTouch(cxTouchType type,cxVec2f pos)
{
    cxEngine this = cxEngineInstance();
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
    CX_SIGNAL_FIRE(this->onTouch, CX_FUNC_TYPE(cxAny, cxTouch *),CX_SLOT_OBJECT,&this->touch);
    return cxViewTouch(this->window, &this->touch);
}











