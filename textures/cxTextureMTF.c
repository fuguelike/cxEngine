//
//  cxTextureMTF.c
//  cxEngineStatic
//
//  Created by xuhua on 2/25/15.
//  Copyright (c) 2015 xuhua. All rights reserved.
//

#include <streams/cxAssetsStream.h>
#include <streams/cxMemStream.h>
#include <textures/cxTexturePVR.h>
#include <textures/cxTexturePKM.h>
#include "cxTextureMTF.h"

CX_TYPE(cxTextureMTF, cxTexture)
{
    
}
CX_INIT(cxTextureMTF, cxTexture)
{
    this->caches = CX_ALLOC(cxHash);
}
CX_FREE(cxTextureMTF, cxTexture)
{
    CX_RELEASE(this->caches);
}
CX_TERM(cxTextureMTF, cxTexture)

cxTexture cxTextureMTFGet(cxAny pthis,cxInt idx)
{
    CX_ASSERT_THIS(pthis, cxTextureMTF);
    CX_ASSERT(idx >= 0 && idx < this->header.count, "idx range");
    return cxHashGet(this->caches, cxHashIntKey(idx));
}

static cxBool cxTextureMTFMake(cxAny pthis,cxStr data,cxInt idx)
{
    CX_ASSERT_THIS(pthis, cxTextureMTF);
    CX_ASSERT(idx >= 0 && idx < this->header.count, "idx range");
    cxTexture ret = NULL;
    cxInt off = sizeof(cxTextureMTFHeader) + idx * this->header.length;
    cxStr d = cxStrNoCopy(cxStrBody(data) + off, this->header.length);
    if (d == NULL) {
        return false;
    }
    cxStream stream = cxMemStreamCreateRefStr(d);
    if(this->header.type == 1){
        ret = cxTexturePVRLoadStream(stream);
    }
    if(this->header.type == 2){
        ret = cxTexturePKMLoadStream(stream);
    }
    if(ret == NULL){
        return false;
    }
    cxHashSet(this->caches, cxHashIntKey(idx), ret);
    return true;
}
cxTexture cxTextureMTFCreate(cxConstChars file)
{
    cxTextureMTF this = CX_CREATE(cxTextureMTF);
    cxStr data = cxAssetsData(file);
    if(!cxStrOK(data)){
        return NULL;
    }
    //set header
    memcpy(&this->header, cxStrBody(data), sizeof(cxTextureMTFHeader));
    //load all texture
    for(cxInt i=0; i < this->header.count;i++){
        cxTextureMTFMake(this, data, i);
    }
    return (cxTexture)this;
}







