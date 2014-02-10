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
#include "cxActionRoot.h"
#include "cxInitType.h"
#include "cxViewRoot.h"
#include "cxEngine.h"
#include "cxAutoPool.h"
#include "cxOpenGL.h"
#include "cxUtil.h"
#include "cxHashRoot.h"
#include "cxDB.h"
#include "cxFreeType.h"

static cxEngine instance = NULL;
static cxBool isExit = false;

static cxInt cxEngineLuaSetDesignSize(lua_State *L)
{
    cxEngine this = cxEngineInstance();
    this->dessize = cxLuaSize2fValue(L, 1, this->dessize);
    return 0;
}

static cxInt cxEngineLuaGetDesignSize(lua_State *L)
{
    cxEngine this = cxEngineInstance();
    cxLuaPushSize2fv(L, this->dessize);
    return 1;
}

static cxInt cxEngineLuaSetShowBorder(lua_State *L)
{
    cxEngine this = cxEngineInstance();
    this->isShowBorder = lua_toboolean(L, 1);
    return 0;
}

static cxInt cxEngineLuaGetShowBorder(lua_State *L)
{
    cxEngine this = cxEngineInstance();
    lua_pushboolean(L, this->isShowBorder);
    return 1;
}

static cxInt cxEngineLuaGetScreenSize(lua_State *L)
{
    cxEngine this = cxEngineInstance();
    cxLuaPushSize2fv(L, this->winsize);
    return 1;
}

static cxInt cxEngineLuaAppendEvent(lua_State *L)
{
    CX_LUA_EVENT_BEG(cxEngine);
    CX_LUA_EVENT_APPEND(onExit);
    CX_LUA_EVENT_END(cxEngine);
}

CX_LUA_METHOD_BEG(cxEngine)
    {"On",cxEngineLuaAppendEvent},
    {"ScreenSize",cxEngineLuaGetScreenSize},
    CX_LUA_PROPERTY(cxEngine, ShowBorder),
    CX_LUA_PROPERTY(cxEngine, DesignSize),
CX_LUA_METHOD_END(cxEngine)

void __cxEngineTypeInit()
{
    CX_LUA_LOAD_TYPE(cxEngine);
}

static cxInt cxEngineLuaLoader(lua_State *L)
{
    cxChar path[PATH_MAX] = {0};
    cxConstChars file = luaL_checkstring(L, 1);
    cxConstChars ext = strrchr(file, '.');
    cxString data = NULL;
    if(ext == NULL || !cxConstCharsEqu(ext, ".lua")){
        snprintf(path, PATH_MAX, "%s.lua",file);
        data = cxEngineGetLuaScript(path);
    }
    if(data == NULL){
        snprintf(path, PATH_MAX, "%s",file);
        data = cxEngineGetLuaScript(path);
    }
    if(data != NULL){
        luaL_loadbuffer(L, cxStringBody(data), cxStringLength(data), file);
        return 1;
    }
    luaL_error(L, "error loading file %s error",path);
    return 1;
}

static void cxEngineAddLuaLoader(lua_State *L)
{
    lua_getglobal(L, "package");
    lua_getfield(L, -1, "loaders");
    lua_pushcfunction(L, cxEngineLuaLoader);
    for(cxInt i = lua_objlen(L, -2) + 1; i > 2; --i){
        lua_rawgeti(L, -2, i - 1);
        lua_rawseti(L, -3, i);
    }
    lua_rawseti(L, -2, 2);
    lua_setfield(L, -2, "loaders");
    lua_pop(L, 1);
}

static int cxEngineLuaPanic(lua_State*L)
{
    CX_ERROR("PANIC: unprotected error in call to Lua API (%s)\n",lua_tostring(L, -1));
    return 0;
}

static void *cxEngineLuaAllocFunc(void *ud, void *ptr, size_t osize, size_t nsize)
{
    if(nsize == 0){
        allocator->free(ptr);
        return NULL;
    }else{
        return allocator->realloc(ptr,nsize);
    }
}

cxBool cxEngineLuaRunFile(cxConstChars file)
{
    cxString data = cxEngineGetLuaScript(file);
    return cxEngineLuaRunString(data);
}

cxBool cxEngineLuaRunString(cxString code)
{
    return cxEngineLuaRunChars(cxStringBody(code));
}

