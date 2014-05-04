//
//  cxTextureXML.c
//  cxEngine
//
//  Created by xuhua on 10/11/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <streams/cxAssetsStream.h>
#include <core/cxJson.h>
#include "cxTextureJSON.h"
#include "cxTextureFactory.h"

static cxBool cxTextureJSONLoad(cxAny this,cxStream stream)
{
    cxBool ret = false;
    cxTextureJSON texture = this;
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
    cxConstChars simagePath = cxJsonConstChars(meta, "image");
    CX_ASSERT(simagePath != NULL, "json image null");
    //make texture
    texture->innerTexture = cxTextureFactoryLoadFile(simagePath);
    ret = (texture->innerTexture != NULL);
    CX_ASSERT(ret, "get innerTexture error");
    CX_RETAIN(texture->innerTexture);
    //for jpg pkm atlas texture
    texture->super.isAtlas = cxJsonBool(meta, "atlas", false);
    texture->super.size = texture->innerTexture->size;
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
        e->x = cxJsonDouble(frame, "x", 0);
        e->y = cxJsonDouble(frame, "y", 0);
        e->w = cxJsonDouble(frame, "w", 0);
        e->h = cxJsonDouble(frame, "h", 0);
        cxHashSet(texture->super.keys, cxHashStrKey(sn), e);
        CX_RELEASE(e);
    }
    CX_JSON_ARRAY_EACH_END(frames, item);
    return ret;
}

static void cxTextureJSONBind(cxAny this)
{
    cxTextureJSON json = this;
    CX_ASSERT(json->innerTexture != NULL, "xml inner texture error");
    cxTextureBind(json->innerTexture);
}

CX_OBJECT_INIT(cxTextureJSON, cxTexture)
{
    CX_METHOD_OVERRIDE(this->super.Bind, cxTextureJSONBind);
    CX_METHOD_OVERRIDE(this->super.Load, cxTextureJSONLoad);
}
CX_OBJECT_FREE(cxTextureJSON, cxTexture)
{
    CX_RELEASE(this->innerTexture);
}
CX_OBJECT_TERM(cxTextureJSON, cxTexture)

cxTexture cxTextureJSONLoadStream(cxStream stream)
{
    cxTexture json = CX_CREATE(cxTextureJSON);
    cxTextureLoad(json,stream);
    return json;
}



