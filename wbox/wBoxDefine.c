//
//  cxBoxDefine.c
//  wbox
//
//  Created by xuhua on 4/20/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "wBoxDefine.h"

#define W_RAND_COLOR cxRand(wUnitColorRed, wUnitColorYellow)

void wUnitInit(wUnit *unit,wUnitType type)
{
    memset(unit, 0, sizeof(wUnit));
    switch (type) {
        case wUnitType_I:{
            /*
                ****
                ++++
            */
            unit->type = type;
            unit->dir = wUnitDirUp;
            unit->rotIdx = cxVec2iv(2, 1);
            unit->grids[0][1] = W_RAND_COLOR;
            unit->grids[1][1] = W_RAND_COLOR;
            unit->grids[2][1] = W_RAND_COLOR;
            unit->grids[3][1] = W_RAND_COLOR;
            break;
        }
        case wUnitType_J:{
            /*
             *+++
             ***+
             */
            unit->type = type;
            unit->dir = wUnitDirUp;
            unit->rotIdx = cxVec2iv(1, 0);
            unit->grids[0][1] = W_RAND_COLOR;
            unit->grids[0][0] = W_RAND_COLOR;
            unit->grids[1][0] = W_RAND_COLOR;
            unit->grids[2][0] = W_RAND_COLOR;
            break;
        }
        case wUnitType_L:{
            /*
             +++*
             +***
             */
            unit->type = type;
            unit->dir = wUnitDirUp;
            unit->rotIdx = cxVec2iv(2, 0);
            unit->grids[3][1] = W_RAND_COLOR;
            unit->grids[3][0] = W_RAND_COLOR;
            unit->grids[2][0] = W_RAND_COLOR;
            unit->grids[1][0] = W_RAND_COLOR;
            break;
        }
        case wUnitType_O:{
            /*
             +**+
             +**+
             ++++
             ++++
             */
            unit->type = type;
            unit->dir = wUnitDirUp;
            unit->rotIdx = cxVec2iv(-1, -1);//don't rot
            unit->grids[1][0] = W_RAND_COLOR;
            unit->grids[2][0] = W_RAND_COLOR;
            unit->grids[1][1] = W_RAND_COLOR;
            unit->grids[2][1] = W_RAND_COLOR;
            break;
        }
        case wUnitType_S:{
            /*
             ++**
             +**+
             ++++
             ++++
             */
            unit->type = type;
            unit->dir = wUnitDirUp;
            unit->rotIdx = cxVec2iv(2, 1);
            unit->grids[1][0] = W_RAND_COLOR;
            unit->grids[2][0] = W_RAND_COLOR;
            unit->grids[3][1] = W_RAND_COLOR;
            unit->grids[2][1] = W_RAND_COLOR;
            break;
        }
        case wUnitType_Z:{
            /*
             **++
             +**+
             ++++
             ++++
             */
            unit->type = type;
            unit->dir = wUnitDirUp;
            unit->rotIdx = cxVec2iv(1, 1);
            unit->grids[1][0] = W_RAND_COLOR;
            unit->grids[2][0] = W_RAND_COLOR;
            unit->grids[0][1] = W_RAND_COLOR;
            unit->grids[1][1] = W_RAND_COLOR;
            break;
        }
        case wUnitType_T:{
            /*
             +*++
             ***+
             ++++
             ++++
             */
            unit->type = type;
            unit->dir = wUnitDirUp;
            unit->rotIdx = cxVec2iv(1, 0);
            unit->grids[1][0] = W_RAND_COLOR;
            unit->grids[2][0] = W_RAND_COLOR;
            unit->grids[0][0] = W_RAND_COLOR;
            unit->grids[1][1] = W_RAND_COLOR;
            break;
        }
        default:{
            CX_ASSERT_FALSE("type error");
            break;
        }
    }
}
















