//
//  cxSprites.c
//  cxCore
//
//  Created by xuhua on 10/5/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxSprites.h"

static void cxSpriteUpdateBox(cxSprites this,cxSprite sp)
{
    cxInt idx = cxViewGetAssist(sp);
    cxBoxPoint *box = cxAtlasBoxPoint(this, idx);
    cxBoxTex2f tex = cxSpriteBoxTex(sp);
    cxColor4f color = cxViewGetColor(sp);
    cxVec3f pos;
    box->lb.colors = color;
    box->lb.texcoords = tex.lb;
    box->rb.colors = color;
    box->rb.texcoords = tex.rb;
    box->lt.colors = color;
    box->lt.texcoords = tex.lt;
    box->rt.colors = color;
    box->rt.texcoords = tex.rt;
    
    cxBox4f sbox = cxViewGetBox(sp);
    cxMatrix4f *normal = cxViewGetNormalMatrix(sp);
    cxMatrix4f *anchor = cxViewGetAnchorMatrix(sp);
    
    pos = cxVec3fv(sbox.l, sbox.b, 0);
    kmVec3MultiplyMat4(&pos, &pos, normal);
    kmVec3MultiplyMat4(&pos, &pos, anchor);
    box->lb.vertices = pos;
    
    pos = cxVec3fv(sbox.r, sbox.b, 0);
    kmVec3MultiplyMat4(&pos, &pos, normal);
    kmVec3MultiplyMat4(&pos, &pos, anchor);
    box->rb.vertices = pos;
    
    pos = cxVec3fv(sbox.l, sbox.t, 0);
    kmVec3MultiplyMat4(&pos, &pos, normal);
    kmVec3MultiplyMat4(&pos, &pos, anchor);
    box->lt.vertices = pos;
    
    pos = cxVec3fv(sbox.r, sbox.t, 0);
    kmVec3MultiplyMat4(&pos, &pos, normal);
    kmVec3MultiplyMat4(&pos, &pos, anchor);
    box->rt.vertices = pos;
    
    cxAtlasSetDirty(this, true);
}

static void cxSpriteOnDirty(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxSprite);
    CX_ASSERT_VALUE(cxViewGetParentView(this), cxSprites, parent);
    cxSpriteUpdateBox(parent, this);
    cxAtlasSetDirty(parent, true);
}

static void cxSpritesOnAppend(cxAny pview,cxAny nview)
{
    CX_ASSERT_THIS(pview, cxSprites);
    CX_ASSERT_VALUE(nview, cxSprite, item);
    cxViewSetIsDraw(item, false);
    CX_ADD(cxView, item, onDirty, cxSpriteOnDirty);
    cxInt idx = cxArrayLength(this->sprites);
    cxViewSetAssist(item, idx);
    
    cxAtlasAppendEmpty(this);
    cxArrayAppend(this->sprites, item);
    
    cxSpriteUpdateBox(this, item);
}

static void cxSpritesOnRemove(cxAny pview,cxAny nview)
{
    CX_ASSERT_THIS(pview, cxSprites);
    CX_ASSERT_VALUE(nview, cxSprite, item);
    cxViewSetIsDraw(item, true);
    cxInt idx = cxViewGetAssist(item);
    cxSprite last = cxArrayLast(this->sprites);
    if(last != item){
        cxViewSetAssist(last, idx);
    }
    cxArrayFastRemove(this->sprites, idx);
    cxAtlasFastRemove(this, idx);
}

CX_OBJECT_TYPE(cxSprites, cxAtlas)
{
    
}
CX_OBJECT_INIT(cxSprites, cxAtlas)
{
    CX_SET(cxView, this, onAppend, cxSpritesOnAppend);
    CX_SET(cxView, this, onRemove, cxSpritesOnRemove);
    this->sprites = CX_ALLOC(cxArray);
}
CX_OBJECT_FREE(cxSprites, cxAtlas)
{
    CX_RELEASE(this->sprites);
}
CX_OBJECT_TERM(cxSprites, cxAtlas)
