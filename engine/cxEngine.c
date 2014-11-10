//
//  cxEngine.c
//  cxEngine
//
//  Created by xuhua on 9/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxEngine.h"
#include "cxValue.h"
#include "cxSpatial.h"
#include "cxPlayer.h"

#include <textures/cxTextureCache.h>
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

#include <algorithm/cxAStar.h>

#include <net/cxLooper.h>
#include <net/cxTCP.h>

cxEngine engine = NULL;

void cxEngineRecvJson(cxString json)
{
    CX_RETURN(engine == NULL || !engine->isInit);
    cxMessagePost(cxEngineNoticRecvJson, json);
}

void cxEngineExit()
{
    CX_RETURN(engine == NULL);
    cxEnginePause();
    cxEngineDestroy();
    cxEngineTerminate();
}

void cxEnginePause()
{
    CX_RETURN(engine == NULL || !engine->isInit);
    cxEngine engine = cxEngineInstance();
    engine->isPause = true;
    cxMessagePost(cxEngineNoticPause, engine);
    cxPauseMusic();
}

static void cxRegisterEngineTypes()
{
    //register core
    CX_SET_TYPE(cxIconv);
    CX_SET_TYPE(cxOpenGL);
    CX_SET_TYPE(cxTexCoord);
    CX_SET_TYPE(cxTexture);
    CX_SET_TYPE(cxView);
    CX_SET_TYPE(cxAction);
    CX_SET_TYPE(cxJson);
    CX_SET_TYPE(cxTrack);
    CX_SET_TYPE(cxBuffer);
    CX_SET_TYPE(cxPlayer);
    CX_SET_TYPE(cxTouchItem);
    CX_SET_TYPE(cxAStar);
    CX_SET_TYPE(cxGroup);
    CX_SET_TYPE(cxShader);
    CX_SET_TYPE(cxValue);
    CX_SET_TYPE(cxSpatial);
    CX_SET_TYPE(cxCurveItem);
    CX_SET_TYPE(cxCurve);
    
    //register shader
    CX_SET_TYPE(cxShaderAlpha);
    CX_SET_TYPE(cxShaderClipping);
    CX_SET_TYPE(cxShaderDefault);
    CX_SET_TYPE(cxShaderColor);
    CX_SET_TYPE(cxShaderMultiple);
    CX_SET_TYPE(cxShaderTTF);
    
    //register streams
    CX_SET_TYPE(cxAssetsStream);
    CX_SET_TYPE(cxFileStream);
    CX_SET_TYPE(cxMemStream);
    CX_SET_TYPE(cxMMapStream);
    
    //register textures
    CX_SET_TYPE(cxTextureCache);
    CX_SET_TYPE(cxTextureJPG);
    CX_SET_TYPE(cxTexturePKM);
    CX_SET_TYPE(cxTexturePNG);
    CX_SET_TYPE(cxTextureTXT);
    CX_SET_TYPE(cxTexturePVR);
    CX_SET_TYPE(cxTextureJSON);
    
    //register views
    CX_SET_TYPE(cxWindow);
    CX_SET_TYPE(cxSprite);
    CX_SET_TYPE(cxScroll);
    CX_SET_TYPE(cxTable);
    CX_SET_TYPE(cxWindow);
    CX_SET_TYPE(cxClipping);
    CX_SET_TYPE(cxLoading);
    CX_SET_TYPE(cxAsync);
    CX_SET_TYPE(cxPolygon);
    CX_SET_TYPE(cxAtlas);
    CX_SET_TYPE(cxButton);
    CX_SET_TYPE(cxLabelTTF);
    CX_SET_TYPE(cxLabelBMP);
    CX_SET_TYPE(cxAlert);
    CX_SET_TYPE(cxSpine);
    
    //register actions
    CX_SET_TYPE(cxMultiple);
    CX_SET_TYPE(cxAnimateItem);
    CX_SET_TYPE(cxAnimate);
    CX_SET_TYPE(cxFade);
    CX_SET_TYPE(cxFollow);
    CX_SET_TYPE(cxJump);
    CX_SET_TYPE(cxMove);
    CX_SET_TYPE(cxParabola);
    CX_SET_TYPE(cxParticle);
    CX_SET_TYPE(cxRotate);
    CX_SET_TYPE(cxRunner);
    CX_SET_TYPE(cxScale);
    CX_SET_TYPE(cxSpline);
    CX_SET_TYPE(cxTimer);
    CX_SET_TYPE(cxTint);
    CX_SET_TYPE(cxTimeLine);
    CX_SET_TYPE(cxBezier);
    CX_SET_TYPE(cxSkeleton);
    CX_SET_TYPE(cxDriver);
    //
    CX_SET_TYPE(cxLooper);
    CX_SET_TYPE(cxTCP);
    //
    CX_SET_TYPE(cxEngine);
}

