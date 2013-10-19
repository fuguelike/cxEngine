//
//  cxArrayTests.m
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxArray.h>
#include <core/cxAutoPool.h>
#import <XCTest/XCTest.h>

@interface cxArrayTests : XCTestCase{
    cxArray array;
}
@end

@implementation cxArrayTests

- (void)setUp
{
    [super setUp];
    cxAutoPoolPush();
    array = CX_ALLOC(cxArray);
}

- (void)tearDown
{
    CX_RELEASE(array);
    cxAutoPoolPop();
    [super tearDown];
}

-(void)testAppend
{
    cxObject obj = CX_ALLOC(cxObject);
    cxArrayAppend(array, obj);
    CX_RELEASE(obj);
    
    XCTAssertTrue(cxArrayLength(array) == 1);
    
    cxArrayClean(array);
    XCTAssertTrue(cxArrayLength(array) == 0);
    
    for(int i=0; i < 1000;i++) {
        obj = CX_ALLOC(cxObject);
        cxArrayAppend(array, obj);
        CX_RELEASE(obj);
    }
    XCTAssertTrue(cxArrayLength(array) == 1000);
    
    cxArrayClean(array);
    XCTAssertTrue(cxArrayLength(array) == 0);
}


-(void)testRemove
{
    cxObject obj = CX_ALLOC(cxObject);
    cxArrayAppend(array, obj);
    CX_RELEASE(obj);
    
    cxArrayRemove(array, obj);
    XCTAssertTrue(cxArrayLength(array) == 0);
    
    for(int i=0; i < 1000;i++) {
        obj = CX_ALLOC(cxObject);
        cxArrayAppend(array, obj);
        CX_RELEASE(obj);
        cxArrayRemove(array, obj);
    }
    
    XCTAssertTrue(cxArrayLength(array) == 0);
}
@end










