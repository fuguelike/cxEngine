//
//  Map.c
//  Home
//
//  Created by xuhua on 8/28/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <algorithm/cxTile.h>
#include <views/cxLabelTTF.h>
#include <views/cxSprite.h>
#include <engine/cxEngine.h>
#include "Map.h"
#include "Node.h"
#include "Move.h"
#include "Bullet.h"
#include <types/Attack.h>
#include <types/Defence.h>
#include <types/Wall.h>

static cxInt MapSortCmpFunc(cxConstAny lv,cxConstAny rv)
{
    cxListElement *lp = (cxListElement *)lv;
    cxListElement *rp = (cxListElement *)rv;
    CX_ASSERT_VALUE(lp->any, cxView, v1);
    CX_ASSERT_VALUE(rp->any, cxView, v2);
    return v2->Position.y - v1->Position.y;
}

void MapSortNode(cxAny pmap)
{
    CX_ASSERT_THIS(pmap, Map);
    this->isSort = true;
}

void MapSetMode(cxAny pmap,MapMode mode)
{
    CX_ASSERT_THIS(pmap, Map);
    this->mode = mode;
}


cxBool MapInit(cxAny pmap)
{
    CX_ASSERT_THIS(pmap, Map);
    
    cxSize2f size = cxViewGetSize(this);
    
    //test
    cxSpriteSetTextureURL(this, "bg2.png");
    cxBoxTex2f tex = cxBoxTex2fDefault();
    cxVec2f pos = cxVec2fv(size.w / 4, size.h / 4);
    cxColor4f color = cxColor4fv(1, 1, 1, 1);
    cxAtlasAppendBoxPoint(this, pos, cxSize2fv(size.w/2.0f, size.h/2.0f), tex, color);
    
    cxBoxTex2f tex1 = cxBoxTex2fFlip(tex, true, false);
    pos = cxVec2fv(-size.w / 4, size.h / 4);
    cxAtlasAppendBoxPoint(this, pos, cxSize2fv(size.w/2.0f, size.h/2.0f), tex1, color);

    cxBoxTex2f tex2 = cxBoxTex2fFlip(tex, true, true);
    pos = cxVec2fv(-size.w / 4, -size.h / 4);
    cxAtlasAppendBoxPoint(this, pos, cxSize2fv(size.w/2.0f, size.h/2.0f), tex2, color);

    cxBoxTex2f tex3 = cxBoxTex2fFlip(tex, false, true);
    pos = cxVec2fv(size.w / 4, -size.h / 4);
    cxAtlasAppendBoxPoint(this, pos, cxSize2fv(size.w/2.0f, size.h/2.0f), tex3, color);
    
//    for (cxInt x = 0; x < global.unitNum.x; x ++) {
//        for (cxInt y = 0; y < global.unitNum.y ; y++) {
//            cxSprite sp = cxSpriteCreateWithURL("bg1.png");
//            cxViewSetSize(sp, global.unitSize);
//            cxViewSetPosition(sp, MapIndexToPos(this, cxVec2fv(x + 0.5f, y + 0.5f)));
//            cxViewAppend(this, sp);
//        }
//    }
    //按Y位置排序
    return true;
}

static void MapUpdate(cxAny pview)
{
    CX_ASSERT_THIS(pview, Map);
    if(this->isSort){
        cxViewSortWithFunc(this, MapSortCmpFunc);
        this->isSort = false;
    }
}

static cpBB NodeIndexBB(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    //这个坐标其实是物体中间的坐标
    cxVec2f idx = NodeGetIndex(this);
    cxFloat w2 = (cxFloat)this->Size.w / 2.0f;
    cxFloat h2 = (cxFloat)this->Size.h / 2.0f;
    return cpBBNew(idx.x - w2, idx.y - h2, idx.x + w2, idx.y + h2);
}

