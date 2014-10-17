//
//  WarMap.c
//  cxCore
//
//  Created by xuhua on 10/2/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <chipmunk/chipmunk.h>
#include <engine/cxEngine.h>
#include <views/cxSprite.h>
#include <views/cxLabelTTF.h>
#include "Define.h"
#include "WarMap.h"

CX_TYPE(WarMapUnit, cxSprite)
{
    
}
CX_INIT(WarMapUnit, cxSprite)
{
    
}
CX_FREE(WarMapUnit, cxSprite)
{
    
}
CX_TERM(WarMapUnit, cxSprite)

//加载位置上的单元格
static void WarMapLoadUnit(WarMap this,cxVec2i idx)
{
    //load x,y item
    WarMapUnit item = CX_CREATE(WarMapUnit);
    cxSpriteSetTextureURL(item, "bg1.png");
    cxLabelTTF ttf = cxLabelTTFCreate(UTF8("%d,%d",idx.x,idx.y), NULL, 40);
    cxViewAppend(item, ttf);
    WarMapAppendUnit(this, idx, item);
    CX_METHOD_RUN(item->onAppend,item);
}
//移除单元格子
static void WarMapUnLoadUnit(WarMap this,WarMapUnit item)
{
    CX_METHOD_RUN(item->onRemoved,item);
    WarMapRemoveUnit(item);
}

static void WarMapOnDirty(cxAny sender)
{
    CX_ASSERT_THIS(sender, WarMap);
    //获取屏幕中心点对应的map坐标
    cxHitInfo hit = cxViewHitTest(this, cxVec2fv(0, 0));
    if(!hit.hited){
        return;
    }
    //减小检测压力
    cxVec2i cidx = WarMapUnitIndex(this, hit.position);
    if(cxVec2iEqu(cidx, this->centerIdx)){
        return;
    }
    //获取中心格子坐标
    this->centerIdx = cidx;
    //加载中心点附近的格子
    cxInt sx = this->centerIdx.x - global.warShowNum.x;
    sx = CX_MAX(sx, 0);
    cxInt sy = this->centerIdx.y - global.warShowNum.y;
    sy = CX_MAX(sy, 0);
    cxInt cx = this->centerIdx.x + global.warShowNum.x;
    cx = CX_MIN(cx, global.warUnitNum.x);
    cxInt cy = this->centerIdx.y + global.warShowNum.y;
    cy = CX_MIN(cy, global.warUnitNum.y);
    for(cxInt x=sx;x < cx;x++)
    for(cxInt y=sy;y < cy;y++){
        cxVec2i idx = cxVec2iv(x, y);
        cxAny item = WarMapItem(this, idx);
        if(item != NULL){
            continue;
        }
        WarMapLoadUnit(this,idx);
    }
    //移除太远的item
    CX_HASH_FOREACH(this->items, ele, tmp){
        WarMapUnit item = ele->any;
        cxInt ax = abs(item->Index.x - this->centerIdx.x);
        cxInt ay = abs(item->Index.y - this->centerIdx.y);
        if(ax > global.warShowNum.x || ay > global.warShowNum.y){
            WarMapUnLoadUnit(this,item);
        }
    }
}

static cpBB UnitIndexBB(cxAny pview)
{
    CX_ASSERT_THIS(pview, WarMapUnit);
    cxVec2f pos = cxViewGetPosition(this);
    cxSize2f size = cxViewGetSize(this);
    cxFloat w = size.w / 2.0f;
    cxFloat h = size.h / 2.0f;
    return cpBBNew(pos.x - w, pos.y - h, pos.x + w, pos.y + h);
}

static void WarMapUpdate(cxAny sender)
{
    
}

CX_TYPE(WarMap, cxView)
{
    
}
CX_INIT(WarMap, cxView)
{
    CX_ADD(cxView, this, onUpdate, WarMapUpdate);
    CX_ADD(cxView, this, onDirty, WarMapOnDirty);
    this->tribes = cxSpatialAlloc(UnitIndexBB);
    this->items = CX_ALLOC(cxHash);
    this->centerIdx = cxVec2iv(-1, -1);
    cxViewSetSize(this, global.warMapSize);
}
CX_FREE(WarMap, cxView)
{
    CX_RELEASE(this->items);
    CX_RELEASE(this->tribes);
}
CX_TERM(WarMap, cxView)

cxAny WarMapItem(cxAny pmap,cxVec2i idx)
{
    CX_ASSERT_THIS(pmap, WarMap);
    cxInt index = WarMapItemKey(idx);
    cxHashKey key = cxHashIntKey(index);
    return cxHashGet(this->items, key);
}

void WarMapRemoveUnit(cxAny punit)
{
    CX_ASSERT_THIS(punit, WarMapUnit);
    WarMap map = WarMapUnitGetMap(this);
    cxInt index = WarMapItemKey(this->Index);
    cxHashKey key = cxHashIntKey(index);
    cxHashDel(map->items, key);
    cxViewRemove(this);
}

void WarMapAppendUnit(cxAny pmap,cxVec2i idx,cxAny punit)
{
    CX_ASSERT_THIS(pmap, WarMap);
    CX_ASSERT_VALUE(punit, WarMapUnit, unit);
    WarMapUnitSetIndex(unit, idx);
    WarMapUnitSetMap(unit, pmap);
    cxVec2f pos = WarMapUnitPosition(this, idx);
    cxViewSetPosition(unit, pos);
    cxViewSetSize(unit, global.warUnitSize);
    cxViewAppend(this, unit);
    cxInt index = WarMapItemKey(idx);
    cxHashKey key = cxHashIntKey(index);
    cxHashSet(this->items, key, unit);
}

cxVec2f WarMapUnitPosition(cxAny pmap,cxVec2i idx)
{
    CX_ASSERT_THIS(pmap, WarMap);
    cxSize2f size = cxViewGetSize(this);
    cxVec2f pos = cxVec2fv(global.warUnitSize.w * idx.x, global.warUnitSize.h * idx.y);
    pos.x -= (size.w - global.warUnitSize.w)/2.0f;
    pos.y -= (size.h - global.warUnitSize.h)/2.0f;
    return pos;
}

cxVec2i WarMapUnitIndex(cxAny pmap,cxVec2f pos)
{
    CX_ASSERT_THIS(pmap, WarMap);
    cxSize2f size = cxViewGetSize(this);
    pos.x += (size.w - global.warUnitSize.w)/2.0f;
    pos.y += (size.h - global.warUnitSize.h)/2.0f;
    return cxVec2iv(pos.x / global.warUnitSize.w, pos.y / global.warUnitSize.h);
}

CX_TYPE(WarScene, cxScroll)
{
    
}
CX_INIT(WarScene, cxScroll)
{

}
CX_FREE(WarScene, cxScroll)
{
    
}
CX_TERM(WarScene, cxScroll)

WarScene WarSceneCreate()
{
    cxLoader loader = cxLoaderCreate("war.json");
    CX_ASSERT_VALUE(cxLoaderGetRoot(loader), WarScene, scene);
    scene->warMap = cxLoaderGet(loader, "map");
    CX_ASSERT_TYPE(scene->warMap, WarMap);
    return scene;
}

