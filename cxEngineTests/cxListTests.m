//
//  cxListTests.m
//  cxEngine
//
//  Created by xuhua on 9/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#import <XCTest/XCTest.h>
#include <core/cxList.h>
#include <core/cxAutoPool.h>

@interface cxListTests : XCTestCase

@end

@implementation cxListTests

- (void)setUp
{
    [super setUp];
    cxAutoPoolPush();
}

- (void)tearDown
{
    cxAutoPoolPop();
    [super tearDown];
}

- (void)testList
{
    cxObject obj = CX_ALLOC(cxObject);
    cxList list = CX_ALLOC(cxList);
    
    cxListElement *e = cxListAppend(list, obj);
    XCTAssertTrue(cxListLength(list) == 1);
    
    e = cxListFirst(list);
    XCTAssertTrue(e->object == obj);
    
    cxListRemove(list, e);
    XCTAssertTrue(cxListLength(list) == 0);
    
    CX_RELEASE(list);
    CX_RELEASE(obj);
}

@end
