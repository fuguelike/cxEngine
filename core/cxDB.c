//
//  cxDB.c
//  cxEngine
//
//  Created by xuhua on 10/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxEngine.h"
#include "cxDB.h"

CX_OBJECT_INIT(cxDBTxn, cxObject)
{

}
CX_OBJECT_FREE(cxDBTxn, cxObject)
{
    if(this->txn != NULL){
        this->txn->commit(this->txn,0);
        this->txn = NULL;
    }
}
CX_OBJECT_TERM(cxDBTxn, cxObject)


///////////////////////////////////////////////////////////////////////////////////////////

CX_OBJECT_INIT(cxDB, cxObject)
{
    this->type = DB_BTREE;
    CX_ASSERT(this->ret == 0, "create db error");
}
CX_OBJECT_FREE(cxDB, cxObject)
{
    this->dbptr->close(this->dbptr,0);
    CX_RELEASE(this->file);
    CX_RELEASE(this->table);
    CX_RELEASE(this->env);
    CX_METHOD_RELEASE(this->OpenAfter);
    CX_METHOD_RELEASE(this->OpenBefore);
}
CX_OBJECT_TERM(cxDB, cxObject)

cxDBEnv cxDBGetEnv(cxAny db)
{
    cxDB this = db;
    return this->env;
}

void cxDBSetEnv(cxAny db,cxDBEnv env)
{
    cxDB this = db;
    CX_RETAIN_SWAP(this->env, env);
}

cxBool cxDBOpen(cxAny db,cxString file,cxString table,cxBool rdonly)
{
    CX_ASSERT(file != NULL && table != NULL, "args error");
    cxDBEnv env = cxDBGetEnv(db);
    cxDB this = db;
    if(rdonly){
        this->flags = DB_RDONLY;
    }else{
        this->flags = DB_CREATE;
    }
    this->ret = db_create(&this->dbptr, env->env, 0);
    CX_RETURN(this->ret != 0,false);
    this->ret = CX_METHOD_FIRE(this->ret, this->OpenBefore,db);
    CX_RETURN(this->ret != 0,false);
    cxDBTxn txn = cxStackTop(env->txn);
    this->ret = this->dbptr->open(this->dbptr,cxDBTxnPtr(txn),cxStringBody(file),cxStringBody(table),this->type,this->flags,0);
    if(this->ret != 0){
        CX_ERROR("open db error %s:%s",cxStringBody(file),cxStringBody(table));
        return false;
    }
    CX_RETURN(this->ret != 0,false);
    CX_RETAIN_SWAP(this->file, file);
    CX_RETAIN_SWAP(this->table, table);
    return CX_METHOD_FIRE(true, this->OpenAfter,db);
}

cxBool cxDBHas(cxAny db,cxString key)
{
    cxDB this = db;
    cxDBEnv env = cxDBGetEnv(db);
    cxDBTxn txn = cxStackTop(env->txn);
    DBT k={0};
    k.data = (void *)cxStringBody(key);
    k.size = cxStringLength(key);
    k.flags = DB_DBT_USERMEM;
    return (this->dbptr->exists(this->dbptr,cxDBTxnPtr(txn),&k,0) == 0);
}

cxBool cxDBDel(cxAny db,cxString key)
{
    cxDB this = db;
    cxDBEnv env = cxDBGetEnv(db);
    CX_ASSERT(this->flags & DB_RDONLY, "only env ,can't del data");
    cxDBTxn txn = cxStackTop(env->txn);
    DBT k={0};
    k.data = (void *)cxStringBody(key);
    k.size = cxStringLength(key);
    k.flags = DB_DBT_USERMEM;
    return (this->dbptr->del(this->dbptr,cxDBTxnPtr(txn),&k,0) == 0);
}

