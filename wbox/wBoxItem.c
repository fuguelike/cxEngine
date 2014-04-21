//
//  wBoxItem.c
//  wbox
//
//  Created by xuhua on 4/20/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "wBoxItem.h"
#include "wBoxMain.h"

CX_OBJECT_INIT(wBoxItem, cxSprite)
{
    
}
CX_OBJECT_FREE(wBoxItem, cxSprite)
{

}
CX_OBJECT_TERM(wBoxItem, cxSprite)

void wBoxItemSetColor(cxAny ite,wUnitColor color)
{
    
}

wBoxItem wBoxItemCreate(cxAny box,cxVec2i idx)
{
    wBoxMain m = box;
    wBoxItem this = CX_CREATE(wBoxItem);
    this->box = box;
    cxViewSetSize(this, m->unitSize);
    cxVec2f pos = wBoxMainToPos(box, idx);
    cxViewSetPos(this, pos);
    cxSpriteSetTextureURL(this, "texture.xml?blue.png", false);
    return this;
}