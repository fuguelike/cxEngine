//
//  cxMain.c
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <core/cxEngine.h>
#include <core/cxView.h>
#include <actions/cxRotate.h>
#include <views/cxLabelBMP.h>

void cxEngineInit(cxEngine engine)
{
    
}

void cxEngineMain(cxEngine engine)
{
    cxLabelBMP b = cxLabelBMPCreate(UTF8("font.json"), UTF8("Hello Everybody"));
    cxWindowPushView(b);
}

void cxEngineFree(cxEngine engine)
{
    
}
