#include "hashBucket.hpp"

using namespace std ;

int HashBucketMgr::initialize()
{
    int rc = _OK ;
    HashBucket hb ;
    for (int i = 0; i < HASH_MAP_MIN_SIZE; ++i) {
        hb = new (nothrow) HashBucket ;
        if ( !hb )
        {
            printf("EOO_ERROR,can't initialize _bucketMgr");
            rc = _ERR ;
        }
        _bucketMgr.push_back(hb) ;
    }
    _cur_size = _bucketMgr.size() ;
    return rc ;
}

int HashBucketMgr::isEleExist ( char *data, int id )
{
    int rc = _OK ;
    int probe = 0 ;
    int hashNum = 0 ;
    HashEle ele ;
    
    rc = _processData( int id, data, hashNum, ele);
    if ( !rc ) {
        probe = 10 ;
        printf("ERR in HashBucketMgr isEleExist, probe is %d\n", probe) ;
        rc = _ERR ;
    } ;
    rc = _bucketMgr[ HASHNUM(hashNum) ].isEleExist (hashNum, ele) ;
    if ( !rc ) {
        probe = 15 ;
        printf("ERR in HashBucketMgr isEleExist, probe is %d\n", probe) ;
        rc = _ERR ;
    } ;
    return rc ;
}

int HashBucketMgr::insertEle( char *data, int id )
{
    int rc = _OK ;
    int probe = 0 ;
    int hashNum = 0 ;
    HashEle ele ;
    
    rc = _processData( id, data, hashNum, ele);
    if ( !rc ) {
        probe = 10 ;
        printf("ERR in HashBucketMgr insertEle, probe is %d\n", probe) ;
        rc = _ERR ;
    } ;
    if ( BUCKET_MAX_SIZE < _bucketMgr[ HASHNUM(hashNum) ].getSize() )
    {
        rc = _extendSize();
        if ( !rc ) {
            probe = 20 ;
            printf("ERR in HashBucketMgr _extendSize, probe is %d\n", probe) ;
            rc = _ERR ;
        } ;
    }
    rc = _bucketMgr[ HASHNUM(hashNum) ].insertEle (hashNum, ele) ;
    if ( !rc ) {
        probe = 30 ;
        printf("ERR in HashBucketMgr insertEle, probe is %d\n", probe) ;
        rc = _ERR ;
    } ;
    return rc ;
}

int HashBucketMgr::removeEle( int id )
{
    int rc = _OK ;
    int probe = 0 ;
    int hashNum = 0 ;
    HashEle ele ;
    
    rc = _processData( id, NULL, hashNum, ele);
    if ( !rc ) {
        probe = 10 ;
        printf("ERR in HashBucketMgr removeEle, probe is %d\n", probe) ;
        rc = _ERR ;
    } ;
    rc = _bucketMgr[ HASHNUM(hashNum) ].removeEle (hashNum, ele) ;
    if ( !rc ) {
        probe = 15 ;
        printf("ERR in HashBucketMgr removeEle, probe is %d\n", probe) ;
        rc = _ERR ;
    } ;
    return rc ;
}

int HashBucketMgr::findEle( int id )
{
    int rc = _OK ;
    int probe = 0 ;
    int hashNum = 0 ;
    HashEle ele ;
    
    rc = _processData( data, hashNum, ele);
    if ( !rc ) {
        probe = 10 ;
        printf("ERR in HashBucketMgr findEle, probe is %d\n", probe) ;
        rc = _ERR ;
    } ;
    rc = _bucketMgr[ HASHNUM(hashNum) ].findELe (hashNum, ele) ;
    if ( !rc ) {
        probe = 15 ;
        printf("ERR in HashBucketMgr findEle, probe is %d\n", probe) ;
        rc = _ERR ;
    } ;
    return rc ;
}

int HashBucketMgr::_processData( int id, char *data, int &hashNum, HashEle &ele )
{
    int rc = _OK ;
    
    ele.id = id ;
    ele.data = data ;
    
    hashNum = OSSHash ( id, sizeof(id) );
    return rc ;
}

int HashBucketMgr::HashBucket::insertEle( int hashNum, HashEle ele )
{
    int rc = _OK ;
    
    _bucket.insert( pair<int, HashEle>(hashNum,ele)) ;
    
    return rc ;
}

