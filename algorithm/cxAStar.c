//
//  cxAStar.c
//  cxEngine
//
//  Created by xuhua on 1/25/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxAStar.h"

CX_OBJECT_DEF(cxANode, cxObject)

CX_OBJECT_END(cxANode)

CX_OBJECT_INIT(cxANode, cxObject)
{
    
}
CX_OBJECT_FREE(cxANode, cxObject)
{
    
}
CX_OBJECT_TERM(cxANode, cxObject)


CX_OBJECT_INIT(cxAStar, cxObject)
{
    
}
CX_OBJECT_FREE(cxAStar, cxObject)
{
    
}
CX_OBJECT_TERM(cxAStar, cxObject)

cxBool cxAStarRun(cxAStar this,cxVec2i src,cxVec2i dst)
{
    this->src = src;
    this->dst = dst;
    if(cxVec2iEqu(this->src, this->dst)){
        return false;
    }
    return false;
}

cxAStar cxAStarCreate(cxVec2i max)
{
    cxAStar this = CX_CREATE(cxAStar);
    this->max = max;
    return this;
}

