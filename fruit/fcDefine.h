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

typedef struct {
    cxVec2i points[DM_MAP_WIDTH*DM_MAP_HEIGHT];
    cxInt number;
}fcPath;

typedef enum {
    fcGroupTypeNone = 0,
    fcGroupTypeDefenser,   //防御方
    fcGroupTypeAttacker,    //进攻方
}fcGroupType;

typedef enum {
    fcSpriteTypeNone = 0,       //什么都不是
    fcSpriteTypeDecoration,     //装饰物,例如地面，道路，花花草草
    fcSpriteTypeContainer,      //容器，例如水果仓库，水果篮子
    fcSpriteTypeAttacker,       //攻击物，例如12生效宠物,投射机
    fcSpriteTypeProducer,       //生成者，例如果树,神树
}fcSpriteType;

typedef enum {
    fcAttackerTypeNone = 0,
    fcAttackerTypeSmallMachine,     //投射机 - 小型
    fcAttackerTypeNormalMachine,    //投射机 - 中型
    fcAttackerTypeBigMachine,       //投射机 - 大型
    fcAttackerTypeSuperMachine,     //投射机 - 重型
}fcAttackerType;

typedef enum {
    fcFruitTypeNone = 0,
    fcFruitTypeFollow,      //跟踪水果
}fcFruitType;

CX_C_END

#endif
