//
//  cxLayer.c
//  Engine
//
//  Created by xuhua on 9/6/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxLayer.h"

void cxLayerSpriteOnTransform(cxAny pview)
{
    cxLayer layer = cxViewParent(pview);
    cxSprite this  = pview;
    cxBoxTex2f coord = cxSpriteBoxTex(this);
    cxColor4f color = cxViewColor(this);
    CX_ASSERT(this->data != NULL, "data error");
    cxBoxPoint *bp = this->data;
    
    bp->lb.colors = color;
    bp->lt.colors = color;
    bp->rb.colors = color;
    bp->rt.colors = color;
    
    bp->lb.texcoords = coord.lb;
    bp->lt.texcoords = coord.lt;
    bp->rb.texcoords = coord.rb;
    bp->rt.texcoords = coord.rt;
    
    cxBox4f box = cxViewBox(this);
    cxVec3f pos;
    
    pos = cxVec3fv(box.l, box.b, 0);
    kmVec3MultiplyMat4(&pos, &pos, &this->cxView.normalMatrix);
    kmVec3MultiplyMat4(&pos, &pos, &this->cxView.anchorMatrix);
    bp->lb.vertices = pos;
    
    pos = cxVec3fv(box.l, box.t, 0);
    kmVec3MultiplyMat4(&pos, &pos, &this->cxView.normalMatrix);
    kmVec3MultiplyMat4(&pos, &pos, &this->cxView.anchorMatrix);
    bp->lt.vertices = pos;
    
    pos = cxVec3fv(box.r, box.b, 0);
    kmVec3MultiplyMat4(&pos, &pos, &this->cxView.normalMatrix);
    kmVec3MultiplyMat4(&pos, &pos, &this->cxView.anchorMatrix);
    bp->rb.vertices = pos;
    
    pos = cxVec3fv(box.r, box.t, 0);
    kmVec3MultiplyMat4(&pos, &pos, &this->cxView.normalMatrix);
    kmVec3MultiplyMat4(&pos, &pos, &this->cxView.anchorMatrix);
    bp->rt.vertices = pos;
    
    cxAtlasSetDirty(layer, true);
}

void cxLayerAppend(cxAny pview,cxAny subview)
{
    CX_ASSERT(pview != NULL && subview != NULL, "args null");
    CX_ASSERT(CX_INSTANCE_OF(subview, cxSprite), "subview only support cxSprite type");
    cxLayer this = pview;
    cxSprite sprite = subview;
    cxViewAppendImp(this, sprite);
    //atlas will draw sprite use VAO or VBO
    cxViewSetOnlyTransform(sprite, true);
    CX_EVENT_APPEND(sprite->cxView.onTransform, cxLayerSpriteOnTransform);
    cxVec2f pos = cxViewPosition(sprite);
    cxSize2f size = cxViewSize(sprite);
    cxBoxTex2f coord = cxSpriteBoxTex(sprite);
    cxColor4f color = cxViewColor(sprite);
    cxBoxPoint bp = cxAtlasCreateBoxPoint(pos, size, coord, color);
    sprite->data = cxAtlasAppend(this, &bp);
}

void cxLayerRemove(cxAny pview)
{
    cxSprite this  = pview;
    CX_ASSERT(this->data != NULL, "data null");
    cxBoxPoint *bp = this->data;
    cxLayer layer = cxViewParent(pview);
    cxAtlasRemovePoint(layer, bp);
    cxViewRemove(this);
}

CX_OBJECT_TYPE(cxLayer, cxAtlas)
{
    
}
CX_OBJECT_INIT(cxLayer, cxAtlas)
{
    CX_METHOD_SET(this->cxAtlas.cxSprite.cxView.Append,cxLayerAppend);
    CX_METHOD_SET(this->cxAtlas.cxSprite.cxView.Remove, cxLayerRemove);
}
CX_OBJECT_FREE(cxLayer, cxAtlas)
{

}
CX_OBJECT_TERM(cxLayer, cxAtlas)