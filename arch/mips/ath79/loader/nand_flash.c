#include <linux/smp.h>
#include <asm/addrspace.h>
#include <asm/byteorder.h>
#include <asm/page.h>
#include <asm/mipsregs.h>
#include <asm/r4kcache.h>
#include <asm/cacheops.h>
#include <linux/io.h>

#include <linux/bch.h>
#include <linux/mtd/nand.h>

#include <asm/mach-ath79/ath79.h>
#include <asm/mach-ath79/ar71xx_regs.h>
#include <asm/mach-ath79/nand.h>

#include "nandloader_common.h"
#include "sha1.h"

extern int uart_tstc(void);
extern char uart_getc(void);
static int
ath_nand_status(uint32_t* rddata);

static void __iomem *ath79_nand_base;

static inline void ath79_nand_wr(unsigned reg, u32 val)
{
   ath_reg_wr( ath79_nand_base + reg, val);
}

static inline u32 ath79_nand_rr(unsigned reg)
{
   return ath_reg_rd ( ath79_nand_base + reg );
}

#define ath_nand_get_cmd_end_status(void)       \
        (ath79_nand_rr(ATH79_NF_INT_STATUS) & ATH79_NF_CMD_END_INT)

#define ath_nand_clear_int_status()     ath79_nand_wr(ATH79_NF_INT_STATUS, 0)

typedef union {
	uint8_t		byte_id[8];

	/*
	 * The following is applicable for numonyx and hynix. have to
	 * check for other manufacturers.
	 */
	struct {
		uint8_t	sa1	: 1,	// Serial access time (bit 1)
			org	: 1,	// Organisation
			bs	: 2,	// Block size
			sa0	: 1,	// Serial access time (bit 0)
			ss	: 1,	// Spare size per 512 bytes
			ps	: 2,	// Page Size

			wc	: 1,	// Write Cache
			ilp	: 1,	// Interleaved Programming
			nsp	: 2,	// No. of simult prog pages
			ct	: 2,	// Cell type
			dp	: 2,	// Die/Package

			did,		// Device id
			vid,		// Vendor id

			res1	: 2,	// Reserved
			pls	: 2,	// Plane size
			pn	: 2,	// Plane number
			res2	: 2;	// Reserved
	} id;
} ath_nand_id_t;

typedef struct {
	uint8_t		vid,
			did,
			b3,
			addrcyc,
			pgsz,
			blk,
			spare;
} ath_nand_vend_data_t;

ath_nand_vend_data_t ath_nand_arr[] = {
	{ 0x20, 0xda, 0x10,	5,	3,	1,	1 },	// NU2g3B2D
	{ 0x20, 0xf1, 0x00,	4,	3,	1,	1 },	// NU1g3B2C
	{ 0x20, 0xdc, 0x10,	5,	3,	1,	1 },	// NU4g3B2D
	{ 0x20, 0xd3, 0x10,	5,	4,	1,	1 },	// NU8g3F2A
	{ 0x20, 0xd3, 0x14,	5,	3,	2,	1 },	// NU8g3C2B
	{ 0xad, 0xf1, 0x00,	4,	3,	1,	1 },	// HY1g2b
	{ 0xad, 0xda, 0x10,	5,	3,	1,	1 }	// HY2g2b
};

