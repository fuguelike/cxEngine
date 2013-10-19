//
//  cxHashTests.m
//  cxEngine
//
//  Created by xuhua on 9/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#import <XCTest/XCTest.h>
#include <core/cxHash.h>
#include <core/cxAutoPool.h>

@interface cxHashTests : XCTestCase

@end

@implementation cxHashTests

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

- (void)testHash
{
    cxHash hash = CX_ALLOC(cxHash);
    cxObject obj = CX_ALLOC(cxObject);
    cxHashSet(hash, cxHashIntKey(100), obj);
    
    cxObject obj2 = cxHashGet(hash, cxHashIntKey(100));
    XCTAssertTrue(obj == obj2);
    XCTAssertTrue(obj->cxRefcount == 2);
    
    XCTAssertTrue(cxHashLength(hash) == 1);
    
    cxHashDel(hash, cxHashIntKey(100));
    XCTAssertTrue(cxHashLength(hash) == 0);
    XCTAssertTrue(obj->cxRefcount == 1);
    
    CX_RELEASE(obj);
    
    CX_RELEASE(hash);
}

@end
