//
//  cxSkeleton.c
//  cxEngine
//
//  Created by xuhua on 9/14/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <views/cxSpine.h>
#include "cxSkeleton.h"

//create animate
//    this->state = spAnimationState_create(spAnimationStateData_create(this->skeleton->data));
//    spAnimationStateData_setMixByName(this->state->data, "walk", "jump", 0.2f);
//    spAnimationStateData_setMixByName(this->state->data, "jump", "run", 0.4f);
//    spAnimation* animation = spSkeletonData_findAnimation(this->skeleton->data, "walk");
//	if(animation != NULL){
//        spAnimationState_setAnimation(this->state, 10, animation, true);
//    }
//    animation = spSkeletonData_findAnimation(this->skeleton->data, "jump");
//    if(animation != NULL){
//        spAnimationState_addAnimation(this->state, 10, animation, true, 3);
//    }
//    animation = spSkeletonData_findAnimation(this->skeleton->data, "run");
//    if(animation != NULL){
//        spAnimationState_addAnimation(this->state, 10, animation, true, 0);
//    }
//	this->state->rendererObject = this;
/*
 {
     "track": [
        {
            "id": 1,
            "name": "walk",
            "loop": true
            "add": [
                {
                    "name":"jump",
                    "loop":true,
                    "delay":3
                },
                {
                    "name":"run",
                    "loop":true,
                    "delay":0
                }
            ]
        }
    ]
}
 */

static cxBool cxSkeletionParseAnimation(cxSkeleton this,cxJson tracks)
{
    cxSpine spine = CX_TYPE(cxSpine, this->cxAction.view);
    CX_JSON_ARRAY_EACH_BEG(tracks, track)
    {
        //set
        cxInt id = cxJsonInt(track, "id", -1);
        if(id >= 0){
            cxConstChars name = cxJsonConstChars(track, "name");
            cxBool loop = cxJsonBool(track, "loop", false);
            spAnimation *animation = spSkeletonData_findAnimation(spine->skeletonData, name);
            if(animation == NULL){
                CX_WARN("animation %s not found",name);
                continue;
            }
            spAnimationState_setAnimation(this->state, id, animation, loop);
        }else{
            continue;
        }
        //add
        cxJson adds = cxJsonArray(track, "add");
        CX_JSON_ARRAY_EACH_BEG(adds, item)
        {
            cxConstChars name = cxJsonConstChars(item, "name");
            cxBool loop = cxJsonBool(item, "loop", false);
            cxFloat delay = cxJsonDouble(item, "delay", 0);
            spAnimation *animation = spSkeletonData_findAnimation(spine->skeletonData, name);
            if(animation == NULL){
                CX_WARN("animation %s not found",name);
                continue;
            }
            spAnimationState_addAnimation(this->state, id, animation, loop, delay);
        }
        CX_JSON_ARRAY_EACH_END(adds, item)
    }
    CX_JSON_ARRAY_EACH_END(tracks, track)
    return true;
}

static void cxSkeletionFree(cxSkeleton this)
{
    if(this->state != NULL){
        spAnimationState_dispose(this->state);
        this->state = NULL;
    }
}

static void cxSkeletonStateListener(spAnimationState* state, int trackIndex, spEventType type, spEvent* event,int loopCount)
{
    CX_ASSERT_THIS(state->rendererObject, cxSkeleton);
    this->track = spAnimationState_getCurrent(state, trackIndex);
    this->index = trackIndex;
    this->type = type;
    this->event = event;
    this->count = loopCount;
    CX_EVENT_FIRE(this, onEvent);
}

static void cxSkeletonInit(cxAny pav)
{
    CX_ASSERT_THIS(pav, cxSkeleton);
    cxSpine spine = CX_TYPE_CAST(this->cxAction.view,cxSpine);
    cxSkeletionFree(this);
    this->state = spAnimationState_create(spine->stateData);
    if(this->state == NULL){
        CX_ERROR("create animation state error");
        cxActionStop(this);
        return;
    }
    this->state->rendererObject = this;
    this->state->listener = cxSkeletonStateListener;
    if(this->tracks == NULL){
        CX_WARN("not set animation,action stop");
        cxActionStop(this);
        return;
    }
    if(!cxSkeletionParseAnimation(this, this->tracks)){
        CX_WARN("parse animation error,action stop");
        cxActionStop(this);
        return;
    }
}

static void cxSkeletonStep(cxAny pav,cxFloat dt,cxFloat time)
{
    CX_ASSERT_THIS(pav, cxSkeleton);
    CX_ASSERT(this->state != NULL, "animation state null");
    cxSpine spine = CX_TYPE_CAST(this->cxAction.view,cxSpine);
    CX_ASSERT(spine != NULL && spine->skeleton != NULL, "target spine view not init");
	spAnimationState_update(this->state, dt);
	spAnimationState_apply(this->state, spine->skeleton);
}

static cxBool cxSkeletonExit(cxAny pav)
{
    return false;
}

CX_SETTER_DEF(cxSkeleton, tracks)
{
    CX_RETAIN_SWAP(this->tracks, value);
}

CX_OBJECT_TYPE(cxSkeleton, cxAction)
{
    CX_PROPERTY_SETTER(cxSkeleton, tracks);
}
CX_OBJECT_INIT(cxSkeleton, cxAction)
{
    this->cxAction.time = -1;
    CX_METHOD_SET(this->cxAction.Init, cxSkeletonInit);
    CX_METHOD_SET(this->cxAction.Step, cxSkeletonStep);
    CX_METHOD_SET(this->cxAction.Exit, cxSkeletonExit);
}
CX_OBJECT_FREE(cxSkeleton, cxAction)
{
    CX_EVENT_RELEASE(this->onEvent);
    CX_RELEASE(this->tracks);
    cxSkeletionFree(this);
}
CX_OBJECT_TERM(cxSkeleton, cxAction)


