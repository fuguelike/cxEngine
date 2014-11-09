//
//  cxPolygon.c
//  cxEngineIOS
//
//  Created by xuhua on 1/8/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#include "cxPolygon.h"

static void cxPolygonMalloc(cxPolygon this)
{
    this->points = allocator->realloc(this->points,sizeof(cxVec3f) * this->capacity);
    this->colors = allocator->realloc(this->colors,sizeof(cxColor4f) * this->capacity);
    this->texs = allocator->realloc(this->texs,sizeof(cxTex2f) * this->capacity);
}

CX_SETTER_DEF(cxPolygon, points)
{
    cxJson points = cxJsonToArray(value);
    CX_JSON_ARRAY_EACH_BEG(points, point);
    cxPoint p = {0};
    p.colors = cxViewGetColor(this);
    p.texcoords = cxTex2fv(0, 0);
    p.vertices = cxVec3fv(0, 0, 0);
    p.vertices = cxJsonVec3f(point, "pos", p.vertices);
    p.texcoords = cxJsonTex2f(point, "coord", p.texcoords);
    p.colors = cxJsonColor4f(point, "color", p.colors);
    cxPolygonAppend(this, p);
    CX_JSON_ARRAY_EACH_END(points, point);
}

CX_METHOD_DEF(cxPolygon,Draw,void)
{
    CX_RETURN(this->number < 3);
    cxSpriteBindTexture(this);
    cxOpenGLActiveAttribs(cxVertexAttribFlagPosColorTex);
    cxOpenGLVertexAttribPointer(cxVertexAttribPosition, 3, sizeof(cxVec3f), this->points);
    cxOpenGLVertexAttribPointer(cxVertexAttribTexcoord, 2, sizeof(cxTex2f), this->texs);
    cxOpenGLVertexAttribPointer(cxVertexAttribColor,    4, sizeof(cxColor4f), this->colors);
    cxOpenGLDrawArrays(GL_TRIANGLE_STRIP, 0, this->number);
}

CX_TYPE(cxPolygon, cxSprite)
{
    CX_SETTER(cxPolygon, points);
    CX_METHOD(cxPolygon, Draw);
}
CX_INIT(cxPolygon, cxSprite)
{
    this->capacity = 8;
    cxPolygonMalloc(this);
}
CX_FREE(cxPolygon, cxSprite)
{
    allocator->free(this->points);
    allocator->free(this->colors);
    allocator->free(this->texs);
}
CX_TERM(cxPolygon, cxSprite)

void cxPolygonClear(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxPolygon);
    this->number = 0;
}

void cxPolygonAppend(cxAny pview,cxPoint point)
{
    CX_ASSERT_THIS(pview, cxPolygon);
    if((this->capacity - 1) <= this->number){
        cxPolygonResize(pview, 8);
    }
    this->points[this->number] = point.vertices;
    this->colors[this->number] = point.colors;
    this->texs[this->number] = point.texcoords;
    this->number ++;
}

void cxPolygonResize(cxAny pview,cxInt add)
{
    CX_ASSERT_THIS(pview, cxPolygon);
    this->capacity = this->capacity + add;
    cxPolygonMalloc(this);
}



