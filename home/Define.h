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

#define MAP_ROW         40
#define MAP_COL         40
#define ARRIVE_MIN_DIS  (15)    //到达点最小距离

//固定数量数组
typedef struct {
    cxAny items[64];
    cxInt number;
}FixArray;

#define FixArrayAppend(_a_,_i_) (_a_).items[(_a_).number++] = _i_

typedef struct {
    cxVec2i unitNum;    //单元数量
    cxSize2f unitSize;  //单元大小
    cxFloat sideLen;    //菱形单元边长
    cxFloat angle;      //大角度
    cxFloat mapRate;    //地图宽度比例
}Global;

extern Global global;
void GlobalInit(cxEngine engine);

cxFloat SideDistance(cxInt sideNum);

//8方向使用 22.5 45.0角度
cxFloat AngleToIndex(cxFloat angle,cxInt *index);

//node Bind原因
typedef enum {
    NodeBindReasonNone = 0,   //未知
    NodeBindReasonMove,       //bind朝目标移动移动状态
    NodeBindReasonAttack,     //bind攻击目标战斗状态
    NodeBindReasonShoot       //bind射击状态
}NodeBindReason;

//地图模式
typedef enum {
    MapModeNone = 0,
    MapModeNormal,          //普通模式
    MapModeFight,           //战斗模式
}MapMode;

//节点主类型
typedef enum {
    NodeTypeNone        = 0,
    NodeTypeResource    = 1 << 0,       //资源类型
    NodeTypeDefence     = 1 << 1,       //主动防御类型
    NodeTypeAttack      = 1 << 2,       //进攻单位类型
    NodeTypeBlock       = 1 << 3,       //可阻挡进攻类,如城墙，城门
    NodeTypeDecoration  = 1 << 4,       //非攻击装饰类型
}NodeType;

//节点子类型
typedef enum {
    NodeSubTypeNone     = 0,
    NodeSubTypeSoldier  = 1 << 0,      //近身攻击的士兵
    NodeSubTypeTurret   = 1 << 1,      //炮塔
    NodeSubTypeArcher   = 1 << 2,      //弓箭手
    NodeSubTypeWall     = 1 << 3,      //城墙
}NodeSubType;

//组合类型
typedef struct {
    NodeType mainType;
    NodeSubType subType;
    cxRange2f range;        //搜索这个组合使用的范围
}NodeCombined;

#define NodeCombinedMake(t,s) (NodeCombined){t,s,{0,0}}

#define MAX_RANGE   cxRange2fv(0, MAP_ROW*2)

//搜索顺序最大32
#define MAX_ORDERS 32
typedef struct {
    NodeCombined types[MAX_ORDERS];
    cxInt number;
}NodeSearchOrder;

typedef enum {
    AttackTypeNone = 0,     //
    AttackTypeDirect,       //直接攻击
    AttackTypeBullet,     //间接攻击 使用bullet基类
}AttackType;

//附近点搜索信息
typedef struct {
    cxAny node;         //最近的view
    cxVec2f idx;        //与此点的
    cxFloat dis;        //距离
    NodeCombined type;
} NodeNearestInfo;

//两点搜索信息
typedef struct {
    cxAny node;         //最近的view
    cxVec2f a;          //第1点
    cxVec2f b;          //第2点
    cxFloat dis;        //距离
    NodeCombined type;  //搜索的类型
} NodeSegmentInfo;

CX_C_END


#endif