static cxBool cxEngineSetLocalized(cxEngine this,cxConstChars key)
{
    CX_RETURN(cxStringOK(this->Localized),true);
    CX_ASSERT(cxStringOK(this->Country), "not set Country");
    CX_ASSERT(cxStringOK(this->Lang), "not set Lang");
    cxConstChars country = cxStringBody(this->Country);
    cxConstChars lang = cxStringBody(this->Lang);
    cxPath path = cxPathParse(key);
    cxChar file[1024]={0};
    //1 zh-CN
    file[snprintf(file, 1024, "strings/%s-%s/%s",lang,country,path->path)] = '\0';
    if(cxAssetsExists(file)){
        CX_RETAIN_SWAP(this->Localized, cxStringCreate("strings/%s-%s",lang,country));
        return true;
    }
    //2 zh
    file[snprintf(file, 1024, "strings/%s/%s",lang,path->path)] = '\0';
    if(cxAssetsExists(file)){
        CX_RETAIN_SWAP(this->Localized, cxStringCreate("strings/%s",lang));
        return true;
    }
    //3 default dir
    cxString dpath = cxDefaultLocalized();
    if(dpath != NULL){
        CX_RETAIN_SWAP(this->Localized,dpath);
        return true;
    }
    return false;
}

json_t *cxEngineLocalizeder(cxConstChars key)
{
    cxEngine this = cxEngineInstance();
    CX_ASSERT(cxConstCharsOK(key) && this != NULL, "args error or enstance null");
    if(!cxEngineSetLocalized(this,key)){
        return jsonCreateString(key);
    }
    cxChar url[1024]={0};
    snprintf(url, 1024, "%s/%s",cxStringBody(this->Localized),key);
    cxPath path = cxPathParse(url);
    if(path->count < 2){
        return jsonCreateString(key);
    }
    cxJson json = cxEngineLoadJson(path->path);
    if(json == NULL){
        return jsonCreateString(key);
    }
    json_t *value = json_object_get(CX_JSON_PTR(json), path->key);
    if(value == NULL){
        return jsonCreateString(key);
    }
    return value;
}

//showBorder show element border if cxBool
//showSize,engine show size .cxSize2f
static void cxEngineLoadConfig(cxEngine engine)
{
    //showBorder
    cxBool showBorder = cxJsonBool(engine->Config, "cxShowBorder", false);
    cxEngineSetIsDebug(showBorder);
    //desSize
    cxSize2f desSize = cxJsonSize2f(engine->Config, "cxDesignSize", cxSize2fv(0, 0));
    cxEngineSetDesSize(desSize);
    //version
    engine->Version = cxJsonInt(engine->Config, "cxVersion", 1);
}

static json_t *cxEngineJsonRunGetter(cxConstChars key)
{
    cxEngine engine = cxEngineInstance();
    cxJson value = NULL;
    if(cxPropertyRunGetter(engine, key, (cxAny *)&value)){
        return value != NULL ? CX_JSON_PTR(value):NULL;
    }
    return NULL;
}

void cxEngineStartup()
{
#if !defined(NDEBUG)
    CX_LOGGER("cxEngine Version: %d,Run Mode:DEBUG",CX_ENGINE_VERSION);
#else
    CX_LOGGER("cxEngine Version: %d,Run Mode:RELEASE",CX_ENGINE_VERSION);
#endif
    CX_ASSERT(engine == NULL, "repeat init engine");
    cxGlobalInit();
    cxRegisterEngineTypes();
    engine = CX_ALLOC(cxEngine);
    //set localized lang
    cxEngineSetLang(cxLocalizedLang());
    CX_LOGGER("system lang:%s",cxStringBody(cxEngineGetLang()));
    cxEngineSetCountry(cxLocalizedCountry());
    CX_LOGGER("system country:%s",cxStringBody(cxEngineGetCountry()));
    //read Manifest.json config file
    CX_RETAIN_SET(engine->Config, cxEngineLoadJson("Manifest.json"));
    if(engine->Config != NULL){
        cxEngineLoadConfig(engine);
    }
    //use init engine
    cxEngineInit(engine);
}

