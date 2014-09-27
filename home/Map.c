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
    cxView v1 = CX_TYPE_CAST(lp->any,cxView);
    cxView v2 = CX_TYPE_CAST(rp->any,cxView);
    return v2->position.y - v1->position.y;
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
    
    cxSize2f size = cxViewSize(this);
    
    //test
    cxSpriteSetTextureURL(this, "bg3.png");
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
    
//    for (cxInt x = 0; x < 42; x ++) {
//        for (cxInt y = 0; y < 42 ; y++) {
//            cxSprite sp = cxSpriteCreateWithURL("0.png");
//            cxViewSetSize(sp, global.unitSize);
//            cxViewSetPos(sp, MapIdxToPos(this, cxVec2fv(x, y)));
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
    cxVec2f idx = this->index;
    return cpBBNew(idx.x, idx.y, idx.x + this->size.w, idx.y + this->size.h);
}

static cxInt MapSearchEarlyExit(cxAny pstar, cxInt vcount, cxVec2i *curr,cxVec2i *target)
{
    CX_ASSERT_THIS(pstar, cxAStar);
    MapSearchInfo *info = this->data;
    Map map = CX_TYPE_CAST(info->map,Map);
    cxAny item = MapItem(map, *curr);
    //如果格子上已经是目标提前完成
    if(item == info->dnode){
        return 1;
    }
    return 0;
}

static cxBool MapSearchIsAppend(cxAny pstar,cxVec2i *idx)
{
    CX_ASSERT_THIS(pstar, cxAStar);
    MapSearchInfo *info = this->data;
    Map map = CX_TYPE_CAST(info->map,Map);
    cxVec2f index = cxVec2fv(idx->x, idx->y);
    //如果索引超出最大范围
    if(!MapIsValidIdx(map, *idx)){
        return false;
    }
    cxVec2f sidx = NodeFloatIndex(info->snode);
    cxVec2f didx = NodeFloatIndex(info->dnode);
    //开始点与搜索点之间距离超过搜索范围
    cxFloat dis = kmVec2DistanceBetween(&sidx, &index);
    if(dis > info->dis){
        return false;
    }
    //目标点与搜索点之间距离超过范围
    dis = kmVec2DistanceBetween(&didx, &index);
    if(dis > info->dis){
        return false;
    }
    //
    Node node = MapItem(map, *idx);
    //空位置
    if(node == NULL){
        return true;
    }
    //开始点和结束点
    if(node == info->snode || node == info->dnode){
        return true;
    }
    //如果是阻挡类型
    if(node->type.mainType == NodeTypeBlock && !NodeCheckDie(node)){
        //记录第一个阻挡物
        if(info->block == NULL)info->block = node;
        return false;
    }
    return true;
}

CX_OBJECT_TYPE(Map, cxAtlas)
{
    
}
CX_OBJECT_INIT(Map, cxAtlas)
{
    //只有自己能接收事件
    cxViewSetTouchFlags(this, cxViewTouchFlagsSelf);
    //当选中按钮执行
    
    
    CX_EVENT_APPEND(CX_TYPE(cxView, this)->onUpdate, MapUpdate);
    this->mode = MapModeNormal;
    this->isSort = true;
    
    //路劲搜索算法
    this->astar = CX_ALLOC(cxAStar);
    cxAStarSetType(this->astar, cxAStarTypeA8);
    CX_METHOD_SET(this->astar->IsAppend, MapSearchIsAppend);
    CX_METHOD_SET(this->astar->EarlyExit, MapSearchEarlyExit);
    
    //类型搜索索引
    cxInt cells = global.unitNum.x * global.unitNum.y;
    this->items = cxSpatialAlloc(1.0f, cells, NodeIndexBB);
    
    //node层
    this->nLayer = cxViewAppendType(this,cxView);
    
    //弹药效果层
    this->bLayer = cxViewAppendType(this,cxView);
    cxViewAppend(this, this->bLayer);
    
    //所有节点存储
    this->nodes = CX_ALLOC(cxHash);
    
    //路径缓冲
    this->paths = CX_ALLOC(cxHash);
    
    cxEngine engine = cxEngineInstance();
    
    cxSize2f size = cxSize2fv(engine->winsize.w * global.mapRate, 0);
    size.h = size.w * 0.75f;
    global.unitSize = cxSize2fv(size.w/global.unitNum.x, size.h/global.unitNum.y);
    
    cxViewSetSize(this, size);
    cxViewSetSize(this->bLayer, size);
    cxViewSetSize(this->nLayer, size);
}
CX_OBJECT_FREE(Map, cxAtlas)
{
    CX_RELEASE(this->paths);
    CX_RELEASE(this->items);
    CX_RELEASE(this->astar);
    CX_RELEASE(this->nodes);
}
CX_OBJECT_TERM(Map, cxAtlas)

