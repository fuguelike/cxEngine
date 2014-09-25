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
#include <types/Attack.h>
#include <types/Defence.h>
#include <types/Wall.h>

static cxInt MapSortCmpFunc(cxConstAny lv,cxConstAny rv)
{
    cxListElement *lp = (cxListElement *)lv;
    cxListElement *rp = (cxListElement *)rv;
    cxView v1 = CX_TYPE_CAST(cxView, lp->any);
    cxView v2 = CX_TYPE_CAST(cxView, rp->any);
    return v2->position.y - v1->position.y;
}

void MapNodeOnNewIdx(cxAny pmap,cxAny pnode)
{
//    CX_ASSERT_THIS(pmap, Map);
//    Node node = CX_TYPE_CAST(Node, pnode);
//    CX_LOGGER("%p new idx=%f idx.y=%f",pnode,node->idx.x,node->idx.y);
}

void MapSortNode(cxAny pmap)
{
    CX_ASSERT_THIS(pmap, Map);
    this->isSort = true;
}

static void MapNodesEach(cxAny ps,cxAny pview,cxAny data)
{
    Node node = pview;
    cxBool *has = data;
    if(!(*has) && node->isSelected){
        *has = true;
    }
}

static cxBool hasSelectNode(Map this)
{
    cxBool has = false;
    cxSpatialEach(this->items, MapNodesEach, &has);
    return has;
}

static cxBool MapEditTouch(cxAny pview,cxTouchItems *points)
{
    CX_ASSERT_THIS(pview, Map);
    if(points->number != 1){
        return false;
    }
    cxTouchItem item = points->items[0];
    Node snode = this->node;
    if(item->type == cxTouchTypeMove){
        return false;
    }
    if(snode == NULL){
        return false;
    }
    if(item->type == cxTouchTypeUp && snode->isValidIdx){
        NodeUpdateIdx(snode);
    }
    cxBool isSelected = hasSelectNode(this);
    if(item->type == cxTouchTypeUp && !isSelected){
        NodeResetPosition(snode);
        cxViewSetColor(snode, cxRED);
        this->node = NULL;
    }
    return false;
}

