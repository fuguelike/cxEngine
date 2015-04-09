//
//  cxAtlas.h
//  cxEngine
//
//  Created by xuhua on 10/17/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxAtlas_h
#define cxEngine_cxAtlas_h

#include <engine/cxFrames.h>
#include "cxSprite.h"

CX_C_BEGIN

typedef struct {
    cxBool  enable; //enable scale9
    cxBox4f box;    //box
}cxAtlasScale9;

CX_DEF(cxAtlas, cxSprite)
    GLuint vaoid;
    GLuint vboid[2];
    cxInt capacity;
    cxInt number;
    cxBoxPoint *boxes;
    GLushort *indices;
    cxBool border;
    cxBool isInit;
    cxAtlasScale9 scale9;
    cxBool isDirty;
    cxFrames frames;
    CX_FIELD_DEF(cxInt FrameIdx);
CX_END(cxAtlas, cxSprite)

CX_INLINE cxBox4f cxAtlasGetScale9Box(cxAny pthis)
{
    CX_ASSERT_THIS(pthis, cxAtlas);
    return this->scale9.box;
}

CX_FIELD_SET(cxAtlas, cxInt, FrameIdx);

CX_INLINE cxBoxPoint *cxAtlasBoxPoint(cxAny pthis,cxInt idx)
{
    CX_ASSERT_THIS(pthis, cxAtlas);
    CX_ASSERT(idx >= 0 && idx < this->capacity, "idx error");
    return &this->boxes[idx];
}

CX_INLINE cxInt cxAtlasGetNumber(cxAny pthis)
{
    CX_ASSERT_THIS(pthis, cxAtlas);
    return this->number;
}

cxAny cxAtlasCreate(cxConstChars url);

CX_INLINE void cxAtlasShowBorder(cxAny pview,cxBool show)
{
    CX_ASSERT_THIS(pview, cxAtlas);
    this->border = show;
}

void cxAtlasSetNumber(cxAny pview,cxInt number);

void cxAtlasDrawInit(cxAny pview);

void cxAtlasAppendEmpty(cxAny pview);

void cxAtlasShowAt(cxAny pview,cxInt index,cxBool show);

void cxAtlasUpdateAlpha(cxAny pview,cxInt index,cxFloat alpha);

void cxAtlasUpdateTexture(cxAny pview,cxInt index,cxConstChars key);

void cxAtlasUpdateBoxTex(cxAny pview,cxInt index,cxBoxTex2f boxtex);

void cxAtlasUpdateColor(cxAny pview,cxInt index,cxColor4f color);

void cxAtlasUpdatePosSize(cxAny pview,cxInt index,cxVec2f pos,cxSize2f size);

cxInt cxAtlasAppendBoxPointWithKey(cxAny pview,cxVec2f pos,cxSize2f size,cxColor4f color,cxBool flipx,cxBool flipy,cxConstChars key);

cxInt cxAtlasAppendBoxPoint(cxAny pview,cxVec2f pos,cxSize2f size,cxBoxTex2f tex,cxColor4f color);

cxBoxPoint cxAtlasCreateBoxPoint(cxVec2f pos,cxSize2f size,cxBoxTex2f tex,cxColor4f color);

void cxTexCoordSetBoxPoint(cxAny pview,cxBoxPoint *bp,cxVec2f pos,cxSize2f size,cxColor4f color, cxBool flipx,cxBool flipy,cxTexCoord coord);

cxTexCoord cxAtlasSetBoxPoint(cxAny pview,cxBoxPoint *bp,cxVec2f pos,cxSize2f size,cxColor4f color, cxBool flipx,cxBool flipy,cxConstChars key);

cxInt cxAtlasAppendWithKey(cxAny pview,cxVec2f pos,cxSize2f size,cxColor4f color, cxBool flipx,cxBool flipy,cxConstChars key);

void cxAtlasSetCapacity(cxAny pview,cxInt capacity);

void cxAtlasSetScale9(cxAny pview,cxBox4f box);

void cxAtlasSetDirty(cxAny pview);

void cxAtlasUpdateScale9(cxAny pview);

void cxAtlasFastRemove(cxAny pview,cxInt index);

void cxAtlasClear(cxAny pview);

cxInt cxAtlasAppend(cxAny pview,cxBoxPoint *point);

void cxAtlasUpdateAt(cxAny pview,cxInt index, cxBoxPoint *point);

CX_C_END

#endif
