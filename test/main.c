//
//  cxMain.c
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <engine/cxEngine.h>

void cxEngineType(cxEngine engine)
{
    
}

void cxEngineInit(cxEngine engine)
{
    cxEngineSetDesignSize(cxSize2fv(640, 960));
    engine->isShowBorder = false;
}

#include <actions/cxAnimate.h>
cxAnimate x;

static cxBool touch(cxAny pview,cxTouchItems *fires,cxTouchItems *points)
{
    static cxBool f = false;
    cxTouchItem item = CX_TOUCH_ITEM(fires, 0);
    if(item->type != cxTouchTypeDown){
        cxLoader loader = cxLoaderCreate("cxSprite.json");
        cxViewAppend(pview, loader->object);
        cxViewSetPos(loader->object, item->position);
        
        cxView this = pview;
        CX_LOGGER("view count %d",cxListLength(this->subViews));
        return false;
    }
//    cxAnimateSetGroupName(x, f?"cry":"run");
    f =!f;
    return false;
}

void cxEngineMain(cxEngine engine)
{
    CX_METHOD_SET(engine->window->cxView.Touch, touch);
    cxLoader loader = cxLoaderCreate("cxSprite.json");
    cxWindowPushView(loader->object);
//    x = cxLoaderGet(loader, "action");
//    CX_ASSERT( x != NULL, "get x error");
}

void cxEngineFree(cxEngine engine)
{
    
}
