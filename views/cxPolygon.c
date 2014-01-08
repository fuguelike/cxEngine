//
//  cxPolygon.c
//  cxEngineIOS
//
//  Created by xuhua on 1/8/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <core/cxEngine.h>
#include "cxPolygon.h"

static void cxPolygonReadAttr(cxReaderAttrInfo *info)
{
    cxSpriteReadAttr(info);
    cxConstChars points = cxXMLAttr(info->reader, "cxPolygon.points");
    CX_RETURN(points == NULL);
    cxTypes type = cxEngineDataSet(points);
    CX_ASSERT(cxTypesIsType(type, cxTypesArray), "must is array");
    cxArray list = type->any;
    CX_ARRAY_FOREACH(list, ele){
        cxNumber bp = cxArrayObject(ele);
        cxPolygonAppend(info->object, cxNumberToPoint(bp));
    }
}

static void cxPolygonDraw(cxAny pview)
{
    cxPolygon this = pview;
    cxSpriteBindTexture(pview);
    cxOpenGLActiveAttribs(cxVertexAttribFlagPosColorTex);
    glVertexAttribPointer(cxVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(cxVec3f), this->points);
    glVertexAttribPointer(cxVertexAttribTexcoord, 2, GL_FLOAT, GL_FALSE, sizeof(cxTex2f), this->texs);
    glVertexAttribPointer(cxVertexAttribColor,    4, GL_FLOAT, GL_FALSE, sizeof(cxColor4f), this->colors);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, this->number);
}

CX_OBJECT_INIT(cxPolygon, cxSprite)
{
    cxObjectSetReadAttrFunc(this, cxPolygonReadAttr);
    CX_METHOD_OVERRIDE(this->super.super.Draw, cxPolygonDraw);
    this->capacity = 8;
    this->points = allocator->malloc(sizeof(cxVec3f) * this->capacity);
    this->colors = allocator->malloc(sizeof(cxColor4f) * this->capacity);
    this->texs = allocator->malloc(sizeof(cxTex2f) * this->capacity);
}
CX_OBJECT_FREE(cxPolygon, cxSprite)
{
    allocator->free(this->points);
    allocator->free(this->colors);
    allocator->free(this->texs);
}
CX_OBJECT_TERM(cxPolygon, cxSprite)

void cxPolygonAppend(cxAny pview,cxPoint point)
{
    cxPolygon this = pview;
    this->points[this->number] = point.vertices;
    this->colors[this->number] = point.colors;
    this->texs[this->number] = point.texcoords;
    this->number ++;
}

void cxPolygonResize(cxAny pview,cxInt add)
{
    cxPolygon this = pview;
    this->capacity = this->capacity + add;
    this->points = allocator->realloc(this->points,sizeof(cxVec3f) * this->capacity);
    this->colors = allocator->realloc(this->points,sizeof(cxColor4f) * this->capacity);
    this->texs = allocator->realloc(this->points,sizeof(cxTex2f) * this->capacity);
}



