//
//  cxTexture.c
//  cxEngine
//
//  Created by xuhua on 9/28/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <textures/cxTextureCache.h>
#include "cxShader.h"
#include "cxTexture.h"
#include "cxEngine.h"

CX_TYPE(cxTexCoord, cxObject)
{}
CX_INIT(cxTexCoord, cxObject)
{}
CX_FREE(cxTexCoord, cxObject)
{}
CX_TERM(cxTexCoord, cxObject)

cxTexCoord cxTexCoordMiss(cxConstChars key)
{
    cxTexCoord this = CX_CREATE(cxTexCoord);
    this->frame = cxRect4fv(0, 0, 2, 2);
    this->sourceSize = cxSize2fv(2, 2);
    this->spriteSourceSize = cxRect4fv(0, 0, 2, 2);
    CX_WARN("%s texture miss,use cxTexCoordMiss replace",key);
    return this;
}

cxBox4f cxTexCoordTrimmedFix(cxTexCoord coord,cxBox4f vbox,cxSize2f size,cxVec2f offset,cxVec2f scale,cxBool flipx,cxBool flipy)
{
    cxFloat xs = 1.0f;
    cxFloat ys = 1.0f;
    if(coord != NULL && coord->trimmed){
        vbox.l = coord->spriteSourceSize.x - coord->sourceSize.w/2.0f;
        if(flipx){
            vbox.l = -vbox.l - coord->spriteSourceSize.w;
        }
        vbox.r = vbox.l + coord->spriteSourceSize.w;
        vbox.t = coord->sourceSize.h/2.0f - coord->spriteSourceSize.y;
        if(flipy){
            vbox.t = -vbox.t + coord->spriteSourceSize.h;
        }
        vbox.b = vbox.t - coord->spriteSourceSize.h;
        xs = size.w / coord->sourceSize.w;
        ys = size.h / coord->sourceSize.h;
    }
    vbox.l = (vbox.l * xs + offset.x) * scale.x;
    vbox.r = (vbox.r * xs + offset.x) * scale.x;
    vbox.t = (vbox.t * ys + offset.y) * scale.y;
    vbox.b = (vbox.b * ys + offset.y) * scale.y;
    return vbox;
}

CX_METHOD_DEF(cxTexture, Load,cxBool,cxStream stream)
{
    return false;
}
CX_METHOD_DEF(cxTexture, Bind, void)
{
    cxOpenGLBindTexture(this->textureId,0);
}
CX_TYPE(cxTexture, cxObject)
{
    CX_METHOD(cxTexture, Load);
    CX_METHOD(cxTexture, Bind);
}
CX_INIT(cxTexture, cxObject)
{
    this->texParam = cxtpv(GL_LINEAR,GL_LINEAR,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE);
    this->keys = CX_ALLOC(cxHash);
    this->isSetParam = true;
    this->scale = 1.0f;
}
CX_FREE(cxTexture, cxObject)
{
    cxOpenGLDeleteTexture(this->textureId, 0);
    CX_RELEASE(this->keys);
    CX_RELEASE(this->shader);
}
CX_TERM(cxTexture, cxObject)

void cxTextureLoad(cxAny ptex,cxStream stream)
{
    CX_ASSERT_THIS(ptex, cxTexture);
    CX_RETURN(this->isLoad);
    this->isLoad = CX_CALL(this, Load, CX_M(cxBool,cxStream),stream);
}

cxTexCoord cxTextureCoord(cxAny ptex,cxConstChars key)
{
    CX_ASSERT_THIS(ptex, cxTexture);
    cxAny coord = cxHashGet(this->keys, cxHashStrKey(key));
    if(coord == NULL)coord = cxTexCoordMiss(key);
    return coord;
}

cxSize2f cxTextureSize(cxAny ptex,cxConstChars key,cxSize2f dv)
{
    CX_ASSERT_THIS(ptex, cxTexture);
    cxTexCoord coord = cxTextureCoord(this, key);
    if(coord == NULL){
        return dv;
    }
    return coord->sourceSize;
}

cxBoxTex2f cxTextureBoxCoord(cxAny ptex,cxTexCoord coord,cxBox4f pixel,cxBool flipx,cxBool flipy)
{
    CX_ASSERT_THIS(ptex, cxTexture);
    cxBoxTex2f rv = cxBoxTex2fDefault;
    if(coord == NULL){
        return cxBoxTex2fFlip(rv, flipx, flipy,pixel);
    }
    if(coord->rotated){
        cxFloat l = (coord->frame.x + pixel.t) / this->size.w;
        cxFloat r = (coord->frame.x + coord->frame.h - pixel.t - pixel.b)/this->size.w;
        cxFloat t = (coord->frame.y + pixel.l)/this->size.h;
        cxFloat b = (coord->frame.y + coord->frame.w - pixel.l - pixel.r)/this->size.h;
        if(flipx)CX_SWAP(t,b);
        if(flipy)CX_SWAP(l,r);
        rv.lb = cxTex2fv(l,t);
        rv.rb = cxTex2fv(l,b);
        rv.rt = cxTex2fv(r,b);
        rv.lt = cxTex2fv(r,t);
    }else{
        cxFloat l = (coord->frame.x + pixel.l) / this->size.w;
        cxFloat r = (coord->frame.x + coord->frame.w - pixel.l - pixel.r)/this->size.w;
        cxFloat t = (coord->frame.y + pixel.t)/this->size.h;
        cxFloat b = (coord->frame.y + coord->frame.h - pixel.t - pixel.b)/this->size.h;
        if(flipx)CX_SWAP(l,r);
        if(flipy)CX_SWAP(t,b);
        rv.lb = cxTex2fv(l,b);
        rv.rb = cxTex2fv(r,b);
        rv.rt = cxTex2fv(r,t);
        rv.lt = cxTex2fv(l,t);
    }
    return rv;
}

