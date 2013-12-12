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
    this->window = CX_ALLOC(cxWindow);
    this->scripts = CX_ALLOC(cxHash);
    this->events = CX_ALLOC(cxHash);
    this->datasets = CX_ALLOC(cxHash);
    this->actions = CX_ALLOC(cxHash);
    this->dbenvs = CX_ALLOC(cxHash);
    this->bmpfonts = CX_ALLOC(cxHash);
    this->functions = CX_ALLOC(cxHash);
}
CX_OBJECT_FREE(cxEngine, cxObject)
{
    CX_RELEASE(this->functions);
    CX_RELEASE(this->bmpfonts);
    CX_RELEASE(this->actions);
    CX_RELEASE(this->lang);
    CX_RELEASE(this->datasets);
    CX_RELEASE(this->events);
    CX_RELEASE(this->scripts);
    CX_RELEASE(this->dbenvs);
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

void cxEngineSetLocalLang(cxString lang)
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

void cxEngineRegisteFunc(cxConstChars name,cxAnyFunc func)
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

//public xml attr function

static cxString cxEngineLocalizedString(cxEventArg arg)
{
    CX_ASSERT(arg != NULL, "args error");
    cxConstChars url = cxEventArgToString(arg);
    CX_ASSERT(url != NULL, "args error");
    cxEngine this = cxEngineInstance();
    cxUrlPath path = cxUrlPathParse(url);
    cxTypes types = cxEngineDataSet(CX_CONST_STRING("%s/%s?%s",cxStringBody(this->lang),path->path,path->key));
    CX_RETURN(types == NULL || !cxTypesIsType(types, cxTypesString), NULL);
    return types->assist;
}

static cxString cxEngineDataString(cxEventArg arg)
{
    CX_ASSERT(arg != NULL, "args error");
    cxConstChars url = cxEventArgToString(arg);
    CX_ASSERT(url != NULL, "args error");
    cxTypes types = cxEngineDataSet(url);
    return types != NULL ? types->assist : NULL;
}

static cxNumber cxEngineDataNumber(cxEventArg arg)
{
    CX_ASSERT(arg != NULL, "args error");
    cxConstChars url = cxEventArgToString(arg);
    CX_ASSERT(url != NULL, "args error");
    cxTypes types = cxEngineDataSet(url);
    CX_ASSERT(cxTypesIsType(types, cxTypesNumber),"type error");
    return types->assist;
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

void cxEngineSystemInit()
{
    //set locate lang
    cxEngineSetLocalLang(cxStringConstChars("en"));//cxLocaleLang());
    //cxActionRun({'src':'actions.xml?btnEnter','view':'id','cache':true, 'curve':'ExpOut', 'delay':0.3})
    cxEngineRegisteEvent("cxActionRun", cxViewRunActionEvent);
    //cxPlay({'src':'aa.wav','loop':true})
    cxEngineRegisteEvent("cxPlay", cxPlaySoundEvent);
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
    
    //cxLocalizedString('items.xml?exp') -> cxString get en/items.xml?exp
    cxEngineRegisteFunc("cxLocalizedString", (cxAnyFunc)cxEngineLocalizedString);
    
    //cxDataString('items.xml?exp')  -> cxString
    cxEngineRegisteFunc("cxDataString", (cxAnyFunc)cxEngineDataString);
    
    //cxDataTypes('items.xml?spline')
    cxEngineRegisteFunc("cxDataTypes", (cxAnyFunc)cxDataSetTypes);
    
    //cxDataInt('items,xml?int') -> cxNumber
    cxEngineRegisteFunc("cxDataNumber", (cxAnyFunc)cxEngineDataNumber);
    
    //relative screen width and height value -> cxNumber
    cxEngineRegisteFunc("cxRelativeW", (cxAnyFunc)cxEngineRelativeWidth);
    cxEngineRegisteFunc("cxRelativeH", (cxAnyFunc)cxEngineRelativeHeight);
    
    //cxBinNumber('110111') -> cxNumber
    cxEngineRegisteFunc("cxBinNumber", (cxAnyFunc)cxEngineBinNumber);
    
    //cxHexNumber('ffbb00') -> cxNumber
    cxEngineRegisteFunc("cxHexNumber", (cxAnyFunc)cxEngineHexNumber);
    
    //cxTextureCreate('candy.xml?red.png')
    cxEngineRegisteFunc("cxTextureCreate", (cxAnyFunc)cxTextureCreateForXML);
    
    //fixScale by window.scale.x or scale.y
    cxEngineRegisteFunc("cxFixScaleW", (cxAnyFunc)cxFixScaleByWidth);
    cxEngineRegisteFunc("cxFixScaleH", (cxAnyFunc)cxFixScaleByHeight);
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
    cxHashXML sets = cxHashGet(this->datasets, cxHashStrKey(path->path));
    if(sets == NULL){
        sets = cxHashXMLCreate(path->path);
        CX_RETURN(sets == NULL, NULL);
        cxHashSet(this->datasets, cxHashStrKey(path->path), sets);
    }
    return path->count == 2 ? cxHashGet(sets->items, cxHashStrKey(path->key)) : NULL;
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