static unsigned long crc32_table[] = {
      0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L,
      0x706af48fL, 0xe963a535L, 0x9e6495a3L, 0x0edb8832L, 0x79dcb8a4L,
      0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L,
      0x90bf1d91L, 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
      0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L, 0x136c9856L,
      0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L,
      0xfa0f3d63L, 0x8d080df5L, 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L,
      0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
      0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L,
      0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L, 0x26d930acL, 0x51de003aL,
      0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L,
      0xb8bda50fL, 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
      0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL, 0x76dc4190L,
      0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL,
      0x9fbfe4a5L, 0xe8b8d433L, 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL,
      0xe10e9818L, 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
      0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL,
      0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L, 0x65b0d9c6L, 0x12b7e950L,
      0x8bbeb8eaL, 0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L,
      0xfbd44c65L, 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
      0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL, 0x4369e96aL,
      0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L,
      0xaa0a4c5fL, 0xdd0d7cc9L, 0x5005713cL, 0x270241aaL, 0xbe0b1010L,
      0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
      0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L,
      0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL, 0xedb88320L, 0x9abfb3b6L,
      0x03b6e20cL, 0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L,
      0x73dc1683L, 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
      0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L, 0xf00f9344L,
      0x8708a3d2L, 0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL,
      0x196c3671L, 0x6e6b06e7L, 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL,
      0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
      0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L,
      0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL, 0xd80d2bdaL, 0xaf0a1b4cL,
      0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL,
      0x4669be79L, 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
      0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL, 0xc5ba3bbeL,
      0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L,
      0x2cd99e8bL, 0x5bdeae1dL, 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL,
      0x026d930aL, 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
      0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL,
      0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L, 0x86d3d2d4L, 0xf1d4e242L,
      0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L,
      0x18b74777L, 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
      0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L, 0xa00ae278L,
      0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L,
      0x4969474dL, 0x3e6e77dbL, 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L,
      0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
      0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L,
      0xcdd70693L, 0x54de5729L, 0x23d967bfL, 0xb3667a2eL, 0xc4614ab8L,
      0x5d681b02L, 0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL,
      0x2d02ef8dL
};

static inline
u_int32_t crc_accumulate_8(u_int32_t crc, u_int8_t ch)
{
    u_int32_t new_crc;

    new_crc = crc32_table[(crc ^ ch) & 0xff] ^ (crc >> 8);

    return new_crc;
}

static
u_int32_t crc_accumulate_32(u_int32_t crc, u_int32_t val)
{
    crc = crc_accumulate_8(crc, val);
    crc = crc_accumulate_8(crc, val >> 8);
    crc = crc_accumulate_8(crc, val >> 16);
    crc = crc_accumulate_8(crc, val >> 24);
    return crc;
}

#define NUM_ATH_NAND (sizeof(ath_nand_arr) / sizeof(ath_nand_arr[0]))

#define TRY_MAX 1000

static ath_nand_id_t         __ath_nand_id;
static ath_nand_id_t        *nand_id = &__ath_nand_id;
static ath_nand_vend_data_t *nand_vend = NULL;

static struct bch_control *bch = NULL;

extern int printf(const char* fmt, ...);
extern void udelay(unsigned long);

static inline int
nand_status_loop_check(void)
{
    int count = 0;

    while(ath79_nand_rr(ATH79_NF_STATUS) != 0xff && count++ <= TRY_MAX)
        udelay(1);

    return count > TRY_MAX;
}

static int
nand_read_id(void)
{
    unsigned char   *pa = (unsigned char *)KSEG1;
    unsigned        rddata;

    ath_nand_clear_int_status();
    ath79_nand_wr(ATH79_NF_DMA_ADDR,
               ((unsigned int)pa) & ~KSEG1);       // DMA Start Addr XXX assuming we're in Kseg1
    ath79_nand_wr(ATH79_NF_ADDR0_0, 0x0);                    // ADDR0_0 Reg Settings
    ath79_nand_wr(ATH79_NF_ADDR0_1, 0x0);                    // ADDR0_1 Reg Settings
    ath79_nand_wr(ATH79_NF_DMA_COUNT, sizeof(*nand_id));     // DMA count
    ath79_nand_wr(ATH79_NF_PG_SIZE, 0x8);                    // Custom Page Size
    ath79_nand_wr(ATH79_NF_DMA_CTRL, 0xcc);                  // DMA Control Reg
    ath79_nand_wr(ATH79_NF_COMMAND, 0x9061);                  // READ ID
    while (ath_nand_get_cmd_end_status() == 0);

    ath_nand_status(&rddata);
    if ((rddata & ATH79_NF_RD_STATUS_MASK) != ATH79_NF_STATUS_OK) {
       printf("%s: ath nand status = 0x%x\n", __func__, rddata);
    }
    memcpy(nand_id, pa, sizeof(*nand_id));

    return nand_status_loop_check();
}

