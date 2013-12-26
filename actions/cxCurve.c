//
//  cxCurve.c
//  cxEngine
//
//  Created by xuhua on 10/10/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxAction.h>
#include "cxCurve.h"

#define M_PI_X2 (M_PI * 2)


CX_OBJECT_INIT(cxCurveItem, cxObject)
{
    
}
CX_OBJECT_FREE(cxCurveItem, cxObject)
{
    
}
CX_OBJECT_TERM(cxCurveItem, cxObject)

cxFloat cxCurveBackIn(cxAny pav,cxFloat time)
{
    cxFloat overshoot = 1.70158f;
    return (time * time * ((overshoot + 1.0f) * time - overshoot));
}

cxFloat cxCurveBackOut(cxAny pav,cxFloat time)
{
    cxFloat overshoot = 1.70158f;
    time = time - 1.0f;
    return (time * time * ((overshoot + 1) * time + overshoot) + 1);
}

cxFloat cxCurveBackInOut(cxAny pav,cxFloat time)
{
    cxFloat overshoot = 1.70158f * 1.525f;
    time = time * 2.0f;
    if (time < 1.0f){
        return ((time * time * ((overshoot + 1) * time - overshoot)) / 2);
    }else{
        time = time - 2.0f;
        return ((time * time * ((overshoot + 1) * time + overshoot)) / 2 + 1);
    }
}

static cxFloat bounceTime(cxFloat time)
{
    if (time < 1 / 2.75){
        return 7.5625f * time * time;
    }else if (time < 2 / 2.75){
        time -= 1.5f / 2.75f;
        return 7.5625f * time * time + 0.75f;
    }else if(time < 2.5 / 2.75){
        time -= 2.25f / 2.75f;
        return 7.5625f * time * time + 0.9375f;
    }
    time -= 2.625f / 2.75f;
    return 7.5625f * time * time + 0.984375f;
}

cxFloat cxCurveBounceIn(cxAny pav,cxFloat time)
{
    return 1.0f - bounceTime(1.0f - time);
}

cxFloat cxCurveBounceOut(cxAny pav,cxFloat time)
{
    return bounceTime(time);
}

cxFloat cxCurveBounceInOut(cxAny pav,cxFloat time)
{
    cxFloat newtime = 0;
    if (time < 0.5f){
        time = time * 2;
        newtime = (1 - bounceTime(1 - time)) * 0.5f;
    }else{
        newtime = bounceTime(time * 2 - 1) * 0.5f + 0.5f;
    }
    return newtime;
}

cxFloat cxCurveElasticIn(cxAny pav,cxFloat time)
{
    cxAction this = pav;
    cxFloat newtime = 0;
    if (time == 0 || time == 1){
        newtime = time;
    }else{
        cxFloat s = this->assist.v1 / 4;
        time = time - 1;
        newtime = -powf(2, 10 * time) * sinf((time - s) * M_PI_X2 / this->assist.v1);
    }
    return newtime;
}

cxFloat cxCurveElasticOut(cxAny pav,cxFloat time)
{
    cxAction this = pav;
    cxFloat newtime = 0;
    if (time == 0 || time == 1){
        newtime = time;
    }else{
        cxFloat s = this->assist.v1 / 4;
        newtime = powf(2, -10 * time) * sinf((time - s) * M_PI_X2 / this->assist.v1) + 1;
    }
    return newtime;
}

cxFloat cxCurveElasticInOut(cxAny pav,cxFloat time)
{
    cxAction this = pav;
    cxFloat newtime = 0;
    if (time == 0 || time == 1){
        newtime = time;
    }else{
        time = time * 2;
        if(this->assist.v1){
            this->assist.v1 = 0.3f * 1.5f;
        }
        cxFloat s = this->assist.v1 / 4;
        time = time - 1;
        if (time < 0){
            newtime = -0.5f * powf(2, 10 * time) * sinf((time -s) * M_PI_X2 / this->assist.v1);
        }else{
            newtime = powf(2, -10 * time) * sinf((time - s) * M_PI_X2 / this->assist.v1) * 0.5f + 1;
        }
    }
    return newtime;
}

cxFloat cxCurveSineIn(cxAny pav,cxFloat time)
{
    return (-1 * cosf(time * (cxFloat)M_PI_2) + 1);
}

cxFloat cxCurveSineOut(cxAny pav,cxFloat time)
{
    return (sinf(time * (cxFloat)M_PI_2));
}

cxFloat cxCurveSineInOut(cxAny pav,cxFloat time)
{
    return (-0.5f * (cosf((cxFloat)M_PI * time) - 1));
}

cxFloat cxCurveEaseIn(cxAny pav,cxFloat time)
{
    return powf(time, 2.0f);
}

