#include "grp_mksofs.h"

#include "rawdisk.h"
#include "core.h"
#include "bin_mksofs.h"

#include <string.h>
#include <inttypes.h>

namespace sofs19
{
    void grpFillSuperBlock(const char *name, uint32_t ntotal, uint32_t itotal, uint32_t nbref)
    {
        soProbe(602, "%s(%s, %u, %u, %u)\n", __FUNCTION__, name, ntotal, itotal, nbref);

        /* change the following line by your code */
       // binFillSuperBlock(name, ntotal, itotal, nbref);

       SOSuperBlock sb;
       sb.magic = 0xFFFF;
       strncpy(sb.name,name,PARTITION_NAME_SIZE + 1);
       sb.version = VERSION_NUMBER;
       sb.mntstat = 1;
       sb.mntcnt = 0;
       sb.ntotal = ntotal;

       sb.itotal = itotal;
       sb.it_size = itotal/IPB;
       sb.ifree = itotal-1;
       sb.ihead = 1;
       sb.itail = itotal-1;

       sb.dz_start = (itotal/IPB)+1;
       sb.dz_total = ntotal-sb.dz_start;
       sb.dz_free = sb.dz_total-nbref-1;
       if(sb.dz_free > HEAD_CACHE_SIZE){
            sb.head_blk = 1;
            sb.head_idx = 1;
            sb.tail_blk = nbref;
            uint32_t ocupado = sb.dz_free-(HEAD_CACHE_SIZE+(nbref-1)*RPB)+(nbref-1);
            sb.tail_idx = ocupado+1;
       }else{
            sb.head_blk = NullReference;
            sb.head_idx = NullReference;
            sb.tail_blk = NullReference;
            sb.tail_idx = NullReference;
       }
       
       
       if(sb.dz_free >= HEAD_CACHE_SIZE){
            sb.head_cache.idx = 0;
       }else{
           sb.head_cache.idx = HEAD_CACHE_SIZE-sb.dz_free;
       }
       uint32_t bloco = nbref+1;
       for (int i = 0; i < HEAD_CACHE_SIZE; i++){
                if(i < sb.head_cache.idx){
                    sb.head_cache.ref[i] = NullReference;
                }else{
                    sb.head_cache.ref[i] = bloco;
                    bloco++;
                }
            }
       

       sb.tail_cache.idx = 0;
       for (int i = 0; i < TAIL_CACHE_SIZE; i++)
       {
           sb.tail_cache.ref[i] = NullReference;
       }
       

        soWriteRawBlock(0,&sb);



    }
};

