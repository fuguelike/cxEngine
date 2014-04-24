//
//  cxTypes.c
//  cxEngine
//
//  Created by xuhua on 9/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxHash.h"
#include "cxTypes.h"
#include "cxArray.h"
#include "cxString.h"
#include "cxNumber.h"

CX_OBJECT_INIT(cxTypes, cxObject)
{
    
}
CX_OBJECT_FREE(cxTypes, cxObject)
{
    CX_RELEASE(this->any);
}
CX_OBJECT_TERM(cxType, cxObject)

cxTypes cxStringTypesCreate()
{
    cxTypes this = CX_CREATE(cxTypes);
    this->type = cxTypesString;
    return this;
}

cxTypes cxHashTypesCreate()
{
    cxTypes this = CX_CREATE(cxTypes);
    this->type = cxTypesHash;
    this->any = CX_ALLOC(cxHash);
    return this;
}

cxTypes cxNumberTypesCreate()
{
    cxTypes this = CX_CREATE(cxTypes);
    this->type = cxTypesNumber;
    return this;
}

cxTypes cxArrayTypesCreate()
{
    cxTypes this = CX_CREATE(cxTypes);
    this->type = cxTypesArray;
    this->any = CX_ALLOC(cxArray);
    return this;
}

cxTypes cxDBTypesCreate(cxAny db)
{
    cxTypes this = CX_CREATE(cxTypes);
    this->type = cxTypesDB;
    CX_RETAIN_SWAP(this->any, db);
    return this;
}

