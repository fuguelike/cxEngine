/*
 Copyright (c) 2012, Sean Heber. All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 
 1. Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 
 2. Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 
 3. Neither the name of Sean Heber nor the names of its contributors may
 be used to endorse or promote products derived from this software without
 specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL SEAN HEBER BE LIABLE FOR ANY DIRECT,
 INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "cxAStar.h"
#include <math.h>
#include <string.h>

typedef struct __ASNeighborList *ASNeighborList;

typedef struct {
    size_t  nodeSize;                                                                               // the size of the structure being used for the nodes - important since nodes are copied into the resulting path
    void    (*nodeNeighbors)(ASNeighborList neighbors, void *node, void *context);                  // add nodes to the neighbor list if they are connected to this node
    float   (*pathCostHeuristic)(void *fromNode, void *toNode, void *context);                      // estimated cost to transition from the first node to the second node -- optional, uses 0 if not specified
    int     (*earlyExit)(size_t visitedCount, void *visitingNode, void *goalNode, void *context);   // early termination, return 1 for success, -1 for failure, 0 to continue searching -- optional
    int     (*nodeComparator)(void *node1, void *node2, void *context);                             // must return a sort order for the nodes (-1, 0, 1) -- optional, uses memcmp if not specified
} ASPathNodeSource;

// use in the nodeNeighbors callback to return neighbors
void ASNeighborListAdd(ASNeighborList neighbors, void *node, float edgeCost);

// if goalNode is NULL, it searches the entire graph and returns the cheapest deepest path
// context is optional and is simply passed through to the callback functions
// startNode and nodeSource is required
// as a path is created, the relevant nodes are copied into the path
cxBool ASPathCreate(const ASPathNodeSource *nodeSource, void *context, void *startNode, void *goalNode,cxAnyArray points);

static void AStarPathNodeNeighbors(ASNeighborList neighbors, void *node, void *context)
{
    CX_ASSERT_THIS(context, cxAStar);
    CX_CALL(this, Neighbors, CX_M(void,cxAny,cxVec2i *),neighbors,node);
}

static float AStarPathNodeHeuristic(void *fromNode, void *toNode, void *context)
{
    CX_ASSERT_THIS(context, cxAStar);
    return CX_CALL(this, Heuristic, CX_M(cxFloat,cxVec2i *,cxVec2i *),fromNode,toNode);
}

static int AStarEarlyExit(size_t visitedCount, void *visitingNode, void *goalNode, void *context)
{
    CX_ASSERT_THIS(context, cxAStar);
    cxAnyArrayAppend(this->visits, visitingNode);
    return CX_CALL(this, EarlyExit, CX_M(cxInt,cxInt,cxVec2i *,cxVec2i *),(cxInt)visitedCount,visitingNode,goalNode);
}

static int AStarNodeComparator(void *node1, void *node2, void *context)
{
    CX_ASSERT_THIS(context, cxAStar);
    return CX_CALL(this, Comparator, CX_M(cxInt,cxVec2i *,cxVec2i *),node1,node2);
}

static const ASPathNodeSource cxAStarSource =
{
    sizeof(cxVec2i),
    &AStarPathNodeNeighbors,
    &AStarPathNodeHeuristic,
    &AStarEarlyExit,
    &AStarNodeComparator
};

void cxAStarAppendNeighbors(cxAny list,cxVec2i point,cxFloat edgeCost)
{
    ASNeighborListAdd(list, &point, edgeCost);
}

CX_METHOD_DEF(cxAStar, Neighbors, void, cxAny list, cxVec2i *node)
{
    cxVec2i right = cxVec2iv(node->x + 1, node->y);
    if(CX_CALL(this, IsAppend, CX_M(cxBool,cxVec2i *),&right)){
        cxAStarAppendNeighbors(list, right, 1);
    }
    cxVec2i left = cxVec2iv(node->x - 1, node->y);
    if(CX_CALL(this, IsAppend, CX_M(cxBool,cxVec2i *),&left)){
        cxAStarAppendNeighbors(list, left, 1);
    }
    cxVec2i up = cxVec2iv(node->x, node->y + 1);
    if(CX_CALL(this, IsAppend, CX_M(cxBool,cxVec2i *),&up)){
        cxAStarAppendNeighbors(list, up, 1);
    }
    cxVec2i down = cxVec2iv(node->x, node->y - 1);
    if(CX_CALL(this, IsAppend, CX_M(cxBool,cxVec2i *),&down)){
        cxAStarAppendNeighbors(list, down, 1);
    }
    if(this->Type == cxAStarTypeA4){
        return;
    }
    cxVec2i leftUp = cxVec2iv(node->x - 1, node->y + 1);
    if(CX_CALL(this, IsAppend, CX_M(cxBool,cxVec2i *),&leftUp)){
        cxAStarAppendNeighbors(list, leftUp, 1);
    }
    cxVec2i leftDown = cxVec2iv(node->x - 1, node->y - 1);
    if(CX_CALL(this, IsAppend, CX_M(cxBool,cxVec2i *),&leftDown)){
        cxAStarAppendNeighbors(list, leftDown, 1);
    }
    cxVec2i rightUp = cxVec2iv(node->x + 1, node->y + 1);
    if(CX_CALL(this, IsAppend, CX_M(cxBool,cxVec2i *),&rightUp)){
        cxAStarAppendNeighbors(list, rightUp, 1);
    }
    cxVec2i rightDown = cxVec2iv(node->x + 1, node->y - 1);
    if(CX_CALL(this, IsAppend, CX_M(cxBool,cxVec2i *),&rightDown)){
        cxAStarAppendNeighbors(list, rightDown, 1);
    }
}
CX_METHOD_DEF(cxAStar, Heuristic, cxFloat, cxVec2i *from,cxVec2i *to)
{
    return (fabs(from->x - to->x) + fabs(from->y - to->y));
}
//返回值:>0成功到达,=0继续,<0失败退出
CX_METHOD_DEF(cxAStar, EarlyExit, cxInt, cxInt vcount, cxVec2i *curr,cxVec2i *target)
{
    return 0;
}
CX_METHOD_DEF(cxAStar, Comparator, cxInt, cxVec2i *lv,cxVec2i *rv)
{
    return memcmp(lv, rv, sizeof(cxVec2i));
}
CX_METHOD_DEF(cxAStar, IsAppend, cxBool, cxVec2i *curr)
{
    return false;
}
CX_TYPE(cxAStar, cxObject)
{
    CX_METHOD(cxAStar, IsAppend);
    CX_METHOD(cxAStar, Comparator);
    CX_METHOD(cxAStar, EarlyExit);
    CX_METHOD(cxAStar, Heuristic);
    CX_METHOD(cxAStar, Neighbors);
}
CX_INIT(cxAStar, cxObject)
{
    this->points = cxAnyArrayAlloc(cxVec2i);
    this->visits = cxAnyArrayAlloc(cxVec2i);
    this->Type = cxAStarTypeA8;
}
CX_FREE(cxAStar, cxObject)
{
    CX_RELEASE(this->visits);
    CX_RELEASE(this->points);
}
CX_TERM(cxAStar, cxObject)

void cxAStarPrintPoints(cxAny pobj)
{
    cxAStar this = pobj;
    cxInt i = 0;
    CX_ANY_ARRAY_FOREACH(this->points, point, cxVec2i){
        CX_LOGGER("%d x=%d,y=%d",i,point->x,point->y);
        i++;
    }
}

void cxAStarClearPath(cxAny pobj)
{
    CX_ASSERT_THIS(pobj, cxAStar);
    cxAnyArrayClear(this->points);
    cxAnyArrayClear(this->visits);
}

cxBool cxAStarRun(cxAny pobj,cxVec2i from,cxVec2i to,cxAny data)
{
    CX_ASSERT_THIS(pobj, cxAStar);
    cxObjectSetUserData(this, data);
    cxAStarClearPath(this);
    return ASPathCreate(&cxAStarSource, pobj, &from, &to, this->points);
}


struct __ASNeighborList {
    const ASPathNodeSource *source;
    size_t capacity;
    size_t count;
    float *costs;
    void *nodeKeys;
};

struct __ASPath {
    size_t nodeSize;
    size_t count;
    float cost;
    int8_t nodeKeys[];
};

typedef struct {
    unsigned isClosed:1;
    unsigned isOpen:1;
    unsigned isGoal:1;
    unsigned hasParent:1;
    unsigned hasEstimatedCost:1;
    float estimatedCost;
    float cost;
    size_t openIndex;
    size_t parentIndex;
    int8_t nodeKey[];
} NodeRecord;

struct __VisitedNodes {
    const ASPathNodeSource *source;
    void *context;
    size_t nodeRecordsCapacity;
    size_t nodeRecordsCount;
    void *nodeRecords;
    size_t *nodeRecordsIndex;
    size_t openNodesCapacity;
    size_t openNodesCount;
    size_t *openNodes;
};
typedef struct __VisitedNodes *VisitedNodes;

typedef struct {
    VisitedNodes nodes;
    size_t index;
} Node;

static const Node NodeNull = {NULL, -1};

static inline VisitedNodes VisitedNodesCreate(const ASPathNodeSource *source, void *context)
{
    VisitedNodes nodes =  allocator->calloc(1, sizeof(struct __VisitedNodes));
    nodes->source = source;
    nodes->context = context;
    return nodes;
}

static inline void VisitedNodesDestroy(VisitedNodes visitedNodes)
{
    allocator->free(visitedNodes->nodeRecordsIndex);
    allocator->free(visitedNodes->nodeRecords);
    allocator->free(visitedNodes->openNodes);
    allocator->free(visitedNodes);
}

static inline int NodeIsNull(Node n)
{
    return (n.nodes == NodeNull.nodes) && (n.index == NodeNull.index);
}

static inline Node NodeMake(VisitedNodes nodes, size_t index)
{
    return (Node){nodes, index};
}

static inline NodeRecord *NodeGetRecord(Node node)
{
    return node.nodes->nodeRecords + (node.index * (node.nodes->source->nodeSize + sizeof(NodeRecord)));
}

static inline void *GetNodeKey(Node node)
{
    return NodeGetRecord(node)->nodeKey;
}

static inline int NodeIsInOpenSet(Node n)
{
    return NodeGetRecord(n)->isOpen;
}

static inline int NodeIsInClosedSet(Node n)
{
    return NodeGetRecord(n)->isClosed;
}

static inline void RemoveNodeFromClosedSet(Node n)
{
    NodeGetRecord(n)->isClosed = 0;
}

static inline void AddNodeToClosedSet(Node n)
{
    NodeGetRecord(n)->isClosed = 1;
}

static inline float GetNodeRank(Node n)
{
    NodeRecord *record = NodeGetRecord(n);
    return record->estimatedCost + record->cost;
}

static inline float GetNodeCost(Node n)
{
    return NodeGetRecord(n)->cost;
}

//static inline float GetNodeEstimatedCost(Node n)
//{
//    return NodeGetRecord(n)->estimatedCost;
//}

static inline void SetNodeEstimatedCost(Node n, float estimatedCost)
{
    NodeRecord *record = NodeGetRecord(n);
    record->estimatedCost = estimatedCost;
    record->hasEstimatedCost = 1;
}

static inline int NodeHasEstimatedCost(Node n)
{
    return NodeGetRecord(n)->hasEstimatedCost;
}

static inline void SetNodeIsGoal(Node n)
{
    if (!NodeIsNull(n)) {
        NodeGetRecord(n)->isGoal = 1;
    }
}

static inline int NodeIsGoal(Node n)
{
    return !NodeIsNull(n) && NodeGetRecord(n)->isGoal;
}

static inline Node GetParentNode(Node n)
{
    NodeRecord *record = NodeGetRecord(n);
    if (record->hasParent) {
        return NodeMake(n.nodes, record->parentIndex);
    } else {
        return NodeNull;
    }
}

static inline int NodeRankCompare(Node n1, Node n2)
{
    const float rank1 = GetNodeRank(n1);
    const float rank2 = GetNodeRank(n2);
    if (rank1 < rank2) {
        return -1;
    } else if (rank1 > rank2) {
        return 1;
    } else {
        return 0;
    }
}

static inline float GetPathCostHeuristic(Node a, Node b)
{
    if (a.nodes->source->pathCostHeuristic && !NodeIsNull(a) && !NodeIsNull(b)) {
        return a.nodes->source->pathCostHeuristic(GetNodeKey(a), GetNodeKey(b), a.nodes->context);
    } else {
        return 0;
    }
}

static inline int NodeKeyCompare(Node node, void *nodeKey)
{
    if (node.nodes->source->nodeComparator) {
        return node.nodes->source->nodeComparator(GetNodeKey(node), nodeKey, node.nodes->context);
    } else {
        return memcmp(GetNodeKey(node), nodeKey, node.nodes->source->nodeSize);
    }
}

static inline Node GetNode(VisitedNodes nodes, void *nodeKey)
{
    if (!nodeKey) {
        return NodeNull;
    }
    
    size_t first = 0;

    if (nodes->nodeRecordsCount > 0) {
        size_t last = nodes->nodeRecordsCount-1;

        while (first <= last) {
            const size_t mid = (first + last) / 2;
            const int comp = NodeKeyCompare(NodeMake(nodes, nodes->nodeRecordsIndex[mid]), nodeKey);

            if (comp < 0) {
                first = mid + 1;
            } else if (comp > 0 && mid > 0) {
                last = mid - 1;
            } else if (comp > 0) {
                break;
            } else {
                return NodeMake(nodes, nodes->nodeRecordsIndex[mid]);
            }
        }
    }
    
    if (nodes->nodeRecordsCount == nodes->nodeRecordsCapacity) {
        nodes->nodeRecordsCapacity = 1 + (nodes->nodeRecordsCapacity * 2);
        nodes->nodeRecords = allocator->realloc(nodes->nodeRecords, nodes->nodeRecordsCapacity * (sizeof(NodeRecord) + nodes->source->nodeSize));
        nodes->nodeRecordsIndex = allocator->realloc(nodes->nodeRecordsIndex, nodes->nodeRecordsCapacity * sizeof(size_t));
    }
    
    Node node = NodeMake(nodes, nodes->nodeRecordsCount);
    nodes->nodeRecordsCount++;
    
    memmove(&nodes->nodeRecordsIndex[first+1], &nodes->nodeRecordsIndex[first], (nodes->nodeRecordsCapacity - first - 1) * sizeof(size_t));
    nodes->nodeRecordsIndex[first] = node.index;
    
    NodeRecord *record = NodeGetRecord(node);
    memset(record, 0, sizeof(NodeRecord));
    memcpy(record->nodeKey, nodeKey, nodes->source->nodeSize);

    return node;
}

static inline void SwapOpenSetNodesAtIndexes(VisitedNodes nodes, size_t index1, size_t index2)
{
    if (index1 != index2) {
        NodeRecord *record1 = NodeGetRecord(NodeMake(nodes, nodes->openNodes[index1]));
        NodeRecord *record2 = NodeGetRecord(NodeMake(nodes, nodes->openNodes[index2]));
        
        const size_t tempOpenIndex = record1->openIndex;
        record1->openIndex = record2->openIndex;
        record2->openIndex = tempOpenIndex;
        
        const size_t tempNodeIndex = nodes->openNodes[index1];
        nodes->openNodes[index1] = nodes->openNodes[index2];
        nodes->openNodes[index2] = tempNodeIndex;
    }
}

static inline void DidRemoveFromOpenSetAtIndex(VisitedNodes nodes, size_t index)
{
    size_t smallestIndex = index;
    
    do {
        if (smallestIndex != index) {
            SwapOpenSetNodesAtIndexes(nodes, smallestIndex, index);
            index = smallestIndex;
        }

        const size_t leftIndex = (2 * index) + 1;
        const size_t rightIndex = (2 * index) + 2;
        
        if (leftIndex < nodes->openNodesCount && NodeRankCompare(NodeMake(nodes, nodes->openNodes[leftIndex]), NodeMake(nodes, nodes->openNodes[smallestIndex])) < 0) {
            smallestIndex = leftIndex;
        }
        
        if (rightIndex < nodes->openNodesCount && NodeRankCompare(NodeMake(nodes, nodes->openNodes[rightIndex]), NodeMake(nodes, nodes->openNodes[smallestIndex])) < 0) {
            smallestIndex = rightIndex;
        }
    } while (smallestIndex != index);
}

static inline void RemoveNodeFromOpenSet(Node n)
{
    NodeRecord *record = NodeGetRecord(n);

    if (record->isOpen) {
        record->isOpen = 0;
        n.nodes->openNodesCount--;
        
        const size_t index = record->openIndex;
        SwapOpenSetNodesAtIndexes(n.nodes, index, n.nodes->openNodesCount);
        DidRemoveFromOpenSetAtIndex(n.nodes, index);
    }
}

static inline void DidInsertIntoOpenSetAtIndex(VisitedNodes nodes, size_t index)
{
    while (index > 0) {
        const size_t parentIndex = floorf((index-1) / 2);
        
        if (NodeRankCompare(NodeMake(nodes, nodes->openNodes[parentIndex]), NodeMake(nodes, nodes->openNodes[index])) < 0) {
            break;
        } else {
            SwapOpenSetNodesAtIndexes(nodes, parentIndex, index);
            index = parentIndex;
        }
    }
}

static inline void AddNodeToOpenSet(Node n, float cost, Node parent)
{
    NodeRecord *record = NodeGetRecord(n);

    if (!NodeIsNull(parent)) {
        record->hasParent = 1;
        record->parentIndex = parent.index;
    } else {
        record->hasParent = 0;
    }

    if (n.nodes->openNodesCount == n.nodes->openNodesCapacity) {
        n.nodes->openNodesCapacity = 1 + (n.nodes->openNodesCapacity * 2);
        n.nodes->openNodes = allocator->realloc(n.nodes->openNodes, n.nodes->openNodesCapacity * sizeof(size_t));
    }

    const size_t openIndex = n.nodes->openNodesCount;
    n.nodes->openNodes[openIndex] = n.index;
    n.nodes->openNodesCount++;

    record->openIndex = openIndex;
    record->isOpen = 1;
    record->cost = cost;

    DidInsertIntoOpenSetAtIndex(n.nodes, openIndex);
}

static inline int HasOpenNode(VisitedNodes nodes)
{
    return nodes->openNodesCount > 0;
}

static inline Node GetOpenNode(VisitedNodes nodes)
{
    return NodeMake(nodes, nodes->openNodes[0]);
}

static inline ASNeighborList NeighborListCreate(const ASPathNodeSource *source)
{
    ASNeighborList list = allocator->calloc(1, sizeof(struct __ASNeighborList));
    list->source = source;
    return list;
}

static inline void NeighborListDestroy(ASNeighborList list)
{
    allocator->free(list->costs);
    allocator->free(list->nodeKeys);
    allocator->free(list);
}

static inline float NeighborListGetEdgeCost(ASNeighborList list, size_t index)
{
    return list->costs[index];
}

static void *NeighborListGetNodeKey(ASNeighborList list, size_t index)
{
    return list->nodeKeys + (index * list->source->nodeSize);
}

/********************************************/

