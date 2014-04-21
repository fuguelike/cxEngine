//
//  cxBoxDefine.h
//  cxEngine
//
//  Created by xuhua on 4/20/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef wbox_cxBoxDefine_h
#define wbox_cxBoxDefine_h

#include <core/cxEngine.h>

CX_C_BEGIN

#define W_BOX_COL 10

#define W_BOX_ROW 20

cxBool wBoxCheckIdx(cxVec2i idx);

cxVec2i wBoxInitIdx(cxVec2i idx);

typedef enum {
    wUnitColorNone = 0,
    wUnitColorRed,      //红
    wUnitColorOrange,   //橙
    wUnitColorBlue,     //蓝
    wUnitColorGreen,    //绿
    wUnitColorPurple,   //紫
    wUnitColorYellow,   //黄
}wUnitColor;

typedef enum {
    wUnitTypeNone = 0,
    wUnitType_I,
    wUnitType_J,
    wUnitType_L,
    wUnitType_O,
    wUnitType_S,
    wUnitType_Z,
    wUnitType_T,
}wUnitType;

typedef enum {
    wUnitDirNone = 0,
    wUnitDirUp,
    wUnitDirRight,
    wUnitDirDown,
    wUnitDirLeft
}wUnitDir;

//4x4
typedef struct {
    cxVec2i idx;
    cxVec2i rotIdx;     // x y >= 0 can rot
    cxUInt grids[4][4]; // -> wUnitColorRed - wUnitColorYellow
    wUnitType type;
    wUnitDir dir;    
}wUnit;

void wUnitInit(wUnit *unit,wUnitType type);

void wUnitRotate(wUnit *unit);

CX_C_END

#endif
