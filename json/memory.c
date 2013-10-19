/*
 * Copyright (c) 2009-2012 Petri Lehtinen <petri@digip.org>
 * Copyright (c) 2011-2012 Basile Starynkevitch <basile@starynkevitch.net>
 *
 * Jansson is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */

#include <stdlib.h>
#include <string.h>
#include <core/cxBase.h>
#include "jansson.h"
#include "jansson_private.h"

void *jsonp_malloc(size_t size)
{
    if(!size)
        return NULL;

    return allocator->malloc((cxInt)size);
}

void jsonp_free(void *ptr)
{
    if(!ptr)
        return;
    allocator->free(ptr);
}

char *jsonp_strdup(const char *str)
{
    char *new_str;

    new_str = allocator->malloc((cxInt)strlen(str) + 1);
    if(!new_str)
        return NULL;

    strcpy(new_str, str);
    return new_str;
}

