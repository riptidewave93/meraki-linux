#ifndef __NANDLOADER_COMMON__
#define __NANDLOADER_COMMON__

typedef unsigned int ath_reg_t;
extern void uart_puts(const char *str);
extern int printf(const char *fmt, ...);
extern void udelay(unsigned long usec);
extern void show_soc_banner(void);

#define ath_reg_rd(_phys)       (*(volatile ath_reg_t *)KSEG1ADDR(_phys))

#define ath_reg_wr_nf(_phys, _val) \
        ((*(volatile ath_reg_t *)KSEG1ADDR(_phys)) = (_val))

#define ath_reg_wr(_phys, _val) do {    \
        ath_reg_wr_nf(_phys, _val);     \
        ath_reg_rd(_phys);              \
} while(0)

#define ath_reg_rmw_set(_reg, _mask)    do {                    \
        ath_reg_wr((_reg), (ath_reg_rd((_reg)) | (_mask)));     \
        ath_reg_rd((_reg));                                     \
} while(0)

#define ath_reg_rmw_clear(_reg, _mask) do {                     \
        ath_reg_wr((_reg), (ath_reg_rd((_reg)) & ~(_mask)));    \
        ath_reg_rd((_reg));                                     \
} while(0)

#define ANNOUNCE_LITERAL(str) do { static char strptr[] = {str}; uart_puts(strptr); } while (0);
#define ANNOUNCE_PROGRESS() do { uart_puts(__func__); } while (0);
#define ANNOUNCE_OK() ANNOUNCE_LITERAL(" ok\n")

#endif
