//
//  cxDB.h
//  cxEngine
//
//  Created by xuhua on 10/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxDB_h
#define cxEngine_cxDB_h

#include <db.h>
#include "cxEngine.h"
#include "cxBase.h"

CX_C_BEGIN

CX_OBJECT_DEF(cxDBTxn,cxObject)
    DB_TXN *txn;
CX_OBJECT_END(cxDBTxn)

#define cxDBTxnPtr(t) (t != NULL ? t->txn : NULL)

CX_OBJECT_DEF(cxDBEnv, cxObject)
    DB_ENV *env;
    cxInt ret;
    CX_SLOT_ALLOC(onPause);
    cxInt progress;
    cxBool opened;
    cxStack txn;
CX_OBJECT_END(cxDBEnv)

void cxDBEnvBeginTxn();

void cxDBEnvAbortTxn();

void cxDBEnvCommitTxn();

cxDBEnv cxDBEnvInstance();

void cxDBEnvDestroy();

void cxDBEnvSetPoint(cxBool force);

cxBool cxDBEnvOpen();

typedef cxBool (*cxDBFunc)(cxAny dbptr);

CX_OBJECT_DEF(cxDB, cxObject)
    DB *dbptr;
    cxString file;
    cxString table;
    cxInt ret;
    DBTYPE type;
    cxUInt flags;
    CX_METHOD_DEF(cxDBFunc, OpenBefore);
    CX_METHOD_DEF(cxDBFunc, OpenAfter);
CX_OBJECT_END(cxDB)

cxAny cxDBTypesGet(cxConstChars src);

cxInt cxDBCount(cxAny db);

cxBool cxDBHas(cxAny db,cxString key);

cxBool cxDBDel(cxAny db,cxString key);

cxBool cxDBPut(cxAny db,cxString key,cxString value);

cxString cxDBGet(cxAny db,cxString key);

CX_OBJECT_DEF(cxDBTree, cxDB)

CX_OBJECT_END(cxDBTree)

cxDBTree cxDBTreeCreate(cxString file,cxString table);

CX_OBJECT_DEF(cxDBHash, cxDB)

CX_OBJECT_END(cxDBHash)

cxDBHash cxDBHashCreate(cxString file,cxString table);

CX_C_END

#endif
