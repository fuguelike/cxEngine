//
//  cxLayer.c
//  Engine
//
//  Created by xuhua on 9/6/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxLayer.h"

CX_OBJECT_TYPE(cxElement, cxSprite)
{
    
}
CX_OBJECT_INIT(cxElement, cxSprite)
{
    
}
CX_OBJECT_FREE(cxElement, cxSprite)
{
    
}
CX_OBJECT_TERM(cxElement, cxSprite)

void cxElementUpdateBoxPoint(cxAny pview,cxBoxPoint *bp)
{
    CX_ASSERT_THIS(pview, cxElement);
    CX_ASSERT(bp != NULL, "args null");
    
    cxBoxTex2f coord = cxSpriteBoxTex(this);
    cxBoxColor4f color = cxSpriteBoxColor(this);
    
    cxMatrix4f *normalMatrix = cxViewNormalMatrix(this);
    cxMatrix4f *anchorMatrix = cxViewAnchorMatrix(this);
    
    bp->lb.colors = color.lb;
    bp->lt.colors = color.lt;
    bp->rb.colors = color.rb;
    bp->rt.colors = color.rt;
    
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

static void cxElementOnTransform(cxAny pview)
{
    cxLayer layer = cxViewParent(pview);
    cxElement this  = pview;
    CX_ASSERT(this->layerBox != NULL, "data error");
    cxElementUpdateBoxPoint(this, this->layerBox);
    cxAtlasSetDirty(layer, true);
}

static void cxLayerOnRemove(cxAny pview,cxAny oldview)
{
    cxLayer this  = pview;
    cxElement sprite = oldview;
    CX_ASSERT(sprite->layerBox != NULL, "data null");
    cxBoxPoint *bp = sprite->layerBox;
    cxAtlasRemovePoint(this, bp);
}

static void cxLayerOnAppend(cxAny pview,cxAny newview)
{
    CX_ASSERT(pview != NULL && newview != NULL, "args null");
    CX_ASSERT_TYPE(newview, cxElement);
    cxLayer this = pview;
    cxElement sprite = newview;
    //atlas will draw cxElement, use VAO or VBO
    cxViewSetOnlyTransform(sprite, true);
    CX_EVENT_APPEND(sprite->cxSprite.cxView.onTransform, cxElementOnTransform);
    cxBoxPoint bp = {0};
    cxElementUpdateBoxPoint(sprite, &bp);
    sprite->layerBox = cxAtlasAppend(this, &bp);
}

CX_OBJECT_TYPE(cxLayer, cxAtlas)
{
    
}
CX_OBJECT_INIT(cxLayer, cxAtlas)
{
    CX_METHOD_SET(CX_TYPE(cxView, this)->onRemove, cxLayerOnRemove);
    CX_METHOD_SET(CX_TYPE(cxView, this)->onAppend, cxLayerOnAppend);
}
CX_OBJECT_FREE(cxLayer, cxAtlas)
{

}
CX_OBJECT_TERM(cxLayer, cxAtlas)









