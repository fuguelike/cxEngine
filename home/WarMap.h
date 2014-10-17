//
//  WarMap.h
//  cxCore
//
//  Created by xuhua on 10/2/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef Home_WarMap_h
#define Home_WarMap_h

#include <engine/cxView.h>
#include <engine/cxSpatial.h>
#include <views/cxSprite.h>
#include <views/cxScroll.h>

CX_C_BEGIN

//一维 -》二维
CX_INLINE cxVec2i WarMapItemIndex(cxInt index)
{
    cxVec2i ret;
    ret.x = index % global.warUnitNum.x;
    ret.y = index / global.warUnitNum.x;
    return ret;
}
//二维 -》一维
CX_INLINE cxInt WarMapItemKey(cxVec2i idx)
{
    return idx.y * global.warUnitNum.x + idx.x;
}

CX_DEF(WarMapUnit, cxSprite)
    CX_FIELD_DEF(cxAny Map);
    CX_FIELD_DEF(cxVec2i Index);
    CX_METHOD_DEF(void, onAppend,cxAny);    //当加入地图后
    CX_METHOD_DEF(void, onRemoved,cxAny);   //在移除之前触发
CX_END(WarMapUnit, cxSprite)

CX_FIELD_IMP(WarMapUnit, cxAny, Map);
CX_FIELD_IMP(WarMapUnit, cxVec2i, Index);

CX_DEF(WarMap, cxView)
    cxVec2i centerIdx;          //屏幕中心对应的map格子坐标
    cxInt currIndex;        //检测单元计数,每次Update检测一个单元
    cxSpatial tribes;       //部落，资源等
    cxHash items;
CX_END(WarMap, cxView)

cxAny WarMapItem(cxAny pmap,cxVec2i idx);

//移除块单元
void WarMapRemoveUnit(cxAny punit);
//添加块单元
void WarMapAppendUnit(cxAny pmap,cxVec2i idx,cxAny punit);

//获取一块的显示位置
cxVec2f WarMapUnitPosition(cxAny pmap,cxVec2i idx);

//从屏幕坐标获取格子坐标
cxVec2i WarMapUnitIndex(cxAny pmap,cxVec2f pos);

CX_DEF(WarScene, cxScroll)
    WarMap warMap;
CX_END(WarScene, cxScroll)

WarScene WarSceneCreate();

CX_C_END

#endif