static int
nand_hw_reset(void)
{
    u32 reg;
    if (soc_is_ar934x())
      reg = AR934X_RESET_REG_RESET_MODULE;
    else
      reg = QCA955X_RESET_REG_RESET_MODULE;

    nand_id = (ath_nand_id_t *)KSEG1ADDR(nand_id);

    // clear ethernet reset
    ath79_reset_wr(reg, ath79_reset_rr(reg) & ~AR71XX_RESET_GE1_PHY);
    udelay(250);

    // Put into reset
    ath79_reset_wr(reg, ath79_reset_rr(reg) | AR934X_RESET_NAND);
    udelay(250);

    ath79_reset_wr(reg, ath79_reset_rr(reg) & ~(AR934X_RESET_NAND));
    udelay(100);

    ath79_nand_wr(ATH79_NF_INT_MASK, ATH79_NF_CMD_END_INT);
    ath_nand_clear_int_status();

    // TIMINGS_ASYN Reg Settings
    ath79_nand_wr(ATH79_NF_TIMINGS_ASYN, ATH79_NF_TIMING_ASYN);

    // NAND Mem Control Reg
    ath79_nand_wr(ATH79_NF_MEM_CTRL, 0xff00);

    // Reset Command
    ath79_nand_wr(ATH79_NF_COMMAND, 0xff00);

    while (ath_nand_get_cmd_end_status() == 0);
    udelay(1000);

    return nand_status_loop_check();
}


#define ECC_BUF_SIZE 7
#define PAGE_BUF_SIZE 2048
#define SUBPAGE_BUF_SIZE 512
#define OOBSIZE 64
#define PAGE_STEPS (PAGE_BUF_SIZE/SUBPAGE_BUF_SIZE)
#define ECC_BYTES_PER_PAGE (ECC_BUF_SIZE * PAGE_STEPS)
#define ECC_OOB_OFFSET (OOBSIZE-ECC_BYTES_PER_PAGE)

uint8_t *eccmask = NULL;

int
nand_flash_init(void)
{
    int i;

    ath79_nand_base = (void __iomem *)
      KSEG1ADDR((soc_is_ar934x() ?
            AR934X_NAND_BASE :
            QCA955X_NAND_BASE));

    if (nand_hw_reset() || nand_read_id()) {
        printf("device reset failed\n");
        return -1;
    }

    // TIMINGS_ASYN Reg Settings
    ath79_nand_wr(ATH79_NF_TIMINGS_ASYN, ATH79_NF_TIMING_ASYN);

    for (i = 0; i < NUM_ATH_NAND; i++) {
        if ((nand_id->id.vid == ath_nand_arr[i].vid) &&
            (nand_id->id.did == ath_nand_arr[i].did) &&
            (nand_id->byte_id[1] == ath_nand_arr[i].b3)) {
            ath79_nand_wr(ATH79_NF_CTRL,
                       (ath_nand_arr[i].addrcyc) |
                       (ath_nand_arr[i].blk << 6) |
                       (ath_nand_arr[i].pgsz << 8) |
                       (ATH79_NF_CTRL_CUSTOM_SIZE_EN << 11));
            nand_vend = &ath_nand_arr[i];
            break;
        }
    }

    if (i == NUM_ATH_NAND) {
        printf("Could not identify NAND chip\n");
        printf("---->0x%02x\n\t0x%02x\n\t0x%02x\n",
               nand_id->id.vid,
               nand_id->id.did,
               nand_id->byte_id[1]);

        return -1;
    }

    if (!bch) {
        unsigned int m, t;
        uint8_t *erased_page;
        m = fls(1 + 8*SUBPAGE_BUF_SIZE);
        t = ECC_BUF_SIZE*8/m;
        bch = init_bch(m, t, 0);
        eccmask = (uint8_t*)my_kmalloc(ECC_BYTES_PER_PAGE);
        erased_page = (uint8_t*)my_kmalloc(SUBPAGE_BUF_SIZE);

        if (bch && eccmask && erased_page) {
            for (i = 0; i < SUBPAGE_BUF_SIZE; i++)
                erased_page[i] = 0xff;
            for (i = 0; i < ECC_BYTES_PER_PAGE; i++)
                eccmask[i] = 0;

            encode_bch(bch, erased_page, SUBPAGE_BUF_SIZE, eccmask);
            kfree(erased_page);
            erased_page = NULL;
        } else {
            printf("failed to init bch\n");
            if (bch)
                free_bch(bch);
            if (eccmask)
                kfree(eccmask);
            if (erased_page)
                kfree(erased_page);
            bch = NULL;
            eccmask = NULL;
            erased_page = NULL;
        }
    }

    return 0;
}

