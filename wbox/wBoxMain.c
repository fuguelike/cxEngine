//
//  wMainBox.c
//  wbox
//
//  Created by xuhua on 4/20/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <core/cxEngine.h>
#include <views/cxSprite.h>
#include "wBoxMain.h"


CX_OBJECT_INIT(wBoxMain, cxView)
{
    cxEngine engine = cxEngineInstance();
    cxFloat h = engine->winsize.h / 23;
    cxFloat w = h;
    this->unitSize = cxSize2fv(w, h);
    cxViewSetSize(this, cxSize2fv(w * W_BOX_COL, h * W_BOX_ROW));
    cxViewSetAutoResizeMask(this, cxViewAutoResizeBottom);
    
//    //test
//    for(int i=0;i < 10; i++){
//        for (int j=0; j < 20; j++) {
//            cxSprite s = cxSpriteCreateWithURL("texture.xml?blue.png");
//            cxVec2f pos = wBoxMainToPos(this, cxVec2iv(i, j));
//            cxViewSetPos(s, pos);
//            cxViewSetSize(s, this->unitSize);
//            cxViewAppend(this, s);
//        }
//    }
    
    wUnit u;
    wUnitInit(&u, wUnitType_T);
    for(int i=0; i < 4; i++){
        for (int j=0; j < 2; j++) {
            cxSprite s;
            if(u.grids[i][j] > 0){
                s = cxSpriteCreateWithURL("texture.xml?blue.png");
            }else{
                s = cxSpriteCreateWithURL("texture.xml?white.png");
            }
            cxVec2f pos = wBoxMainToPos(this, cxVec2iv(i, j));
            cxViewSetPos(s, pos);
            cxViewSetSize(s, this->unitSize);
            cxViewAppend(this, s);
        }
    }
}
CX_OBJECT_FREE(wBoxMain, cxView)
{
    
}
CX_OBJECT_TERM(wBoxMain, cxView)

cxVec2i wBoxMainToIdx(cxAny box,cxVec2f pos)
{
    wBoxMain this = box;
    cxSize2f size = cxViewSize(this);
    cxFloat x = pos.x + size.w / 2.0f;
    cxFloat y = pos.y + size.h / 2.0f;
    return cxVec2iv(x/this->unitSize.w, y/this->unitSize.h);
}

cxVec2f wBoxMainToPos(cxAny box,cxVec2i idx)
{
    wBoxMain this = box;
    cxSize2f size = cxViewSize(this);
    cxFloat x = idx.x * this->unitSize.w - size.w / 2.0f;
    cxFloat y = idx.y * this->unitSize.h - size.h / 2.0f;
    return cxVec2fv(x + this->unitSize.w/2.0f, y + this->unitSize.h/2.0f);
}