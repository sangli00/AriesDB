/**=============================================================================
 *  * #     FileName: trx0scn.cc
 *  * #         Desc: transaction use scn read rec  
 *  * #       Author: L.Sang
 *  * #        Email: hellosangli@gmail.com
 *  *
 *  * #   created Date: 2022-04-18
 *  * =============================================================================*/

#include "trx0scn.h"

trx_scn_sys_t *trx_scn_sys = NULL;

Global_SCN_t get_global_scn();

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

    trx_scn_sys = malloc(sizeof(trx_scn_sys_t));

    /* alloc share memory */
	trx_scn_sys->scn_block = (store_scn_block *)mmap(NULL, size, 
                                PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    memset(trx_scn_sys->scn_block,0x0,size);


    return;
}

/** write scn to mtr */
static void
write_max_scn_id(Global_SCN_t scn)
{
    mtr_t mtr;
	byte *log_ptr;

	mtr_start(&mtr);

    if (mlog_open(&mtr, 11 + 8, log_ptr)) {
        log_ptr = mlog_write_initial_log_record_low(MLOG_SCN, 0,0, log_ptr, &mtr);

        mach_write_to_8(log_ptr, scn);
        mlog_close(&mtr, log_ptr + 8);
    }

	mtr.commit();

	return;
}

/* get scn from trx_scn_sys */
Global_SCN_t get_global_scn()
{
    ut_ad(trx_scn_sys != NULL);

    Global_SCN_t scn = trx_scn_sys->max_scn_id.fetch_add(1);

    if(scn % SCN_SYS_SCN_WRITE_MARGIN == 0)
    {
        write_max_scn_id(scn);
    }

    return scn;
}

/** trx commit,write scn to scn buffer */
void 
trx_commit_in_scn(trx_t *trx,Global_SCN_t scn)
{
    return;
} 