static cxInt MapSearchEarlyExit(cxAny pstar, cxInt vcount, cxVec2i *curr,cxVec2i *target)
{
    CX_ASSERT_THIS(pstar, cxAStar);
    MapSearchInfo *info = this->data;
    CX_ASSERT_VALUE(info->map, Map, map);
    CX_ASSERT_VALUE(info->dst, Node, dnode);
    CX_ASSERT_VALUE(info->src, Node, snode);
    //如果目标是敌军继续寻路
    if(dnode->Type.mainType == NodeTypeEnemy){
        return 0;
    }
    Node cnode = MapItem(map, *curr);
    //如果格子上已经是目标提前完成
    if(cnode == info->dst){
        return 1;
    }
    //如果不检测到达攻击距离
    if(!info->attacked){
        return 0;
    }
    //如果上一点已经到达
    if(info->arrive){
        return 1;
    }
    //如果此点可以攻击到目标返回成功,使用中心点计算
    cxVec2f cidx = cxVec2fv(curr->x + 0.5f, curr->y + 0.5f);
    cxVec2f didx = NodeGetIndex(dnode);
    //当前点与目标点之间的距离
    cxFloat dis = kmVec2DistanceBetween(&cidx, &didx);
    //如果可以攻击到就成功返回
    if(NodeIsArriveDistance(snode, dnode, dis)){
        info->arrive = true;
    }
    return 0;
}

static cxBool MapSearchIsAppend(cxAny pstar,cxVec2i *idx)
{
    CX_ASSERT_THIS(pstar, cxAStar);
    MapSearchInfo *info = this->data;
    CX_ASSERT_VALUE(info->map, Map, map);
    //如果索引超出最大范围
    if(!MapIsValidIdx(map, *idx)){
        return false;
    }
    //+0.5f表示距离从中心点算 只搜索中心点附近的位置
    cxVec2f index = cxVec2fv(idx->x+0.5f, idx->y+0.5f);
    cxFloat dis = kmVec2DistanceBetween(&info->mid, &index);
    if(dis > info->ab){
        return false;
    }
    //没有物体
    Node node = MapItem(map, *idx);
    if(node == NULL){
        return true;
    }
    //开始或者结束物体
    if(node == info->src || node == info->dst){
        return true;
    }
    NodeCombined type = NodeGetType(node);
    //如果是阻挡类型
    if(type.mainType == NodeTypeBlock && !NodeCheckDie(node)){
        return false;
    }
    //其他node只可以在最外一格活动
    return true;
}

cxBool MapSearchPath(cxAny snode,cxAny dnode,cxBool isAttacked)
{
    Map map = NodeGetMap(snode);
    CX_ASSERT_VALUE(snode, Node, sx);
    CX_ASSERT_VALUE(dnode, Node, dx);
    MapSearchInfo info = {NULL};
    info.attacked = isAttacked;
    info.arrive = false;
    info.map = map;
    cxVec2f didx = NodeGetIndex(dx);
    cxVec2f sidx = NodeGetIndex(sx);
    //设置开始node和结束node
    info.src = snode;
    info.dst = dnode;
    //获取中心点
    info.mid = cxVec2fMidPoint(sidx, didx);
    //获取半径
    info.ab = NodeDistance(snode, dnode);
    info.ab += CX_MAX(dx->Size.w, dx->Size.h);
    info.ab += CX_MAX(sx->Size.w, sx->Size.h);
    info.ab = info.ab / 2.0f;
    //获取起点和终点
    cxVec2i a = cxVec2fTo2i(sidx);
    cxVec2i b = cxVec2fTo2i(didx);
    if(!MapIsValidIdx(map, a) || !MapIsValidIdx(map, b)){
        CX_WARN("error index use at map path search");
        return false;
    }
    //从缓存获取路径
    if(MapCachePath(map, a, b)){
        return true;
    }
    //开始搜索,如果成功保存路径到缓存
    if(!cxAStarRun(map->astar, a, b, &info)) {
        return false;
    }
    return MapCacheSavePath(map, a, b);
}

cxAny MapHitNode(cxAny pmap,cxVec2f index,NodeCombined type)
{
    CX_ASSERT_THIS(pmap, Map);
    cxArray items = MapSelectedItems(this, index, type);
    return cxArrayFirst(items);
}