static int
ath_nand_status(uint32_t* rddata)
{
    int count = 0;

    if (nand_status_loop_check())
        return -1;

    count = 0;
    while (!(ath79_nand_rr(ATH79_NF_DMA_CTRL) & 1) && count++ <= TRY_MAX)
        udelay(1);

    if (count > TRY_MAX)
        return -1;

    ath79_nand_wr(ATH79_NF_COMMAND, 0x07024); // READ STATUS

    if (nand_status_loop_check())
        return -1;

    *rddata = ath79_nand_rr(ATH79_NF_RD_STATUS);

    return 0;
}

static int
bch_correct_page(struct bch_control *bch, uint8_t *buf, uint8_t *eccbuf)
{
    int i, j, result, stat;
    unsigned int errloc[8];

    for (i = 0, result = 0; i < PAGE_STEPS; i++, result += stat) {
        uint8_t *data = buf + i * SUBPAGE_BUF_SIZE;

        for (j = 0; j < ECC_BUF_SIZE; j++)
            eccbuf[i * ECC_BUF_SIZE + j] ^= ~eccmask[j];

        stat = decode_bch(bch, data, SUBPAGE_BUF_SIZE, &eccbuf[i * ECC_BUF_SIZE], NULL, NULL, errloc);
        if (stat < 0) {
            printf("ecc failed to calculate\n");
            return stat;
        } else if (stat > 0) {
            for (j = 0; j < stat; j++) {
                if (errloc[j] < 8*SUBPAGE_BUF_SIZE) {
                    data[errloc[j] >> 3] ^= (1 << (errloc[j] & 7));
                }
            }
        }
    }
    return result;
}

struct bch_crc_params {
    struct bch_control *bch;
    uint32_t *page_data;
    int length;
    int ecc_failed;
    uint8_t *eccbuf;
    uint32_t crc;
    uint32_t crc_rollback;
};

int
nand_read(unsigned int row, unsigned int count, uint8_t *buf, void(*cb)(struct bch_crc_params *), struct bch_crc_params* param)
{
    unsigned int rstatus, retries = 0;

    while (retries++ <= TRY_MAX) {
        // ADDR0_0 Reg Settings
        ath79_nand_wr(ATH79_NF_ADDR0_0, row << 16);

        // ADDR0_1 Reg Settings
        ath79_nand_wr(ATH79_NF_ADDR0_1, row >> 16);

        // DMA Start Addr
        ath79_nand_wr(ATH79_NF_DMA_ADDR, ((unsigned int)buf));

        // DMA count
        ath79_nand_wr(ATH79_NF_DMA_COUNT, count);

        // Custom Page Size
        ath79_nand_wr(ATH79_NF_PG_SIZE, count);

        // DMA Control Reg
        ath79_nand_wr(ATH79_NF_DMA_CTRL, 0xcc);

        // READ PAGE
        ath79_nand_wr(ATH79_NF_COMMAND, 0x30006a);

        if (cb)
            cb(param);

        if (ath_nand_status(&rstatus)) {
            printf("E");
            nand_flash_init();
        } else if ((rstatus & ATH79_NF_RD_STATUS_MASK) == ATH79_NF_STATUS_OK) {
            break;
        }
    }

    if (retries > TRY_MAX)
        return -1;

    return 0;
}