void MapAppendBullet(cxAny bullet)
{
    CX_ASSERT_THIS(bullet, Bullet);
    Map map = BulletMap(this);
    cxViewAppend(map->bLayer, bullet);
}

void MapRemoveBullet(cxAny bullet)
{
    CX_ASSERT_THIS(bullet, Bullet);
    cxViewRemove(this);
}

void MapAppendNode(cxAny node)
{
    CX_ASSERT_THIS(node, Node);
    Map map = NodeMap(this);
    cxViewAppend(map->nLayer, node);
    cxSpatialInsert(map->items, node);
}

void MapRemoveNodes(cxAny pmap)
{
    CX_ASSERT_THIS(pmap, Map);
    cxList views = cxViewSubViews(this->nLayer);
    CX_LIST_FOREACH(views, ele){
        MapRemoveNode(ele->any);
    }
}

void MapRemoveNode(cxAny node)
{
    CX_ASSERT_THIS(node, Node);
    Map map = NodeMap(this);
    cxSpatialRemove(map->items, node);
    //从nodes列表分离,注册动态目标不会从这个列表分离
    MapDetachNode(node);
    cxViewRemove(node);
}

static cpCollisionID MapIndexQueryFunc(cxAny pmap, cxAny pview, cpCollisionID id, void *data)
{
    NodeNearestInfo *info = data;
    Node node = CX_TYPE_CAST(pview,Node);
    //死了的不参与搜索
    if(NodeCheckDie(node)){
        return id;
    }
    //不匹配类型
    if(info->type.mainType != NodeTypeNone && !(info->type.mainType & node->type.mainType)){
        return id;
    }
    //不匹配子类型
    if(info->type.subType != NodeSubTypeNone && !(info->type.subType & node->type.subType)){
        return id;
    }
    //计算距离，获取最近的node
    cxFloat d = NodeDistance(info->src, node);
    if(d > info->dis){
        return id;
    }
    info->dis = d;
    //node必须在范围内在范围内
    if(info->dis >= info->type.range.min && info->dis <= info->type.range.max){
        info->node = node;
    }
    return id;
}

//搜索离src最近的单位
cxAny MapNearestQuery(cxAny src,NodeCombined type)
{
    CX_ASSERT_THIS(src, Node);
    Map map = NodeMap(this);
    NodeNearestInfo ret = {0};
    ret.src = src;
    ret.dis = INT32_MAX;
    ret.type = type;
    cxVec2f idx = NodeFloatIndex(this);
    cpBB bb = cpBBNewForCircle(idx, cpfmax(type.range.max, 0.0f));
    cpSpatialIndexQuery(map->items->index, map, bb, MapIndexQueryFunc, &ret);
    return ret.node;
}

static cpFloat MapSegmentQueryFunc(cxAny pmap, cxAny pview, void *data)
{
    NodeSegmentInfo *info = data;
    Node node = CX_TYPE_CAST(pview,Node);
    //死了的不参与搜索
    if(NodeCheckDie(node)){
        return 1.0f;
    }
    //不匹配类型
    if(info->type.mainType != NodeTypeNone && !(info->type.mainType & node->type.mainType)){
        return 1.0f;
    }
    //不匹配子类型
    if(info->type.subType != NodeSubTypeNone && !(info->type.subType & node->type.subType)){
        return 1.0f;
    }
    //获取当前点与原点之间的距离,如果比目标点更远直接返回
    cxFloat nb = NodeDistance(info->src, node);
    if(nb > info->ab){
        return 1.0f;
    }
    //如果不能到达目标返回继续搜索
    if(!MapSearchPath(info->src, node)){
        return 1.0f;
    }
    //获取当前点与目标距离，获取距离目标最近的node
    cxFloat dis = NodeDistance(node, info->dst);
    if(dis < info->dis){
        info->dis = dis;
        info->node = node;
    }
    return 1.0f;
}

