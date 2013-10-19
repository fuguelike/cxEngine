//
//  cxStackTests.m
//  cxEngine
//
//  Created by xuhua on 9/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#import <XCTest/XCTest.h>
#include <core/cxStack.h>
#include <core/cxAutoPool.h>

@interface cxStackTests : XCTestCase

@end

@implementation cxStackTests

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

- (void)testStack
{
    cxObject obj = CX_ALLOC(cxObject);
    cxStack stack = CX_ALLOC(cxStack);
    
    cxStackPush(stack, obj);
    XCTAssertTrue(obj->cxRefcount == 2);
    XCTAssertTrue(cxStackLength(stack) == 1);
    
    cxStackPop(stack);
    XCTAssertTrue(cxStackLength(stack) == 0);
    XCTAssertTrue(obj->cxRefcount == 1);
    
    CX_RELEASE(stack);
    CX_RELEASE(obj);
}

@end
