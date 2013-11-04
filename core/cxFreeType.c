//
//  cxFreeType.c
//  cxEngine
//
//  Created by xuhua on 10/20/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <core/cxEngine.h>
#include <streams/cxAssetsStream.h>
#include "cxFreeType.h"

CX_OBJECT_INIT(cxFreeFontChar, cxObject)
{
}
CX_OBJECT_FREE(cxFreeFontChar, cxObject)
{
    allocator->free(this->data);
}
CX_OBJECT_TERM(cxFreeFontChar, cxObject)

static cxFreeType instance = NULL;

CX_OBJECT_INIT(cxFreeFont, cxObject)
{
    this->chars = CX_ALLOC(cxHash);
}
CX_OBJECT_FREE(cxFreeFont, cxObject)
{
    CX_RELEASE(this->chars);
    if(this->face != NULL){
        FT_Done_Face(this->face);
    }
    CX_RELEASE(this->data);
}
CX_OBJECT_TERM(cxFreeFont, cxObject)

static void cxFreeFontSetBitmapData(cxFreeFontChar schar,FT_Bitmap *bitmap,int width,int height)
{
    uint8_t bitsiz = sizeof(uint8_t) * 8;
    for(register int j=0; j  < height ; j++){
        int bitnum = (bitmap->pitch * bitsiz) / width;
        for(register int i=0; i < width; i++){
            int b = bitmap->buffer[i * bitnum / bitsiz + j * bitmap->pitch];
            int idx = bitsiz - (i * bitnum) % bitsiz - bitnum;
            int m = b >> idx;
            int v = m & ((1 << bitnum) - 1);
            if(bitmap->pixel_mode == FT_PIXEL_MODE_MONO){
                v = v > 0 ? 0xFF : 0x00;
            }
            schar->data[i + width * j] = v;
        }
    }
}

void cxFreeFontSetSize(cxFreeFont this,cxInt width,cxInt height)
{
    FT_Set_Pixel_Sizes(this->face, width, height);
}

cxFreeFontChar cxFreeFontLoadChar(cxFreeFont this,cxFreeFontCharKey key)
{
    cxFreeFontChar schar = cxHashGet(this->chars, cxHashIntKey(key.key));
    if(schar != NULL){
        return schar;
    }
    FT_UInt idx = FT_Get_Char_Index(this->face, (FT_ULong)key.wchar);
    if(idx == 0){
        CX_ERROR("FT_Get_Char_Index error %d",key.wchar);
        return NULL;
    }
    if(FT_Load_Glyph(this->face, idx, FT_LOAD_RENDER) != FT_Err_Ok){
        CX_ERROR("FT_Load_Glyph error");
        return NULL;
    }
    FT_Glyph glyph = NULL;
    if(FT_Get_Glyph(this->face->glyph, &glyph) != FT_Err_Ok){
        CX_ERROR("FT_Get_Glyph error");
        return NULL;
    }
    if(glyph->format != FT_GLYPH_FORMAT_BITMAP){
        FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, NULL, true);
    }
    FT_BitmapGlyph bitmap = (FT_BitmapGlyph)glyph;
    schar = CX_ALLOC(cxFreeFontChar);
    schar->ax = glyph->advance.x;
    schar->ay = glyph->advance.y;
    schar->left = bitmap->left;
    schar->top = bitmap->top;
    schar->width = bitmap->bitmap.width;
    schar->height = bitmap->bitmap.rows;
    if(schar->width > 0 && schar->height > 0){
        schar->data = allocator->malloc(schar->width * schar->height);
        cxFreeFontSetBitmapData(schar, &bitmap->bitmap, schar->width, schar->height);
    }
    schar->key = key;
    cxHashSet(this->chars, cxHashIntKey(key.key), schar);
    CX_RELEASE(schar);
    FT_Done_Glyph(glyph);
    return schar;
}

cxFreeFont cxFreeTypeCreateFont(cxConstChars file)
{
    cxFreeType this = cxFreeTypeInstance();
    cxFreeFont font = cxHashGet(this->fonts, cxHashStrKey(file));
    if(font != NULL){
        return font;
    }
    font = CX_CREATE(cxFreeFont);
    cxStream stream = cxAssetsStreamCreate(file);
    if(stream == NULL){
        CX_ERROR("create face file %s error",file);
        return NULL;
    }
    cxString data = stream->interface->AllBytes(stream);
    if(data == NULL){
        CX_ERROR("read data error from stream");
        return NULL;
    }
    if(FT_New_Memory_Face(this->library, (FT_Byte *)cxStringBody(data), cxStringLength(data), 0, &font->face) != FT_Err_Ok){
        CX_ERROR("create face from file %s",file);
        return NULL;
    }
    if(FT_Select_Charmap(font->face, FT_ENCODING_UNICODE) != FT_Err_Ok){
        CX_ERROR("font file error:%s",file);
        return NULL;
    }
    CX_RETAIN_SWAP(font->data, data);
    cxHashSet(this->fonts, cxHashStrKey(file), font);
    return font;
}

static void cxFreeTypeOnMemory(cxAny freetype)
{
    cxFreeType this = freetype;
    cxHashClean(this->fonts);
}

CX_OBJECT_INIT(cxFreeType, cxObject)
{
    cxEngine engine = cxEngineInstance();
    this->error = FT_Init_FreeType(&this->library);
    if(this->error != FT_Err_Ok){
        CX_ERROR("init freeType error %d",this->error);
    }
    this->fonts = CX_ALLOC(cxHash);
    CX_SLOT_QUICK(engine->onMemory, this, onMemory, cxFreeTypeOnMemory);
}
CX_OBJECT_FREE(cxFreeType, cxObject)
{
    CX_SLOT_RELEASE(this->onMemory);
    FT_Done_FreeType(this->library);
    CX_RELEASE(this->fonts);
}
CX_OBJECT_TERM(cxFreeType, cxObject)

cxFreeType cxFreeTypeInstance()
{
    if(instance == NULL){
        instance = CX_ALLOC(cxFreeType);
    }
    return instance;
}

void cxFreeTypeDestroy()
{
    CX_RELEASE(instance);
    instance = NULL;
}



