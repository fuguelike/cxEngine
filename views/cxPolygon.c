//
//  cxPolygon.c
//  cxEngineIOS
//
//  Created by xuhua on 1/8/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <core/cxEngine.h>
#include "cxPolygon.h"

static void cxPolygonDraw(cxAny pview)
{
    cxPolygon this = pview;
    CX_RETURN(this->number < 3);
    cxSpriteBindTexture(pview);
    cxOpenGLActiveAttribs(cxVertexAttribFlagPosColorTex);
    glVertexAttribPointer(cxVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(cxVec3f), this->points);
    glVertexAttribPointer(cxVertexAttribTexcoord, 2, GL_FLOAT, GL_FALSE, sizeof(cxTex2f), this->texs);
    glVertexAttribPointer(cxVertexAttribColor,    4, GL_FLOAT, GL_FALSE, sizeof(cxColor4f), this->colors);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, this->number);
}

static void cxPolygonMalloc(cxPolygon this)
{
    this->points = allocator->realloc(this->points,sizeof(cxVec3f) * this->capacity);
    this->colors = allocator->realloc(this->colors,sizeof(cxColor4f) * this->capacity);
    this->texs = allocator->realloc(this->texs,sizeof(cxTex2f) * this->capacity);
}

void __cxPolygonInitObject(cxAny object,cxAny json,cxAny hash)
{
    cxJson points = cxJsonArray(json, "points");
    CX_JSON_ARRAY_EACH_BEG(points, point);
    {
        cxPoint p = {0};
        p.colors = cxColor4fv(1, 1, 1, 1);
        p.texcoords = cxTex2fv(0, 0);
        p.vertices = cxVec3fv(0, 0, 0);
        p.vertices.x = cxJsonDouble(point, "p.x", p.vertices.x);
        p.vertices.y = cxJsonDouble(point, "p.y", p.vertices.y);
        p.texcoords.u = cxJsonDouble(point, "t.u", p.texcoords.u);
        p.texcoords.v = cxJsonDouble(point, "t.v", p.texcoords.v);
        p.colors.a = cxJsonDouble(point, "c.a", p.colors.a);
        p.colors.r = cxJsonDouble(point, "c.r", p.colors.r);
        p.colors.g = cxJsonDouble(point, "c.g", p.colors.g);
        p.colors.b = cxJsonDouble(point, "c.b", p.colors.b);
        cxPolygonAppend(object, p);
    }
    CX_JSON_ARRAY_EACH_END(points, point);
    CX_OBJECT_INIT_SUPER(cxSprite);
}

CX_OBJECT_INIT(cxPolygon, cxSprite)
{
    CX_OBJECT_INIT_OVERRIDE(cxPolygon);
    CX_METHOD_SET(this->super.super.Draw, cxPolygonDraw);
    this->capacity = 8;
    cxPolygonMalloc(this);
}
CX_OBJECT_FREE(cxPolygon, cxSprite)
{
    allocator->free(this->points);
    allocator->free(this->colors);
    allocator->free(this->texs);
}
CX_OBJECT_TERM(cxPolygon, cxSprite)

void cxPolygonClean(cxAny pview)
{
    cxPolygon this = pview;
    this->number = 0;
}

void cxPolygonAppend(cxAny pview,cxPoint point)
{
    cxPolygon this = pview;
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
    cxPolygon this = pview;
    this->capacity = this->capacity + add;
    cxPolygonMalloc(this);
}



