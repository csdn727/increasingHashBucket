#ifndfef _HASHBUCKET__
#define  _HASHBUCKET__

#include <vector>
#include <map>
#include <stdio.h>

using namespace std ;

#define HASH_MAP_MAX_SIZE 1000;
#define HASH_MAP_MIN_SIZE 512;
#define BUCKET_MAX_SIZE 20
#define _OK 0;
#define _ERR

#define HASHNUM(x) \
    if ( x > _cur_size ) \
       return (x % _cur_size ) ; \
    return x;

struct hashEle
{
    int id ;
    char * data ;
}HashEle ;

class HashBucketMgr
{
    private :
    class HashBucket
    {
       private:
        // _bucket is composed of hashNum and HashEle
       MutilMap < int, HashEle > _bucket ;
       public :
       int insertEle( int hashNum, HashEle &ele ) ;
       int isEleExist( int hashNum, HashEle &ele ) ;
       int removeEle( int hashNum, HashEle &ele ) ;
       int findEle( int hashNum, HashEle &ele ) ;
       int reposition ( vector< pair< int, HashELe > > &repos ,int cur_size ) ;
        void snapshot() ;
       inline bool getSize()
        {
            return _bucket.size() ;
        } ;
    }
    private :
    vector<HashBucket> _bucketMgr ;
    int _cur_size ;
    private :
    int _processData( int id, char * data, int &hashNum, HashEle & ele ) ;
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
        HashBucket hb = NULL ;
        for (int i = 0; i < cur_size;  ++i) {
            hb = _bucketMgr[i] ;
            if ( hb )
                delete hb ;
        }
    }
}
#endif

    
    
