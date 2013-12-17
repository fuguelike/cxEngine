//
//  cxEngine.c
//  cxEngine
//
//  Created by xuhua on 9/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <kazmath/matrix.h>
#include <streams/cxAssetsStream.h>
#include <core/cxActionRoot.h>
#include <socket/cxEventBase.h>
#include <views/cxParticle.h>
#include <views/cxChipmunk.h>
#include <actions/cxActionSet.h>
#include "cxViewRoot.h"
#include "cxEngine.h"
#include "cxAutoPool.h"
#include "cxOpenGL.h"
#include "cxUtil.h"
#include "cxHashRoot.h"
#include "cxDB.h"

static cxEngine instance = NULL;
static cxBool isExit = false;

void cxEngineRecvJson(cxString json)
{
    CX_RETURN(isExit);
    cxEngine engine = cxEngineInstance();
    CX_SIGNAL_FIRE(engine->onRecvJson, CX_FUNC_TYPE(cxAny,cxString), CX_SLOT_OBJECT, json);
}

void cxEngineExit()
{
    CX_RETURN(isExit);
    cxEngine engine = cxEngineInstance();
    CX_EVENT_FIRE(engine, onFree);
    cxEnginePause();
    cxEngineDestroy();
    cxAllocatorFree();
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
    //set locate lang
    cxEngineSetLocalized(cxLocalizedLang());
    //load appConfig.xml
    cxEngineDataSet("appConfig.xml");
    //init event list and att method
    cxEngineSystemInit();
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
    CX_RETURN(!engine->isInit || engine->isPause);
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

static void cxEngineLookAt(cxMatrix4f *matrix,const cxVec2f point)
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
    this->frameInterval = 1.0f/60.0f;
    this->isShowBorder = true;
    this->isTouch = true;
    this->contentScaleFactor = 1.0f;
    this->scale = cxVec2fv(1.0f, 1.0f);
    
    this->window    = CX_ALLOC(cxWindow);
    this->scripts   = CX_ALLOC(cxHash);
    this->events    = CX_ALLOC(cxHash);
    this->datasets  = CX_ALLOC(cxHash);
    this->actions   = CX_ALLOC(cxHash);
    this->dbenvs    = CX_ALLOC(cxHash);
    this->bmpfonts  = CX_ALLOC(cxHash);
    this->functions = CX_ALLOC(cxHash);
    this->typeFunctions = CX_ALLOC(cxHash);
}
CX_OBJECT_FREE(cxEngine, cxObject)
{
    CX_RELEASE(this->typeFunctions);
    CX_RELEASE(this->functions);
    CX_RELEASE(this->bmpfonts);
    CX_RELEASE(this->actions);
    CX_RELEASE(this->lang);
    CX_RELEASE(this->datasets);
    CX_RELEASE(this->events);
    CX_RELEASE(this->scripts);
    CX_RELEASE(this->dbenvs);
    
    CX_SIGNAL_RELEASE(this->onRecvJson);
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
    cxAutoPoolDestroy();
}
CX_OBJECT_TERM(cxEngine, cxObject)

