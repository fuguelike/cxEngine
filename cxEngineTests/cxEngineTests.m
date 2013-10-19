//
//  cxEngineTests.m
//  cxEngineTests
//
//  Created by xuhua on 9/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxEngine.h>
#import <XCTest/XCTest.h>
#include <core/cxAutoPool.h>

@interface cxEngineTests : XCTestCase

@end

@implementation cxEngineTests

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

-(void)testObject
{
    cxObject obj = CX_ALLOC(cxObject);
    XCTAssertTrue(obj->cxRefcount == 1);
    
    CX_RETAIN(obj);
    XCTAssertTrue(obj->cxRefcount == 2);
    
    CX_RELEASE(obj);
    XCTAssertTrue(obj->cxRefcount == 1);
    
    CX_RETAIN(obj);
    XCTAssertTrue(obj->cxRefcount == 2);
    
    CX_RELEASE(obj);
    CX_RELEASE(obj);
}
@end
