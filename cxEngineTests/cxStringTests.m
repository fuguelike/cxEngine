//
//  cxStringTests.m
//  cxEngine
//
//  Created by xuhua on 9/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#import <XCTest/XCTest.h>
#include <core/cxString.h>
#include <core/cxAutoPool.h>

@interface cxStringTests : XCTestCase

@end

@implementation cxStringTests

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

-(void)testCreate
{
    
    cxString s1 = cxStringConstChars("aabb");
    cxString s2 = cxStringCreate("aabb");
    XCTAssertTrue(cxStringEqu(s1, s2));
    
    cxString s3 = cxStringCreate("%d",100);
    cxString s4 = cxStringConstChars("100");
    XCTAssertTrue(cxStringEqu(s4, s3));
    
    
}

- (void)testString
{
    cxString s = cxStringStatic("aabb");
    cxConstChars x = cxStringBody(s);
    XCTAssertTrue(strcmp("aabb", x) == 0);
}

@end