void ASNeighborListAdd(ASNeighborList list, void *node, float edgeCost)
{
    if (list->count == list->capacity) {
        list->capacity = 1 + (list->capacity * 2);
        list->costs = allocator->realloc(list->costs, sizeof(float) * list->capacity);
        list->nodeKeys = allocator->realloc(list->nodeKeys, list->source->nodeSize * list->capacity);
    }
    list->costs[list->count] = edgeCost;
    memcpy(list->nodeKeys + (list->count * list->source->nodeSize), node, list->source->nodeSize);
    list->count++;
}

cxBool ASPathCreate(const ASPathNodeSource *source, void *context, void *startNodeKey, void *goalNodeKey,cxAnyArray points)
{
    cxBool ret = false;
    if (!startNodeKey || !source || !source->nodeNeighbors || source->nodeSize == 0) {
        return ret;
    }
    VisitedNodes visitedNodes = VisitedNodesCreate(source, context);
    ASNeighborList neighborList = NeighborListCreate(source);
    Node current = GetNode(visitedNodes, startNodeKey);
    Node goalNode = GetNode(visitedNodes, goalNodeKey);
    SetNodeIsGoal(goalNode);
    
    SetNodeEstimatedCost(current,  GetPathCostHeuristic(current, goalNode));
    AddNodeToOpenSet(current, 0, NodeNull);
    
    while (HasOpenNode(visitedNodes) && !NodeIsGoal((current = GetOpenNode(visitedNodes)))) {
        //退出回调
        if (source->earlyExit) {
            const int shouldExit = source->earlyExit(visitedNodes->nodeRecordsCount, GetNodeKey(current), goalNodeKey, context);
            //成功
            if (shouldExit > 0) {
                //成功到达
                SetNodeIsGoal(current);
                break;
            } else if (shouldExit < 0) {
                //失败退出
                break;
            }
        }
        RemoveNodeFromOpenSet(current);
        AddNodeToClosedSet(current);
        neighborList->count = 0;
        source->nodeNeighbors(neighborList, GetNodeKey(current), context);
        for (size_t n=0; n<neighborList->count; n++) {
            const float cost = GetNodeCost(current) + NeighborListGetEdgeCost(neighborList, n);
            Node neighbor = GetNode(visitedNodes, NeighborListGetNodeKey(neighborList, n));
            if (!NodeHasEstimatedCost(neighbor)) {
                SetNodeEstimatedCost(neighbor, GetPathCostHeuristic(neighbor, goalNode));
            }
            if (NodeIsInOpenSet(neighbor) && cost < GetNodeCost(neighbor)) {
                RemoveNodeFromOpenSet(neighbor);
            }
            if (NodeIsInClosedSet(neighbor) && cost < GetNodeCost(neighbor)) {
                RemoveNodeFromClosedSet(neighbor);
            }
            if (!NodeIsInOpenSet(neighbor) && !NodeIsInClosedSet(neighbor)) {
                AddNodeToOpenSet(neighbor, cost, current);
            }
        }
    }
    if (NodeIsNull(goalNode)) {
        SetNodeIsGoal(current);
    }
    //到达目标点
    ret = NodeIsGoal(current);
    //返回获取路径，使用时反向获取
    Node n = current;
    while (!NodeIsNull(n)) {
        cxVec2i *p = GetNodeKey(n);
        cxAnyArrayAppend(points, p);
        n = GetParentNode(n);
    }
    NeighborListDestroy(neighborList);
    VisitedNodesDestroy(visitedNodes);
    return ret;
}