cxBool MapInit(cxAny pmap,cxJson data)
{
    CX_ASSERT_THIS(pmap, Map);

    cxEngine engine = cxEngineInstance();
    
    cxSize2f size = cxSize2fv(engine->winsize.w * 1.2f, 0);
    size.h = size.w * 0.75f;
    global.unitSize = cxSize2fv(size.w/global.unitNum.x, size.h/global.unitNum.y);
    cxViewSetSize(this, size);
    cxViewSetSize(this->bullet, size);

    
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

    //按Y位置排序
    MapSortNode(this);
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

static cxBool MapFightTouch(cxAny pview,cxTouchItems *points)
{
    CX_ASSERT_THIS(pview, Map);
    if(points->number != 1){
        return false;
    }
    cxTouchItem item = points->items[0];
    cxVec2f cpos;
    if(!cxViewHitTest(pview, item->position, &cpos)){
        return false;
    }
    if(item->type == cxTouchTypeDown){
        this->isSelectUnit = true;
        return false;
    }
    if(item->type == cxTouchTypeUp && item->movement < 30){
        cxVec2f idx = MapPosToIdx(this, cpos);
        CX_LOGGER("fight mode selected:%f %f",idx.x,idx.y);
        //test
        if(this->tag == 1){
            Attack node = AttackCreate(this, cxSize2fv(1, 1), idx);
            NodeSetLife(node, 200);
            NodeSetPower(node, 20);
            MapAppendNode(node);
        }else if(this->tag == 2){
            Defence node = DefenceCreate(this, cxSize2fv(2, 2), idx);
            cxViewSetColor(node, cxRED);
            NodeSetPower(node, 40);
            NodeSetLife(node, 200);
            MapAppendNode(node);
        }else if(this->tag == 3){
            if(MapNode(this, idx.x, idx.y) != NULL){
                return false;
            }
            Wall node = WallCreate(this, cxSize2fv(1, 1), idx);
            cxViewSetColor(node, cxBLACK);
            NodeSetLife(node, 100);
            MapAppendNode(node);
        }else if(this->tag == 4){
            
        }
        return true;
    }
    return false;
}

static void mapSubType(cxAny dst,cxAny src)
{
    CX_ASSERT_THIS(dst, Map);
    this->tag = cxViewTag(src);
}

static cpBB NodeIndexBB(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    cxFloat l = this->idx.x * global.sideLen;
    cxFloat b = this->idx.y * global.sideLen;
    cxFloat r = (this->idx.x + this->size.w) * global.sideLen;
    cxFloat t = (this->idx.y + this->size.h) * global.sideLen;
    return cpBBNew(l, b, r, t);
}

CX_SETTER_DEF(Map, mode)
{
    cxConstChars mode = cxJsonToConstChars(value);
    if(cxConstCharsHas(mode, "normal")){
        this->mode = MapModeNormal;
        CX_METHOD_SET(CX_TYPE(cxView, this)->Touch, MapEditTouch);
    }else if(cxConstCharsHas(mode, "fight")){
        this->mode = MapModeFight;
        CX_METHOD_SET(CX_TYPE(cxView, this)->Touch, MapFightTouch);
    }
}

static cxBool MapSearchIsAppend(cxAny pstar,cxVec2i *idx)
{
    CX_ASSERT_THIS(pstar, cxAStar);
    MapSearchInfo *info = this->data;
    Map map = CX_TYPE_CAST(Map, info->map);
    cxVec2f index = cxVec2fv(idx->x, idx->y);
    //如果索引超出最大范围
    if(!MapIsValidIdx(map, index)){
        return false;
    }
    Node sx = info->snode;
    Node dx = info->dnode;
    //开始点与搜索点之间距离超过搜索范围
    cxFloat dis = kmVec2DistanceBetween(&sx->idx, &index);
    if(dis > info->dis){
        return false;
    }
    //目标点与搜索点之间距离超过范围
    dis = kmVec2DistanceBetween(&dx->idx, &index);
    if(dis > info->dis){
        return false;
    }
    //
    Node node = MapNode(map, idx->x, idx->y);
    if(node == NULL){
        return true;
    }
    if(node == sx || node == dx){
        return true;
    }
    //如果是阻挡类型
    if(node->type.mainType == NodeTypeBlock && !NodeIsDie(node)){
        //记录第一个阻挡物
        if(info->block == NULL)info->block = node;
        return false;
    }
    return true;
}

static cxInt MapEarlyExit(cxAny pstar, cxInt vcount, cxVec2i *vnode,cxVec2i *gnode)
{
    CX_ASSERT_THIS(pstar, cxAStar);
    MapSearchInfo *info = this->data;
    Map map = CX_TYPE_CAST(Map, info->map);
    Node sx = info->snode;
    Node dx = info->dnode;
    cxVec2f p1 = MapIdxToPos(map, cxVec2fv(vnode->x, vnode->y));
    cxVec2f p2 = cxViewPosition(dx);
    if(NodeArriveAttackWithPoint(sx, dx, p1, p2)){
        return 1;
    }
    return 0;
}

CX_OBJECT_TYPE(Map, cxAtlas)
{
    CX_PROPERTY_SETTER(Map, mode);
}
CX_OBJECT_INIT(Map, cxAtlas)
{
    //当选中按钮执行
    cxMessageAppend(this, mapSubType, "selectSubType");
    
    CX_EVENT_APPEND(CX_TYPE(cxView, this)->onUpdate, MapUpdate);
    this->mode = MapModeNormal;
    this->isSort = true;
    
    //路劲搜索算法
    this->astar = CX_ALLOC(cxAStar);
    cxAStarSetType(this->astar, cxAStarTypeA8);
    CX_METHOD_SET(this->astar->IsAppend, MapSearchIsAppend);
    CX_METHOD_SET(this->astar->EarlyExit, MapEarlyExit);
    //类型搜索索引
    cxInt cells = global.unitNum.x * global.unitNum.y * 2;
    cxFloat dim = global.sideLen;
    this->items = cxSpatialAlloc(dim, cells, NodeIndexBB);
    
    //弹药效果层
    this->bullet = CX_CREATE(cxView);
    cxViewAppend(this, this->bullet);
    
    //所有节点存储
    this->nodes = CX_ALLOC(cxHash);
    
    //路径索引
    this->paths = CX_ALLOC(cxHash);
}
CX_OBJECT_FREE(Map, cxAtlas)
{
    cxMessageRemove(this);
    CX_RELEASE(this->paths);
    CX_RELEASE(this->items);
    CX_RELEASE(this->bullet);
    CX_RELEASE(this->astar);
    CX_RELEASE(this->nodes);
}
CX_OBJECT_TERM(Map, cxAtlas)

//是否可以放置node
cxBool MapCanSetNode(cxAny node)
{
    CX_ASSERT_THIS(node, Node);
    Map map = NodeMap(this);
    cxVec2i idx = NodeIndex(this);
    cxSize2i size = NodeSize(this);
    for(cxInt x = idx.x; x < idx.x + size.w; x++)
    for(cxInt y = idx.y; y < idx.y + size.h; y++) {
        if(MapItem(map, cxVec2fv(x, y)) != NULL){
            return false;
        }
    }
    return true;
}

void MapAppendNode(cxAny node)
{
    CX_ASSERT_THIS(node, Node);
    Map map = NodeMap(this);
    cxViewAppend(map, node);
    cxSpatialInsert(map->items, node);
}

void MapRemoveNode(cxAny node)
{
    CX_ASSERT_THIS(node, Node);
    Map map = NodeMap(this);
    cxSpatialRemove(map->items, node);
    //从nodes列表分离
    MapDetachNode(node);
    cxViewRemove(node);
}

typedef struct {
    cxAny node;         //最近的view
    cxVec2f idx;        //与此点的
    cxFloat dis;        //距离
    cxRange2f range;    //范围内
    NodeCombined type;
} NodeNearestInfo;

typedef struct {
    cxAny node;         //最近的view
    cxVec2f a;          //第1点
    cxVec2f b;          //第2点
    cxFloat dis;        //距离
    cxFloat exit;       //退出值
    NodeCombined type;  //搜索的类型
} NodeSegmentInfo;

static cpCollisionID NodeIndexQueryFunc(cxAny ps, cxAny pview, cpCollisionID id, void *data)
{
    NodeNearestInfo *info = data;
    Node node = CX_TYPE_CAST(Node, pview);
    //不匹配类型
    if(info->type.mainType != NodeTypeNone && !(info->type.mainType & node->type.mainType)){
        return id;
    }
    //不匹配子类型
    if(info->type.subType != NodeSubTypeNone && !(info->type.subType & node->type.subType)){
        return id;
    }
    //死了的不参与搜索
    if(NodeIsDie(node)){
        return id;
    }
    //计算距离
    cxVec2f tidx = cxVec2fv(node->idx.x * global.sideLen, node->idx.y * global.sideLen);
    cxFloat d = kmVec2DistanceBetween(&info->idx, &tidx);
    if(d > info->dis){
        return id;
    }
    info->dis = d;
    //在范围内
    if(info->dis >= info->range.min && info->dis <= info->range.max){
        info->node = node;
    }
    return id;
}

static cpFloat NodeSegmentQueryFunc(cxAny ps, cxAny pview, void *data)
{
    NodeSegmentInfo *info = data;
    Node node = CX_TYPE_CAST(Node, pview);
    //不匹配类型
    if(info->type.mainType != NodeTypeNone && !(info->type.mainType & node->type.mainType)){
        return info->exit;
    }
    //不匹配子类型
    if(info->type.subType != NodeSubTypeNone && !(info->type.subType & node->type.subType)){
        return info->exit;
    }
    cxVec2f tidx = SCALE_VEC2F(node->idx);
    //计算a点距离
    cxFloat ad = kmVec2DistanceBetween(&info->a, &tidx);
    //计算b点距离
    cxFloat bd = 0;//kmVec2DistanceBetween(&info->b, &tidx);
    //距离和
    cxFloat dis = ad + bd;
    if(dis > info->dis){
        return info->exit;
    }
    info->dis = dis;
    info->node = node;
    return info->exit;
}

static cxAny NodeSegment(cxAny ps,cxVec2f a,cxVec2f b,NodeCombined type)
{
    CX_ASSERT_THIS(ps, cxSpatial);
    NodeSegmentInfo ret = {0};
    ret.a = SCALE_VEC2F(a);
    ret.b = SCALE_VEC2F(b);
    ret.dis = INT32_MAX;
    ret.type = type;
    ret.exit = 1.0f;
    cpSpatialIndexSegmentQuery(this->index, this, ret.a, ret.b, ret.exit, NodeSegmentQueryFunc, &ret);
    return ret.node;
}

static cxAny NodeNearest(cxAny ps,cxVec2f idx,cxRange2f range,NodeCombined type)
{
    CX_ASSERT_THIS(ps, cxSpatial);
    NodeNearestInfo ret = {0};
    ret.idx = SCALE_VEC2F(idx);
    ret.dis = INT32_MAX;
    ret.range = SCALE_RANGE(range);
    ret.type = type;
    cpBB bb = cpBBNewForCircle(ret.idx, cpfmax(ret.range.max, 0.0f));
    cpSpatialIndexQuery(this->index, this, bb, NodeIndexQueryFunc, &ret);
    return ret.node;
}

//搜索离curr最近的单位
cxAny MapNearestQuery(cxAny curr,NodeCombined type)
{
    CX_ASSERT_THIS(curr, Node);
    Map map = NodeMap(this);
    return NodeNearest(map->items, this->idx, type.range, type);
}

//搜索src dst之间的单位
cxAny MapSegmentQuery(cxAny src,cxAny dst,NodeCombined type)
{
    Node sp = CX_TYPE_CAST(Node, src);
    Node dp = CX_TYPE_CAST(Node, dst);
    Map map = NodeMap(sp);
    return NodeSegment(map->items, sp->idx, dp->idx, type);
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

#define CACHE_PATH_IDX(a,b) ((a.y * global.unitNum.x + a.x) << 16) + (b.y * global.unitNum.x + b.x)

void MapCacheDelPath(cxAny pmap,cxVec2i a,cxVec2i b)
{
    CX_ASSERT_THIS(pmap, Map);
    cxInt idx = CACHE_PATH_IDX(a,b);
    cxHashKey key = cxHashLongKey(idx);
    cxHashDel(this->paths, key);
}

void MapCacheSetPath(cxAny pmap,cxVec2i a,cxVec2i b,cxAnyArray path)
{
    CX_ASSERT_THIS(pmap, Map);
    CX_ASSERT(cxAnyArrayLength(path) > 0, "save empty path points");
    cxInt idx = CACHE_PATH_IDX(a,b);
    cxHashKey key = cxHashLongKey(idx);
    //复制一份保存
    cxHashSet(this->paths, key, cxAnyArrayClone(path));
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

PathResult MapSearchPath(cxAny snode,cxAny dnode)
{
    PathResult ret = {false,NULL};
    Map map = NodeMap(snode);
    Node sx = CX_TYPE_CAST(Node, snode);
    Node dx = CX_TYPE_CAST(Node, dnode);
    //获取离snode最近的dnode本地点
    cxVec2f didx = NodeNearestPoint(dnode, sx->idx);
    cxVec2i a = cxVec2iv(sx->idx.x, sx->idx.y);
    cxVec2i b = cxVec2iv(didx.x, didx.y);
    //获取缓存路径
    if(MapCachePath(map, a, b)){
        ret.success = true;
        goto completed;
    }
    //
    MapSearchInfo info = {NULL};
    //设置搜索参数
    info.dis = kmVec2DistanceBetween(&sx->idx, &dx->idx);
    info.map = map;
    info.snode = snode;
    info.dnode = dnode;
    //开始搜索,如果成功保存路径到缓存
    if(cxAStarRun(map->astar, a, b, &info)) {
        MapCacheSetPath(map, a, b, map->astar->points);
        ret.success = true;
        goto completed;
    }
    ret.block = info.block;
completed:
    return ret;
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

cxAny MapNode(cxAny pmap,cxInt x,cxInt y)
{
    CX_ASSERT_THIS(pmap, Map);
    cxHashKey key = cxHashIntKey(y * global.unitNum.x + x);
    return cxHashGet(this->nodes, key);
}
                            
cxAny MapItem(cxAny pmap,cxVec2f idx)
{
    CX_ASSERT_THIS(pmap, Map);
    if(!MapIsValidIdx(this, idx)){
        return NULL;
    }
    return MapNode(pmap, idx.x, idx.y);
}

void MapDetachNode(cxAny node)
{
    CX_ASSERT_THIS(node, Node);
    Map map = CX_TYPE_CAST(Map, this->map);
    cxSize2i size = NodeSize(node);
    cxVec2i curr = NodeIndex(node);
    for(cxInt x = curr.x; x < curr.x + size.w; x++)
    for(cxInt y = curr.y; y < curr.y + size.h; y++){
        MapDelNode(map, x, y);
    }
}

void MapFillNode(cxAny pmap,cxVec2i idx,cxAny node)
{
    CX_ASSERT_THIS(pmap, Map);
    CX_ASSERT_TYPE(node, Node);
    Node snode = node;
    cxSize2i size = NodeSize(node);
    if(!MapIsValidIdx(this, snode->idx)){
        goto setnewpos;
    }
    MapDetachNode(node);
setnewpos:
    //将建筑设置到新位置
    for(cxInt x = idx.x; x < idx.x + size.w; x++)
    for(cxInt y = idx.y; y < idx.y + size.h; y++) {
        MapSetNode(this, x, y, node);
    }
}

cxVec2f MapPosToIdx(cxAny pmap,cxVec2f pos)
{
    CX_ASSERT_THIS(pmap, Map);
    cxSize2f size = cxViewSize(this);
    pos.y += (size.h - global.unitSize.h)/2.0f;
    return cxTilePosToIdx(pos, global.unitSize);
}

cxBool MapIsValidIdx(cxAny pmap,cxVec2f idx)
{
    return idx.x >= 0 && idx.x < global.unitNum.x && idx.y >= 0 && idx.y < global.unitNum.y;
}

cxVec2f MapIdxToPos(cxAny pmap,cxVec2f idx)
{
    CX_ASSERT_THIS(pmap, Map);
    cxSize2f size = cxViewSize(this);
    cxVec2f pos = cxTileIdxToPos(idx, global.unitSize);
    pos.y -= (size.h - global.unitSize.h)/2.0f;
    return pos;
}











