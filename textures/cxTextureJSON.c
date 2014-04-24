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
    cxConstChars simagePath = cxJsonString(meta, "image");
    CX_ASSERT(simagePath != NULL, "json image null");
    
    texture->innerTexture = cxTextureFactoryLoadFile(simagePath);
    ret = (texture->innerTexture != NULL);
    CX_ASSERT(ret, "xml innert texture error");
    CX_RETAIN(texture->innerTexture);
    
    //for jpg pkm atlas texture
    texture->super.isAtlas = cxJsonBool(meta, "atlas", false);
    texture->super.size = texture->innerTexture->size;
    
    cxJson frames = cxJsonArray(json, "frames");
    json_t *value = NULL;
    cxInt index = 0;
    json_array_foreach(CX_JSON_PTR(frames), index, value){
        cxTexCoord e = CX_ALLOC(cxTexCoord);
        
        json_t *tmp = json_object_get(value, "filename");
        cxConstChars sn = json_string_value(tmp);
        
        tmp = json_object_get(value, "rotated");
        e->isRotation = json_boolean(tmp);
        
        json_t *frame = json_object_get(value, "frame");
        
        tmp = json_object_get(frame, "x");
        e->x = json_number_value(tmp);
        
        tmp = json_object_get(frame, "y");
        e->y = json_number_value(tmp);
        
        tmp = json_object_get(frame, "w");
        e->w = json_number_value(tmp);
        
        tmp = json_object_get(frame, "h");
        e->h = json_number_value(tmp);
        
        cxHashSet(texture->super.keys, cxHashStrKey(sn), e);
        CX_RELEASE(e);
    }
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



