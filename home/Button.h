//
//  Button.h
//  Home
//
//  Created by xuhua on 9/12/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef Home_Button_h
#define Home_Button_h

#include <views/cxButton.h>
#include <views/cxLabelTTF.h>

CX_C_BEGIN

CX_DEF(Button, cxButton)
    CX_EVENT_ALLOC(onTap);
    cxLabelTTF ttf;
CX_END(Button, cxButton)

CX_C_END

#endif