void cxEngineClear()
{
    CX_RETURN(engine == NULL);
    cxEngine engine = cxEngineInstance();
    cxTextureCacheClear(engine->textures);
    cxHashClear(engine->files);
    cxHashClear(engine->assets);
}

void cxEngineResume()
{
    CX_RETURN(engine == NULL || !engine->isInit);
    cxEngine engine = cxEngineInstance();
    engine->isPause = false;
    cxMessagePost(cxEngineNoticResume, engine);
    cxResumeMusic();
}

void cxEngineMemory()
{
    CX_RETURN(engine == NULL || !engine->isInit);
    cxEngine engine = cxEngineInstance();
    cxMessagePost(cxEngineNoticMemory, engine);
}

void cxEngineDraw(cxFloat dt)
{
    CX_RETURN(engine == NULL || !engine->isInit || engine->isPause);
    cxEngine engine = cxEngineInstance();
    cxMemPoolBegin();
    cxOpenGLClear();
    cxLooperUpdate(engine->looper);
    CX_CALL(engine, Update, CX_M(void));
    kmGLPushMatrix();
    cxViewDrawView(engine->Window);
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
    cxViewSetSize(engine->Window, engine->WinSize);
    if(!cxSize2fZero(engine->DesSize)){
        engine->Scale.x = engine->WinSize.w/engine->DesSize.w;
        engine->Scale.y = engine->WinSize.h/engine->DesSize.h;
    }else{
        engine->Scale.x = 1.0f;
        engine->Scale.y = 1.0f;
    }
    cxFloat zeye = engine->WinSize.h / 1.1566f;
    kmMat4 perspective;
    kmGLMatrixMode(KM_GL_PROJECTION);
    kmGLLoadIdentity();
    kmMat4PerspectiveProjection(&perspective, 60.0f, engine->WinSize.w / engine->WinSize.h, 0.0f, zeye * 2);
    kmGLMultMatrix(&perspective);
    kmGLMatrixMode(KM_GL_MODELVIEW);
    kmGLLoadIdentity();
    cxMatrix4f matrix;
    cxEngineLookAt(&matrix,zeye,cxVec2fv(0, 0));
    kmGLMultMatrix(&matrix);
    cxOpenGLSetDepthTest(false);
    cxOpenGLViewport(cxBox4fv(0, engine->WinSize.w, 0, engine->WinSize.h));
    if(!engine->isInit){
        cxOpenGLCheckFeature();
        cxEngineMain(engine);
        cxViewEnter(engine->Window);
    }
    cxViewLayout(engine->Window);
    engine->isInit = true;
}

