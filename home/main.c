//
//  cxMain.c
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <engine/cxEngine.h>
#include <engine/cxActionMgr.h>
#include "Map.h"
#include "Node.h"
#include "Scene.h"
#include "Button.h"
#include "Range.h"
#include "Move.h"
#include "Bullet.h"
#include "FightMap.h"
#include <types/Attack.h>
#include <types/Defence.h>
#include <types/Wall.h>
#include <types/Longer.h>
#include <types/Flyable.h>

void cxEngineType(cxEngine engine)
{
    CX_TYPE_REG(Map);
    CX_TYPE_REG(Node);
    CX_TYPE_REG(Scene);
    CX_TYPE_REG(Button);
    CX_TYPE_REG(Range);
    CX_TYPE_REG(Move);
    
    CX_TYPE_REG(FightMap);
    
    CX_TYPE_REG(Bullet);
    CX_TYPE_REG(Attack);
    CX_TYPE_REG(Defence);
    CX_TYPE_REG(Wall);
    CX_TYPE_REG(Flyable);
    CX_TYPE_REG(Longer);
}

#include <socket/cxEventBase.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <socket/cxUDP.h>

static void selectButton(cxAny pview)
{
    cxMessagePost("selectSubType", pview);
}

//struct sockaddr_in in;
//socklen_t addr_len = sizeof(struct sockaddr_in);
//struct event udp;
//
//void readdata(evutil_socket_t sockt, short which, void *arg)
//{
//    char bb[256]={0};
//    read(sockt, bb, 256);
////    int bytes = recvfrom(sockt, bb, 5, 0, (struct sockaddr *)&in, &addr_len);
//    CX_LOGGER("%s",bb);
//}

void cxEngineInit(cxEngine engine)
{
    
    cxUDP udp = cxUDPCreate("218.244.131.156", 8002);
    cxUDPWrite(udp, "123456", 6);
    
//    int fd = socket(AF_INET, SOCK_DGRAM, 0);
//    
//    struct in_addr s;
//    memset(&in, 0, sizeof(in));
//    in.sin_family = AF_INET;
//    inet_pton(AF_INET, "192.168.1.100", (void *)&s);
//    in.sin_addr.s_addr = s.s_addr;
//    in.sin_port = htons(8002);
////    int ret = bind(fd, (struct sockaddr*)&in, sizeof(in));
////    CX_ASSERT(ret == 0, "bind udp error");
//    
////    write(fd, "12345", 5);
//    int bytes = sendto(fd, "12345", 5, 0, (struct sockaddr *)&in, addr_len);
//    
//    
//    cxEventBase base = cxEventBaseInstance();
//    
//    event_set(&udp, fd, EV_READ | EV_PERSIST, readdata, engine);
//    event_base_set(base->base, &udp);
//    event_add(&udp, NULL);
//    
//    while (true){
//        char bb[256]={0};
//        bytes = recvfrom(fd, bb, 5, 0, (struct sockaddr *)&in, &addr_len);
//        CX_LOGGER("%s",bb);
//    }
    cxEngineSetDesignSize(cxSize2fv(2048, 1536));
#if !defined(NDEBUG)
    engine->isShowBorder = true;
#else
    engine->isShowBorder = false;
#endif
    
    //战斗用动画管理器
    cxActionMgr fight = CX_CREATE(cxActionMgr);
    fight->scale = 1.0f;
    cxActionMgrSet(FIGHT_ACTION_GROUP, fight);
}
void cxEngineMain(cxEngine engine)
{
    //全局数据初始化
    GlobalInit(engine);
    
    cxLoader loader = cxLoaderCreate("main.json");
    CX_ASSERT_VALUE(cxLoaderGetRoot(loader), Scene, scene);
    scene->fightMap = cxLoaderGet(loader, "map");
    CX_ASSERT_TYPE(scene->fightMap, FightMap);
    
    //test code
    CX_LOADER_DEF(loader, Button, btnSelectTurret);
    CX_ADD(Button, btnSelectTurret, onTap, selectButton);
    
    CX_LOADER_DEF(loader, Button, btnSelectSoldier);
    CX_ADD(Button, btnSelectSoldier, onTap, selectButton);
    
    CX_LOADER_DEF(loader, Button, btnSelectArcher);
    CX_ADD(Button, btnSelectArcher, onTap, selectButton);
    
    CX_LOADER_DEF(loader, Button, btnTest);
    CX_ADD(Button, btnTest, onTap, selectButton);
    
    CX_LOADER_DEF(loader, Button, btnLonger);
    CX_ADD(Button, btnLonger, onTap, selectButton);
    
    CX_LOADER_DEF(loader, Button, btnClear);
    CX_ADD(Button, btnClear, onTap, selectButton);
    
    CX_LOADER_DEF(loader, Button, btnFlyable);
    CX_ADD(Button, btnFlyable, onTap, selectButton);
    
    FightMapInit(scene->fightMap);
    cxWindowPushView(scene);
}

void cxEngineFree(cxEngine engine)
{
    
}