cxBMPFont cxEngineLoadBMPFont(cxConstChars file)
{
    cxEngine this = cxEngineInstance();
    cxBMPFont font = cxHashGet(this->bmpfonts, cxHashStrKey(file));
    if(font != NULL){
        return font;
    }
    font = cxBMPFontCreate(file);
    if(font == NULL){
        return NULL;
    }
    cxHashSet(this->bmpfonts, cxHashStrKey(file), font);
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
        CX_LOGGER("cxEngine Version:%d",CX_ENGINE_VERSION);
        instance = CX_ALLOC(cxEngine);
        cxAllocatorInit();
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

void cxEngineRegisteTypeFunc(cxConstType type,cxConstChars name,cxAny func)
{
    CX_ASSERT(name != NULL || func == NULL || type == NULL, "args error");
    cxEngine this = cxEngineInstance();
    cxHash typeFuncs = cxHashGet(this->typeFunctions, cxHashStrKey(type));
    if(typeFuncs == NULL){
        typeFuncs = CX_ALLOC(cxHash);
        cxHashSet(this->typeFunctions, cxHashStrKey(type), typeFuncs);
        CX_RELEASE(typeFuncs);
    }
    cxFuncItem item = cxHashGet(typeFuncs, cxHashStrKey(name));
    CX_ASSERT(item == NULL, "type %s.%s function registered",type,name);
    item = CX_ALLOC(cxFuncItem);
    item->func = func;
    cxHashSet(typeFuncs, cxHashStrKey(name), item);
    CX_RELEASE(item);
}

void cxEngineRegisteFunc(cxConstChars name,cxAny func)
{
    cxEngine this = cxEngineInstance();
    CX_ASSERT(name != NULL, "args error");
    if(func == NULL){
        cxHashDel(this->functions, cxHashStrKey(name));
        return;
    }
    cxFuncItem item = cxHashGet(this->functions, cxHashStrKey(name));
    CX_ASSERT(item == NULL, "name %s function registered",name);
    item = CX_ALLOC(cxFuncItem);
    item->func = func;
    cxHashSet(this->functions, cxHashStrKey(name), item);
    CX_RELEASE(item);
}

void cxEngineRegisteEvent(cxConstChars name,cxEventFunc func)
{
    cxEngine this = cxEngineInstance();
    if(func == NULL){
        cxHashDel(this->events, cxHashStrKey(name));
        return;
    }
    cxEventItem event = cxHashGet(this->events, cxHashStrKey(name));
    CX_ASSERT(event == NULL, "name %s event registered",name);
    event = CX_ALLOC(cxEventItem);
    event->func = func;
    cxHashSet(this->events, cxHashStrKey(name), event);
    CX_RELEASE(event);
}

cxEventItem cxEngineGetEvent(cxConstChars name)
{
    cxEngine this = cxEngineInstance();
    return cxHashGet(this->events, cxHashStrKey(name));
}

cxFuncItem cxEngineGetFunc(cxConstChars name)
{
    cxEngine this = cxEngineInstance();
    return cxHashGet(this->functions, cxHashStrKey(name));
}

cxFuncItem cxEngineGetTypeFunc(cxConstType type,cxConstChars name)
{
    cxEngine this = cxEngineInstance();
    cxHash typeFuncs = cxHashGet(this->typeFunctions, cxHashStrKey(type));
    CX_RETURN(typeFuncs == NULL, NULL);
    return cxHashGet(typeFuncs, cxHashStrKey(name));
}

//public xml attr function

static cxString cxEngineLocalizedText(cxEventArg arg)
{
    cxEngine this = cxEngineInstance();
    CX_ASSERT(arg != NULL, "args error");
    
    cxConstChars url = cxEventArgToString(arg);
    CX_ASSERT(url != NULL, "args error");
    cxUrlPath path = cxUrlPathParse(url);
    
    cxTypes langTypes= cxEngineDataSet("appConfig.xml?lang");
    CX_ASSERT(langTypes != NULL, "appConfig.xml must set lang filed");
    cxString dir = cxHashGet(langTypes->any, cxHashStrKey(cxStringBody(this->lang)));
    if(dir == NULL){
        dir = cxHashFirst(langTypes->any);
    }
    CX_ASSERT(dir != NULL, "get lang dir error");
    cxConstChars file = CX_CONST_STRING("%s/%s",cxStringBody(dir),path->path);

    cxTypes types = cxEngineDataSet(CX_CONST_STRING("%s?%s",file,path->key));
    CX_RETURN(types == NULL, NULL);
    
    CX_ASSERT(cxTypesIsType(types, cxTypesString), "must is cxTypesString type");
    return types->any;
}

static cxString cxEngineDataString(cxEventArg arg)
{
    CX_ASSERT(arg != NULL, "args error");
    cxConstChars url = cxEventArgToString(arg);
    CX_ASSERT(url != NULL, "args error");
    cxTypes types = cxEngineDataSet(url);
    return types != NULL ? types->any : NULL;
}

static cxNumber cxEngineDataNumber(cxEventArg arg)
{
    CX_ASSERT(arg != NULL, "args error");
    cxConstChars url = cxEventArgToString(arg);
    CX_ASSERT(url != NULL, "args error");
    cxTypes types = cxEngineDataSet(url);
    CX_ASSERT(cxTypesIsType(types, cxTypesNumber),"type error");
    return types->any;
}

static cxNumber cxEngineRelativeWidth(cxEventArg arg)
{
    CX_ASSERT(arg != NULL, "args error");
    cxEngine engine = cxEngineInstance();
    cxFloat v = cxEventArgToDouble(arg, 1.0f);
    return cxNumberFloat(v * engine->winsize.w);
}

static cxNumber cxEngineRelativeHeight(cxEventArg arg)
{
    CX_ASSERT(arg != NULL, "args error");
    cxEngine engine = cxEngineInstance();
    cxFloat v = cxEventArgToDouble(arg, 1.0f);
    return cxNumberFloat(v * engine->winsize.h);
}

static cxNumber cxEngineBinNumber(cxEventArg arg)
{
    CX_ASSERT(arg != NULL, "args error");
    cxConstChars str = cxEventArgToString(arg);
    CX_ASSERT(str != NULL, "args is string ps:'100010'");
    cxUInt v = cxBinaryToUInt(str);
    return cxNumberUInt(v);
}

static cxNumber cxEngineHexNumber(cxEventArg arg)
{
    CX_ASSERT(arg != NULL, "args error");
    cxConstChars str = cxEventArgToString(arg);
    CX_ASSERT(str != NULL, "args is string ps:'100010'");
    cxUInt v = cxHexToUInt(str);
    return cxNumberUInt(v);
}

static cxTextureAttr cxTextureCreateForXML(cxEventArg arg)
{
    cxTextureAttr rv = CX_CREATE(cxTextureAttr);
    CX_ASSERT(arg != NULL, "args error");
    cxConstChars str = cxEventArgToString(arg);
    CX_ASSERT(str != NULL, "string null");
    cxUrlPath path = cxUrlPathParse(str);
    if(path->count == 0){
        return NULL;
    }
    cxTexture texture = NULL;
    if(path->count > 0){
        texture = cxTextureCreate(path->path);
        rv->size = texture->size;
        CX_RETAIN_SWAP(rv->texture, texture);
    }
    if(path->count > 1 && texture != NULL){
        rv->box = cxTextureBox(texture, path->key);
        rv->size = cxTextureSize(texture, path->key);
    }
    return rv;
}

static cxNumber cxFixScaleByWidth(cxEventArg arg)
{
    CX_ASSERT(arg != NULL, "args error");
    cxEngine engine = cxEngineInstance();
    return cxNumberVec2f(cxVec2fv(engine->scale.x, engine->scale.x));
}

static cxNumber cxFixScaleByHeight(cxEventArg arg)
{
    CX_ASSERT(arg != NULL, "args error");
    cxEngine engine = cxEngineInstance();
    return cxNumberVec2f(cxVec2fv(engine->scale.y, engine->scale.y));
}

static cxTypes cxDataSetTypes(cxEventArg arg)
{
    CX_ASSERT(arg != NULL, "args error");
    cxConstChars url = cxEventArgToString(arg);
    CX_ASSERT(url != NULL, "args error");
    return cxEngineDataSet(url);
}

static cxNumber cxEngineIsAndroid(cxEventArg arg)
{
#if (CX_TARGET_PLATFORM == CX_PLATFORM_ANDROID)
    return cxNumberBool(true);
#else
    return cxNumberBool(false);
#endif
}

static cxNumber cxEngineIsIOS(cxEventArg arg)
{
#if (CX_TARGET_PLATFORM == CX_TARGET_OS_IPHONE)
    return cxNumberBool(true);
#else
    return cxNumberBool(false);
#endif
}

void cxEngineSystemInit()
{
    //cxActionRun({'src':'actions.xml?btnEnter','view':'id','cache':true, 'curve':'ExpOut', 'delay':0.3})
    cxEngineRegisteEvent("cxActionRun", cxViewRunActionEvent);
    //cxPlayEffect({'src':'aa.wav','loop':true})
    cxEngineRegisteEvent("cxPlayEffect", cxPlaySoundEvent);
    //cxPlayMusic({'src':'aa.mp3','loop':true})
    cxEngineRegisteEvent("cxPlayMusic", cxPlayMusicEvent);
    //cxLogger('msg')
    cxEngineRegisteEvent("cxLogger", cxPrintMessageEvent);
    //cxPushView('url')
    cxEngineRegisteEvent("cxPushView", cxViewPushViewEvent);
    //cxPopView
    cxEngineRegisteEvent("cxPopView", cxViewPopViewEvent);
    //cxReplaceView('url')
    cxEngineRegisteEvent("cxReplaceView", cxViewReplaceViewEvent);
    //cxPost('eventkey')
    cxEngineRegisteEvent("cxPost", cxMessagePostEvent);
    //use in ->cxSprite up
    cxEngineRegisteEvent("cxSetTexture", cxSpriteSetTextureEvent);
    //use in ->cxView up
    cxEngineRegisteEvent("cxSetView", cxViewSetViewEvent);
    
    //cxLocalizedText('items.xml?exp') -> cxString get en/items.xml?exp
    cxEngineRegisteFunc("cxLocalizedText", cxEngineLocalizedText);
    
    //cxDataString('items.xml?exp')  -> cxString
    cxEngineRegisteFunc("cxDataString", cxEngineDataString);
    
    //cxDataTypes('items.xml?spline')
    cxEngineRegisteFunc("cxDataTypes", cxDataSetTypes);
    
    //cxDataInt('items,xml?int') -> cxNumber
    cxEngineRegisteFunc("cxDataNumber", cxEngineDataNumber);
    
    //relative screen width and height value -> cxNumber
    //cxRelativeW(1.0)
    cxEngineRegisteFunc("cxRelativeW", cxEngineRelativeWidth);
    cxEngineRegisteFunc("cxRelativeH", cxEngineRelativeHeight);
    
    //fixScale by window.scale.x or scale.y
    cxEngineRegisteFunc("cxFixScaleW", cxFixScaleByWidth);
    cxEngineRegisteFunc("cxFixScaleH", cxFixScaleByHeight);
    
    //cxBinNumber('110111') -> cxNumber
    cxEngineRegisteFunc("cxBinNumber", cxEngineBinNumber);
    
    //cxHexNumber('ffbb00') -> cxNumber
    cxEngineRegisteFunc("cxHexNumber", cxEngineHexNumber);
    
    //cxTextureCreate('candy.xml?red.png')
    cxEngineRegisteFunc("cxTextureCreate", cxTextureCreateForXML);
    
    //platform cond attr func,invoke when prepare xml
    cxEngineRegisteFunc("cxIsAndroid", cxEngineIsAndroid);
    cxEngineRegisteFunc("cxIsIOS", cxEngineIsIOS);
    
    //cxParticle func
    cxParticleTypeInit();
    
    //cxActionRoot
    cxActionRootTypeInit();
    
    //cxChipmunk
    cxChipmunkTypeInit();
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
        cxString data = cxStreamAllBytes(stream);
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

cxAny cxEngineDataSet(cxConstChars url)
{
    CX_RETURN(url == NULL, NULL);
    cxEngine this = cxEngineInstance();
    cxUrlPath path = cxUrlPathParse(url);
    CX_RETURN(path == NULL, NULL);
    cxHashRoot sets = cxHashGet(this->datasets, cxHashStrKey(path->path));
    if(sets == NULL){
        sets = cxHashRootCreate(path->path);
        CX_RETURN(sets == NULL, NULL);
        cxHashSet(this->datasets, cxHashStrKey(path->path), sets);
    }
    cxAny ret = NULL;
    if(path->count >= 2){
        ret = cxHashGet(sets->items, cxHashStrKey(path->key));
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

void cxEngineEnableTouch(cxBool enable)
{
    cxEngine this = cxEngineInstance();
    this->isTouch = enable;
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











