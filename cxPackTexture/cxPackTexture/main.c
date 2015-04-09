//
//  main.c
//  cxPackTexture
//
//  Created by xuhua on 11/30/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <ctype.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <libpng/png.h>
#include <cxcore/cxBase.h>
#include <lzma/cxLzma.h>

#include <streams/cxAssetsStream.h>
#include <streams/cxFileStream.h>
#include <streams/cxMemStream.h>
#include <streams/cxMMapStream.h>

cxBool cxWriteFile(cxString file,cxString data)
{
    FILE *fd = fopen(cxStringBody(file), "wb");
    CX_RETURN(fd == NULL, false);
    cxInt bytes = (cxInt)fwrite(cxStringBody(data), 1, cxStringLength(data), fd);
    fclose(fd);
    return bytes == cxStringLength(data);
}

cxInt cxFileFD(cxConstChars file,cxInt *off,cxInt *length)
{
    CX_ASSERT(off != NULL && length != NULL && file != NULL, "args error");
    struct stat stat={0};
    cxConstChars body = file;
    if(lstat(body, &stat) != 0){
        return -1;
    }
    *length = (cxInt)stat.st_size;
    *off = 0;
    return open(file, O_RDONLY,00644);
}

cxString cxAssetsPath(cxConstChars file)
{
    return cxStringConstChars(file);
}

cxString cxDocumentPath(cxConstChars file)
{
    return cxStringConstChars(file);
}

cxInt cxAssertsFD(cxConstChars file,cxInt *off,cxInt *length)
{
    CX_ASSERT(off != NULL && length != NULL && file != NULL, "args error");
    cxString path = cxAssetsPath(file);
    CX_ASSERT(path != NULL, "get file %s path failed",file);
    return cxFileFD(cxStringBody(path), off, length);
}

cxAny cxJsonLoader(cxConstChars src)
{
    return NULL;
}
cxAny cxJsonLocalized(cxConstChars key)
{
    return NULL;
}
cxAny cxJsonProperty(cxConstChars key)
{
    return NULL;
}

void cxUtilPrint(cxConstChars type,cxConstChars file,int line,cxConstChars format,va_list ap)
{
    char buffer[CX_MAX_LOGGER_LENGTH];
    vsnprintf(buffer, CX_MAX_LOGGER_LENGTH, format, ap);
    printf("[%s:%d] %s:%s\n",file,line,type,buffer);
    fflush(stdout);
}

//for cxcore end
void cxTypeFinished(cxType this)
{
    
}

static cxString readPNG(cxConstChars path,cxInt *w,cxInt *h,cxInt format)
{
    cxString data = cxAssetsData(path);
    png_image image={0};
    image.version = PNG_IMAGE_VERSION;
    if(!png_image_begin_read_from_memory(&image, cxStringBody(data), cxStringLength(data))){
        CX_ERROR("read png data failed");
        return NULL;
    }
    image.format = format;
    cxInt bufsiz = PNG_IMAGE_SIZE(image);
    cxAny buffer = allocator->malloc(bufsiz);
    if(!png_image_finish_read(&image, NULL, buffer, 0, NULL)){
        CX_ERROR("read png data error");
        return NULL;
    }
    *w = image.width;
    *h = image.height;
    return cxLzmaCompressed(cxStringAttachMem(buffer, bufsiz));
}

