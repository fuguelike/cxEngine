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

#ifndef AStar_h
#define AStar_h

#include <cxcore/cxBase.h>

CX_C_BEGIN

typedef enum {
    cxAStarTypeCC,  //coustom
    cxAStarTypeA8,  //8 angle
    cxAStarTypeA4   //4 angle
}cxAStarType;

CX_OBJECT_DEF(cxAStar, cxObject)
    cxAStarType type;
    CX_METHOD_DEF(void, Neighbors, cxAny, cxAny list, cxVec2i *node);
    CX_METHOD_DEF(cxFloat, Heuristic, cxAny, cxVec2i *from,cxVec2i *to);
    CX_METHOD_DEF(cxInt, EarlyExit, cxAny, cxInt vcount, cxVec2i *vnode,cxVec2i *gnode);
    CX_METHOD_DEF(cxInt, Comparator, cxAny, cxVec2i *lv,cxVec2i *rv);
    CX_METHOD_DEF(cxBool, IsAppend, cxAny, cxVec2i *node);
    cxAny data;
    cxAnyArray points;
CX_OBJECT_END(cxAStar, cxObject)

void cxAStarSetType(cxAny pobj,cxAStarType type);

void cxAStarNeighbors(cxAny pbj, cxAny list, cxVec2i *node);

//use at Neighbors
void cxAStarAppendNeighbors(cxAny list,cxVec2i point,cxFloat edgeCost);

void cxAStarPrintPoints(cxAny pobj);

cxAnyArray cxAStarRun(cxAny pobj,cxVec2i from,cxVec2i to,cxAny data);

CX_C_END

#endif