cxAny cxEngineTimer(cxFloat freq,cxInt repeat)
{
    cxEngine engine = cxEngineInstance();
    return cxViewAppendTimer(engine->Window, freq, repeat);
}
//for cxcore
cxAny cxJsonLoader(cxConstChars src)
{
    CX_ASSERT(engine != NULL, "engine not init");
    return CX_CALL(engine, JsonLoader, CX_M(cxAny,cxConstChars), src);
}
cxAny cxJsonLocalized(cxConstChars key)
{
    CX_ASSERT(engine != NULL, "engine not init");
    return CX_CALL(engine, JsonLocalized, CX_M(cxAny,cxConstChars), key);
}
cxAny cxJsonProperty(cxConstChars key)
{
    CX_ASSERT(engine != NULL, "engine not init");
    return CX_CALL(engine, JsonProperty, CX_M(cxAny,cxConstChars), key);
}
//for cxcore end
void cxTypeFinished(cxType this)
{
    
}
CX_GETTER_DEF(cxEngine, WinSize)
{
    cxSize2f size = cxEngineGetWinSize();
    return cxJsonSize2fToJson(size);
}
CX_GETTER_DEF(cxEngine, WinScale)
{
    cxVec2f scale = cxEngineGetScale();
    return cxJsonVec2fToJson(scale);
}
CX_GETTER_DEF(cxEngine, DesSize)
{
    cxSize2f size = cxEngineGetDesSize();
    return cxJsonSize2fToJson(size);
}
CX_METHOD_DEF(cxEngine, JsonLoader, cxAny, cxConstChars src)
{
    return cxEngineJsonReader(src);
}
CX_METHOD_DEF(cxEngine, JsonLocalized, cxAny, cxConstChars key)
{
    return cxEngineLocalizeder(key);
}
CX_METHOD_DEF(cxEngine, JsonProperty, cxAny, cxConstChars key)
{
    return cxEngineJsonRunGetter(key);
}
CX_METHOD_DEF(cxEngine, Update, void)
{
    
}
CX_TYPE(cxEngine, cxObject)
{
    CX_METHOD(cxEngine, Update);
    CX_METHOD(cxEngine, JsonLoader);
    CX_METHOD(cxEngine, JsonLocalized);
    CX_METHOD(cxEngine, JsonProperty);
    
    CX_GETTER(cxEngine, WinSize);
    CX_GETTER(cxEngine, DesSize);
    CX_GETTER(cxEngine, WinScale);
    
    cxEngineType(this);
}
CX_INIT(cxEngine, cxObject)
{
    this->Interval  = 1.0f/60.0f;
    this->Delta     = 1.0f/60.0f;
    this->IsDebug   = false;
    this->isTouch   = true;
    this->Scale     = cxVec2fv(1.0f, 1.0f);
    this->files     = CX_ALLOC(cxHash);
    this->items     = CX_ALLOC(cxHash);
    this->groups    = CX_ALLOC(cxHash);
    this->assets    = CX_ALLOC(cxHash);
    this->Window    = CX_ALLOC(cxWindow);
    this->curve     = CX_ALLOC(cxCurve);
    this->opengl    = CX_ALLOC(cxOpenGL);
    this->iconv     = CX_ALLOC(cxIconv);
    this->player    = CX_ALLOC(cxPlayer);
    this->textures  = CX_ALLOC(cxTextureCache);
    this->looper    = CX_ALLOC(cxLooper);
}
CX_FREE(cxEngine, cxObject)
{
    CX_RELEASE(this->looper);
    CX_RELEASE(this->groups);
    CX_RELEASE(this->items);
    CX_RELEASE(this->files);
    CX_RELEASE(this->assets);
    CX_RELEASE(this->Lang);
    CX_RELEASE(this->Localized);
    CX_RELEASE(this->Country);
    CX_RELEASE(this->Window);
    CX_RELEASE(this->curve);
    CX_RELEASE(this->opengl);
    CX_RELEASE(this->iconv);
    CX_RELEASE(this->player);
    CX_RELEASE(this->textures);
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
    return cxJsonMakeObject(json);
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
    if(json != NULL){
        return json;
    }
    cxString data = cxAssetsData(file);
    CX_ASSERT(data != NULL, "load json file %s failed");
    json = cxJsonCreate(data);
    cxHashSet(this->files, key, json);
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
    cxEngineFree(engine);
    CX_RELEASE(engine);
    engine = NULL;
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

//file.json?key
cxString cxLocalizedString(cxConstChars key)
{
    json_t *json = cxEngineLocalizeder(key);
    return cxStringConstChars(json_string_value(json));
}

void cxEngineEnableTouch(cxBool enable)
{
    cxEngine this = cxEngineInstance();
    this->isTouch = enable;
}

cxBool cxEngineFireKey(cxKeyType type,cxInt code)
{
    CX_RETURN(engine == NULL || engine->isPause || !engine->isInit, false);
    cxEngine this = cxEngineInstance();
    this->key.type = type;
    this->key.code = code;
    return cxViewFireKey(this->Window, &this->key);
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
    CX_RETURN(engine == NULL || engine->isPause || !engine->isInit, false);
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
    cxBool ret = cxViewFireTouch(this->Window, &this->points);
    //remove up cancel point
    for(cxInt i=0; i < delItems.number; i++){
        item = delItems.items[i];
        cxHashDel(this->items, cxHashLongKey(item->key));
    }
    return ret;
}











