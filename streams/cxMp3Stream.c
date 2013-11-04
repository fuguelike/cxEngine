//
//  cxMp3Stream.c
//  cxEngineIOS
//
//  Created by xuhua on 11/1/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <core/cxUtil.h>
#include "cxMp3Stream.h"

static cxBool cxMp3StreamOpen(cxAny stream)
{
    cxMp3Stream this = stream;
    this->fd = open(cxStringBody(this->path), O_RDONLY, 0600);;
    if(this->fd <= 0){
        CX_ERROR("open strean file %s failed",cxStringBody(this->path));
        return false;
    }
    int error = 0;
    this->mh = mpg123_new(NULL, &error);
    if(this->mh == NULL){
        CX_ERROR("mpg123 new failed");
        return false;
    }
    if(mpg123_open_fd(this->mh, this->fd) != MPG123_OK){
        CX_ERROR("open mpg file failed %s",cxStringBody(this->path));
        return false;
    }
    this->bufsiz = mpg123_outblock(this->mh);
    this->buffer = allocator->malloc(this->bufsiz);
    mpg123_getformat(this->mh, &this->freq, &this->channels, &this->encoding);
    this->super.canRead = true;
    this->super.canSeek = true;
    this->super.isOpen = true;
    return true;
}

//read to ->buffer
static cxInt cxMp3StreamRead(cxAny stream,cxPointer buffer,cxInt size)
{
    cxMp3Stream this = stream;
    if(!this->super.canRead){
        return 0;
    }
    size_t done = 0;
    cxInt rv = mpg123_read(this->mh, buffer, size, &done);
    //error
    if(rv < 0 && rv != MPG123_DONE){
        CX_ERROR("read mp3 stream error");
        return 0;
    }
    return done;
}

static cxInt cxMp3StreamWrite(cxAny stream,cxPointer buffer,cxInt size)
{
    return 0;
}

static cxOff cxMp3StreamPosition(cxAny stream)
{
    cxMp3Stream this = stream;
    return (cxOff)mpg123_tellframe(this->mh);
}

static cxBool cxMp3StreamSeek(cxAny stream,cxOff off,cxInt flags)
{
    cxMp3Stream this = stream;
    return mpg123_seek_frame(this->mh, off, flags) >= 0;
}

static cxString cxMp3StreamAllBytes(cxAny stream)
{
    cxStream this = stream;
    if(!this->canRead){
        this->interface->Open(this);
    }
    if(!this->canRead){
        CX_ERROR("file stream can't read");
        return NULL;
    }
    cxInt bufsiz = 0;
    cxPointer buffer = cxMp3StreamBuffer(this, &bufsiz);
    cxString data = CX_CREATE(cxString);
    cxInt bytes = 0;
    while((bytes = this->interface->Read(this,buffer,bufsiz)) > 0){
        cxStringAppend(data, buffer,bytes);
    }
    this->interface->Close(this);
    return data;
}

static void cxMp3StreamClose(cxAny stream)
{
    cxMp3Stream this = stream;
    if(this->mh != NULL){
        mpg123_close(this->mh);
        mpg123_delete(this->mh);
        this->mh = NULL;
    }
    if(this->fd){
        close(this->fd);
        this->fd = 0;
    }
    if(this->buffer != NULL){
        allocator->free(this->buffer);
        this->buffer = NULL;
    }
    cxStreamClose(this);
}

static const cxStreamInterface mp3Interface = {
    .Open       = cxMp3StreamOpen,
    .Read       = cxMp3StreamRead,
    .Write      = cxMp3StreamWrite,
    .Seek       = cxMp3StreamSeek,
    .Close      = cxMp3StreamClose,
    .Position   = cxMp3StreamPosition,
    .AllBytes   = cxMp3StreamAllBytes,
};

cxPointer cxMp3StreamBuffer(cxAny mp3,cxInt *bytes)
{
    cxMp3Stream this = mp3;
    *bytes = this->bufsiz;
    return this->buffer;
}

CX_OBJECT_INIT(cxMp3Stream, cxStream)
{
    this->super.interface = &mp3Interface;
}
CX_OBJECT_FREE(cxMp3Stream, cxStream)
{
    cxMp3StreamClose(this);
    CX_RELEASE(this->path);
}
CX_OBJECT_TERM(cxMp3Stream, cxStream)

cxStream cxMp3StreamCreate(cxConstChars file)
{
    cxMp3Stream this = CX_CREATE(cxMp3Stream);
    cxString path = cxAssetsPath(file);
    CX_RETAIN_SWAP(this->path, path);
    return (cxStream)this;
}