//搜索src dst之间的单位
cxAny MapSegmentQuery(cxAny src,cxAny dst,NodeCombined type)
{
    Node sp = CX_TYPE_CAST(src,Node);
    Node dp = CX_TYPE_CAST(dst,Node);
    Map map = NodeMap(sp);
    NodeSegmentInfo ret = {0};
    ret.src = src;
    ret.dst = dst;
    ret.ab = NodeDistance(sp, dp);
    ret.dis = INT32_MAX;
    ret.type = type;
    cxVec2f a = NodeFloatIndex(sp);
    cxVec2f b = NodeFloatIndex(dp);
    cpSpatialIndexSegmentQuery(map->items->index, map, a, b, 1.0f, MapSegmentQueryFunc, &ret);
    return ret.node;
}

cxAnyArray MapVisiedPoints(cxAny pmap)
{
    CX_ASSERT_THIS(pmap, Map);
    return this->astar->visits;
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
    cxAStarCleanPath(this->astar);
    cxAnyArrayAppends(this->astar->points, path);
    return true;
}

cxBool MapSearchPath(cxAny snode,cxAny dnode)
{
    Map map = NodeMap(snode);
    Node sx = CX_TYPE_CAST(snode,Node);
    Node dx = CX_TYPE_CAST(dnode,Node);
    cxVec2f didx = NodeFloatIndex(dnode);
    cxVec2f sidx = NodeFloatIndex(snode);
    cxVec2i a = cxVec2iv(sidx.x, sidx.y);
    CX_ASSERT(MapIsValidIdx(map, a), "index error");
    cxVec2i b = cxVec2iv(didx.x, didx.y);
    CX_ASSERT(MapIsValidIdx(map, b), "index error");
    //从缓存获取路径
    if(MapCachePath(map, a, b)){
        return true;
    }
    //
    MapSearchInfo info = {NULL};
    //设置搜索参数
    cxFloat sz = CX_MAX(sx->size.w, sx->size.h);
    cxFloat dz = CX_MAX(dx->size.w, dx->size.h);
    info.dis = kmVec2DistanceBetween(&sidx, &didx) + sz + dz;
    info.map = map;
    info.snode = snode;
    info.dnode = dnode;
    //开始搜索,如果成功保存路径到缓存
    if(cxAStarRun(map->astar, a, b, &info)) {
        return MapCacheSavePath(map, a, b);
    }
    return false;
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
    Map map = CX_TYPE_CAST(this->map,Map);
    //静态目标才能放人nodes
    if(!NodeIsStatic(node)){
        return;
    }
    cxSize2i size = NodeSize(node);
    cxVec2i idx = this->initIdx;
    for(cxInt x = idx.x; x < idx.x + size.w; x++)
    for(cxInt y = idx.y; y < idx.y + size.h; y++){
        MapDelNode(map, x, y);
    }
}

void MapFillNode(cxAny pmap,cxVec2i idx,cxAny node)
{
    CX_ASSERT_THIS(pmap, Map);
    CX_ASSERT_TYPE(node, Node);
    if(!MapIsValidIdx(this,idx)){
        return;
    }
    //静态目标才能放人nodes
    if(!NodeIsStatic(node)){
        return;;
    }
    cxSize2i size = NodeSize(node);
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
    cxSize2f size = cxViewSize(this);
    pos.y += size.h/2.0f;
    return cxTilePosToIdx(pos, global.unitSize);
}

cxVec2i MapPosToIndex(cxAny pmap,cxVec2f pos)
{
    CX_ASSERT_THIS(pmap, Map);
    cxSize2f size = cxViewSize(this);
    pos.y += size.h/2.0f;
    cxVec2f idx = cxTilePosToIdx(pos, global.unitSize);
    return cxVec2iv(idx.x, idx.y);
}

cxVec2f MapIndexToPos(cxAny pmap,cxVec2f idx)
{
    CX_ASSERT_THIS(pmap, Map);
    cxSize2f size = cxViewSize(this);
    cxVec2f pos = cxTileIdxToPos(idx, global.unitSize);
    pos.y -= size.h/2.0f;
    return pos;
}











