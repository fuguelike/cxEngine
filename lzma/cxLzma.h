//
//  cxLzma.h
//  cxEngine
//
//  Created by xuhua on 10/30/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxEngine_cxLzma_h
#define cxEngine_cxLzma_h

#include <cxcore/cxBase.h>

CX_C_BEGIN

void cxLzmaSetFlags(cxString flags);

cxString cxLzmaCompress(cxString data);

cxString cxLzmaUncompress(cxString data);

CX_C_END

#endif
