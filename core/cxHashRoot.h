//
//  cxHashRoot.h
//  cxEngine
//
//  Created by xuhua on 10/19/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxHashRoot_h
#define cxEngine_cxHashRoot_h

#include "cxBase.h"
#include "cxHash.h"
#include "cxXMLScript.h"

CX_C_BEGIN

CX_OBJECT_DEF(cxHashRoot, cxObject)
    cxHash items;
    cxBool isError;
CX_OBJECT_END(cxHashRoot)

cxTypes cxHashRootReadString(cxHashRoot root,xmlTextReaderPtr reader);

void cxHashRootReadDBEnv(cxHashRoot root,xmlTextReaderPtr reader);

cxBool cxHashRootLoad(cxAny root,cxConstChars xml);

cxHashRoot cxHashRootCreate(cxConstChars xml);

cxTypes cxHashRootReadNumber(cxHashRoot root,cxConstChars temp,xmlTextReaderPtr reader);

cxBool cxHashRootLoadWithReader(cxHashRoot root,xmlTextReaderPtr reader);

cxTypes cxHashRootReadHash(cxHashRoot root,xmlTextReaderPtr reader);

cxTypes cxHashRootReadArray(cxHashRoot root,xmlTextReaderPtr reader);

CX_C_END

#endif
