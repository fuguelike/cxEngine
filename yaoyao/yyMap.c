//
//  yyMap.c
//  yaoyao
//
//  Created by xuhua on 3/15/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <textures/cxTextureFactory.h>
#include <core/cxEngine.h>
#include "yyMap.h"
#include "yySprite.h"

cxConstChars mapJson =
"{\"map\":["
"[0  ,0  ,0  ,256,0  ,0  ,0  ,256,0  ,0  ],"
"[0  ,0  ,0  ,256,0  ,0  ,0  ,256,0  ,0  ],"
"[256,256,256,256,0  ,0  ,0  ,256,0  ,0  ],"
"[0  ,0  ,0  ,256,0  ,0  ,0  ,256,0  ,0  ],"
"[0  ,0  ,0  ,256,0  ,0  ,0  ,256,0  ,0  ],"
"[0  ,0  ,0  ,256,256,256,256,256,256,256],"
"[0  ,0  ,0  ,256,0  ,0  ,0  ,0  ,0  ,256],"
"[256,256,256,256,0  ,0  ,0  ,0  ,0  ,256],"
"[0  ,0  ,0  ,256,0  ,0  ,0  ,0  ,0  ,256],"
"[0  ,0  ,0  ,256,256,0  ,0  ,0  ,0  ,256]"
"]}";

/*  坐标系
    9
    8
    7
    6
    5
    4
    3
    2
    1
 x  0 1 2 3 4 5 6 7 8 9
 */

cxVec2f yyMapGetPos(yyMap this,cxVec2i idx)
{
    cxSize2f mapSize = cxViewSize(this);
    cxFloat x = idx.x * this->itemSize.w - mapSize.w/2.0f + this->itemSize.w/2.0f;
    cxFloat y = idx.y * this->itemSize.h - mapSize.h/2.0f + this->itemSize.h/2.0f;
    return cxVec2fv(x, y);
}

cxVec2i yyMapGetIdx(yyMap this,cxVec2f pos)
{
    cxSize2f mapSize = cxViewSize(this);
    cxFloat x = pos.x + mapSize.w/2.0f;
    cxFloat y = pos.y + mapSize.h/2.0f;
    return cxVec2iv(x/this->itemSize.w, y/this->itemSize.h);
}

cxBool yyMapCanMove(yyMap this,cxVec2i idx)
{
    return this->items[idx.x][idx.y] > YY_MAP_MOVE_VALUE;
}

void yyMapSetBackground(yyMap this,cxVec2i idx,cxUInt value)
{
    this->items[idx.x][idx.y] = value;
    cxVec2f pos = yyMapGetPos(this, idx);
    cxBoxTex2f box = cxTextureBox(this->super.super.texture, yyMapCanMove(this,idx) ?"map-enable.png":"map-disable.png");
    cxAtlasAppendBoxPoint(this, pos, this->itemSize, box , cxColor4fv(1, 1, 1, 1));
}

cxBool yyCheckIdx(cxVec2i idx)
{
    return idx.x >= 0 && idx.x < YY_MAP_COL && idx.y >= 0 && idx.y < YY_MAP_ROW;
}

static yyAngle yyGetAngle(cxVec2i s,cxVec2i e)
{
    cxVec2i l = cxVec2iv(s.x - 1, s.y);
    if(cxVec2iEqu(l, e)){
        return yyAngleLeft;
    }
    cxVec2i r = cxVec2iv(s.x + 1, s.y);
    if(cxVec2iEqu(r, e)){
        return yyAngleRight;
    }
    cxVec2i u = cxVec2iv(s.x, s.y + 1);
    if(cxVec2iEqu(u, e)){
        return yyAngleUp;
    }
    cxVec2i d = cxVec2iv(s.x, s.y - 1);
    if(cxVec2iEqu(d, e)){
        return yyAngleDown;
    }
    return yyAngleNone;
}

static cxBool yyMapTouch(cxAny pview,cxTouch *touch)
{
    yyMap this = pview;
    cxVec2f vPoint = cxVec2fv(0, 0);
    if(!cxViewHitTest(pview, touch->current, &vPoint)){
        return false;
    }
    cxVec2i idx = yyMapGetIdx(this, vPoint);
    if(!yyCheckIdx(idx)){
        this->isTouchStart = false;
        return false;
    }
    if(touch->type == cxTouchTypeDown && yyMapCanMove(this, idx)){
        this->bIdx = idx;
        this->isTouchStart = true;
        return true;
    }
    if(touch->type == cxTouchTypeMove && this->isTouchStart && !cxVec2iEqu(this->bIdx, idx) && yyMapCanMove(this, idx)){
        this->eIdx = idx;
        yyAngle angle = yyGetAngle(this->bIdx, this->eIdx);
        yySprite sp = yySpriteCreate(this, 0, this->bIdx);
        yySpriteSetAngle(sp, angle);
        cxViewAppend(this, sp);
        this->isTouchStart = false;
        return true;
    }
    if(touch->type == cxTouchTypeUp){
        this->isTouchStart = false;
        return false;
    }
    return false;
}

cxBool yyMapLoad(yyMap this,cxInt mapIdx)
{
    //parse json map data
    this->mapJson = cxJsonCreate(cxStringConstChars(mapJson));
    CX_RETAIN(this->mapJson);
    
    //load map item
    json_t *map = json_object_get(CX_JSON_PTR(this->mapJson), "map");
    CX_ASSERT(map != NULL, "miss map field");
    cxInt y = 0;
    json_t *v1 = NULL;
    json_array_foreach(map, y, v1){
        cxInt x = 0;
        json_t *v2 = NULL;
        json_array_foreach(v1, x, v2){
            cxInt xv = x;
            cxInt yv = YY_MAP_COL - y - 1;
            cxUInt value = (cxUInt)json_integer_value(v2);
            yyMapSetBackground(this, cxVec2iv(xv, yv), value);
        }
    }
    //
    return true;
}

CX_OBJECT_INIT(yyMap, cxAtlas)
{
    CX_METHOD_OVERRIDE(CXT(cxView,this)->Touch, yyMapTouch);
    
    cxAtlasSetNumber(this, YY_MAP_ROW * YY_MAP_COL);
    cxTexture texture = cxTextureCreate("map.xml");
    CX_ASSERT(texture != NULL, "get map.xml texture falied");
    cxSpriteSetTexture(this, texture);
    
    cxSize2f size = cxEngineInstance()->winsize;
    cxFloat w = size.w - 10;
    cxFloat h = w;
    cxViewSetSize(this, cxSize2fv(w, h));
    this->itemSize = cxSize2fv(w/YY_MAP_ROW, h/YY_MAP_COL);
    
    yyMapLoad(this, 0);
}
CX_OBJECT_FREE(yyMap, cxAtlas)
{
    CX_RELEASE(this->mapJson);
}
CX_OBJECT_TERM(yyMap, cxAtlas)









