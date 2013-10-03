#include "hashBucket.hpp"
#include "stdlib.h"
#include <vector>

int main()
{
    char  * data = NULL;
    vector<char *> v;
    int id = 0 ;
    
    HashBucketMgr hbMgr;
    hbMgr.initialize() ;
    
    while (id < 2000) {
        data = (char *)malloc(sizeof(char) * 2) ;
        data[0] = (char)('a'+(id%26)) ;
        data[1] = '\0' ;
        hbMgr.insertEle( data, id);
        id++ ;
        v.push_back(data);
    } ;
    for ( int i = 0; i < v.size(); ++i) {
        if ( v[i] )
            free (v[i]);
    } ;
    return 0 ;
}