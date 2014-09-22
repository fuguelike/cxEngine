//
//  Define.h
//  home
//
//  Created by xuhua on 9/5/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef Home_Define_h
#define Home_Define_h

#include <engine/cxEngine.h>

CX_C_BEGIN

#define MAP_ROW         42
#define MAP_COL         42
#define MAP_BORDER      1
#define SEARCH_DELAY    0.1f    //搜索延迟
#define ARRIVE_MIN_DIS  (15)      //到达点最小距离

typedef struct {
    cxVec2i unitNum;    //单元数量
    cxSize2f unitSize;  //单元大小
    cxFloat sideLen;    //菱形单元边长
    cxFloat angle;      //大角度
}Global;

extern Global global;
void GlobalInit(cxEngine engine);

//node 状态
typedef enum {
    NodeStateNormal,    //一般状态
    NodeStateMove,      //移动状态
    NodeStateFight      //战斗状态
}NodeState;

//地图模式
typedef enum {
    MapModeNone = 0,
    MapModeNormal,          //普通模式
    MapModeFight,           //战斗模式
}MapMode;

//节点主类型
typedef enum {
    NodeTypeNone = 0,
    NodeTypeResource,       //资源类型
    NodeTypeDefence,        //主动防御类型
    NodeTypeAttack,         //主动攻击类型
    NodeTypeOther,          //其他，包括，辅助，军营
}NodeType;

//节点子类型
typedef enum {
    NodeSubTypeNone = 0,
    NodeSubTypeSoldier,       //近身攻击的士兵
}NodeSubType;

CX_C_END


#endif
