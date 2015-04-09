//
//  cxWindow.c
//  cxEngine
//
//  Created by xuhua on 10/2/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#include "cxWindow.h"

CX_METHOD_DEF(cxWindow,OnKey,cxBool,const cxKey *key)
{
    if(key->code == CX_KEYCODE_BACK && key->type == cxKeyTypeUp){
        cxEngineTerminate();
        return true;
    }
    return false;
}

//cxVec2f curr={0};
//
//cxVec2f s1 = {0};
//cxVec2f s2 = {0};
//
//CX_METHOD_DEF(cxWindow, OnDraw, void)
//{
//    //参数
//    cxVec2f src = cxVec2fv(20, 20);
//    cxFloat srcBody = 100;
//    cxVec2f dst = cxVec2fv(200, 500);
//    cxFloat dstBody = 300;
//    //结果
//    cxVec2f ret1 = src;
//    cxVec2f ret2 = dst;
//    //算法
//    
//    cxBox4f box = cxBox4fNewForCircle(cxVec2fv(0, 0), 100);
//    
//    cxLineBoxInfo b = cxLineBoxIsIntersect(cxLineVec2fv(s1, s2), box, cxBoxEdgeFour|cxBoxEdgeInside);
//    cxColor3f c = b.num > 0 ? cxGREEN:cxORANGE;
//    cxDrawBox(box, c);
//    
//    
////    cxVec2f x1 = cxVec2fv(0, 100);
////    cxVec2f x2 = curr;
//    
////    cxBool inter = cxTwoLineIsIntersect(src, dst, x1, x2);
//    
////    cxInt r = cxVec2fAtLineLR(src, dst, curr);
////
//    
//    //绘制
//    cxVec2f v[2];
////    v[0] = src;
////    v[1] = dst;
//    
////    cxDrawPoint(src, cxBLUE, srcBody);
////    cxDrawPoint(dst, cxGRAY, dstBody);
//    
////    cxDrawLines(v, 2, cxRED);
////
//    v[0] = s1;
//    v[1] = s2;
//    cxDrawLines(v, 2,cxYELLOW);
//    
////    jd = cxTwoLineIntersectPoint(src, dst, s1, s2);
//    
//    
////    cxDrawPoint(ret1, cxRED, 10);
//    
//    for(cxInt i=0; i < b.num;i++){
//        cxDrawPoint(b.ps[i].point, cxRED, 20);
//    }
//    
//}
//CX_METHOD_DEF(cxWindow, OnTouch, cxBool,const cxTouchItems *points)
//{
//    if(points->number != 2){
//        return false;
//    }
//    cxTouchItem item1 = points->items[0];
//    cxHitInfo info1 = cxViewHitTest(this, item1->position);
//    cxTouchItem item2 = points->items[1];
//    cxHitInfo info2 = cxViewHitTest(this, item2->position);
//    if(!info1.hited){
//        return false;
//    }
//    s1 = info1.position;
//    s2 = info2.position;
//    return false;
//}
CX_TYPE(cxWindow, cxView)
{

    CX_METHOD(cxWindow, OnKey);
//    CX_METHOD(cxWindow, OnDraw);
//    CX_METHOD(cxWindow, OnTouch);
}
CX_INIT(cxWindow, cxView)
{
    this->views = CX_ALLOC(cxStack);
}
CX_FREE(cxWindow, cxView)
{
    CX_RELEASE(this->views);
}
CX_TERM(cxWindow, cxView)

void cxWindowClear()
{
    cxViewClear(engine->Window);
    cxStackClear(engine->Window->views);
}

cxAny cxWindowHeadView()
{
    cxWindow window = cxEngineGetWindow();
    cxList subviews = cxViewGetSubViews(window);
    return cxListHead(subviews);
}

cxAny cxWindowTailView()
{
    cxWindow window = cxEngineGetWindow();
    cxList subviews = cxViewGetSubViews(window);
    return cxListTail(subviews);
}

cxAny cxWindowTopView()
{
    cxEngine engine = cxEngineInstance();
    return cxStackTop(engine->Window->views);
}

void cxWindowAppendTop(cxAny pview)
{
    cxView top = cxWindowTopView();
    CX_ASSERT(top != NULL, "top view null");
    cxViewAppend(top, pview);
}

void cxWindowAppend(cxAny pview)
{
    CX_ASSERT_TYPE(pview, cxView);
    cxWindow window = cxEngineGetWindow();
    cxViewAppend(window, pview);
}

void cxWindowPushView(cxAny pview)
{
    CX_ASSERT_TYPE(pview, cxView);
    cxEngine engine = cxEngineInstance();
    cxView new = pview;
    cxView top = cxWindowTopView();
    if(top != NULL && cxViewGetSleepTop(new)){
        cxViewSetIsSleep(top, true);
        cxViewExit(top);
    }
    cxStackPush(engine->Window->views, new);
    cxViewAppend(engine->Window, new);
}

void cxWindowPopView()
{
    cxEngine engine = cxEngineInstance();
    cxBool sleepTop = false;
    cxView prev = cxWindowTopView();
    cxStackPop(engine->Window->views);
    cxView top = cxWindowTopView();
    if(prev != NULL){
        sleepTop = cxViewGetSleepTop(prev);
        cxViewRemove(prev);
    }
    if(top != NULL && sleepTop){
        cxViewEnter(top);
        cxViewSetIsSleep(top, false);
    }
}

void cxWindowReplaceView(cxAny pview)
{
    CX_ASSERT_TYPE(pview, cxView);
    cxWindowPopView();
    cxWindowPushView(pview);
}