cxBoxTex2f cxTextureBoxPixel(cxAny ptex,cxConstChars key,cxBox4f pixel,cxBool flipx,cxBool flipy)
{
    CX_ASSERT_THIS(ptex, cxTexture);
    cxBoxTex2f rv = cxBoxTex2fDefault;
    CX_RETURN(key == NULL, rv);
    cxTexCoord coord = cxHashGet(this->keys, cxHashStrKey(key));
    CX_RETURN(coord == NULL, rv);
    return cxTextureBoxCoord(ptex, coord, pixel, flipx, flipy);
}

cxBoxTex2f cxTextureBox(cxAny ptex,cxConstChars key)
{
    CX_ASSERT_THIS(ptex, cxTexture);
    return cxTextureBoxPixel(this, key, cxBox4fx(0), false, false);
}

void cxDrawClippingTexture(cxAny ptex,const cxVec2f pos,const cxSize2f size,cxConstChars tkey)
{
    CX_ASSERT_THIS(ptex, cxTexture);
    cxTextureDraw(this, pos, size, cxBox4fx(0), tkey, cxShaderClippingKey);
}

void cxTextureDraw(cxAny ptex,const cxVec2f pos,const cxSize2f size,cxBox4f pixel,cxConstChars tkey,cxConstChars skey)
{
    CX_ASSERT_THIS(ptex, cxTexture);
    cxBoxVec3f vbox;
    cxFloat wh = size.w / 2.0f;
    cxFloat hh = size.h / 2.0f;
    vbox.lb = cxVec3fv(pos.x - wh, pos.y - hh, 0);
    vbox.lt = cxVec3fv(pos.x - wh, pos.y + hh, 0);
    vbox.rb = cxVec3fv(pos.x + wh, pos.y - hh, 0);
    vbox.rt = cxVec3fv(pos.x + wh, pos.y + hh, 0);
    cxBoxTex2f tbox = cxBoxTex2fDefault;
    if(tkey != NULL){
        tbox = cxTextureBoxPixel(ptex, tkey, pixel, false, false);
    }
    cxTextureBind(this);
    cxOpenGLSetBlendFactor(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    cxOpenGLUsingShader(skey);
    glEnableVertexAttribArray(cxVertexAttribPosition);
    glVertexAttribPointer(cxVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(cxVec3f), &vbox);
    glEnableVertexAttribArray(cxVertexAttribColor);
    glVertexAttribPointer(cxVertexAttribColor, 4, GL_FLOAT, GL_FALSE, sizeof(cxColor4f), &cxBoxColor4fDefault);
    glEnableVertexAttribArray(cxVertexAttribTexcoord);
    glVertexAttribPointer(cxVertexAttribTexcoord, 2, GL_FLOAT, GL_FALSE, sizeof(cxTex2f), &tbox);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void cxTextureBind(cxAny ptex)
{
    CX_ASSERT_THIS(ptex, cxTexture);
    CX_CALL(this, Bind, CX_M(void));
    if(!this->isSetParam){
        cxOpenGLSetTexParameters(this->texParam);
        this->isSetParam = true;
    }
}

void cxTextureSetRepeat(cxAny ptex)
{
    CX_ASSERT_THIS(ptex, cxTexture);
    cxTextureSetParam(this, GL_TEXTURE_WRAP_S, GL_REPEAT);
    cxTextureSetParam(this, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void cxTextureSetParam(cxAny ptex,GLuint type,GLuint value)
{
    CX_ASSERT_THIS(ptex, cxTexture);
    if(type == GL_TEXTURE_MIN_FILTER && this->texParam.minFilter != value){
        this->texParam.minFilter = value;
        this->isSetParam = false;
    }
    if(type == GL_TEXTURE_MAG_FILTER && this->texParam.magFilter != value){
        this->texParam.magFilter = value;
        this->isSetParam = false;
    }
    if(type == GL_TEXTURE_WRAP_S && this->texParam.wrapS != value){
        this->texParam.wrapS = value;
        this->isSetParam = false;
    }
    if(type == GL_TEXTURE_WRAP_T && this->texParam.wrapT != value){
        this->texParam.wrapT = value;
        this->isSetParam = false;
    }
}

cxTextureLoaderInfo cxTextureLoader(cxConstChars url)
{
    cxTextureLoaderInfo info = {0};
    cxChar path[512]={0};
    cxInt type = cxConstCharsTypePath(url, path);
    if(type == 0){
        return info;
    }
    url = path;
    if(type == 2){
        json_t *json = cxEngineLocalizeder(path);
        if(json != NULL && json_is_string(json))url = json_string_value(json);
    }
    cxPath ppath = cxPathParse(url);
    if(ppath == NULL || !cxConstCharsOK(ppath->path)){
        return info;
    }
    info.texture = cxTextureCacheLoadFile(ppath->path);
    if(info.texture == NULL){
        return info;
    }
    if(ppath->count >= 2){
        info.coord = cxTextureCoord(info.texture, ppath->key);
    }
    return info;
}


