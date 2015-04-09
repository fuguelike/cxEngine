//
//  cxEngine.c
//  cxEngine
//
//  Created by xuhua on 9/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxEngine.h"
#include "cxSpatial.h"
#include "cxPlayer.h"
#include "cxFrames.h"
#include "cxGlobal.h"

#include <textures/cxTextureCache.h>
#include <textures/cxTextureJPG.h>
#include <textures/cxTexturePKM.h>
#include <textures/cxTexturePNG.h>
#include <textures/cxTexturePVR.h>
#include <textures/cxTextureTXT.h>
#include <textures/cxTextureJSON.h>
#include <textures/cxTextureXTF.h>
#include <textures/cxTextureMTF.h>

#include <shaders/cxShaderAlpha.h>
#include <shaders/cxShaderDefault.h>
#include <shaders/cxShaderColor.h>
#include <shaders/cxShaderClipping.h>
#include <shaders/cxShaderGray.h>

#include <streams/cxAssetsStream.h>
#include <streams/cxFileStream.h>
#include <streams/cxMemStream.h>
#include <streams/cxMMapStream.h>

#include <views/cxButton.h>
#include <views/cxLoading.h>
#include <views/cxLabel.h>
#include <views/cxWindow.h>
#include <views/cxScroll.h>
#include <views/cxTable.h>
#include <views/cxClipping.h>
#include <views/cxAlert.h>
#include <views/cxProgress.h>
#include <views/cxCircle.h>

#include <actions/cxMultiple.h>
#include <actions/cxMove.h>
#include <actions/cxFade.h>
#include <actions/cxJump.h>
#include <actions/cxRotate.h>
#include <actions/cxTimer.h>
#include <actions/cxTint.h>
#include <actions/cxAnimate.h>
#include <actions/cxFollow.h>
#include <actions/cxParabola.h>
#include <actions/cxRunner.h>
#include <actions/cxScale.h>
#include <actions/cxSpline.h>
#include <actions/cxTimeLine.h>
#include <actions/cxBezier.h>
#include <actions/cxParticle.h>
#include <actions/cxValue.h>

#include <algorithm/cxAStar.h>

#include <net/cxTCP.h>
#include <net/cxHTTP.h>

cxEngine engine = NULL;

void cxEngineRecvJson(cxStr json)
{
    CX_RETURN(engine == NULL || !engine->IsInit);
    cxJson value = cxJsonCreate(json);
    CX_CALL(engine, OnRecvJson, CX_M(void,cxJson),value);
}

void cxEngineExit()
{
    CX_RETURN(engine == NULL);
    cxEnginePause();
    cxEngineClear();
    cxEngineDestroy();
}

void cxEnginePause()
{
    CX_RETURN(engine == NULL || !engine->IsInit);
    cxEngine engine = cxEngineInstance();
    engine->IsPause = true;
    CX_CALL(engine, OnPause, CX_M(void));
    cxPauseMusic();
}

static void cxEngineTypes()
{
    //register core
    CX_SET_TYPE(cxOpenGL);
    CX_SET_TYPE(cxTexCoord);
    CX_SET_TYPE(cxTexture);
    CX_SET_TYPE(cxUIMap);
    CX_SET_TYPE(cxView);
    CX_SET_TYPE(cxAction);
    CX_SET_TYPE(cxTrack);
    CX_SET_TYPE(cxBuffer);
    CX_SET_TYPE(cxPlayer);
    CX_SET_TYPE(cxTouchItem);
    CX_SET_TYPE(cxAStar);
    CX_SET_TYPE(cxGroup);
    CX_SET_TYPE(cxShader);
    CX_SET_TYPE(cxSpatial);
    CX_SET_TYPE(cxCurveItem);
    CX_SET_TYPE(cxCurve);
    CX_SET_TYPE(cxFrame);
    CX_SET_TYPE(cxFrames);
    
    //register shader
    CX_SET_TYPE(cxShaderAlpha);
    CX_SET_TYPE(cxShaderClipping);
    CX_SET_TYPE(cxShaderDefault);
    CX_SET_TYPE(cxShaderColor);
    CX_SET_TYPE(cxShaderGray);
    
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
    CX_SET_TYPE(cxTextureXTF);
    CX_SET_TYPE(cxTextureMTF);
    
    //register views
    CX_SET_TYPE(cxWindow);
    CX_SET_TYPE(cxSprite);
    CX_SET_TYPE(cxScroll);
    CX_SET_TYPE(cxTable);
    CX_SET_TYPE(cxWindow);
    CX_SET_TYPE(cxClipping);
    CX_SET_TYPE(cxLoading);
    CX_SET_TYPE(cxAsync);
    CX_SET_TYPE(cxAtlas);
    CX_SET_TYPE(cxButton);
    CX_SET_TYPE(cxLabel);
    CX_SET_TYPE(cxAlert);
    CX_SET_TYPE(cxProgress);
    CX_SET_TYPE(cxCircle);
    
    //register actions
    CX_SET_TYPE(cxMultiple);
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
    CX_SET_TYPE(cxValue);
    CX_SET_TYPE(cxDriver);
    //
    CX_SET_TYPE(cxLooper);
    CX_SET_TYPE(cxTCP);
    CX_SET_TYPE(cxHTTP);
    //
    CX_SET_TYPE(cxEngine);
}

