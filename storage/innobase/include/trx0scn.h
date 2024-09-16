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

#define SCN_FILE_PATH "scn_file"

constexpr Global_SCN_t SCN_SYS_SCN_WRITE_MARGIN = 1024;

/**
 * @brief write scn buffer to file,default file size is 1GB
 */
constexpr uint32_t SCN_FILE_DISK_SIZE = 1024 * 1024 * 1024; /* 1G */


constexpr Global_SCN_t SCN_INIT_VALUE = 10000;

/**
 * @brief write scn to mtr,mtr type is MLOG_SCN,but sometimes,need write diff length scn to mtr
 * 
 */
enum scn_mtr_type{
    /** alloc SCN_SYS_SCN_WRITE_MARGIN to transaction,need write this scn to mtr */
    GLOBAL_MAX_SCN = 1,

    /* trx commit before,need write trx_id and scn to mtr */
    USER_SCN_TRX_ID
};

/*
store scn in block,block size is 16K
*/
typedef struct store_scn_block
{
    Global_SCN_t scn[SCN_NUMS_OF_BLOCK];
}store_scn_block;

typedef struct scn_offset_info
{
    store_scn_block *block;
    uint32_t entry;
    trx_id_t trx_id;
}scn_offset_info;

typedef struct trx_scn_sys_t
{
    /* any transaction read/write,need get this scn in read_view */
    std::atomic<Global_SCN_t> max_scn_id;
    store_scn_block *scn_buf;
}trx_scn_sys_t;


extern trx_scn_sys_t *trx_scn_sys;

/* get global scn from trx_scn_sys */
extern Global_SCN_t get_global_scn();

/* trx commit by scn */
extern void trx_commit_in_scn(trx_t *trx,trx_id_t id,Global_SCN_t scn);

/**
 * @brief Get the scn by trx id object
 * 
 * @param trx_id 
 * @return Global_SCN_t 
 */
extern Global_SCN_t get_scn_by_trx_id(trx_id_t trx_id);


extern void trx_scn_create();

extern void trx_scn_init(bool create_new_db);


#endif