void*
memcpy(void *dest, const void *src, size_t count)
{
    char *tmp = (char *) dest, *s = (char *) src;

    while (count--)
        *tmp++ = *s++;
    return dest;
}

/**
 * memcmp - Compare two areas of memory
 * @cs: One area of memory
 * @ct: Another area of memory
 * @count: The size of the area.
 */
int memcmp(const void * cs,const void * ct,size_t count)
{
   const unsigned char *su1, *su2;
   int res = 0;

   for( su1 = cs, su2 = ct; 0 < count; ++su1, ++su2, count--)
      if ((res = *su1 - *su2) != 0)
         break;
   return res;
}

#define LOADER_MAGIC         0x4d495053

#define BOOTKERNEL1_PAGE_ADDR 64   //0    + 1 block
#define BOOTKERNEL2_PAGE_ADDR 4160 //8MB  + 1 block

struct image_header {
    u_int32_t magic;        /* Every sufficiently advanced technology... */
    u_int32_t load_addr;    /* Address to load binary */
    u_int32_t size;         /* Size of binary */
    u_int32_t entry;        /* Entry point */
    u_int32_t crc;          /* CRC32 of image */
    u_int32_t reserved[3];  /* Reserved */
};

static uint8_t eccbuf[ECC_BYTES_PER_PAGE];

static void
bch_crc_callback(struct bch_crc_params *params)
{
    int result = 0;
    if (params->eccbuf && params->bch) {
        result = bch_correct_page(params->bch, (uint8_t*)params->page_data, params->eccbuf);
        if (result > 0)
            printf("e%d", result);
        params->ecc_failed = (result < 0);
    }

    if (params->length > 0) {
        uint32_t *begin, *end, crc = params->crc;

        begin = (uint32_t*)params->page_data;
        end = (uint32_t*)((uint8_t*)(params->page_data) + params->length);

        while (begin < end)
            crc = crc_accumulate_32(crc, *(begin++));

        params->crc = crc;
    }
 }

enum bk_load_result {
    BK_OK,
    BK_FAIL,
    BK_SKIP_FIRST_BLOCK,
};