static cxBool cxEngineSetLocalized(cxEngine this,cxConstChars key)
{
    CX_RETURN(cxStrOK(this->Localized),true);
    CX_ASSERT(cxStrOK(this->Country), "not set Country");
    CX_ASSERT(cxStrOK(this->Lang), "not set Lang");
    cxConstChars country = cxStrBody(this->Country);
    cxConstChars lang = cxStrBody(this->Lang);
    cxPath path = cxPathParse(key);
    cxChar file[1024]={0};
    //1 zh-CN
    file[snprintf(file, 1024, "strings/%s-%s/%s",lang,country,path->path)] = '\0';
    if(cxAssetsExists(file)){
        CX_RETAIN_SWAP(this->Localized, cxStrCreate("strings/%s-%s",lang,country));
        return true;
    }
    //2 zh
    file[snprintf(file, 1024, "strings/%s/%s",lang,path->path)] = '\0';
    if(cxAssetsExists(file)){
        CX_RETAIN_SWAP(this->Localized, cxStrCreate("strings/%s",lang));
        return true;
    }
    //3 default dir
    cxStr dpath = cxDefaultLocalized(country,lang);
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
        return jsonCreateStr(key);
    }
    cxChar url[1024]={0};
    snprintf(url, 1024, "%s/%s",cxStrBody(this->Localized),key);
    cxPath path = cxPathParse(url);
    if(path->count <= 1){
        return jsonCreateStr(key);
    }
    cxJson json = cxEngineLoadJson(path->path);
    if(json == NULL){
        return jsonCreateStr(key);
    }
    cxJson jv = cxJsonSelect(json, path->key);
    if(jv == NULL){
        return jsonCreateStr(key);
    }
    if(path->count > 2){
        jv = cxJsonAny(jv, path->value);
    }
    if(jv == NULL){
        return jsonCreateStr(key);
    }
    return CX_JSON_PTR(jv);
}

