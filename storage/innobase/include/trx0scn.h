/**=============================================================================
 *  * #     FileName: trx0scn.h
 *  * #         Desc: scn   
 *  * #       Author: L.Sang
 *  * #        Email: hellosangli@gmail.com
 *  *
 *  * #   created Date: 2022-04-18
 *  * =============================================================================*/

#ifndef trx0scn_h
#define trx0scn_h

#include "trx0types.h"
#include <atomic>

#define SCN_NUMS_OF_BLOCK (2048)
#define SCN_BLOCK_SIZE (16384)

#define SCN_BLOCK_NUMS (1024)

constexpr Global_SCN_t SCN_SYS_SCN_WRITE_MARGIN = 256;

/*
store scn in block,block size is 16K
*/
typedef struct store_scn_block
{
    Global_SCN_t scn[SCN_NUMS_OF_BLOCK];
}store_scn_block;

typedef struct trx_scn_sys_t
{
    /* any transaction read/write,need get this scn in read_view */
    std::atomic<Global_SCN_t> max_scn_id;
    store_scn_block *scn_block;
}trx_scn_sys_t;


extern trx_scn_sys_t *trx_scn_sys;

/* get global scn from trx_scn_sys */
extern Global_SCN_t get_global_scn();

/* trx commit by scn */
extern void trx_commit_in_scn(trx_t *trx,Global_SCN_t scn)

#endif