CX_TYPE(Map, cxAtlas)
{
    
}
CX_INIT(Map, cxAtlas)
{
    //只有自己能接收touch事件
    cxViewSetTouchFlags(this, cxViewTouchFlagsSelf);
    //当选中按钮执行
    
    CX_ADD(cxView, this, onUpdate, MapUpdate);

    this->mode = MapModeNormal;
    this->isSort = true;
    
    //路劲搜索算法
    this->astar = CX_ALLOC(cxAStar);
    cxAStarSetType(this->astar, cxAStarTypeA8);
    CX_SET(cxAStar, this->astar, IsAppend, MapSearchIsAppend);
    CX_SET(cxAStar, this->astar, EarlyExit, MapSearchEarlyExit);
    
    //类型搜索索引
    this->items = cxSpatialAlloc(NodeIndexBB);
    //node层
    this->nLayer = cxViewAppendType(this,cxView);
    //效果层
    this->aLayer = cxViewAppendType(this,cxView);
    
    //创建格子属性数组
    this->attrs = allocator->malloc(global.unitNum.x * global.unitNum.y * sizeof(MapUnitAttr));
    //所有节点存储
    this->nodes = CX_ALLOC(cxHash);
    //路径缓冲
    this->paths = CX_ALLOC(cxHash);
    cxViewSetSize(this, global.mapSize);
    cxViewSetSize(this->aLayer, global.mapSize);
    cxViewSetSize(this->nLayer, global.mapSize);
}
CX_FREE(Map, cxAtlas)
{
    allocator->free(this->attrs);
    CX_RELEASE(this->paths);
    CX_RELEASE(this->items);
    CX_RELEASE(this->astar);
    CX_RELEASE(this->nodes);
}
CX_TERM(Map, cxAtlas)

void MapAppendBullet(cxAny bullet)
{
    CX_ASSERT_THIS(bullet, Bullet);
    Map map = BulletGetMap(this);
    cxViewAppend(map->aLayer, bullet);
}

void MapRemoveBullet(cxAny bullet)
{
    CX_ASSERT_THIS(bullet, Bullet);
    cxViewRemove(this);
}

void MapAppendNode(cxAny node)
{
    CX_ASSERT_THIS(node, Node);
    Map map = NodeGetMap(this);
    cxViewAppend(map->nLayer, node);
    cxSpatialInsert(map->items, node);
}

void MapRemoveNodes(cxAny pmap)
{
    CX_ASSERT_THIS(pmap, Map);
    CX_VIEW_FOREACH_SUBVIEWS(this->nLayer, ele){
        MapRemoveNode(ele->any);
    }
}

void MapRemoveNode(cxAny node)
{
    CX_ASSERT_THIS(node, Node);
    Map map = NodeGetMap(this);
    cxSpatialRemove(map->items, node);
    //从nodes列表分离,动态目标不会从这个列表分离
    MapDetachNode(node);
    cxViewRemove(node);
}

static cpCollisionID MapIndexQueryFunc(cxAny pmap, cxAny pview, cpCollisionID id, void *data)
{
    NodeNearestInfo *info = data;
    CX_ASSERT_VALUE(pview, Node, node);
    //死了的不参与搜索
    if(NodeCheckDie(node)){
        return id;
    }
    NodeCombined type = NodeGetType(node);
    //不匹配主类型
    if(info->type.mainType != NodeTypeNone && !(info->type.mainType & type.mainType)){
        return id;
    }
    //不匹配子类型
    if(info->type.subType != NodeSubTypeNone && !(info->type.subType & type.subType)){
        return id;
    }
    //不包括已经bind的对象
    if(cxViewHasBind(info->src, node)){
        return id;
    }
    //计算距离，获取最近的node
    cxFloat d = NodeDistance(info->src, node);
    if(d > info->dis){
        return id;
    }
    info->dis = d;
    //node必须在范围内
    if(info->dis >= info->range.min && info->dis <= info->range.max){
        info->node = node;
    }
    //如果node不能被src发现返回false
    cxBool ret = CX_METHOD_GET(true, node->IsFinded,node,info->src);
    if(info->node != NULL && !ret){
        info->node = NULL;
    }
    return id;
}

//搜索离src最近的单位
cxAny MapNearestQuery(cxAny src,NodeCombined type,cxRange2f range)
{
    CX_ASSERT_THIS(src, Node);
    Map map = NodeGetMap(this);
    NodeNearestInfo ret = {0};
    ret.src = src;
    ret.dis = INT32_MAX;
    ret.type = type;
    ret.range = range;
    cpBB bb = cpBBNewForCircle(NodeGetIndex(this), cpfmax(range.max, 0.0f));
    cpSpatialIndexQuery(map->items->index, map, bb, MapIndexQueryFunc, &ret);
    return ret.node;
}