int xtf(int argc,const char *argv[])
{
    cxChar pngFile[PATH_MAX]={0};
    cxChar jsonFile[PATH_MAX]={0};
    cxChar atfFile[PATH_MAX]={0};
    if(argc != 4 && argc != 2){
        CX_ERROR("args error:cxPackTexture a.png a.json a.xtf");
        return -1;
    }
    if(argc == 2){
        snprintf(pngFile, PATH_MAX, "%s.png",argv[1]);
        snprintf(jsonFile, PATH_MAX, "%s.json",argv[1]);
        snprintf(atfFile, PATH_MAX, "%s.xtf",argv[1]);
    }else{
        snprintf(pngFile, PATH_MAX, "%s",argv[1]);
        snprintf(jsonFile, PATH_MAX, "%s",argv[2]);
        snprintf(atfFile, PATH_MAX, "%s",argv[3]);
    }
    
    cxTextureXTFHeader header;
    header.flags = 0x81052901;
    
    cxString json = cxAssetsData(jsonFile);
    CX_ASSERT(cxStringOK(json), "read json data error");
    //压缩
    cxString jsonzip = cxLzmaCompressed(json);
    header.jsonLen = cxStringLength(jsonzip);
    header.rgbaNum = 0;
    
    //read png data
    cxInt width,height;
    cxString rgbazip = readPNG(pngFile,&width,&height,PNG_FORMAT_RGBA);
    if(!cxStringOK(rgbazip)){
        return 1;
    }
    header.rgbaNum++;
    cxTextureXTFBlock block1;
    block1.format = cxTextureXTFBlockFormat_RGBA8888;
    block1.width = width;
    block1.height = height;
    block1.rgbaLen = cxStringLength(rgbazip);
    
    FILE *fd = fopen(atfFile, "wb");
    //write header
    fwrite(&header, 1, sizeof(cxTextureXTFHeader), fd);
    //write blocks
    fwrite(&block1, 1, sizeof(cxTextureXTFBlock), fd);
    //write json
    fwrite(cxStringBody(jsonzip), 1, cxStringLength(jsonzip), fd);
    //write block 0
    fwrite(cxStringBody(rgbazip), 1, cxStringLength(rgbazip), fd);
    //write block 1
    fclose(fd);
    return 0;
}
//很多大小相同的纹理打包到一个文件(*.mtf)
//xtf mtf dir 1 144 pvr(pkm) -> dir.mtf 打包1.ext - 144.ext文件
int mtf(int argc,const char *argv[])
{
    //dir = argv[2]
    cxChar mtfFile[PATH_MAX]={0};
    snprintf(mtfFile, PATH_MAX, "%s.mtf",argv[2]);
    int from = atoi(argv[3]);
    int to = atoi(argv[4]);
    if(from < 1 || to <= from ){
        return 1;
    }
    cxTextureMTFHeader header = {0};
    //write length
    cxInt32 fl = 0;
    cxString ds = CX_CREATE(cxString);
    //write type
    for(int i=from;i<=to;i++){
        CX_PATH_DEF(currFile, "%s/%d.%s",argv[2],i,argv[5]);
        cxString data = cxAssetsData(currFile);
        if(fl == 0){
            fl = cxStringLength(data);
        }
        CX_ASSERT(fl == cxStringLength(data), "error,file length error");
        cxStringAppend(ds, cxStringBody(data), cxStringLength(data));
        printf("write file:%s size=%d\n",currFile,cxStringLength(data));
        header.count ++;
    }
    if(strcmp(argv[5], "pvr") == 0){
        header.type = 1;
    }else if(strcmp(argv[5], "pkm") == 0){
        header.type = 2;
    }else{
        CX_ASSERT_FALSE("type error,only pvr,or pkm");
    }
    header.length = fl;
    FILE *fd = fopen(mtfFile, "wb");
    //write header
    fwrite(&header, 1, sizeof(cxTextureMTFHeader), fd);
    //write zip data
    fwrite(cxStringBody(ds), 1, cxStringLength(ds), fd);
    fclose(fd);
    return 0;
}

int main(int argc, const char * argv[]) {
    //init
    cxCoreInit();
    CX_SET_TYPE(cxAssetsStream);
    CX_SET_TYPE(cxFileStream);
    CX_SET_TYPE(cxMemStream);
    CX_SET_TYPE(cxMMapStream);
    cxLzmaSetFlags(UTF8("7798zfo"));
    cxMemPoolPush();
    int ret = 0;
    if(argc == 6 && strcmp(argv[1], "mtf") == 0){
        ret = mtf(argc, argv);
    }else{
        ret = xtf(argc, argv);
    }
    cxMemPoolPop();
    cxCoreFree();
    return ret;
}

















