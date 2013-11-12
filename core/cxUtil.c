//
//  cxUtil.c
//  cxEngine
//
//  Created by xuhua on 9/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <OpenAL/al.h>
#include <streams/cxMp3Stream.h>
#include <openssl/md5.h>
#include <evhttp.h>
#include <sys/time.h>
#include "cxBase.h"
#include "cxUtil.h"

cxString cxMP3SamplesWithData(cxString data,cxUInt *format,cxUInt *freq)
{
    cxString rv = CX_CREATE(cxString);
    cxInt error = 0;
    mpg123_handle *mh = mpg123_new(NULL, &error);
    if(mh == NULL){
        CX_ERROR("new mp3 handle error");
        goto completed;
    }
    if(mpg123_open_feed(mh) != MPG123_OK){
        CX_ERROR("open mp3 feed error");
        goto completed;
    }
    cxInt bufsiz = mpg123_outblock(mh);
    cxPointer buffer = allocator->malloc(bufsiz);
    size_t done = 0;
    mpg123_feed(mh, (const cxUChar *)cxStringBody(data), cxStringLength(data));
    cxLong mfreq = 0;
    cxInt mchannels = 0;
    cxInt mencoding = 0;
    mpg123_getformat(mh, &mfreq, &mchannels, &mencoding);
    *freq = mfreq;
    if(mencoding & MPG123_ENC_16){
        *format = (mchannels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
    }else{
        *format = (mchannels == 1) ? AL_FORMAT_MONO8 : AL_FORMAT_STEREO8;
    }
    while (true) {
        cxInt ret = mpg123_read(mh, buffer, bufsiz, &done);
        if(done > 0){
            cxStringAppend(rv, buffer, done);
        }
        if(ret == MPG123_NEED_MORE || ret == MPG123_ERR || ret == MPG123_DONE){
            break;
        }
    }
    allocator->free(buffer);
completed:
    mpg123_close(mh);
    mpg123_delete(mh);
    return cxStringLength(rv) > 0 ? rv : NULL;
}

static const char hex[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};

cxString cxMP3SamplesWithFile(cxConstChars file,cxUInt *format,cxUInt *freq)
{
    cxMp3Stream this  = (cxMp3Stream)cxMp3StreamCreate(file);
    cxString bytes = cxSreamBytes(this);
    if(bytes == NULL){
        CX_ERROR("get mp3 bytes error");
        return NULL;
    }
    if(this->encoding & MPG123_ENC_16){
        *format = (this->channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
    }else{
        *format = (this->channels == 1) ? AL_FORMAT_MONO8 : AL_FORMAT_STEREO8;
    }
    *freq = this->freq;
    return bytes;
}

cxString cxMD5(cxString v)
{
    CX_ASSERT(v != NULL, "args error");
    cxUChar digest[MD5_DIGEST_LENGTH];
    cxUChar *md = MD5((const cxUChar *)cxStringBody(v), cxStringLength(v), digest);
    CX_RETURN(md == NULL, NULL);
    cxChar md5[MD5_DIGEST_LENGTH*2+1]={0};
    for(cxInt i=0; i<MD5_DIGEST_LENGTH; i++){
        md5[2*i] = hex[(digest[i] & 0xf0)>> 4];
        md5[2*i + 1] = hex[digest[i] & 0x0f];
    }
    return cxStringConstChars(md5);
}

void cxSetRandSeed()
{
    srand(clock());
}

cxBool cxParseKeyValue(cxChar *query,cxChar *key,cxChar *value)
{
    cxBool ret = false;
    struct evkeyvalq kv={0};
    if(evhttp_parse_query_str(query, &kv) != 0){
        return ret;
    }
	struct evkeyval *header;
	for (header = kv.tqh_first; header != NULL; header = header->next.tqe_next) {
        memcpy(key, header->key, strlen(header->key));
        memcpy(value, header->value, strlen(header->value));
        ret = true;
	}
    evhttp_clear_headers(&kv);
    return ret;
}

cxInt cxParseQuery(cxConstChars query,cxChar *key,cxChar *value)
{
    CX_ASSERT(query != NULL, "query args error");
    cxInt rv = 0;
    cxInt len = strlen(query);
    cxChar *sq = strrchr(query, '=');
    if(sq != NULL){
        cxInt cl = len - (cxInt)strlen(sq);
        memcpy(key, query, cl);
        key[cl]='\0';
        memcpy(value, sq + 1, strlen(sq));
        rv = 2;
    }else{
        memcpy(key, query, len + 1);
        rv = 1;
    }
    return rv;
}

cxInt cxParseURL(cxConstChars url,cxChar *path,cxChar *query)
{
    CX_ASSERT(url != NULL && path != NULL && query != NULL, "args error");
    cxInt rv = 0;
    cxInt len = strlen(url);
    cxChar *sq = strrchr(url, '?');
    if(sq != NULL){
        cxInt cl = len - (cxInt)strlen(sq);
        memcpy(path, url, cl);
        path[cl]='\0';
        memcpy(query, sq + 1, strlen(sq));
        rv = 2;
    }else{
        memcpy(path, url, len + 1);
        rv = 1;
    }
    return rv;
}

cxDouble cxTimestamp()
{
    struct timeval val;
    gettimeofday(&val, NULL);
    return val.tv_sec + (cxDouble)val.tv_usec/(cxDouble)1000000.0;
}

void cxUtilInfo(cxConstChars file,int line,cxConstChars format, ...)
{
    va_list ap;
    va_start(ap, format);
    cxUtilPrint("INFO", file, line, format, ap);
    va_end(ap);
}

void cxUtilError(cxConstChars file,int line,cxConstChars format, ...)
{
    va_list ap;
    va_start(ap, format);
    cxUtilPrint("ERROR", file, line, format, ap);
    va_end(ap);
}

void cxUtilWarn(cxConstChars file,int line,cxConstChars format, ...)
{
    va_list ap;
    va_start(ap, format);
    cxUtilPrint("WARN", file, line, format, ap);
    va_end(ap);
}

void cxUtilAssert(cxConstChars file,int line,cxConstChars format, ...)
{
    va_list ap;
    va_start(ap, format);
    cxUtilPrint("Assert", file, line, format, ap);
    va_end(ap);
}