static cpFloat MapSegmentQueryFunc(cxAny pmap, cxAny pview, void *data)
{
    NodeSegmentInfo *info = data;
    CX_ASSERT_VALUE(pview, Node, node);
    //死了的不参与搜索
    if(NodeCheckDie(node)){
        return 1.0f;
    }
    NodeCombined type = NodeGetType(node);
    //不匹配主类型
    if(info->type.mainType != NodeTypeNone && !(info->type.mainType & type.mainType)){
        return 1.0f;
    }
    //不匹配子类型
    if(info->type.subType != NodeSubTypeNone && !(info->type.subType & type.subType)){
        return 1.0f;
    }
    //获取当前点与原点之间的距离,如果比目标点更远直接返回
    cxFloat dis = NodeDistance(info->src, node);
    if(dis > info->ab){
        return 1.0f;
    }
    //如果不能到达此点
    if(!MapSearchPath(info->src, node, false)){
        return 1.0f;
    }
    dis = NodeDistance(node, info->dst);
    if(dis < info->dis){
        info->dis = dis;
        info->node = node;
    }
    return 1.0f;
}

//搜索src dst之间的单位
cxAny MapSegmentQuery(cxAny src,cxAny dst,NodeCombined type)
{
    CX_ASSERT_VALUE(src, Node, sp);
    CX_ASSERT_VALUE(dst, Node, dp);
    Map map = NodeGetMap(sp);
    NodeSegmentInfo ret = {0};
    cxVec2f a = NodeGetIndex(sp);
    cxVec2f b = NodeGetIndex(dp);
    ret.src = src;
    ret.dst = dst;
    ret.ab = kmVec2DistanceBetween(&a, &b);
    ret.dis = INT32_MAX;
    ret.type = type;
    cpSpatialIndexSegmentQuery(map->items->index, map, a, b, 1.0f, MapSegmentQueryFunc, &ret);
    return ret.node;
}

static cpCollisionID MapItemsQueryFunc(cxAny pmap, cxAny pview, cpCollisionID id, void *data)
{
    NearestItemsInfo *info = data;
    CX_ASSERT_VALUE(pview, Node, node);
    //死了的不参与搜索
    if(NodeCheckDie(node)){
        return id;
    }
    NodeCombined type = NodeGetType(node);
    //不匹配主类型
    if(info->type.mainType != NodeTypeNone && !(info->type.mainType & type.mainType)){
        return id;
    }
    //不匹配子类型
    if(info->type.subType != NodeSubTypeNone && !(info->type.subType & type.subType)){
        return id;
    }
    cxVec2f npos = NodeGetIndex(node);
    cxFloat dis = kmVec2DistanceBetween(&npos, &info->point);
    if(cxFloatEqu(dis, info->range.min)){
        cxArrayAppend(info->nodes, node);
        return id;
    }
    if(cxFloatEqu(dis, info->range.max)){
        return id;
    }
    if(dis > info->range.min && dis < info->range.max){
        cxArrayAppend(info->nodes, node);
    }
    return id;
}

//搜索某点附近的，范围内的所有node,
cxArray MapNearestItems(cxAny pmap,cxVec2f point,cxRange2f range,NodeCombined type)
{
    CX_ASSERT_THIS(pmap, Map);
    NearestItemsInfo ret = {0};
    ret.nodes = CX_CREATE(cxArray);
    ret.point = point;
    ret.type = type;
    ret.range = range;
    cpBB bb = cpBBNewForCircle(point, cpfmax(range.max, 0.0f));
    cpSpatialIndexQuery(this->items->index, this, bb, MapItemsQueryFunc, &ret);
    return ret.nodes;
}

static cpCollisionID MapSelectedItemsFunc(cxAny pmap, cxAny pview, cpCollisionID id, void *data)
{
    SelectedItemsInfo *info = data;
    CX_ASSERT_VALUE(pview, Node, node);
    NodeCombined type = NodeGetType(node);
    //不匹配主类型
    if(info->type.mainType != NodeTypeNone && !(info->type.mainType & type.mainType)){
        return id;
    }
    //不匹配子类型
    if(info->type.subType != NodeSubTypeNone && !(info->type.subType & type.subType)){
        return id;
    }
    //如果点击在node内部
    if(NodeHited(node, info->point)){
        cxArrayAppend(info->nodes, node);
    }
    return id;
}