static enum bk_load_result
nand_load_bk(int start_row, void(**kernel_entry)(int, char**, int))
{
#define PAGE_MARKED_BAD(page_oob_addr) (((uint8_t*)(page_oob_addr))[NAND_LARGE_BADBLOCK_POS] != 0xff)

    struct bch_crc_params params = {0};
    int pages_per_block, page_size, oob_size, remaining;
    int addr = start_row;
    u_int32_t load_addr, crc = ~0UL;
    struct image_header hdr, *tmp = (struct image_header *)KSEG1;

    page_size = (1 << 10) << nand_id->id.ps;
    pages_per_block = ((1 << 16) << nand_id->id.bs) / page_size;
    oob_size = (8 << nand_id->id.ss) * (page_size / 512);

    hdr.magic = 0xdeadbeef;
    if (nand_read(addr, page_size + oob_size, (uint8_t*)tmp, NULL, NULL))
        return BK_FAIL;

    if (PAGE_MARKED_BAD((uint8_t*)tmp + page_size))
        return BK_SKIP_FIRST_BLOCK;

    if (bch)
        bch_correct_page(bch, (uint8_t*)tmp, ((uint8_t*)tmp) + page_size + ECC_OOB_OFFSET);

    hdr = *tmp;

    printf("hdr: [0x%x : 0x%x : 0x%x : 0x%x, : 0x%x]\n",
           hdr.magic, hdr.load_addr, hdr.size, hdr.entry, hdr.crc);

    if (hdr.magic != LOADER_MAGIC) {
        printf("magic is not correct\n");
        return BK_FAIL;
    }

    crc = crc_accumulate_32(crc, hdr.magic);
    crc = crc_accumulate_32(crc, hdr.load_addr);
    crc = crc_accumulate_32(crc, hdr.size);
    crc = crc_accumulate_32(crc, hdr.entry);
    crc = crc_accumulate_32(crc, 0); //0 for crc
    crc = crc_accumulate_32(crc, hdr.reserved[0]);
    crc = crc_accumulate_32(crc, hdr.reserved[1]);
    crc = crc_accumulate_32(crc, hdr.reserved[2]);

    memcpy((u_int8_t*)KSEG1ADDR(hdr.load_addr), tmp+1, page_size - sizeof(hdr));

    params.bch = bch;
    params.eccbuf = NULL; //don't re-do the ecc of the first page
    params.length = page_size - sizeof(hdr);
    params.crc = crc;
    params.page_data = (uint32_t*)KSEG1ADDR(hdr.load_addr);

    for ((load_addr = hdr.load_addr + (page_size - sizeof(hdr)),
          remaining = hdr.size - page_size + sizeof(hdr),
          addr++)
             ;
         (remaining > 0) || ((addr % pages_per_block) != 0)
             ;
         (load_addr += page_size,
          remaining -= page_size,
          addr++)) {

        //performs ecc/crc on last page while dma is working
        void(*cb)(struct bch_crc_params *) = bch_crc_callback;

 skip_bad_block:
        if (nand_read(addr, page_size + oob_size, (uint8_t*)KSEG1ADDR(load_addr), cb, &params))
            return BK_FAIL;

        params.length = remaining < page_size ? remaining : page_size;
        params.page_data = (uint32_t*)KSEG1ADDR(load_addr);
        if (addr % pages_per_block == 0) {
            printf(".");
            params.crc_rollback = params.crc;
        }

        if (PAGE_MARKED_BAD(KSEG1ADDR(load_addr + page_size))) {
            uint32_t backup_by;
            if (addr - start_row < pages_per_block)
                return BK_SKIP_FIRST_BLOCK;

            //rollback everything to the end of the last good block
            backup_by = addr % pages_per_block;
            load_addr -= page_size * backup_by;
            params.crc = params.crc_rollback;

            addr = (addr - backup_by) + pages_per_block;
            printf("B");
            cb = NULL;
            goto skip_bad_block;
        }

        //copy the ecc buf out, as the oob region will get clobbered by the next nand_read
        memcpy(eccbuf
               , (uint8_t*)KSEG1ADDR(load_addr + page_size + ECC_OOB_OFFSET)
               , sizeof(eccbuf));
        params.eccbuf = eccbuf;
    }

    printf("\n");
    if (params.length > 0)
        bch_crc_callback(&params);

    if (params.ecc_failed)
        return BK_FAIL;

    (*kernel_entry) = (void(*)(int, char**, int))hdr.entry;

    if (~params.crc != hdr.crc) {
        printf("crc mismatch! expect: %x, got %x\n", hdr.crc, ~params.crc);
        return BK_FAIL;
    }

    return BK_OK;
}

/* 128KiB = 64 blocks
 *
 *  MR18 NAND Layout
 *
 *  512 KiB      Bootloader
 *  8192 KiB     Part 1
 *  8192 KiB     Part 2
 *  114176 KiB   UBI
 *
 *  page size = 2048 (1 << 11)
 *  linear address to nand row conversion
 *   = (linear_address >> page_size_bits) << 16
 *
 *  part1_page_addr = (bootloader_size_in_bytes) >> 11)
 *  part2_page_addr = (bootloader_size_in_bytes + part1_size_in_bytes) >> 11)
 *
 */
#define PART1_PAGE_ADDR    256
#define PART2_PAGE_ADDR    4352

#define PART_LOAD_ADDR   0x80060000

struct part_header {
#define FOO_MAGIC         0x8e73ed8a
    u_int32_t magic;        /* Every sufficiently advanced technology... */
    u_int32_t header_length;   /* Length of header */
    u_int32_t data_length;  /* Length of kernel image */
    u_int8_t  data_sha1[20];/* SHA1 of kernel image */
}__attribute__((__packed__));

