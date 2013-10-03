#include "hashBucket.hpp"

int main()
{
    char data[1] ;
    int id = 0 ;
    
    HashBucketMgr hbMgr;
    hbMgr.initialize() ;
    
    while (id < 5000) {
        data[0] = (char)('a'+(id%26)) ;
        hbMgr.insertEle( data, id);
    }
    
    return 0 ;
}