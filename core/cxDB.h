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
    cxString type;
    cxUInt flags;
CX_OBJECT_END(cxDBEnv)

cxDBEnv cxDBEnvCreate(cxConstChars type,cxBool logger);

void cxDBEnvBeginTxn(cxDBEnv this);

void cxDBEnvAbortTxn(cxDBEnv this);

void cxDBEnvCommitTxn(cxDBEnv this);

void cxDBEnvCheckPoint(cxDBEnv this,cxBool force);

typedef cxBool (*cxDBFunc)(cxAny dbptr);

CX_OBJECT_DEF(cxDB, cxObject)
    cxDBEnv env;
    DB *dbptr;
    cxString file;
    cxString table;
    cxInt ret;
    DBTYPE type;
    cxUInt flags;
    CX_METHOD_DEF(cxDBFunc, OpenBefore);
    CX_METHOD_DEF(cxDBFunc, OpenAfter);
CX_OBJECT_END(cxDB)

cxBool cxDBOpen(cxAny db,cxString file,cxString table,cxBool rdonly);

cxDBEnv cxDBGetEnv(cxAny db);

void cxDBSetEnv(cxAny db,cxDBEnv env);

cxAny cxDBTypesGet(cxConstChars src);

cxInt cxDBCount(cxAny db);

cxBool cxDBHas(cxAny db,cxString key);

cxBool cxDBDel(cxAny db,cxString key);

cxBool cxDBSet(cxAny db,cxString key,cxString value);

cxString cxDBGet(cxAny db,cxString key);

cxDB cxDBTreeCreate(cxDBEnv env,cxString file,cxString table,cxBool rdonly);

cxDB cxDBHashCreate(cxDBEnv env,cxString file,cxString table,cxBool rdonly);

cxAny cxDBCreate(cxDBEnv env,cxConstChars file,cxConstChars table,cxConstChars type,cxBool rdonly);

CX_C_END

#endif