//showBorder show element border if cxBool
//showSize,engine show size .cxSize2f
static void cxEngineLoadManifest(cxEngine engine)
{
    //read Manifest.json config file
    cxJson manifest = cxEngineLoadJson("Manifest.json");
    if(manifest == NULL){
        return;
    }
    CX_RETAIN_SWAP(engine->Manifest, manifest);
    //showBorder
    cxBool showBorder = cxJsonBool(engine->Manifest, "cxShowBorder", false);
    cxEngineSetIsDebug(showBorder);
    //desSize
    cxSize2f desSize = cxJsonSize2f(engine->Manifest, "cxDesignSize", cxSize2fv(0, 0));
    cxEngineSetDesSize(desSize);
    //version
    engine->Version = cxJsonInt(engine->Manifest, "cxVersion", 1);
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

void cxEngineStartup(cxBool layout)
{
#if !defined(NDEBUG)
    CX_LOGGER("cxEngine Version: %d,Run Mode:DEBUG",CX_ENGINE_VERSION);
#else
    CX_LOGGER("cxEngine Version: %d,Run Mode:RELEASE",CX_ENGINE_VERSION);
#endif
    //clear views texture assert cache
    if(engine != NULL){
        cxEngineFree(engine);
        cxEngineClear();
        engine->IsInit = false;
    }else{
        cxGlobalInit();
        cxEngineTypes();
        engine = CX_ALLOC(cxEngine);
    }
    //set localized lang
    cxEngineSetLang(cxLocalizedLang());
    CX_LOGGER("system lang:%s",cxStrBody(cxEngineGetLang()));
    cxEngineSetCountry(cxLocalizedCountry());
    CX_LOGGER("system country:%s",cxStrBody(cxEngineGetCountry()));
    //load config
    cxEngineLoadManifest(engine);
    //init engine
    cxEngineInit(engine);
    if(layout){
        cxEngineLayout(engine->WinSize.w, engine->WinSize.h);
    }
}

void cxEngineClear()
{
    CX_ASSERT(engine != NULL, "engine not init");
    cxWindowClear();
    cxOpenGLClear(engine->opengl);
    cxTextureCacheClear(engine->textures);
    cxPlayerClear(engine->player);
    cxHashClear(engine->files);
    cxHashClear(engine->assets);
    cxHashClear(engine->groups);
    cxGlobalClear();
    cxEngineSetIsDebug(false);
}

void cxEngineResume()
{
    CX_RETURN(engine == NULL || !engine->IsInit);
    cxEngine engine = cxEngineInstance();
    engine->IsPause = false;
    CX_CALL(engine, OnResume, CX_M(void));
    cxResumeMusic();
}

void cxEngineMemory()
{
    CX_RETURN(engine == NULL || !engine->IsInit);
    cxEngine engine = cxEngineInstance();
    CX_CALL(engine, OnMemory, CX_M(void));
}

void cxEngineDraw(cxFloat dt)
{
    cxMemPoolBegin();
    cxEngine engine = cxEngineInstance();
    if(engine == NULL){
        goto completed;
    }
    if(!engine->IsInit || engine->IsPause){
        goto completed;
    }
    engine->FixDelta = dt;
    cxDouble now = cxTimestamp();
    if(engine->prevTime  == 0){
        engine->TimeDelta = engine->FixDelta;
    }else{
        engine->TimeDelta = now - engine->prevTime;
    }
    engine->prevTime = now;
    for(cxInt i=0; i < engine->Accel;i++){
        engine->TotalFrames++;
        CX_CALL(engine, OnUpdate, CX_M(void));
    }
    if(engine->Accel > 1){
        CX_CALL(engine, OnAccel, CX_M(void));
    }
    if(!engine->IsInit || engine->IsPause){
        goto completed;
    }
    cxOpenGLClearColorBuffer();
    cxViewDrawView(engine->Window);
completed:
    cxMemPoolClear();
}

void cxEngineLookAt(cxMatrix4f *matrix,cxFloat zeye,const cxVec3f point)
{
    cxVec3f eye;
    cxVec3f center;
    cxVec3f up;
    kmVec3Fill(&eye, point.x, point.y, zeye);
    kmVec3Fill(&center, point.x, point.y, point.z);
    kmVec3Fill(&up, 0.0f, 1.0f, 0.0f);
    kmMat4Identity(matrix);
    kmMat4LookAt(matrix, &eye, &center, &up);
}

void cxEngineSet3DProjection(cxVec3f offset)
{
    cxFloat zeye = (engine->WinSize.h / 1.1566f);
    
    kmGLMatrixMode(KM_GL_PROJECTION);
    kmGLLoadIdentity();
    
    kmMat4 perspective;
    kmMat4PerspectiveProjection(&perspective, 60.0f, engine->WinSize.w / engine->WinSize.h, 0, zeye*2.0);
    kmGLMultMatrix(&perspective);
    
    kmGLMatrixMode(KM_GL_MODELVIEW);
    kmGLLoadIdentity();

    cxMatrix4f lookAt;
    cxEngineLookAt(&lookAt, zeye, offset);
    kmGLMultMatrix(&lookAt);
    
    cxOpenGLSetDepthTest(false);
    cxOpenGLViewport(cxBox4fv(0, engine->WinSize.w, 0, engine->WinSize.h));
}

void cxEngineSet2DProjection(cxVec3f offset)
{
    kmGLMatrixMode(KM_GL_PROJECTION);
    kmGLLoadIdentity();
    
    kmMat4 orthoMatrix;
    kmMat4OrthographicProjection(&orthoMatrix,0.0f, engine->WinSize.w, 0.0f, engine->WinSize.h, -1.0f, 1.0f);
    kmGLMultMatrix(&orthoMatrix);
    
    kmGLMatrixMode(KM_GL_MODELVIEW);
    kmGLLoadIdentity();
    
    kmMat4 trans;
    kmMat4Translation(&trans, offset.x, offset.y, offset.z);
    kmGLMultMatrix(&trans);
    
    cxOpenGLSetDepthTest(false);
    cxOpenGLViewport(cxBox4fv(0, engine->WinSize.w, 0, engine->WinSize.h));
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
    cxEngineSet3DProjection(cxVec3fv(0, 0, 0));
    if(!engine->IsInit){
        cxOpenGLCheckFeature();
        cxEngineMain(engine);
        cxViewEnter(engine->Window);
    }
    cxViewLayout(engine->Window);
    engine->IsInit = true;
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
//
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
CX_METHOD_DEF(cxEngine, OnUpdate, void)
{
    cxLooperUpdate(this->looper);
    cxMessagePost(cxEngineNoticUpdate, this);
    cxViewUpdateView(this->Window);
}
CX_METHOD_DEF(cxEngine, OnPause, void)
{
    cxMessagePost(cxEngineNoticPause, this);
}
CX_METHOD_DEF(cxEngine, OnResume, void)
{
    cxMessagePost(cxEngineNoticResume, this);
}
CX_METHOD_DEF(cxEngine, OnMemory, void)
{
    cxMessagePost(cxEngineNoticMemory, this);
}
CX_METHOD_DEF(cxEngine, OnRecvJson, void,cxJson json)
{
    cxMessagePost(cxEngineNoticRecvJson, json);
}
CX_METHOD_DEF(cxEngine, OnAccel, void)
{
    this->Accel = 1;
}
CX_TYPE(cxEngine, cxObject)
{
    CX_METHOD(cxEngine, OnAccel);
    CX_METHOD(cxEngine, OnUpdate);
    CX_METHOD(cxEngine, JsonLoader);
    CX_METHOD(cxEngine, JsonLocalized);
    CX_METHOD(cxEngine, JsonProperty);
    CX_METHOD(cxEngine, OnPause);
    CX_METHOD(cxEngine, OnResume);
    CX_METHOD(cxEngine, OnMemory);
    CX_METHOD(cxEngine, OnRecvJson);
    
    CX_GETTER(cxEngine, WinSize);
    CX_GETTER(cxEngine, DesSize);
    CX_GETTER(cxEngine, WinScale);
    
    cxEngineType(this);
}
CX_INIT(cxEngine, cxObject)
{
    kmGLSetCurrentContext(this);
    this->Accel         = 1;
    this->ScaleFactor   = 1.0f;
    this->Interval      = 1.0f/60.0f;
    this->FixDelta      = 1.0f/60.0f;
    this->TimeDelta     = this->FixDelta;
    this->IsDebug       = false;
    this->IsTouch       = true;
    this->Scale         = cxVec2fv(1.0f, 1.0f);
    this->files         = CX_ALLOC(cxHash);
    this->items         = CX_ALLOC(cxHash);
    this->groups        = CX_ALLOC(cxHash);
    this->assets        = CX_ALLOC(cxHash);
    this->curve         = CX_ALLOC(cxCurve);
    this->opengl        = CX_ALLOC(cxOpenGL);
    this->player        = CX_ALLOC(cxPlayer);
    this->textures      = CX_ALLOC(cxTextureCache);
    this->looper        = CX_ALLOC(cxLooper);
    this->Window        = CX_ALLOC(cxWindow);
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
    CX_RELEASE(this->player);
    CX_RELEASE(this->textures);
    CX_RELEASE(this->Manifest);
    kmGLClearCurrentContext();
}
CX_TERM(cxEngine, cxObject)

//a.json?key
cxJson cxEngineJsonReader(cxConstChars src)
{
    cxPath path = cxPathParse(src);
    cxJson json = cxEngineLoadJson(path->path);
    CX_RETURN(json == NULL, NULL);
    //get file.json?key
    if(path->count >= 2){
        json = cxJsonSelect(json, path->key);
    }
    return json;
}

cxAny cxEngineCreateObject(cxConstChars src)
{
    cxJson json = cxEngineJsonReader(src);
    CX_ASSERT(json != NULL, "read json error");
    return cxJsonMakeObject(json);
}

cxStr cxEngineAssetsData(cxConstChars file)
{
    cxHashKey key = cxHashStrKey(file);
    cxEngine this = cxEngineInstance();
    cxStr data = cxHashGet(this->assets, key);
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
    cxStr data = cxAssetsData(file);
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
//
cxStr cxLocalizedFormat(cxConstChars key,...)
{
    cxStr ret = CX_CREATE(cxStr);
    cxStr skey = cxLocalizedStr(key);
    if(!cxStrOK(skey)){
        return NULL;
    }
    va_list ap;
    va_start(ap, key);
    cxStrFormatVA(ret, cxStrBody(skey), ap);
    va_end(ap);
    return ret;
}
cxConstChars cxLocalizedConstCharsVA(cxConstChars key,va_list ap)
{
    CX_ASSERT(cxConstCharsOK(key), "key args error");
    cxStr skey = CX_CREATE(cxStr);
    cxStrFormatVA(skey, key, ap);
    key = cxStrBody(skey);
    cxChar keys[512]={0};
    cxInt type = cxConstCharsTypePath(key, keys);
    if(type == 2){
        key = keys;
    }
    json_t *json = cxEngineLocalizeder(key);
    return json_string_value(json);
}
cxConstChars cxLocalizedConstChars(cxConstChars key,...)
{
    cxConstChars str = NULL;
    va_list ap;
    va_start(ap, key);
    str = cxLocalizedConstCharsVA(key, ap);
    va_end(ap);
    return str;
}
//file.json?key
cxStr cxLocalizedStr(cxConstChars key,...)
{
    cxConstChars str = NULL;
    va_list ap;
    va_start(ap, key);
    str = cxLocalizedConstCharsVA(key, ap);
    va_end(ap);
    if(str == NULL){
        return NULL;
    }
    return cxStrConstChars(str);
}

cxBool cxEngineFireKey(cxKeyType type,cxInt code)
{
    CX_RETURN(engine == NULL || engine->IsPause || !engine->IsInit, false);
    cxEngine this = cxEngineInstance();
    this->key.type = type;
    this->key.code = code;
    return cxViewFireKey(this->Window, &this->key);
}

static void cxEngineUpdateItem(cxDouble now,cxTouchItem item,cxVec2f cpos,cxTouchType type,cxLong key)
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
    item->IsTap = dt < CX_TAP_TIME && item->movement < CX_TAP_MOVEMENT;
    item->previous = cpos;
    item->position = cpos;
    item->type = type;
    item->key = key;
}

static void cxEngineInitItem(cxDouble now,cxTouchItem item,cxVec2f cpos,cxTouchType type,cxLong key)
{
    item->delta = cxVec2fv(0, 0);
    item->previous = cpos;
    item->startTime = now;
    item->speed = cxVec2fv(0, 0);
    item->movement = 0;
    item->startpos = cpos;
    item->position = cpos;
    item->type = type;
    item->key = key;
}

cxBool cxEngineFireTouch(cxTouchType type,cxInt num,cxTouchPoint *points)
{
    CX_RETURN(engine == NULL || engine->IsPause || !engine->IsInit, false);
    cxEngine this = cxEngineInstance();
    //current fires point
    cxDouble now = cxTimestamp();
    cxTouchItems delItems = {0};
    cxBool isFire = false;
    //save point
    for(cxInt i=0; i < num; i++){
        cxTouchPoint p = points[i];
        cxVec2f cpos = cxEngineTouchToWindow(p.xy);
        cxHashKey key = cxHashLongKey(p.id);
        if(type == cxTouchTypeDown){
            cxTouchItem item = CX_ALLOC(cxTouchItem);
            cxHashSet(this->items, key, item);
            CX_RELEASE(item);
            isFire = true;
            cxEngineInitItem(now, item, cpos, type, p.id);
            continue;
        }
        if(type == cxTouchTypeMove){
            cxTouchItem item = cxHashGet(this->items, key);
            CX_ASSERT(item != NULL, "item error %d/%d",p.id,cxHashLength(this->items));
            isFire = !cxVec2fEqu(item->position, cpos);
            if(isFire)cxEngineUpdateItem(now, item, cpos, type,p.id);
            continue;
        }
        if(type == cxTouchTypeUp){
            cxTouchItem item = cxHashGet(this->items, key);
            CX_ASSERT(item != NULL, "item error %d/%d",p.id,cxHashLength(this->items));
            isFire = true;
            cxEngineUpdateItem(now, item, cpos, type,p.id);
            cxTouchItemsAppend(delItems, item);
            continue;
        }
        if(type == cxTouchTypeCancel){
            cxTouchItem item = cxHashGet(this->items, key);
            CX_ASSERT(item != NULL, "item error %d/%d",p.id,cxHashLength(this->items));
            isFire = true;
            cxEngineUpdateItem(now, item, cpos, type,p.id);
            cxTouchItemsAppend(delItems, item);
            continue;
        }
    }
    //get all points
    this->points.number = 0;
    CX_HASH_FOREACH(this->items, ele, tmp){
        cxTouchItemsAppend(this->points, ele->any);
    }
    cxBool ret = cxViewFireTouch(this->Window, &this->points);
    //remove up cancel point
    for(cxInt i=0; i < delItems.number; i++){
        cxTouchItem item = delItems.items[i];
        cxHashDel(this->items, cxHashLongKey(item->key));
    }
    return ret;
}











