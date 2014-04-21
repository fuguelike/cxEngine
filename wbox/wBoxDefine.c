//
//  cxBoxDefine.c
//  wbox
//
//  Created by xuhua on 4/20/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "wBoxDefine.h"

cxBool wBoxCheckIdx(cxVec2i idx)
{
    return idx.x >= 0 && idx.x < W_BOX_COL && idx.y <=0 && idx.y < W_BOX_ROW;
}

cxVec2i wBoxInitIdx(cxVec2i idx)
{
    cxVec2i ii;
    ii.x = idx.x + 3;
    ii.y = idx.y + W_BOX_ROW - 4;
    return ii;
}

#define W_RAND_COLOR cxRand(wUnitColorRed, wUnitColorYellow)

void wUnitInit(wUnit *unit,wUnitType type)
{
    memset(unit, 0, sizeof(wUnit));
    switch (type) {
        case wUnitType_I:{
            /*
                ****
                ++++
                ++++
                ++++
            */
            unit->type = type;
            unit->dir = wUnitDirUp;
            unit->rotIdx = cxVec2iv(2, 3);
            unit->grids[0][3] = W_RAND_COLOR;
            unit->grids[1][3] = W_RAND_COLOR;
            unit->grids[2][3] = W_RAND_COLOR;
            unit->grids[3][3] = W_RAND_COLOR;
            break;
        }
        case wUnitType_J:{
            /*
             *+++
             ***+
             ++++
             ++++
             */
            unit->type = type;
            unit->dir = wUnitDirUp;
            unit->rotIdx = cxVec2iv(1, 2);
            unit->grids[0][3] = W_RAND_COLOR;
            unit->grids[0][2] = W_RAND_COLOR;
            unit->grids[1][2] = W_RAND_COLOR;
            unit->grids[2][2] = W_RAND_COLOR;
            break;
        }
        case wUnitType_L:{
            /*
             +++*
             +***
             ++++
             ++++
             */
            unit->type = type;
            unit->dir = wUnitDirUp;
            unit->rotIdx = cxVec2iv(2, 2);
            unit->grids[3][3] = W_RAND_COLOR;
            unit->grids[3][2] = W_RAND_COLOR;
            unit->grids[2][2] = W_RAND_COLOR;
            unit->grids[1][2] = W_RAND_COLOR;
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
            unit->grids[1][2] = W_RAND_COLOR;
            unit->grids[2][2] = W_RAND_COLOR;
            unit->grids[1][3] = W_RAND_COLOR;
            unit->grids[2][3] = W_RAND_COLOR;
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
            unit->rotIdx = cxVec2iv(2, 3);
            unit->grids[1][2] = W_RAND_COLOR;
            unit->grids[2][2] = W_RAND_COLOR;
            unit->grids[3][3] = W_RAND_COLOR;
            unit->grids[2][3] = W_RAND_COLOR;
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
            unit->rotIdx = cxVec2iv(1, 3);
            unit->grids[1][2] = W_RAND_COLOR;
            unit->grids[2][2] = W_RAND_COLOR;
            unit->grids[0][3] = W_RAND_COLOR;
            unit->grids[1][3] = W_RAND_COLOR;
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
            unit->rotIdx = cxVec2iv(1, 2);
            unit->grids[1][2] = W_RAND_COLOR;
            unit->grids[2][2] = W_RAND_COLOR;
            unit->grids[0][2] = W_RAND_COLOR;
            unit->grids[1][3] = W_RAND_COLOR;
            break;
        }
        default:{
            CX_ASSERT_FALSE("type error");
            break;
        }
    }
}

#define R(x1,y1,x2,y2)  CX_SWAP(unit->grids[x1][y1], unit->grids[x2][y2], cxUInt);

void wUnitRotate(wUnit *unit)
{
    unit->dir ++;
    if(unit->dir > wUnitDirLeft){
        unit->dir = wUnitDirUp;
    }
    switch (unit->type) {
        case wUnitType_I:{
            /*
             ****
             ++++
             ++++
             ++++
             */
            if(unit->dir == wUnitDirUp){
                
            }else if(unit->dir == wUnitDirRight){
                
            }else if(unit->dir == wUnitDirDown){
                
            }else if(unit->dir == wUnitDirLeft){
                
            }
            break;
        }
        case wUnitType_J:{
            /*
             *+++
             ***+
             */
            if(unit->dir == wUnitDirUp){
                 
             }else if(unit->dir == wUnitDirRight){
                 
             }else if(unit->dir == wUnitDirDown){
                 
             }else if(unit->dir == wUnitDirLeft){
                 
             }
            break;
        }
        case wUnitType_L:{
            /*
             +++*
             +***
             */
            if(unit->dir == wUnitDirUp){
                
            }else if(unit->dir == wUnitDirRight){
                
            }else if(unit->dir == wUnitDirDown){
                
            }else if(unit->dir == wUnitDirLeft){
                
            }
            break;
        }
        case wUnitType_O:{
            /*
             +**+
             +**+
             ++++
             ++++
             */
            break;
        }
        case wUnitType_S:{
            /*
             ++**
             +**+
             ++++
             ++++
             */
            if(unit->dir == wUnitDirUp){
                
            }else if(unit->dir == wUnitDirRight){
                
            }else if(unit->dir == wUnitDirDown){
                
            }else if(unit->dir == wUnitDirLeft){
                
            }
            break;
        }
        case wUnitType_Z:{
            if(unit->dir == wUnitDirUp){
                /*
                 **++
                 +**+
                 ++++
                 ++++
                 */
                R(1,3,2,2);
                R(0,2,1,3);
                R(0,1,0,3);
            }else if(unit->dir == wUnitDirRight){
                /*
                 ++*+
                 +**+
                 +*++
                 ++++
                 */
                R(2,2,1,1);
                R(1,3,2,2);
                R(0,3,2,3);
            }else if(unit->dir == wUnitDirDown){
                /*
                 ++++
                 **++
                 +**+
                 ++++
                 */
                R(1,1,0,2);
                R(2,2,1,1);
                R(2,3,2,1);
            }else if(unit->dir == wUnitDirLeft){
                /*
                 +*++
                 **++
                 *+++
                 ++++
                 */
                R(0,2,1,3);
                R(1,1,0,2);
                R(2,1,0,1);
            }
            break;
        }
        case wUnitType_T:{
            if(unit->dir == wUnitDirUp){
                /*
                 +*++
                 ***+
                 ++++
                 ++++
                 */
                R(1,3,2,2);
                R(0,2,1,3);
                R(1,1,0,2);
            }else if(unit->dir == wUnitDirRight){
                /*
                 +*++
                 +**+
                 +*++
                 ++++
                 */
                R(2,2,1,1);
                R(1,3,2,2);
                R(0,2,1,3);
            }else if(unit->dir == wUnitDirDown){
                /*
                 ++++
                 ***+
                 +*++
                 ++++
                 */
                R(1,1,0,2);
                R(2,2,1,1);
                R(1,3,2,2);
            }else if(unit->dir == wUnitDirLeft){
                /*
                 +*++
                 **++
                 +*++
                 ++++
                 */
                R(0,2,1,3);
                R(1,1,0,2);
                R(2,2,1,1);
            }
            break;
        }
        default:{
            CX_ASSERT_FALSE("type error");
            break;
        }
    }
}
















