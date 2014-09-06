//
//  cxAtlas.h
//  cxEngine
//
//  Created by xuhua on 10/17/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxAtlas_h
#define cxEngine_cxAtlas_h

#include "cxSprite.h"

CX_C_BEGIN

typedef struct {
    cxBool  enable;
    cxBox4f box;
}cxAtlasScale9;

CX_OBJECT_DEF(cxAtlas, cxSprite)
    GLuint vaoid;
    GLuint vboid[2];
    cxInt capacity;
    cxInt number;
    cxBoxPoint *boxes;
    GLushort *indices;
    cxBool isDirty;
    cxBool isInit;
    cxAtlasScale9 scale9;
    cxHash items;
CX_OBJECT_END(cxAtlas, cxSprite)

void cxAtlasSetNumber(cxAny pview,cxInt number);

void cxAtlasDraw(cxAny pview);

void cxAtlasSetDirty(cxAny pview,cxBool v);

void cxAtlasDrawInit(cxAny pview);

void cxAtlasResize(cxAny sender);

void cxAtlasAppendBoxPoint(cxAny pview,cxVec2f pos,cxSize2f size,cxBoxTex2f tex,cxColor4f color);

cxBoxPoint cxAtlasCreateBoxPoint(cxVec2f pos,cxSize2f size,cxBoxTex2f tex,cxColor4f color);

void cxAtlasSetCapacity(cxAny pview,cxInt capacity);

void cxAtlasSetScale9(cxAny pview,cxBox4f box);

void cxAtlasUpdateScale9(cxAny pview);

void cxAtlasClean(cxAny pview);

cxBoxPoint *cxAtlasAppend(cxAny pview,cxBoxPoint *point);

//point cxAtlasAppend return
cxInt cxAtlasIndex(cxAny pview,cxBoxPoint *point);

void cxAtlasRemovePoint(cxAny pview,cxBoxPoint *point);

void cxAtlasRemoveAt(cxAny pview,cxInt index);

void cxAtlasUpdateAt(cxAny pview,cxInt index, cxBoxPoint *point);

CX_C_END

#endif
