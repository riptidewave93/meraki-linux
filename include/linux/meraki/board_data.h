#ifndef BOARD_DATA_H
#define BOARD_DATA_H

/* WARNING: the definition of struct ar531x_boarddata (and related
 * functions and constants) must be kept in sync with the associated
 * constants, functions, and definitions in the meraki source
 * directory, located at
 * <router-checkout>/meraki/libmeraki/board_data.hh
 */
struct ar531x_boarddata {
    uint32_t magic;                       /* board data is valid */
#define AR531X_BD_MAGIC 0x35333131        /* "5311", for all 531x platforms */
    uint16_t cksum;                       /* checksum (starting with BD_REV 2) */
    uint16_t rev;                         /* revision of this struct */
#define BD_REV  13
    char     boardName[64];               /* Name of board */
    uint16_t major;                       /* Board major number */
    uint16_t minor;                       /* Board minor number */
    uint32_t config;                      /* Board configuration */
#define BD_ENET0        0x00000001   /* ENET0 is stuffed */
#define BD_ENET1        0x00000002   /* ENET1 is stuffed */
#define BD_UART1        0x00000004   /* UART1 is stuffed */
#define BD_UART0        0x00000008   /* UART0 is stuffed (dma) */
#define BD_RSTFACTORY   0x00000010   /* Reset factory defaults stuffed */
#define BD_SYSLED       0x00000020   /* System LED stuffed */
#define BD_EXTUARTCLK   0x00000040   /* External UART clock */
#define BD_CPUFREQ      0x00000080   /* cpu freq is valid in nvram */
#define BD_SYSFREQ      0x00000100   /* sys freq is set in nvram */
#define BD_WLAN0        0x00000200   /* Enable WLAN0 */
#define BD_MEMCAP       0x00000400   /* CAP SDRAM @ memCap for testing */
#define BD_DISWATCHDOG  0x00000800   /* disable system watchdog */
#define BD_WLAN1        0x00001000   /* Enable WLAN1 (ar5212) */
#define BD_ISCASPER     0x00002000   /* FLAG for AR2312 */
#define BD_WLAN0_2G_EN  0x00004000   /* FLAG for radio0_2G */
#define BD_WLAN0_5G_EN  0x00008000   /* FLAG for radio0_2G */
#define BD_WLAN1_2G_EN  0x00020000   /* FLAG for radio0_2G */
#define BD_WLAN1_5G_EN  0x00040000   /* FLAG for radio0_2G */

#define BD_WDOG_GPIO    0x80000000   /* watchdog gpio is valid in nvram */
    uint16_t resetConfigGpio;             /* Reset factory GPIO pin */
    uint16_t sysLedGpio;                  /* System LED GPIO pin */

    uint32_t cpuFreq;                     /* CPU core frequency in Hz */
    uint32_t sysFreq;                     /* System frequency in Hz */
    uint32_t cntFreq;                     /* Calculated C0_COUNT frequency */

    uint8_t  wlan0Mac[6];
    uint8_t  enet0Mac[6];
    uint8_t  enet1Mac[6];

    uint16_t pciId;                       /* Pseudo PCIID for common code */
    uint16_t memCap;                      /* cap bank1 in MB */

    /* version 3 */
    uint8_t  wlan1Mac[6];                 /* (ar5212) */
    uint8_t  serial_number[12];	          /* serial number - no null padding */
    uint16_t watchdogGpio;                /* System LED GPIO pin */
    uint8_t  secret_number[44];           /* secret number, ascii hex 40 characters + 1 null + 3 for (poor) alignment */
    uint16_t flags2;                      /* New flags field (first used in version 12) */
#define BD_FLAGS2_MFG_DONE 0x0001

    /* version 11 */
    char     hwRev[64];

    /* version 13*/
    uint8_t mesh_prefix;                  /* Per-OUI mesh ID prefix */
    uint8_t _pad[3];
};

#define AR531X_BD_REV3_SIZE offsetof(struct ar531x_boarddata, watchdogGpio);
#define AR531X_BD_REV3_CKSUM_OFFSET (0xff+0xff)

#define AR531X_BD_REV10_SIZE offsetof(struct ar531x_boarddata, hwRev);
#define AR531X_BD_REV11_SIZE offsetof(struct ar531x_boarddata, mesh_prefix);
#define AR531X_BD_REV12_SIZE AR531X_BD_REV11_SIZE  /* Size of structure didn't change */
#define AR531X_BD_REV13_SIZE (sizeof(struct ar531x_boarddata))

static inline unsigned short calc_board_data_checksum(const struct ar531x_boarddata *bd)
{
    struct ar531x_boarddata bd_temp = *bd;
    unsigned char *p = (unsigned char *) &bd_temp;
    int size = 0, i;
    unsigned short cksum = 0;

    bd_temp.cksum = 0;

    if (bd->rev == htons(13)) {
        size = AR531X_BD_REV13_SIZE;
    } else if (bd->rev == htons(12)) {
        size = AR531X_BD_REV12_SIZE;
    } else if (bd->rev == htons(11)) {
        size = AR531X_BD_REV11_SIZE;
    } else if (bd->rev == htons(10)) {
        size = AR531X_BD_REV10_SIZE;
    } else if (bd->rev == htons(3) || bd->rev == htons(5)) {
        size = AR531X_BD_REV3_SIZE;
        cksum += AR531X_BD_REV3_CKSUM_OFFSET;
    } else {
        return 0;
    }

    for (i = 0; i < size; i++) {
        cksum += p[i];
    }
    return cksum;
}

#endif
