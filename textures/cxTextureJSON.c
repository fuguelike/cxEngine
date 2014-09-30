//
//  cxTextureXML.c
//  cxEngine
//
//  Created by xuhua on 10/11/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <streams/cxAssetsStream.h>
#include "cxTextureJSON.h"
#include "cxTextureFactory.h"

static cxBool cxTextureJSONLoad(cxAny ptex,cxStream stream)
{
    cxBool ret = false;
    CX_ASSERT_THIS(ptex, cxTextureJSON);
    CX_ASSERT(stream != NULL, "stream not set");
    cxString data = cxStreamAllBytes(stream);
    if(data == NULL){
        CX_ERROR("read data failed from stream");
        return ret;
    }
    cxJson json = cxJsonCreate(data);
    if(json == NULL){
        CX_ERROR("create json error");
        return ret;
    }
    cxJson meta = cxJsonObject(json, "meta");
    if(meta == NULL){
        CX_ERROR("read json meta object error");
        return ret;
    }
    cxConstChars alphaPath = cxJsonConstChars(meta, "alpha");
    if(alphaPath != NULL){
        CX_RETAIN_SET(this->alphaTexture, cxTextureFactoryLoadFile(alphaPath));
    }
    cxConstChars simagePath = cxJsonConstChars(meta, "image");
    if(simagePath != NULL){
        this->innerTexture = cxTextureFactoryLoadFile(simagePath);
    }
    ret = (this->innerTexture != NULL);
    CX_ASSERT(ret, "get innerTexture error");
    CX_RETAIN(this->innerTexture);
    //for jpg pkm atlas texture
    cxSize2f metasiz = cxJsonSize2f(meta, "size", this->innerTexture->size);
    this->cxTexture.size = this->innerTexture->size;
    this->cxTexture.scale.x = this->cxTexture.size.w / metasiz.w;
    this->cxTexture.scale.y = this->cxTexture.size.h / metasiz.h;
    //parse frames
    cxJson frames = cxJsonArray(json, "frames");
    CX_JSON_ARRAY_EACH_BEG(frames, item)
    {
        cxTexCoord e = CX_ALLOC(cxTexCoord);
        cxConstChars sn = cxJsonConstChars(item, "filename");
        CX_ASSERT(sn != NULL, "filename node miss");
        e->isRotation = cxJsonBool(item, "rotated", false);
        cxJson frame = cxJsonObject(item, "frame");
        CX_ASSERT(frame != NULL, "frame node miss");
        e->x = cxJsonDouble(frame, "x", 0) / this->cxTexture.scale.x;
        e->y = cxJsonDouble(frame, "y", 0) / this->cxTexture.scale.y;
        e->w = cxJsonDouble(frame, "w", 0) / this->cxTexture.scale.x;
        e->h = cxJsonDouble(frame, "h", 0) / this->cxTexture.scale.y;
        cxHashSet(this->cxTexture.keys, cxHashStrKey(sn), e);
        CX_RELEASE(e);
    }
    CX_JSON_ARRAY_EACH_END(frames, item);
    cxConstChars shader = cxJsonConstChars(meta, "shader");
    CX_RETAIN_SET(this->cxTexture.shader, cxOpenGLShaderByName(shader));
    return ret;
}

static void cxTextureJSONBind(cxAny this)
{
    cxTextureJSON json = this;
    if(json->alphaTexture != NULL){
        cxOpenGLBindTexture(json->innerTexture->textureId, 1);
        cxOpenGLBindTexture(json->alphaTexture->textureId, 2);
    }else{
        cxOpenGLBindTexture(json->innerTexture->textureId, 0);
    }
}

CX_OBJECT_TYPE(cxTextureJSON, cxTexture)
{
    
}
CX_OBJECT_INIT(cxTextureJSON, cxTexture)
{
    CX_SET(cxTexture, this, Bind, cxTextureJSONBind);
    CX_SET(cxTexture, this, Load, cxTextureJSONLoad);
}
CX_OBJECT_FREE(cxTextureJSON, cxTexture)
{
    CX_RELEASE(this->alphaTexture);
    CX_RELEASE(this->innerTexture);
}
CX_OBJECT_TERM(cxTextureJSON, cxTexture)

cxTexture cxTextureJSONLoadStream(cxStream stream)
{
    cxTexture json = CX_CREATE(cxTextureJSON);
    cxTextureLoad(json,stream);
    return json;
}



