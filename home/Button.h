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

CX_C_BEGIN

CX_OBJECT_DEF(Button, cxButton)
    CX_EVENT_ALLOC(onTap);
CX_OBJECT_END(Button, cxButton)

CX_C_END

#endif