cxFloat cxCurveEaseOut(cxAny pav,cxFloat time)
{
    return powf(time, 1.0f / 2.0f);
}

cxFloat cxCurveEaseInOut(cxAny pav,cxFloat time)
{
    time *= 2.0f;
    if (time < 1.0f){
        return (0.5f * powf(time, 2.0f));
    }else{
        return (1.0f - 0.5f * powf(2.0f - time, 2.0f));
    }
}

cxFloat cxCurveCubeIn(cxAny pav,cxFloat time)
{
    return powf(time, 3.0f);
}

cxFloat cxCurveCubeOut(cxAny pav,cxFloat time)
{
    return powf(time, 1.0f / 3.0f);
}

cxFloat cxCurveCubeInOut(cxAny pav,cxFloat time)
{
    time *= 2.0f;
    if (time < 1.0f){
        return (0.5f * powf(time, 1.0f/3.0f));
    }else{
        return (1.0f - 0.5f * powf(2.0f-time, 1.0f/3.0f));
    }
}

cxFloat cxCurveCubeInverse(cxAny pav,cxFloat time)
{
    time = time - 1.0f;
    return (powf(time, 3.0f) + 1.0f);
}

cxFloat cxCurveExpIn(cxAny pav,cxFloat time)
{
    return (time == 0 ? 0 : powf(2, 10 * (time/1 - 1)) - 1 * 0.001f);
}

cxFloat cxCurveExpOut(cxAny pav,cxFloat time)
{
    return (time == 1 ? 1 : (-powf(2, -10 * time / 1) + 1));
}

cxFloat cxCurveExpInOut(cxAny pav,cxFloat time)
{
    time /= 0.5f;
    if (time < 1){
        time = 0.5f * powf(2, 10 * (time - 1));
    }else{
        time = 0.5f * (-powf(2, -10 * (time - 1)) + 2);
    }
    return time;
}

static void cxCurveRegister(cxCurve this,cxConstChars name,cxActionCurveFunc func)
{
    cxCurveItem item = CX_ALLOC(cxCurveItem);
    item->func = func;
    cxHashSet(this->curves, cxHashStrKey(name), item);
    CX_RELEASE(item);
}

static cxCurve instance = NULL;

static cxCurve cxCurveInstance()
{
    if(instance == NULL){
        instance = CX_ALLOC(cxCurve);
    }
    return instance;
}

void cxCurveDestroy()
{
    CX_RELEASE(instance);
    instance = NULL;
}

cxCurveItem cxCurveGet(cxConstChars key)
{
    CX_RETURN(key == NULL, NULL);
    cxCurve this = cxCurveInstance();
    return cxHashGet(this->curves, cxHashStrKey(key));
}

CX_OBJECT_INIT(cxCurve, cxObject)
{
    this->curves = CX_ALLOC(cxHash);
    
    cxCurveRegister(this,"EaseIn",cxCurveEaseIn);
    cxCurveRegister(this,"EaseOut",cxCurveEaseOut);
    cxCurveRegister(this,"EaseInOut",cxCurveEaseInOut);
    
    cxCurveRegister(this,"CubeInverse",cxCurveCubeInverse);
    
    cxCurveRegister(this,"CubeIn",cxCurveCubeIn);
    cxCurveRegister(this,"CubeOut",cxCurveCubeOut);
    cxCurveRegister(this,"CubeInOut",cxCurveCubeInOut);
    
    cxCurveRegister(this,"ExpIn",cxCurveExpIn);
    cxCurveRegister(this,"ExpOut",cxCurveExpOut);
    cxCurveRegister(this,"ExpInOut",cxCurveExpInOut);
    
    cxCurveRegister(this,"SineIn",cxCurveSineIn);
    cxCurveRegister(this,"SineOut",cxCurveSineOut);
    cxCurveRegister(this,"SineInOut",cxCurveSineInOut);
    
    //need set cxAction.assist.v1
    cxCurveRegister(this,"ElasticIn",cxCurveElasticIn);
    cxCurveRegister(this,"ElasticOut",cxCurveElasticOut);
    cxCurveRegister(this,"ElasticInOut",cxCurveElasticInOut);
    
    cxCurveRegister(this,"BounceIn",cxCurveBounceIn);
    cxCurveRegister(this,"BounceOut",cxCurveBounceOut);
    cxCurveRegister(this,"BounceInOut",cxCurveBounceInOut);
    
    cxCurveRegister(this,"BackIn",cxCurveBackIn);
    cxCurveRegister(this,"BackOut",cxCurveBackOut);
    cxCurveRegister(this,"BackInOut",cxCurveBackInOut);
    
}
CX_OBJECT_FREE(cxCurve, cxObject)
{
    CX_RELEASE(this->curves);
}
CX_OBJECT_TERM(cxCurve, cxObject)