int HashBucketMgr::HashBucket::isEleExist( int hashNum, HashEle ele )
{
    int rc = _OK ;
    pair<MutilMap< int, HashEle >::iterator,
    MutilMap< int, HashEle >::iterator > ret ;
    ret = _bucket.equal_range(hashNum);
    
    for (MutilMap< int, HashEle>::iterator it = ret.first; it != ret.second; ++it) {
        if ( it->second.id = ele.id )
        {
            goto done ;
        }
    }
    rc = _ERR ;
    printf ("id %d not found in HashBucket::isEleExist \n", ele.id );
    
    done :
    return rc ;
}

int HashBucketMgr::HashBucket::removeEle( int hashNum, HashEle ele )
{
    int rc = _OK ;
    pair<MutilMap< int, HashEle >::iterator,
    MutilMap< int, HashEle >::iterator > ret ;
    ret = _bucket.equal_range(hashNum);
    
    for (MutilMap< int, HashEle>::iterator it = ret.first; it != ret.second; ++it) {
        if ( it->second.id = ele.id )
        {
            _bucket.erase(it);
            goto done ;
        }
    }
    rc = _ERR ;
    printf ("id %d not found\n HashBucket::removeEle\n", ele.id );
    
    done :
    return rc ;
    
}

int HashBucketMgr::HashBucket::findEle( int hashNum, HashEle ele )
{
    int rc = _OK ;
    pair<MutilMap< int, HashEle >::iterator,
    MutilMap< int, HashEle >::iterator > ret ;
    ret = _bucket.equal_range(hashNum);
    
    for (MutilMap< int, HashEle>::iterator it = ret.first; it != ret.second; ++it) {
        if ( it->second.id = ele.id )
        {
            printf ("id %d found in bucket %d\ndata: %s\n", ele.id, HASHNUM(hashNum)
                    ,it.second.data ) ;
            ele = it.second;
            goto done ;
        }
    }
    rc = _ERR ;
    printf ("id %d not found\n", ele.id );
    
    done :
    return rc ;
}

int HashBucketMgr::_extendSize()
{
    int rc = _OK ;
    HashBucket hb ;
    vector< pair< int, HashEle > repos ;
    
    if ( HASH_MAP_MAX_SIZE <= _cur_size )
    {
        printf (" _cur_size is too big ,now is %d\n", _cur_size );
        rc = _ERR ;
        goto done ;
    }
    
    hb = new(nothrow) HashBucket() ;
    if ( !hb )
    {
        printf ("OOM error in _extendSize\n" );
        rc = _ERR ;
        goto done ;
    }
    _bucketMgr.push_back(hb);
    ++_cur_size ;
    
    hb = _bucketMgr[_cur_size-HASH_MAP_MIN_SIZE];
    
    rc = hb.reposition( repos , _cur_size) ;
    if ( rc )
    {
        printf ("reposition err in _extendSize\n");
        rc = _ERR ;
        goto done ;
    }
    
    for ( int i = 0; i < repos.size(); ++i) {
        _bucketMgr[_cur_size-1].insertEle( repos[i] );
    }
    
    done :
    return rc ;
}

int HashBucketMgr::HashBucket::reposition(
                                          vector< pair< int, HashEle> > &repos,
                                          int _cur_size )
{
    int rc = _OK ;
    
    if ( _cur_size > HASH_MAP_MAX_SIZE || _cur_size < HASH_MAP_MIN_SIZE )
    {
        printf ("reposition err in HashBucket::reposition,wrong _cur_size :%d\n", _cur_size);
        rc = _ERR ;
    }
    
    for (MutilMap< int, HashEle>::iterator it = _bucket.first(); it != _bucket.end(); ++it) {
        if ( OSSHash ( id, sizeof(it->first) ) >  _cur_size )
        {
            repos.push_back(it);
            _bucket.erase(it);
        }
    }
    
    return rc ;
}

void HashBucketMgr::snapshot()
{
    printf("SNAPSHOT : \n_cur_size: %d", _cur_size );
    for (int i = 0;  i < _cur_size;  ++i) {
        printf("In bucket %d", i) ;
        _bucketMgr[i].snapshot();
    }
    printf("################################################################\n");
}

void HashBucketMgr::HashBucket::snapshot()
{
    int i = 0;
    for (MutilMap< int, HashEle>::iterator it = _bucket.first(); it != _bucket.end(); ++it) {
        if ( (i++ % 5) == 0) {
            printf("\n");
        }
        printf( "id :%d, data %s\t", it.second.id, it.second.data );
    }
}





















