//
//  fcDefine.h
//  cxEngine
//
//  Created by xuhua on 3/23/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef fruit_fcDefine_h
#define fruit_fcDefine_h

#include <core/cxBase.h>

CX_C_BEGIN

#define DM_MAP_WIDTH    10

#define DM_MAP_HEIGHT   10

#define FC_MOVE_ACTION_ID   1000

typedef struct {
    cxVec2i points[DM_MAP_WIDTH*DM_MAP_HEIGHT];
    cxInt number;
}fcPath;

typedef enum {
    fcSpriteTypeNone = 0,       //你什么都不是
    fcSpriteTypeDecoration,     //装饰物,例如地面，道路，花花草草，起点，终点
    fcSpriteTypeContainer,      //容器，例如水果仓库，水果篮子,宝箱
    fcSpriteTypeIntruder,       //闯关者,例如12生效宠物
    fcSpriteTypeThrower,        //投掷机,例如水果投射机
    fcSpriteTypeProducer,       //生成者，例如果树,神树
    fcSpriteTypeProperty,       //道具
}fcSpriteType;

typedef enum {
    fcThrowerTypeNone = 0,
    fcThrowerTypeSmallMachine,     //投射机 - 小型
    fcThrowerTypeNormalMachine,    //投射机 - 中型
    fcThrowerTypeBigMachine,       //投射机 - 大型
    fcThrowerTypeSuperMachine,     //投射机 - 重型
}fcThrowerType;

typedef enum {
    fcIntruderTypeNone = 0,
}fcIntruderType;

typedef enum {
    fcLocationTypeNone = 0,
    fcLocationTypeBegin,            //起点
    fcLocationTypeEnd,              //终点
}fcLocationType;

typedef enum {
    fcFruitTypeNone = 0,
    fcFruitTypeFollow,      //跟踪水果
    fcFruitTypePierce,      //穿透水果
}fcFruitType;

CX_C_END

#endif