static unsigned char *
sha1(unsigned char sha1_res[20], unsigned char *str, unsigned int l)
{
    SHA1_CTX ctx;

    SHA1Init(&ctx);
    SHA1Update(&ctx, str, l);
    SHA1Final(sha1_res, &ctx);

    return sha1_res;
}

static void
print_sha1(unsigned char *s)
{
   int i = 0;
   for (i = 0; i < 20; i++) {
      printf("%02x", s[i]);
   }
}

static enum bk_load_result
nand_load_part(int start_row, void(**kernel_entry)(int, char**, int))
{
#define PAGE_MARKED_BAD(page_oob_addr) (((uint8_t*)(page_oob_addr))[NAND_LARGE_BADBLOCK_POS] != 0xff)

    struct bch_crc_params params = {0};
    int pages_per_block, page_size, oob_size, remaining;
    int addr = start_row;
    u_int32_t load_addr, crc = ~0UL;
    struct part_header hdr, *tmp = (struct part_header *)KSEG1;
    unsigned long len = 0;
    unsigned long hlen = 0;
    unsigned long magic = 0;
    int r = 0;
    unsigned char sha1_res[20];
    int valid=0;
    int partno;

    page_size = (1 << 10) << nand_id->id.ps;
    pages_per_block = ((1 << 16) << nand_id->id.bs) / page_size;
    oob_size = (8 << nand_id->id.ss) * (page_size / 512);

    partno = ((addr >= PART1_PAGE_ADDR)&&(addr<PART2_PAGE_ADDR)) ? 1 : 2;
    load_addr = PART_LOAD_ADDR;

    if (nand_read(addr, page_size + oob_size, (uint8_t*)tmp, NULL, NULL))
        return BK_FAIL;

    if (PAGE_MARKED_BAD((uint8_t*)tmp + page_size))
        return BK_SKIP_FIRST_BLOCK;

    if (bch)
        bch_correct_page(bch, (uint8_t*)tmp, ((uint8_t*)tmp) + page_size + ECC_OOB_OFFSET);

    hdr = *tmp;

    magic = ntohl(hdr.magic);
    hlen = ntohl(hdr.header_length);
    len = ntohl(hdr.data_length);

    printf("hdr: [0x%x : 0x%x : 0x%x ]\n", magic, hlen, len);
    valid = 1;

#define MAX_LEN (16*1024*1024)
    if (FOO_MAGIC != magic) {
       printf("part%d: invalid magic, expected 0x%08x versus 0x%08x\n", partno, FOO_MAGIC, magic);
       valid = 0;
    }

    if (valid && (hlen < sizeof(struct part_header) || hlen > MAX_LEN || len > MAX_LEN )) {
         printf("part%d: invalid location - overflow on length (hlen:0x%x len:0x%x) specified in checksum header\n",
                partno, hlen, len);
         valid = 0;
    }

    if (!valid)
        return BK_FAIL;

    printf("part%d: Copying image to memory ... ", partno);

    memcpy((u_int8_t*)KSEG1ADDR(load_addr), (uint8_t *)tmp + hlen, page_size - hlen);

    params.bch = bch;
    params.eccbuf = NULL; //don't re-do the ecc of the first page
    params.length = page_size - hlen;
    params.crc = crc;
    params.page_data = (uint32_t*)KSEG1ADDR(load_addr);

    for ((load_addr = load_addr + (page_size - hlen),
          remaining = len - page_size + hlen,
          addr++)
             ;
         (remaining > 0) || ((addr % pages_per_block) != 0)
             ;
         (load_addr += page_size,
          remaining -= page_size,
          addr++)) {

        //performs ecc/crc on last page while dma is working
        void(*cb)(struct bch_crc_params *) = bch_crc_callback;

 skip_bad_block:
        if (nand_read(addr, page_size + oob_size, (uint8_t*)KSEG1ADDR(load_addr), cb, &params))
            return BK_FAIL;

        params.length = remaining < page_size ? remaining : page_size;
        params.page_data = (uint32_t*)KSEG1ADDR(load_addr);
        if (addr % pages_per_block == 0) {
            printf(".");
            params.crc_rollback = params.crc;
        }

        if (PAGE_MARKED_BAD(KSEG1ADDR(load_addr + page_size))) {
            uint32_t backup_by;
            if (addr - start_row < pages_per_block)
                return BK_SKIP_FIRST_BLOCK;

            //rollback everything to the end of the last good block
            backup_by = addr % pages_per_block;
            load_addr -= page_size * backup_by;
            params.crc = params.crc_rollback;

            addr = (addr - backup_by) + pages_per_block;
            printf("B");
            cb = NULL;
            goto skip_bad_block;
        }

        //copy the ecc buf out, as the oob region will get clobbered by the next nand_read
        memcpy(eccbuf
               , (uint8_t*)KSEG1ADDR(load_addr + page_size + ECC_OOB_OFFSET)
               , sizeof(eccbuf));
        params.eccbuf = eccbuf;
    }

    printf("\n");
    if (params.length > 0)
        bch_crc_callback(&params);

    if (params.ecc_failed)
        return BK_FAIL;

    printf("done.\n");

    load_addr = PART_LOAD_ADDR;
    printf("part%d: Checking sha1 (from 0x%08x length %u) ... ", partno, load_addr, (unsigned int)len);
    sha1(sha1_res, (unsigned char *)load_addr, (unsigned int)len);
    r = memcmp(hdr.data_sha1, sha1_res, 20);
    printf("%s", r == 0 ? "match\n" : "mismatch\n");

    if (r != 0) {
       printf("part%d: sha1 calculated: ", partno);
       print_sha1(sha1_res);
       printf(" stored: ");
       print_sha1(hdr.data_sha1);
       printf("\n");
       return BK_FAIL;
    } else {
       printf("part%d: sha1 calculated: ", partno);
       print_sha1(sha1_res);
       printf("\n");
    }

    (*kernel_entry) = (void(*)(int, char**, int))PART_LOAD_ADDR;

    return BK_OK;
}

