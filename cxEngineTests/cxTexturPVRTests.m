//
//  cxTexturPVRTests.m
//  cxEngine
//
//  Created by xuhua on 9/28/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#import <XCTest/XCTest.h>
#include <textures/cxTexturePVR.h>
#include <streams/cxAssetsStream.h>
#include <core/cxAutoPool.h>

@interface cxTexturPVRTests : XCTestCase

@end

@implementation cxTexturPVRTests

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

- (void)testPVR
{

}

@end
