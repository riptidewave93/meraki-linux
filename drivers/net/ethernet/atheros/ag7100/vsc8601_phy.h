/* vsc8601.h
 *
 * History:
 * Jan 14, 2007 wclewis ready common BDI/ECOS/Linux
 * May 24, 2007 Tag before BSP resturcture 
 */

#ifndef _VSC8601_PHY_H
#define _VSC8601_PHY_H

#ifndef CEXTERN
#define  CEXTERN static inline
#endif

#include "ag7100.h"

/* These must be extern to allow linking to ag7100 */
int vsc8601_phy_setup(int unit);

unsigned int 
vsc8601_phy_get_link_status(int unit, int *link, int *fdx, ag7100_phy_speed_t *speed);

int
vsc8601_phy_print_link_status(int unit);
struct ag7100_phy_disp_t vsc8601_phy_disp;

#endif
