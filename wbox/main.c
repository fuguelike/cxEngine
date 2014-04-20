//
//  cxMain.c
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <core/cxEngine.h>
#include "wMainBox.h"

static cxAny wMakeView(cxConstChars temp, xmlTextReaderPtr reader)
{
    if(ELEMENT_IS_TYPE(wMainBox)){
        return CX_CREATE(wMainBox);
    }
    return NULL;
}

void cxEngineInit(cxEngine engine)
{
    CX_METHOD_OVERRIDE(engine->MakeView, wMakeView);
}

#include <views/cxSprite.h>

void cxEngineMain(cxEngine engine)
{
//    cxSize2f size = cxSize2fv(450, 800);
//    size.h = (size.w / 10) * 20;
//    cxFloat w = size.w / 10;
//    cxFloat h = w;
//    
//    cxView box = CX_CREATE(cxView);
//    cxViewSetSize(box, size);
////    cxViewSetAutoResizeBox(box, cxBox4fv(50, 0, 0, 0));
//    cxViewSetAutoResizeMask(box, cxViewAutoResizeLeft);
//    cxViewAppend(engine->window, box);
//
//    for(cxInt j=0; j < 20 ; j++){
//        for(cxInt i=0; i < 10; i++){
//            cxSprite s = cxSpriteCreateWithURL("blue.png");
//            cxViewSetSize(s, cxSize2fv(w, h));
//            cxFloat x = i * w - size.w / 2.0f + w /2.0f;
//            cxFloat y = j * h - size.h / 2.0f + h / 2.0f;
//            cxViewSetPos(s, cxVec2fv(x, y));
//            cxViewAppend(box, s);
//        }
//    }
    
    cxWindowPushXML("main.xml");
}

void cxEngineFree(cxEngine engine)
{
    
}