cxBool cxDBSet(cxAny db,cxString key,cxString value)
{
    cxDB this = db;
    cxDBEnv env = cxDBGetEnv(db);
    CX_ASSERT(this->flags & DB_RDONLY, "only env ,can't set data");
    cxDBTxn txn = cxStackTop(env->txn);
    DBT k={0};
    k.data = (void *)cxStringBody(key);
    k.size = cxStringLength(key);
    k.flags = DB_DBT_USERMEM;
    DBT v={0};
    v.data = (void *)cxStringBody(value);
    v.size = cxStringLength(value);
    v.flags = DB_DBT_USERMEM;
    return this->dbptr->put(this->dbptr,cxDBTxnPtr(txn),&k,&v,0) == 0;
}

cxString cxDBGet(cxAny db,cxString key)
{
    cxDB this = db;
    cxDBEnv env = cxDBGetEnv(db);
    cxDBTxn txn = cxStackTop(env->txn);
    DBT k={0};
    k.data = (void *)cxStringBody(key);
    k.size = cxStringLength(key);
    k.flags = DB_DBT_USERMEM;
    DBT v={0};
    v.flags = DB_DBT_MALLOC;
    if(this->dbptr->get(this->dbptr, cxDBTxnPtr(txn), &k, &v,0) != 0){
        return NULL;
    }
    if(v.size == 0 || v.data == NULL){
        return NULL;
    }
    return cxStringAttach(v.data, v.size);
}

cxInt cxDBCount(cxAny db)
{
    cxDB this = db;
    cxDBEnv env = cxDBGetEnv(db);
    cxDBTxn txn = cxStackTop(env->txn);
    if(this->type == DB_BTREE){
        DB_BTREE_STAT *stat = NULL;
        this->dbptr->stat(this->dbptr,cxDBTxnPtr(txn),&stat,DB_READ_COMMITTED);
        return stat != NULL ? stat->bt_nkeys : 0;
    }
    if(this->type == DB_HASH){
        DB_HASH_STAT *stat = NULL;
        this->dbptr->stat(this->dbptr,cxDBTxnPtr(txn),&stat,DB_READ_COMMITTED);
        return stat != NULL ? stat->hash_nkeys : 0;
    }
    return 0;
}

cxDB cxDBTreeCreate(cxDBEnv env,cxString file,cxString table,cxBool rdonly)
{
    cxDB this = CX_CREATE(cxDB);
    this->type = DB_BTREE;
    cxDBSetEnv(this, env);
    if(!cxDBOpen(this, file, table, rdonly)){
        return NULL;
    }
    return this;
}

