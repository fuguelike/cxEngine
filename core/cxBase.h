//
//  cxBase.h
//  cxEngine
//
//  Created by xuhua on 9/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxBase_h
#define cxEngine_cxBase_h

#include <utlist.h>
#include "cxConfig.h"
#include "cxAllocator.h"
#include "cxTypes.h"

CX_C_BEGIN

typedef struct cxSignal cxSignal;
typedef struct cxSlot cxSlot;
struct cxSignal
{
    cxSignal *prev;
    cxSignal *next;
    cxSlot *slot;
    cxAny func;
    cxAny object;
    cxInt priority;
};
struct cxSlot {
    cxSignal **signal;
    cxSignal *slot;
};

#define CX_SIGNAL_ALLOC(_name_) cxSignal *_name_

#define CX_SIGNAL_RELEASE(_signal_)                             \
do{                                                             \
    cxSignal *_tmp_ = NULL;                                     \
    cxSignal *_ele_ = NULL;                                     \
    DL_FOREACH_SAFE(_signal_, _ele_, _tmp_){                    \
        DL_DELETE(_signal_, _ele_);                             \
        if(_ele_->slot != NULL){                                \
            _ele_->slot->slot = NULL;                           \
            _ele_->slot->signal = NULL;                         \
        }                                                       \
        allocator->free(_ele_);                                 \
    }                                                           \
}while(0)

#define CX_FUNC_TYPE(...) (void (*)(__VA_ARGS__))

#define CX_SLOT_OBJECT _ele_->object

#define CX_SIGNAL_FIRE(_signal_,_ft_,...)                       \
do{                                                             \
    cxSignal *_ele_ = NULL;                                     \
    DL_FOREACH(_signal_, _ele_){                                \
        (_ft_(_ele_->func))(__VA_ARGS__);                       \
    }                                                           \
}while(0)


#define CX_SLOT_ALLOC(name)      cxSlot name

#define CX_SLOT_RELEASE(_slot_)                                 \
if(_slot_.slot != NULL && _slot_.signal != NULL){               \
    DL_DELETE(*_slot_.signal,_slot_.slot);                      \
    allocator->free(_slot_.slot);                               \
    _slot_.signal = NULL;                                       \
    _slot_.slot = NULL;                                         \
}

#define CX_SLOT_CONNECT(_signal_,_object_,_slot_,_func_,_pv_)   \
do{                                                             \
    CX_SLOT_RELEASE(_object_->_slot_);                          \
    cxSignal *_new_ = allocator->calloc(1,sizeof(cxSignal));    \
    _new_->priority = _pv_;                                     \
    _new_->func = _func_;                                       \
    _new_->object = _object_;                                   \
    _new_->slot = &_object_->_slot_;                            \
    _object_->_slot_.slot = _new_;                              \
    _object_->_slot_.signal = &_signal_;                        \
    if(_pv_ == 0){                                              \
        DL_APPEND(_signal_,_new_);                              \
        break;                                                  \
    }                                                           \
    cxSignal *_ele_ = NULL,*_tmp_ = NULL;                       \
    bool append = true;                                         \
    DL_FOREACH_SAFE(_signal_,_ele_,_tmp_){                      \
        if(_pv_ < _ele_->priority){                             \
            continue;                                           \
        }                                                       \
        DL_PREPEND_ELEM(_signal_,_ele_,_new_);                  \
        append = false;                                         \
        break;                                                  \
    }                                                           \
    if(append){                                                 \
        DL_APPEND(_signal_,_new_);                              \
    }                                                           \
}while(0)

#define CX_SLOT_QUICK(_signal_,_object_,_slot_,_func_)          \
    CX_SLOT_CONNECT(_signal_,_object_,_slot_,_func_,0)

typedef void (*cxEventFunc)(cxAny obj,cxAny arg);

typedef struct cxEvent cxEvent;
struct cxEvent
{
    cxEvent *prev,*next;
    cxEventFunc func;
    cxAny object;
    cxAny arg;
};

#define CX_EVENT_ALLOC(name) cxEvent *name

#define CX_EVENT_APPEND(_event_,_func_,_args_)                  \
do{                                                             \
    cxEvent *_newptr_ = allocator->malloc(sizeof(cxEvent));     \
    _newptr_->func = _func_;                                    \
    _newptr_->arg = NULL;                                       \
    CX_RETAIN_SWAP(_newptr_->arg,_args_);                       \
    DL_APPEND(_event_, _newptr_);                               \
}while(0)

#define CX_EVENT_QUICK(_event_,_func_)  CX_EVENT_APPEND(_event_,_func_,NULL)

#define CX_EVENT_PREPEND(_event_,_func_,_args_)                 \
do{                                                             \
    cxEvent *_newptr_ = allocator->malloc(sizeof(cxEvent));     \
    _newptr_->func = _func_;                                    \
    _newptr_->arg = NULL;                                       \
    CX_RETAIN_SWAP(_newptr_->arg,_args_);                       \
    DL_PREPEND(_event_, _newptr_);                              \
}while(0)

#define CX_EVENT_DEL(_event_,_func_)                            \
do{                                                             \
    cxEvent *_tmp_ = NULL;                                      \
    cxEvent *_ele_ = NULL;                                      \
    DL_FOREACH_SAFE(_event_, _ele_, _tmp_){                     \
        if(_ele_->func != _func_){                              \
            continue;                                           \
        }                                                       \
        DL_DELETE(_event_, _ele_);                              \
        CX_RELEASE(_ele_->arg);                                 \
        allocator->free(_ele_);                                 \
    }                                                           \
}while(0)

#define CX_EVENT_RELEASE(_event_)                               \
do{                                                             \
    cxEvent *_tmp_ = NULL;                                      \
    cxEvent *_ele_ = NULL;                                      \
    DL_FOREACH_SAFE(_event_, _ele_, _tmp_){                     \
        DL_DELETE(_event_, _ele_);                              \
        CX_RELEASE(_ele_->arg);                                 \
        allocator->free(_ele_);                                 \
    }                                                           \
}while(0)

#define CX_EVENT_FIRE(_object_,_event_)                         \
do{                                                             \
    cxEvent *_ele_ = NULL;                                      \
    cxEvent *_tmp_=NULL;                                        \
    DL_FOREACH_SAFE(_object_->_event_, _ele_,_tmp_){            \
        cxEventFunc func = _ele_->func;                         \
        func(_object_,_ele_->arg);                              \
    }                                                           \
}while(0)


#define CX_EVENT_CLONE(_src_,_event_,_dst_)                     \
do{                                                             \
    cxEvent *_ele_ = NULL;                                      \
    DL_FOREACH(_src_->_event_, _ele_){                          \
        CX_EVENT_APPEND(_dst_->_event_,_ele_->func,_ele_->arg); \
    }                                                           \
}while(0)

void cxUtilPrint(cxConstChars type,cxConstChars file,int line,cxConstChars format,va_list ap);

//调试信息输出
void cxUtilInfo(cxConstChars file, int line, cxConstChars format, ...);

//错误信息输出
void cxUtilError(cxConstChars file, int line, cxConstChars format, ...);

//警告信息输出
void cxUtilWarn(cxConstChars file, int line, cxConstChars format, ...);

//断言信息输出
void cxUtilAssert(cxConstChars file, int line, cxConstChars format, ...);

CX_C_END

#endif