cxBool cxEngineLuaRunChars(cxConstChars code)
{
    CX_ASSERT(cxConstCharsOK(code), "code args error");
    if(luaL_dostring(gL, code) != 0){
        CX_ERROR("cxLuaLoader run code error:%s",lua_tostring(gL, -1));
        return false;
    }
    return true;
}

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
    CX_EVENT_FIRE(engine, onExit);
    cxEnginePause();
    cxEngineDestroy();
    cxAllocatorFree();
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
    //set locate lang
    cxEngineSetLocalized(cxLocalizedLang());
    //init event list and att method
    cxEngineSystemInit(engine);
    //
    cxPlayerOpen(0, 0);
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
    //
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
    cxAllocatorInit();
    cxAutoPoolInit();
    kmGLInitialize();
    xmlInitGlobals();
    this->frameInterval = 1.0f/60.0f;
    this->isShowBorder = true;
    this->isTouch = true;
    this->scale     = cxVec2fv(1.0f, 1.0f);
    this->window    = CX_ALLOC(cxWindow);
    this->scripts   = CX_ALLOC(cxHash);
    this->datasets  = CX_ALLOC(cxHash);
    this->actions   = CX_ALLOC(cxHash);
    this->dbenvs    = CX_ALLOC(cxHash);
    this->bmpfonts  = CX_ALLOC(cxHash);
    gL = lua_newstate(cxEngineLuaAllocFunc, this);
    CX_ASSERT(gL != NULL, "new lua state error");
    lua_atpanic(gL, cxEngineLuaPanic);
    luaL_openlibs(gL);
    cxEngineAddLuaLoader(gL);
}
CX_OBJECT_FREE(cxEngine, cxObject)
{
    CX_RELEASE(this->bmpfonts);
    CX_RELEASE(this->actions);
    CX_RELEASE(this->lang);
    CX_RELEASE(this->datasets);
    CX_RELEASE(this->scripts);
    CX_RELEASE(this->dbenvs);
    CX_RELEASE(this->window);
    CX_EVENT_RELEASE(this->onExit);
    CX_SIGNAL_RELEASE(this->onRecvJson);
    CX_SIGNAL_RELEASE(this->onTouch);
    CX_SIGNAL_RELEASE(this->onUpdate);
    CX_SIGNAL_RELEASE(this->onPause);
    CX_SIGNAL_RELEASE(this->onResume);
    CX_SIGNAL_RELEASE(this->onMemory);
    CX_METHOD_RELEASE(this->MakeAction);
    CX_METHOD_RELEASE(this->MakeView);
    cxEventBaseDestroy();
    cxCurveDestroy();
    cxOpenGLDestroy();
    cxIconvDestroy();
    cxFreeTypeDestroy();
    cxPlayerDestroy();
    cxMessageDestroy();
    xmlCleanupGlobals();
    kmGLFreeAll();
    cxAutoPoolDestroy();
    lua_close(gL);
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

cxInt cxEnginePlatform()
{
    return CX_TARGET_PLATFORM;
}

cxString cxEngineGetLuaScript(cxConstChars file)
{
    cxXMLScript script = cxEngineGetXMLScript(file);
    return script != NULL ? script->bytes : NULL;
}

cxXMLScript cxEngineGetXMLScript(cxConstChars file)
{
    cxEngine this = cxEngineInstance();
    CX_RETURN(file == NULL,NULL);
    
    cxXMLScript script = cxHashGet(this->scripts, cxHashStrKey(file));
    if(script != NULL){
        return script;
    }
    
    script = CX_CREATE(cxXMLScript);
    cxStream stream = cxAssetsStreamCreate(file);
    CX_RETURN(stream == NULL, NULL);
    
    CX_RETAIN_SWAP(script->bytes, cxStreamAllBytes(stream));
    CX_RETURN(script->bytes == NULL, NULL);
    
    cxHashSet(this->scripts, cxHashStrKey(file), script);
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

cxAny cxEngineDB(cxConstChars url)
{
    cxTypes type = cxEngineTypes(url);
    if(type == NULL || type->any == NULL){
        return NULL;
    }
    return cxTypesIsType(type, cxTypesDB) ? type->any : NULL;
}

cxAny cxEngineTypes(cxConstChars url)
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

cxString cxEngineLocalizedText(cxConstChars url)
{
    cxEngine this = cxEngineInstance();
    cxUrlPath path = cxUrlPathParse(url);
    
    cxTypes langTypes= cxEngineTypes("appConfig.xml?lang");
    CX_ASSERT(langTypes != NULL, "appConfig.xml must set lang filed");
    
    cxString dir = cxHashGet(langTypes->any, cxHashStrKey(cxStringBody(this->lang)));
    if(dir == NULL){
        dir = cxHashFirst(langTypes->any);
    }
    CX_ASSERT(dir != NULL, "get lang dir error");
    
    cxConstChars file = CX_CONST_STRING("%s/%s",cxStringBody(dir),path->path);
    
    cxTypes types = cxEngineTypes(CX_CONST_STRING("%s?%s",file,path->key));
    CX_RETURN(types == NULL || !cxTypesIsType(types, cxTypesString), NULL);
    return types->any;
}

static cxInt cxLocalizedText(lua_State *L)
{
    cxConstChars url = luaL_checkstring(L, 2);
    CX_ASSERT(url != NULL, "args error");
    cxString text = cxEngineLocalizedText(url);
    CX_LUA_PUSH_OBJECT(text);
    return 1;
}

static cxInt cxEventLogger(lua_State *L)
{
    cxConstChars msg = luaL_checkstring(L, 2);
    CX_LOGGER("%s",msg);
    return 0;
}

static cxInt cxEventAction(lua_State *L)
{
    CX_LUA_DEF_THIS(cxObject);
    cxAny view = NULL;
    cxConstChars src = NULL;
    cxConstChars viewId = NULL;
    cxBool cache = false;
    cxConstChars curve = NULL;
    cxFloat delay = 0;
    if(!lua_istable(L, 2)){
        luaL_error(L, "args error");
        return 0;
    }
    //
    lua_getfield(L, 2, "delay");
    if(lua_isnumber(L, -1)){
        delay = lua_tonumber(L, -1);
    }
    lua_pop(L,1);
    //
    lua_getfield(L, 2, "src");
    if(lua_isstring(L, -1)){
        src = lua_tostring(L, -1);
    }
    lua_pop(L, 1);
    CX_RETURN(src == NULL, 0);
    //
    lua_getfield(L, 2, "curve");
    if(lua_isstring(L, -1)){
        curve = lua_tostring(L, -1);
    }
    lua_pop(L, 1);
    //
    lua_getfield(L, 2, "view");
    if(lua_isstring(L, -1)){
        viewId = lua_tostring(L, -1);
    }else if(lua_isuserdata(L, -1)){
        view = CX_LUA_GET_PTR(-1);
    }
    lua_pop(L, 1);
    //
    lua_getfield(L, 2, "cache");
    if(lua_isboolean(L, -1)){
        cache = lua_toboolean(L, -1);
    }
    lua_pop(L, 1);
    //get view by id and cxBase
    if(view != NULL){
        viewId = NULL;
    }else if(this->cxBase == cxBaseTypeAction){
        view = cxActionView(this);
    }else if(this->cxBase == cxBaseTypeView){
        view = this;
    }else{
        luaL_error(L,"base view and action can use this event");
        return 0;
    }
    CX_ASSERT(view != NULL, "this event's sender must base cxAction and cxView");
    if(viewId != NULL){
        view = cxViewRootGet(view, viewId);
        CX_RETURN(view == NULL, 0);
    }
    //get action
    cxAny action = NULL;
    cxBool fromcache =  false;
    if(cache){
        action = cxViewGetCache(view, src);
        fromcache = (action != NULL);
    }
    if(action == NULL){
        action = cxActionRootGet(src);
    }
    CX_RETURN(action == NULL, 0);
    if(!fromcache && cache){
        cxViewSetCache(view, src, action);
    }
    if(fromcache){
        cxActionReset(action);
    }
    //set action corve
    cxCurveItem curveitem = cxCurveGet(curve);
    if(curveitem != NULL){
        cxActionSetCurve(action, curveitem->func);
    }
    //delay
    if(delay > 0){
        cxActionSetDelay(action, delay);
    }
    cxViewAppendAction(view, action);
    return 0;
}

static cxInt cxEventEffect(lua_State *L)
{
    cxConstChars src = NULL;
    cxBool loop = false;
    //
    lua_getfield(L, 2, "loop");
    if(lua_isboolean(L, -1)){
        loop = lua_toboolean(L, -1);
    }
    lua_pop(L,1);
    //
    lua_getfield(L, 2, "src");
    if(lua_isstring(L, -1)){
        src = lua_tostring(L, -1);
    }
    lua_pop(L,1);
    cxPlayEffect(src,loop);
    return 0;
}

static cxInt cxEventMusic(lua_State *L)
{
    cxConstChars src = NULL;
    cxBool loop = false;
    //
    lua_getfield(L, 2, "loop");
    if(lua_isboolean(L, -1)){
        loop = lua_toboolean(L, -1);
    }
    lua_pop(L,1);
    //
    lua_getfield(L, 2, "src");
    if(lua_isstring(L, -1)){
        src = lua_tostring(L, -1);
    }
    lua_pop(L,1);
    cxPlayMusic(src, loop);
    return 0;
}

static cxInt cxEventReplaceView(lua_State *L)
{
    cxConstChars url = luaL_checkstring(L, 2);
    cxViewRoot view = cxViewRootCreate(url);
    CX_ASSERT(view != NULL, "create xml view %s falied ",url);
    cxWindowReplaceView(view,NULL);
    return 0;
}

static cxInt cxEventPushView(lua_State *L)
{
    cxConstChars url = luaL_checkstring(L, 2);
    cxViewRoot view = cxViewRootCreate(url);
    CX_ASSERT(view != NULL, "create xml view %s falied ",url);
    cxWindowPushView(view,NULL);
    return 0;
}

static cxInt cxEventPopView(lua_State *L)
{
    cxWindowPopView(NULL);
    return 0;
}

static cxInt cxEventMessagePost(lua_State *L)
{
    CX_LUA_DEF_THIS(cxObject);
    cxConstChars skey = luaL_checkstring(L, 2);
    cxMessagePost(skey, this);
    return 0;
}

static cxInt cxEventSetTexture(lua_State *L)
{
    CX_LUA_DEF_THIS(cxView);
    cxConstChars url = NULL;
    cxConstChars viewid = NULL;
    cxBool uts = false;
    lua_getfield(L, 2, "src");
    if(lua_isstring(L, -1)){
        url = lua_tostring(L, -1);
    }
    lua_pop(L,1);
    lua_getfield(L, 2, "view");
    if(lua_isstring(L, -1)){
        viewid = lua_tostring(L, -1);
    }
    lua_pop(L,1);
    lua_getfield(L, 2, "useTexSize");
    if(lua_isboolean(L, -1)){
        uts = lua_toboolean(L, -1);
    }
    lua_pop(L,1);
    if(viewid == NULL){
        return 0;
    }
    cxView pview = cxViewRootGet(this, viewid);
    if(pview == NULL){
        return 0;
    }
    //use texture size
    cxSpriteSetTextureURL(pview, url, uts);
    return 0;
}

static cxInt cxEventSetView(lua_State *L)
{
    CX_LUA_DEF_THIS(cxView);
    cxConstChars viewid = NULL;
    lua_getfield(L, 2, "view");
    if(lua_isstring(L, -1)){
        viewid = lua_tostring(L, -1);
    }
    lua_pop(L, 1);
    cxView pview = cxViewRootGet(this, viewid);
    CX_RETURN(pview == NULL, 0);
    
    lua_getfield(L, 2, "scale");
    if(lua_isstring(L, -1)){
        cxVec2f vscale = pview->scale;
        cxReadFloats(lua_tostring(L, -1), &vscale.x);
        cxViewSetScale(pview, vscale);
    }
    lua_pop(L, 1);
    
    lua_getfield(L, 2, "size");
    if(lua_isstring(L, -1)){
        cxSize2f vsize = pview->size;
        cxReadFloats(lua_tostring(L, -1), &vsize.w);
        cxViewSetSize(pview, vsize);
    }
    lua_pop(L, 1);
    
    lua_getfield(L, 2, "position");
    if(lua_isstring(L, -1)){
        cxVec2f vposition = pview->position;
        cxReadFloats(lua_tostring(L, -1), &vposition.x);
        cxViewSetPos(pview, vposition);
    }
    lua_pop(L, 1);

    lua_getfield(L, 2, "degress");
    if(lua_isnumber(L, -1)){
        cxViewSetDegrees(pview, lua_tonumber(L, -1));
    }
    lua_pop(L, 1);
    
    lua_getfield(L, 2, "anchor");
    if(lua_isstring(L, -1)){
        cxVec2f vanchor = pview->anchor;
        cxReadFloats(lua_tostring(L, -1), &vanchor.x);
        cxViewSetAnchor(pview, vanchor);
    }
    lua_pop(L, 1);
    return 0;
}

static cxInt cxDataString(lua_State *L)
{
    cxConstChars url = luaL_checkstring(L, 2);
    cxTypes types = cxEngineTypes(url);
    if(types == NULL || !cxObjectIsType(types->any, cxStringTypeName)){
        lua_pushnil(L);
    }else{
        CX_LUA_PUSH_OBJECT(types->any);
    }
    return 1;
}

static cxInt cxRelativeW(lua_State *L)
{
    cxEngine engine = cxEngineInstance();
    cxFloat v = luaL_checknumber(L, 2);
    cxNumber num = cxNumberFloat(v * engine->winsize.w);
    CX_LUA_PUSH_OBJECT(num);
    return 1;
}

static cxInt cxRelativeH(lua_State *L)
{
    cxEngine engine = cxEngineInstance();
    cxFloat v = luaL_checknumber(L, 2);
    cxNumber num = cxNumberFloat(v * engine->winsize.h);
    CX_LUA_PUSH_OBJECT(num);
    return 1;
}

static cxInt cxBinNumber(lua_State *L)
{
    cxConstChars str = luaL_checkstring(L, 2);
    cxUInt v = cxBinaryToUInt(str);
    cxNumber num = cxNumberUInt(v);
    CX_LUA_PUSH_OBJECT(num);
    return 1;
}

static cxInt cxHexNumber(lua_State *L)
{
    cxConstChars str = luaL_checkstring(L, 2);
    cxUInt v = cxHexToUInt(str);
    cxNumber num = cxNumberUInt(v);
    CX_LUA_PUSH_OBJECT(num);
    return 1;
}

static cxInt cxLoadTexture(lua_State *L)
{
    cxTextureAttr rv = CX_CREATE(cxTextureAttr);
    cxConstChars str = luaL_checkstring(L, 2);
    cxUrlPath path = cxUrlPathParse(str);
    if(path->count == 0){
        lua_pushnil(L);
        return 1;
    }
    cxTexture texture = NULL;
    if(path->count > 0){
        texture = cxTextureFactoryLoadFile(path->path);
        rv->size = texture->size;
        CX_RETAIN_SWAP(rv->texture, texture);
    }
    if(path->count > 1 && texture != NULL){
        rv->box = cxTextureBox(texture, path->key);
        rv->size = cxTextureSize(texture, path->key);
    }
    CX_LUA_PUSH_OBJECT(rv);
    return 1;
}

static cxInt cxFixScaleW(lua_State *L)
{
    cxEngine engine = cxEngineInstance();
    cxNumber num = cxNumberVec2f(cxVec2fv(engine->scale.x, engine->scale.x));
    CX_LUA_PUSH_OBJECT(num);
    return 1;
}

static cxInt cxFixScaleH(lua_State *L)
{
    cxEngine engine = cxEngineInstance();
    cxNumber num = cxNumberVec2f(cxVec2fv(engine->scale.y, engine->scale.y));
    CX_LUA_PUSH_OBJECT(num);
    return 1;
}
static cxInt cxDataTypes(lua_State *L)
{
    cxConstChars url = luaL_checkstring(L, 2);
    cxTypes types = cxEngineTypes(url);
    CX_LUA_PUSH_OBJECT(types);
    return 1;
}

static cxInt cxIsIOS(lua_State *L)
{
    cxNumber num = cxNumberBool(CX_TARGET_PLATFORM == CX_PLATFORM_IOS);
    CX_LUA_PUSH_OBJECT(num);
    return 1;
}

static cxInt cxIsAndroid(lua_State *L)
{
    cxNumber num = cxNumberBool(CX_TARGET_PLATFORM == CX_PLATFORM_ANDROID);
    CX_LUA_PUSH_OBJECT(num);
    return 1;
}

static cxInt cxViewMulripleW(lua_State *L)
{
    CX_LUA_DEF_THIS(cxReaderAttrInfo *);
    cxConstChars v = NULL;
    cxFloat n = 0;
    if(!lua_istable(L, 2)){
        luaL_error(L, "args error");
        return 0;
    }
    lua_getfield(L, 2, "v");
    v = luaL_checkstring(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 2, "n");
    n = luaL_checknumber(L, -1);
    lua_pop(L, 1);
    cxView view = cxViewRootGet(this->root, v);
    CX_ASSERT(view != NULL, "view is null");
    cxNumber num = cxNumberFloat(view->size.w * n);
    CX_LUA_PUSH_OBJECT(num);
    return 1;
}

static cxInt cxViewMulripleH(lua_State *L)
{
    CX_LUA_DEF_THIS(cxReaderAttrInfo *);
    cxConstChars v = NULL;
    cxFloat n = 0;
    if(!lua_istable(L, 2)){
        luaL_error(L, "args error");
        return 0;
    }
    lua_getfield(L, 2, "v");
    v = luaL_checkstring(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 2, "n");
    n = luaL_checknumber(L, -1);
    lua_pop(L, 1);
    
    cxView view = cxViewRootGet(this->root, v);
    CX_ASSERT(view != NULL, "view is null");
    cxNumber num = cxNumberFloat(view->size.h * n);
    CX_LUA_PUSH_OBJECT(num);
    return 1;
}

static cxInt cxSpriteTexture(lua_State *L)
{
    CX_LUA_DEF_THIS(cxReaderAttrInfo *);
    cxConstChars v = NULL;
    cxConstChars k = NULL;
    if(!lua_istable(L, 2)){
        luaL_error(L, "args error");
        return 0;
    }
    lua_getfield(L, 2, "v");
    v = luaL_checkstring(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, 2, "k");
    k = luaL_checkstring(L, -1);
    lua_pop(L, 1);
    if(v == NULL){
        luaL_error(L, "v args error");
        return 0;
    }
    cxTextureAttr attr = CX_CREATE(cxTextureAttr);
    cxSprite sprite = cxViewRootGet(this->root, v);
    CX_ASSERT(sprite != NULL, "sprite is null");
    CX_RETAIN_SWAP(attr->texture, sprite->texture);
    if(k != NULL){
        attr->box = cxTextureBox(sprite->texture, k);
        attr->size = cxTextureSize(sprite->texture, k);
    }
    CX_LUA_PUSH_OBJECT(attr);
    return 1;
}

static cxInt cxExit(lua_State *L)
{
    cxEngineExit();
    return 0;
}

void cxEngineSystemInit(cxEngine engine)
{
    CX_ENGINE_APP_CONFIG(desSize){
        cxNumber num = desSize->any;
        engine->dessize = cxNumberToSize2f(num);
    }
    CX_ENGINE_APP_CONFIG(showBorder){
        cxNumber num = showBorder->any;
        engine->isShowBorder = cxNumberToBool(num);
    }
    //global func cxReaderAttrInfo *,args...
    cxEngineRegisteFunc(cxSpriteTexture);
    cxEngineRegisteFunc(cxViewMulripleW);
    cxEngineRegisteFunc(cxViewMulripleH);
    cxEngineRegisteFunc(cxIsAndroid);
    cxEngineRegisteFunc(cxIsIOS);
    cxEngineRegisteFunc(cxLocalizedText);
    cxEngineRegisteFunc(cxDataString);
    cxEngineRegisteFunc(cxRelativeW);
    cxEngineRegisteFunc(cxRelativeH);
    cxEngineRegisteFunc(cxBinNumber);
    cxEngineRegisteFunc(cxHexNumber);
    cxEngineRegisteFunc(cxLoadTexture);
    cxEngineRegisteFunc(cxFixScaleW);
    cxEngineRegisteFunc(cxFixScaleH);
    cxEngineRegisteFunc(cxDataTypes);
    //global event this,args...
    cxEngineRegisteFunc(cxEventAction);
    cxEngineRegisteFunc(cxEventEffect);
    cxEngineRegisteFunc(cxEventMusic);
    cxEngineRegisteFunc(cxEventLogger);
    cxEngineRegisteFunc(cxEventReplaceView);
    cxEngineRegisteFunc(cxEventPushView);
    cxEngineRegisteFunc(cxEventPopView);
    cxEngineRegisteFunc(cxEventMessagePost);
    cxEngineRegisteFunc(cxEventSetTexture);
    cxEngineRegisteFunc(cxEventSetView);
    cxEngineRegisteFunc(cxExit);
    //init all type
    cxInitTypes();
}