cxDB cxDBHashCreate(cxDBEnv env,cxString file,cxString table,cxBool rdonly)
{
    cxDB this = CX_CREATE(cxDB);
    this->type = DB_HASH;
    cxDBSetEnv(this, env);
    if(!cxDBOpen(this, file, table, rdonly)){
        return NULL;
    }
    return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////

static void cxDBEnvFeedback(DB_ENV *env, int flags, int progress)
{
    cxDBEnv this = env->app_private;
    CX_LOGGER("dbenv %s progress=%d",cxStringBody(this->type),progress);
    this->progress = progress;
}

void cxDBEnvSetPointAuto(cxDBEnv this,cxBool force)
{
    cxUInt flags = 0;
    this->env->get_flags(this->env,&flags);
    if(flags & DB_INIT_TXN){
        this->env->txn_checkpoint(this->env,1024*1024,30,force ? DB_FORCE : 0);
    }
}

static void cxDBEnvOnPause(cxAny pobj)
{
    cxDBEnv this = pobj;
    CX_ASSERT(this->opened, "env not open");
    cxDBEnvSetPointAuto(this, true);
}

void cxDBEnvBeginTxn(cxDBEnv this)
{
    CX_ASSERT(this->opened, "env not open");
    cxDBTxn txn = CX_CREATE(cxDBTxn);
    cxDBTxn top = cxStackTop(this->txn);
    if(this->env->txn_begin(this->env,cxDBTxnPtr(top),&txn->txn,0) == 0){
        cxStackPush(this->txn, txn);
    }
}

void cxDBEnvAbortTxn(cxDBEnv this)
{
    CX_ASSERT(this->opened, "env not open");
    cxDBTxn top = cxStackTop(this->txn);
    if(top != NULL && top->txn != NULL){
        top->txn->abort(top->txn);
        top->txn = NULL;
    }
    cxStackPop(this->txn);
}

void cxDBEnvCommitTxn(cxDBEnv this)
{
    CX_ASSERT(this->opened, "env not open");
    cxDBTxn top = cxStackTop(this->txn);
    if(top != NULL && top->txn != NULL){
        top->txn->commit(top->txn,0);
        top->txn = NULL;
    }
    cxStackPop(this->txn);
}

cxDBEnv cxDBEnvCreate(cxConstChars type,cxBool logger)
{
    cxEngine engine = cxEngineInstance();
    cxDBEnv env = cxHashGet(engine->dbenvs, cxHashStrKey(type));
    if(env != NULL){
        return env;
    }
    env = CX_CREATE(cxDBEnv);
    env->type = cxStringAllocChars(type);
    if(logger){
        env->env->set_flags(env->env,DB_AUTO_COMMIT, 1);
    }
    env->flags = DB_CREATE | DB_INIT_MPOOL;
    cxString home = cxAssetsPath(NULL);
    if(cxConstCharsEqu(type, "document")){
        home = cxDocumentPath(NULL);
        env->flags |= (logger ? (DB_RECOVER|DB_INIT_LOG|DB_INIT_TXN|DB_THREAD): 0);
    }
    if(home == NULL){
        CX_ERROR("get home path error");
        return NULL;
    }
    if(env->env->open(env->env,cxStringBody(home),env->flags,0) != 0){
        CX_ERROR("open home db env %s error",cxStringBody(home));
        return NULL;
    }
    env->opened = true;
    if(env->progress < 100){
        cxDBEnvFeedback(env->env, DB_VERB_RECOVERY, 100);
    }
    cxHashSet(engine->dbenvs, cxHashStrKey(type), env);
    return env;
}

CX_OBJECT_INIT(cxDBEnv, cxObject)
{
    cxEngine engine = cxEngineInstance();
    this->ret = db_env_create(&this->env, 0);
    CX_ASSERT(this->ret == 0, "db env create failed");
    this->env->app_private = this;
    this->env->set_cachesize(this->env,0, 1024*1024*5, 1);
    this->env->set_lg_max(this->env,1024*1024*5);
    this->env->set_flags(this->env,DB_YIELDCPU, 1);
    this->env->set_alloc(this->env,allocator->malloc,allocator->realloc,allocator->free);
    this->env->set_encrypt(this->env,"feelzx*&*1101++0529",DB_ENCRYPT_AES);
    this->env->set_verbose(this->env,DB_VERB_RECOVERY,0);
    this->env->set_feedback(this->env,cxDBEnvFeedback);
    CX_SLOT_CONNECT(engine->onPause, this, onPause, cxDBEnvOnPause);
    this->txn = CX_ALLOC(cxStack);
}
CX_OBJECT_FREE(cxDBEnv, cxObject)
{
    CX_RELEASE(this->txn);
    CX_SLOT_RELEASE(this->onPause);
    cxDBEnvSetPointAuto(this,true);
    this->env->close(this->env,0);
    this->env = NULL;
    CX_RELEASE(this->type);
}
CX_OBJECT_TERM(cxDBEnv, cxObject)

void cxDBEnvCheckPoint(cxDBEnv this,cxBool force)
{
    cxDBEnvSetPointAuto(this,false);
}

cxAny cxDBCreate(cxDBEnv env,cxConstChars file,cxConstChars table,cxConstChars type,cxBool rdonly)
{
    cxAny db = NULL;
    CX_ASSERT(env != NULL && file != NULL && table != NULL && type != NULL, "args error");
    if(cxConstCharsEqu(type, "btree")){
        db = cxDBTreeCreate(env,cxStringConstChars(file),cxStringConstChars(table),rdonly);
    }else if(cxConstCharsEqu(type, "hash")){
        db = cxDBHashCreate(env,cxStringConstChars(file),cxStringConstChars(table),rdonly);
    }
    return db;
}