static enum bk_load_result
nand_load_bk_skipping(int addr, void(**kernel_entry)(int, char **, int),
   enum bk_load_result(*nand_loader)(int, void(**kernel_entry)(int, char**, int)))
{
    enum bk_load_result lr;

    do {
        printf("loading fw at %d\n", addr);
        lr = nand_loader(addr, kernel_entry);
        {
            int page_size = (1 << 10) << nand_id->id.ps;
            int pages_per_block = ((1 << 16) << nand_id->id.bs) / page_size;
            addr += pages_per_block;
        }
    } while (lr == BK_SKIP_FIRST_BLOCK);

    return lr;
}

int
nand_load_fw(void)
{
    void(*kernel_entry)(int, char **, int);
    enum bk_load_result lr1;
    enum bk_load_result(*nand_loader)(int, void(**kernel_entry)(int, char**, int));
    int addr1,addr2;

    if (soc_is_qca955x()) {
       addr1 = PART1_PAGE_ADDR;
       addr2 = PART2_PAGE_ADDR;
       nand_loader = nand_load_part;
    } else {
       addr1 = BOOTKERNEL1_PAGE_ADDR;
       addr2 = BOOTKERNEL2_PAGE_ADDR;
       nand_loader = nand_load_bk;
    }

    /* If the user is pressing '2' on the console
       then boot the kernels in reverse */
    if(uart_tstc()) {
       int tmpaddr;
       if( '2' == uart_getc()) {
           tmpaddr = addr1;
           addr1   = addr2;
           addr2   = tmpaddr;
       }
    }

    lr1 = nand_load_bk_skipping(addr1, &kernel_entry, nand_loader);
    if (lr1 != BK_OK) {
        enum bk_load_result lr2;
        printf("bootkernel 1 failed!\n");
        lr2 = nand_load_bk_skipping(addr2, &kernel_entry, nand_loader);
        if (lr2 != BK_OK) {
            printf("bootkernel 2 failed!\n");
            return -1;
        }
    }

    (*kernel_entry)(0, 0, 0);

    //should never get here
    return 0;
}
