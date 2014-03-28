//
//  fcTreasureBox.h
//  cxEngine
//  存放金币的宝箱
//  Created by xuhua on 3/28/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef fruit_fcTreasureBox_h
#define fruit_fcTreasureBox_h

#include "fcSprite.h"

CX_C_BEGIN

CX_OBJECT_DEF(fcTreasureBox, fcSprite)
    //黄金数量
    cxInt gold;
CX_OBJECT_END(fcTreasureBox)

void fcTreasureBoxInit(fcTreasureBox this,cxAny map);

CX_C_END

#endif