cxArray MapSelectedItems(cxAny pmap,cxVec2f point,NodeCombined type)
{
    CX_ASSERT_THIS(pmap, Map);
    SelectedItemsInfo ret = {0};
    ret.nodes = CX_CREATE(cxArray);
    ret.point = point;
    ret.type = type;
    cpBB bb = cpBBNewForCircle(point, cpfmax(1.0f, 0.0f));
    cpSpatialIndexQuery(this->items->index, this, bb, MapSelectedItemsFunc, &ret);
    return ret.nodes;
}

cxAnyArray MapVisiedPoints(cxAny pmap)
{
    CX_ASSERT_THIS(pmap, Map);
    return this->astar->visits;
}

void MapAppendPoint(cxAny pmap,cxVec2f pos)
{
    CX_ASSERT_THIS(pmap, Map);
    cxVec2i idx = cxVec2iv(pos.x, pos.y);
    cxAnyArrayAppend(this->astar->points, &idx);
}

void MapClearPoints(cxAny pmap)
{
    CX_ASSERT_THIS(pmap, Map);
    cxAStarClearPath(this->astar);
}

cxAnyArray MapSearchPoints(cxAny pmap)
{
    CX_ASSERT_THIS(pmap, Map);
    return this->astar->points;
}

#define CACHE_PATH_IDX(a,b) ((a.y * global.unitNum.y + a.x) << 16) + (b.y * global.unitNum.x + b.x)

void MapCacheDelPath(cxAny pmap,cxVec2i a,cxVec2i b)
{
    CX_ASSERT_THIS(pmap, Map);
    cxInt idx = CACHE_PATH_IDX(a,b);
    cxHashKey key = cxHashLongKey(idx);
    cxHashDel(this->paths, key);
}

cxBool MapCacheSavePath(cxAny pmap,cxVec2i a,cxVec2i b)
{
    CX_ASSERT_THIS(pmap, Map);
    cxAnyArray path = this->astar->points;
    CX_ASSERT(cxAnyArrayLength(path) > 0, "save empty path points");
    cxInt idx = CACHE_PATH_IDX(a,b);
    cxHashKey key = cxHashLongKey(idx);
    cxHashSet(this->paths, key, cxAnyArrayClone(path));
    return true;
}

cxBool MapCachePath(cxAny pmap,cxVec2i a,cxVec2i b)
{
    CX_ASSERT_THIS(pmap, Map);
    cxInt idx = CACHE_PATH_IDX(a,b);
    cxHashKey key = cxHashLongKey(idx);
    cxAnyArray path = cxHashGet(this->paths, key);
    if(path == NULL){
        return false;
    }
    cxAStarClearPath(this->astar);
    cxAnyArrayAppends(this->astar->points, path);
    return true;
}

void MapDelNode(cxAny pmap,cxInt x,cxInt y)
{
    CX_ASSERT_THIS(pmap, Map);
    cxHashKey key = cxHashIntKey(y * global.unitNum.x + x);
    cxHashDel(this->nodes, key);
}

void MapSetNode(cxAny pmap,cxInt x,cxInt y,cxAny node)
{
    CX_ASSERT_THIS(pmap, Map);
    cxHashKey key = cxHashIntKey(y * global.unitNum.x + x);
    cxHashSet(this->nodes, key, node);
}

void MapAddAttr(cxAny pmap,cxVec2i idx,MapUnitAttr attr)
{
    CX_ASSERT_THIS(pmap, Map);
    CX_ASSERT(MapIsValidIdx(this,idx), "idx error");
    cxInt key = idx.y * global.unitNum.x + idx.x;
    this->attrs[key] |= attr;
}

void MapSubAttr(cxAny pmap,cxVec2i idx,MapUnitAttr attr)
{
    CX_ASSERT_THIS(pmap, Map);
    CX_ASSERT(MapIsValidIdx(this,idx), "idx error");
    cxInt key = idx.y * global.unitNum.x + idx.x;
    this->attrs[key] &= ~attr;
}

