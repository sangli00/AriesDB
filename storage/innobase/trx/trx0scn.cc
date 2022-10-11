/**=============================================================================
 *  * #     FileName: trx0scn.cc
 *  * #         Desc: transaction use scn read rec  
 *  * #       Author: L.Sang
 *  * #        Email: hellosangli@gmail.com
 *  *
 *  * #   created Date: 2022-04-18
 *  * =============================================================================*/

#include "trx0scn.h"
#include "mtr0mtr.h"
#include "mtr0types.h"
#include "ut0new.h"
#include "trx0types.h"
#include "trx0sys.h"
#include "trx0purge.h"
#include "univ.i"
#include "srv0start.h"
#include <algorithm>
#include "mtr0types.h"
#include "trx0i_s.h"

trx_scn_sys_t *trx_scn_sys = NULL;

Global_SCN_t get_global_scn();

/**
 * @brief use trx_id get scn_buffer block and return scn entry in block.
 * 
 * @param trx_id 
 * @param block 
 * @return scn block entry  
 */
static void 
get_scn_block(scn_offset_info *info)
{
    ut_ad(trx_scn_sys != NULL);
    
    uint32_t block_offset = 0;  /*block offset */
    uint32_t entry = 0; /* scn entry in blcok */
    store_scn_block *block = NULL;

    /* get block offset */
    block_offset = ((info->trx_id / SCN_NUMS_OF_BLOCK ) % SCN_BLOCK_NUMS) ;

    block = &trx_scn_sys->scn_buf[block_offset];

    entry = (info->trx_id % SCN_NUMS_OF_BLOCK);

    info->block = block;
    info->entry = entry;

    return;
}

/* alloc memroy size */
static size_t
trx_scn_total_mem_size()
{
    size_t size = SCN_BLOCK_NUMS * sizeof(store_scn_block);

    return size;
}

/*
trx scn init 
*/
void
trx_scn_init()
{
    /* get alloc memory size */
    size_t size = trx_scn_total_mem_size();

    trx_scn_sys = (trx_scn_sys_t*)malloc(sizeof(trx_scn_sys_t));

    /* alloc share memory */
	trx_scn_sys->scn_buf = (store_scn_block *)mmap(NULL, size, 
                                PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    memset(trx_scn_sys->scn_buf,0x0,size);


    return;
}

/**
 * @brief alloc scn to any transaction,if max_scn_id % SCN_SYS_SCN_WRITE_MARGIN,need write this scn
 * to mtr,if database carash,recovery apply this mtr to trx_scn_sys->max_scn_id.
 * 
 * @param scn 
 */
static void
write_max_scn_id_mtr(Global_SCN_t scn)
{
    mtr_t mtr;
	byte *log_ptr;

	mtr_start(&mtr);

    /*11 is mtr head,1 is scn type,8 is scn length */
    if (mlog_open(&mtr, 11 + 1 + 8, log_ptr)) {
        log_ptr = mlog_write_initial_log_record_low(MLOG_SCN, 0,0, log_ptr, &mtr);

        mach_write_to_1(log_ptr,GLOBAL_MAX_SCN);
        log_ptr += 1;

        mach_write_to_8(log_ptr, scn);
        mlog_close(&mtr, log_ptr + 8);
    }

	mtr.commit();

	return;
}

/**
 * @brief if transaction is RW,in commit phase will need get scn in trx,but transactin commit before,
 * need write mtr info.if database crash,recovery this scn and trx_id to scn_buffer.
 * 
 * 
 * @param scn 
 * @param trx_id 
 */
void 
write_scn_trx_id_mtr(Global_SCN_t scn,trx_id_t trx_id)
{
    mtr_t mtr;
	byte *log_ptr;

	mtr_start(&mtr);

    /*11 is mtr head,1 is scn type,8 is scn length,8 is trx_id */
    if (mlog_open(&mtr, 11 + 1 + 8 + 8, log_ptr)) {
        log_ptr = mlog_write_initial_log_record_low(MLOG_SCN, 0,0, log_ptr, &mtr);

        mach_write_to_1(log_ptr,USER_SCN_TRX_ID);
        log_ptr += 1;

        mach_write_to_8(log_ptr, scn);
        log_ptr += 8;

        mach_write_to_8(log_ptr, trx_id);
        mlog_close(&mtr, log_ptr + 8);
    }

	mtr.commit();

	return;
}

/**
 * @brief Get the global scn object
 * 
 * @return Global_SCN_t 
 */
Global_SCN_t get_global_scn()
{
    ut_ad(trx_scn_sys != NULL);

    /**
     * @brief atomic get scn
     */
    Global_SCN_t scn = trx_scn_sys->max_scn_id.fetch_add(1);

    if(scn % SCN_SYS_SCN_WRITE_MARGIN == 0)
    {
        write_max_scn_id_mtr(scn);
    }

    return scn;
}

/**
 * @brief trx commit,write scn to scn_buf
 * 
 * @param trx 
 * @param id
 * @param scn 
 */
void 
trx_commit_in_scn(trx_t *trx,trx_id_t id,Global_SCN_t scn)
{
    scn_offset_info info;

    ut_a(id > 0);

    info.trx_id = id;

    get_scn_block(&info);
    
    ut_d(info.block->scn[info.entry] == 0);

    info.block->scn[info.entry] = scn;

    return;
} 

/**
 * @brief Get the scn by trx id object
 * 
 * @param trx_id 
 * @return Global_SCN_t 
 */
Global_SCN_t
get_scn_by_trx_id(trx_id_t trx_id)
{
    Global_SCN_t scn = 0;
    scn_offset_info info;
    
    info.trx_id = trx_id;

    get_scn_block(&info);

    scn = info.block->scn[info.entry];

    return scn;
}


/**
 * @brief trx make scn main thread
 * 
 */
void
trx_scn_thread()
{

}
