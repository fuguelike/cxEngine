//
//  cxStreamTests.m
//  cxEngine
//
//  Created by xuhua on 9/27/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#import <XCTest/XCTest.h>
#include <streams/cxAssetsStream.h>
#include <streams/cxMemStream.h>
#include <core/cxAutoPool.h>

@interface cxStreamTests : XCTestCase

@end

@implementation cxStreamTests

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

-(void)testMemStream
{
    cxStream mem = CX_CREATE(cxMemStream);
    cxBool ret = mem->interface->Open(mem);
    XCTAssertTrue(ret);
    mem->interface->Write(mem,"12345",5);
    XCTAssertTrue(mem->length == 5);
    XCTAssertTrue(mem->interface->Position(mem) == 5);
    mem->interface->Seek(mem,2,SEEK_SET);
    XCTAssertTrue(mem->interface->Position(mem) == 2);
    mem->interface->Seek(mem,2,SEEK_CUR);
    XCTAssertTrue(mem->interface->Position(mem) == 4);
    mem->interface->Seek(mem,-2,SEEK_END);
    XCTAssertTrue(mem->interface->Position(mem) == 3);
    char buf[5];
    mem->interface->Seek(mem,0,SEEK_SET);
    cxInt size = mem->interface->Read(mem,buf,5);
    XCTAssertTrue(size == 5);
    XCTAssertTrue(memcmp(buf, "12345", 5) == 0);
    XCTAssertTrue(mem->interface->Position(mem) == 5);
    
    mem->interface->Write(mem,"67890",5);
    XCTAssertTrue(mem->interface->Position(mem) == 10);
    
    for(int i=0; i < 100; i++){
        mem->interface->Write(mem,"67890",5);
    }
    XCTAssertTrue(mem->interface->Position(mem) == 510);
    cxString data = mem->interface->AllBytes(mem);
    XCTAssertTrue(cxStringLength(data) == 510);
}

- (void)testAssetsStream
{
    cxStream pvr = cxAssetsStreamCreate("res/2.pvr");
    cxBool ret = pvr->interface->Open(pvr);
    XCTAssertTrue(ret);
    cxString bytes = pvr->interface->AllBytes(pvr);
    XCTAssertTrue(cxStringLength(bytes) == pvr->length);
    
}

@end
