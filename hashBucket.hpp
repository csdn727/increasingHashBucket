#ifndef _HASHBUCKET__
#define  _HASHBUCKET__

#include <vector>
#include <map>
#include <stdio.h>


using namespace std ;

#define HASH_MAP_MAX_SIZE 1000
#define HASH_MAP_MIN_SIZE 512
#define BUCKET_MAX_SIZE 3
#define _OK 0
#define _ERR 1

#define HASHNUM(x) \
   ( ( x >= _cur_size )? (x % _cur_size ) : x )


struct HashEle
{
    int id ;
    char * data ;
};


class HashBucketMgr
{
    private :
    class HashBucket
    {
       private:
        // _bucket is composed of hashNum and HashEle
       multimap< int, HashEle > _bucket ;
       public :
       int insertEle( int hashNum, HashEle &ele ) ;
       int isEleExist( int hashNum, HashEle &ele ) ;
       int removeEle( int hashNum, HashEle &ele ) ;
       int findEle( int hashNum, HashEle &ele ) ;
       int reposition ( vector< pair< int, HashEle > > &repos ,int cur_size ) ;
       void snapshot() ;
       inline int getSize()
        {
            return _bucket.size() ;
        } ;
    };
    private :
    vector<HashBucket *> _bucketMgr ;
    int _cur_size ;
    private :
    int _processData( int id, char * data, unsigned int &hashNum, HashEle & ele ) ;
    int _extendSize();
    public :
    int initialize() ;
    int insertEle( char *data, int id ) ;
    int isEleExist( char *data, int id ) ;
    int removeEle( int id ) ;
    int findEle( int id ) ;
    void snapshot() ;
    ~HashBucketMgr()
    {
        HashBucket * p ;
        for (int i = 0;  i < _cur_size; ++i ) {
            p = _bucketMgr[i] ;
            if ( p ) {
                delete p ;
            }
        }
    }

};
#endif

    
    