void MapSetAttr(cxAny pmap,cxVec2i idx,MapUnitAttr attr)
{
    CX_ASSERT_THIS(pmap, Map);
    CX_ASSERT(MapIsValidIdx(this,idx), "idx error");
    cxInt key = idx.y * global.unitNum.x + idx.x;
    this->attrs[key] = attr;
}

MapUnitAttr MapAttr(cxAny pmap,cxVec2i idx)
{
    CX_ASSERT_THIS(pmap, Map);
    CX_ASSERT(MapIsValidIdx(this,idx), "idx error");
    cxInt key = idx.y * global.unitNum.x + idx.x;
    return this->attrs[key];
}
                            
cxAny MapItem(cxAny pmap,cxVec2i idx)
{
    CX_ASSERT_THIS(pmap, Map);
    if(!MapIsValidIdx(this, idx)){
        return NULL;
    }
    cxHashKey key = cxHashIntKey(idx.y * global.unitNum.x + idx.x);
    return cxHashGet(this->nodes, key);
}

void MapDetachNode(cxAny node)
{
    CX_ASSERT_THIS(node, Node);
    Map map = NodeGetMap(this);
    //静态目标才能放人nodes
    if(!NodeGetIsStatic(node)){
        return;
    }
    cxSize2i size = NodeGetSize(node);
    cxVec2i idx = NodeGetInitIndex(this);
    for(cxInt x = idx.x; x < idx.x + size.w; x++)
    for(cxInt y = idx.y; y < idx.y + size.h; y++){
        MapDelNode(map, x, y);
    }
}

cxBool MapIsFillNode(cxAny pmap,cxVec2i idx,cxAny node)
{
    CX_ASSERT_THIS(pmap, Map);
    CX_ASSERT_TYPE(node, Node);
    if(!MapIsValidIdx(this,idx)){
        return false;
    }
    //静态目标才能放人nodes
    if(!NodeGetIsStatic(node)){
        return false;
    }
    cxSize2i size = NodeGetSize(node);
    if(idx.x < 1 || idx.y < 1){
        return false;
    }
    if((idx.x + size.w > global.unitNum.x - 1) || (idx.y + size.h) > global.unitNum.y - 1){
        return false;
    }
    for(cxInt x = idx.x; x < idx.x + size.w; x++)
    for(cxInt y = idx.y; y < idx.y + size.h; y++) {
        cxAny cnode = MapItem(this, cxVec2iv(x, y));
        //有其他node返回
        if(cnode != NULL && cnode != node){
            return false;
        }
    }
    return true;
}

void MapFillNode(cxAny pmap,cxVec2i idx,cxAny node)
{
    CX_ASSERT_THIS(pmap, Map);
    CX_ASSERT_TYPE(node, Node);
    if(!MapIsValidIdx(this,idx)){
        return;
    }
    //静态目标才能放人nodes
    if(!NodeGetIsStatic(node)){
        return;;
    }
    cxSize2i size = NodeGetSize(node);
    for(cxInt x = idx.x; x < idx.x + size.w; x++)
    for(cxInt y = idx.y; y < idx.y + size.h; y++) {
        MapSetNode(this, x, y, node);
    }
}

cxBool MapIsValidIdx(cxAny pmap,cxVec2i idx)
{
    return idx.x >= 0 && idx.x < global.unitNum.x && idx.y >= 0 && idx.y < global.unitNum.y;
}

cxVec2f MapPosToFloat(cxAny pmap,cxVec2f pos)
{
    CX_ASSERT_THIS(pmap, Map);
    cxSize2f size = cxViewGetSize(this);
    pos.y += size.h/2.0f;
    return cxTilePosToIdx(pos, global.unitSize);
}

cxVec2i MapPosToIndex(cxAny pmap,cxVec2f pos)
{
    CX_ASSERT_THIS(pmap, Map);
    cxVec2f idx = MapPosToFloat(this, pos);
    return cxVec2iv(idx.x, idx.y);
}

cxVec2f MapIndexToPos(cxAny pmap,cxVec2f idx)
{
    CX_ASSERT_THIS(pmap, Map);
    cxSize2f size = cxViewGetSize(this);
    cxVec2f pos = cxTileIdxToPos(idx, global.unitSize);
    pos.y -= size.h/2.0f;
    return pos;
}











