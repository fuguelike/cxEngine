//
//  cxLayer.c
//  Engine
//
//  Created by xuhua on 9/6/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxLayer.h"

CX_OBJECT_TYPE(cxLayerSprite, cxSprite)
{
    
}
CX_OBJECT_INIT(cxLayerSprite, cxSprite)
{
    
}
CX_OBJECT_FREE(cxLayerSprite, cxSprite)
{
    
}
CX_OBJECT_TERM(cxLayerSprite, cxSprite)

void cxLayerSpriteUpdateBoxPoint(cxAny pview,cxBoxPoint *bp)
{
    CX_ASSERT(pview != NULL && bp != NULL, "args null");
    cxLayerSprite this  = pview;
    
    cxBoxTex2f coord = cxSpriteBoxTex(this);
    cxColor4f color = cxViewColor(this);
    
    if(!this->cxSprite.cxView.isVisible){
        color.a = 0;
    }
    
    cxMatrix4f *normalMatrix = cxViewNormalMatrix(this);
    cxMatrix4f *anchorMatrix = cxViewAnchorMatrix(this);
    
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
    kmVec3MultiplyMat4(&pos, &pos, normalMatrix);
    kmVec3MultiplyMat4(&pos, &pos, anchorMatrix);
    bp->lb.vertices = pos;
    
    pos = cxVec3fv(box.l, box.t, 0);
    kmVec3MultiplyMat4(&pos, &pos, normalMatrix);
    kmVec3MultiplyMat4(&pos, &pos, anchorMatrix);
    bp->lt.vertices = pos;
    
    pos = cxVec3fv(box.r, box.b, 0);
    kmVec3MultiplyMat4(&pos, &pos, normalMatrix);
    kmVec3MultiplyMat4(&pos, &pos, anchorMatrix);
    bp->rb.vertices = pos;
    
    pos = cxVec3fv(box.r, box.t, 0);
    kmVec3MultiplyMat4(&pos, &pos, normalMatrix);
    kmVec3MultiplyMat4(&pos, &pos, anchorMatrix);
    bp->rt.vertices = pos;
}

static void cxLayerSpriteOnTransform(cxAny pview)
{
    cxLayer layer = cxViewParent(pview);
    cxLayerSprite this  = pview;
    CX_ASSERT(this->layerBox != NULL, "data error");
    cxLayerSpriteUpdateBoxPoint(this, this->layerBox);
    cxAtlasSetDirty(layer, true);
}

static void cxLayerOnRemove(cxAny pview,cxAny oldview)
{
    cxLayer this  = pview;
    cxLayerSprite sprite = oldview;
    CX_ASSERT(sprite->layerBox != NULL, "data null");
    cxBoxPoint *bp = sprite->layerBox;
    cxAtlasRemovePoint(this, bp);
}

static void cxLayerOnAppend(cxAny pview,cxAny newview)
{
    CX_ASSERT(pview != NULL && newview != NULL, "args null");
    CX_ASSERT(CX_INSTANCE_OF(newview, cxLayerSprite), "subview only support base cxLayerSprite type");
    cxLayer this = pview;
    cxLayerSprite sprite = newview;
    //atlas will draw cxLayerSprite, use VAO or VBO
    cxViewSetOnlyTransform(sprite, true);
    CX_EVENT_APPEND(sprite->cxSprite.cxView.onTransform, cxLayerSpriteOnTransform);
    cxBoxPoint bp = {0};
    cxLayerSpriteUpdateBoxPoint(sprite, &bp);
    sprite->layerBox = cxAtlasAppend(this, &bp);
}


CX_OBJECT_TYPE(cxLayer, cxAtlas)
{
    
}
CX_OBJECT_INIT(cxLayer, cxAtlas)
{
    CX_METHOD_SET(this->cxAtlas.cxSprite.cxView.onRemove, cxLayerOnRemove);
    CX_METHOD_SET(this->cxAtlas.cxSprite.cxView.onAppend, cxLayerOnAppend);
}
CX_OBJECT_FREE(cxLayer, cxAtlas)
{

}
CX_OBJECT_TERM(cxLayer, cxAtlas)









