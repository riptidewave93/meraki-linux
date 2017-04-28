#ifndef _AG7100_PHY_H
#define _AG7100_PHY_H

#define phy_reg_read        ag7100_mii_read
#define phy_reg_write       ag7100_mii_write

#include "ag7100.h"

#ifdef __BDI

/* Empty */

#else
#ifdef __ECOS

/* ecos will set the value of CYGNUM_USE_ENET_PHY to one of the following strings
 * based on the cdl. These are defined here in no particuilar way so the
 * #if statements that follow will have something to compare to.
 */
#define AR7100_VSC_ENET_PHY             1
#define AR7100_VSC8601_ENET_PHY         2
#define AR7100_VSC8601_VSC8601_ENET_PHY 3
#define AR7100_VSC8601_VSC73XX_ENET_PHY 4
#define AR7100_ICPLUS_ENET_PHY          5
#define AR7100_REALTEK_ENET_PHY         6
#define AR7100_ADMTEK_ENET_PHY          7
#define AR7100_ATHRF1_ENET_PHY          8
#define AR7100_ATHRS26_ENET_PHY         9
#define AR7100_ATHRS16_ENET_PHY         10

#if (CYGNUM_USE_ENET_PHY == AR7100_VSC_ENET_PHY)
#   define CONFIG_AG7100_VITESSE_PHY
#elif (CYGNUM_USE_ENET_PHY == AR7100_VSC8601_ENET_PHY)
#   define CONFIG_AG7100_VITESSE_PHY
#elif (CYGNUM_USE_ENET_PHY == AR7100_VSC8601_VSC73XX_ENET_PHY)
#   define CONFIG_AG7100_VITESSE_8601_7395_PHY
#elif (CYGNUM_USE_ENET_PHY == AR7100_ICPLUS_ENET_PHY)
#   define CONFIG_AG7100_ICPLUS_PHY
#elif (CYGNUM_USE_ENET_PHY == AR7100_REALTEK_ENET_PHY)
#   define CONFIG_AG7100_REALTEK_PHY
#elif (CYGNUM_USE_ENET_PHY == AR7100_ADMTEK_ENET_PHY)
#   define CONFIG_AG7100_ADM6996FC_PHY
#elif (CYGNUM_USE_ENET_PHY == AR7100_ATHRF1_ENET_PHY)
#   define CONFIG_AG7100_ATHRF1_PHY
#elif (CYGNUM_USE_ENET_PHY == AR7100_ATHRS26_ENET_PHY)
#   define CONFIG_AG7100_ATHRS26_PHY
#elif (CYGNUM_USE_ENET_PHY == AR7100_ATHRS16_ENET_PHY)
#   define CONFIG_AG7100_ATHRS16_PHY
#else
#error unknown PHY type CYGNUM_USE_ENET_PHY
#endif

#include "vsc8601_phy.h"
#include "vsc73xx.h"
#include "ipPhy.h"
#include "rtPhy.h"
#include "adm_phy.h"
#include "athr_phy.h"
#include "athrs26_phy.h"

#define in_interrupt(x)    0
#define schedule_work(x)
#define INIT_WORK(x,y)

#else /* Must be Linux, CONFIGs are defined in .config */

/* Empty */

#endif
#endif

/*
 * Support for runtime selection of PHY
 */

#define AG7100_PHY_TYPES (defined(CONFIG_AG7100_ATHRF1_PHY) + defined(CONFIG_AG7100_ATHRS16_PHY) + \
			  defined(CONFIG_AG7100_ATHRS26_PHY) +	defined(CONFIG_AG7100_VITESSE_PHY) + \
			  defined(CONFIG_AG7100_VITESSE_8601_7395_PHY) + \
			  defined(CONFIG_AG7100_ICPLUS_PHY) + defined(CONFIG_AG7100_REALTEK_PHY) + \
			  defined(CONFIG_AG7100_ADM6996FC_PHY))

#if AG7100_PHY_TYPES < 1
#error Define at least one PHY
#endif

#if AG7100_PHY_TYPES > 1
#define AG7100_PHY_RUNTIME 1

struct ag7100_phy_disp_t {
  int (*phy_setup)(int ethUnit);
  unsigned int (*get_link_status)(int unit, int *link, int *fdx, ag7100_phy_speed_t *speed);
  int (*ioctl)(struct net_device *dev, struct ifreq *ifr, int cmd);
  int mii;
};

#endif /* AG7100_PHY_TYPES > 1 */

static inline int ag7100_do_ioctl_notsupp(struct net_device *dev, struct ifreq *ifr, int cmd)
{
  return -EOPNOTSUPP;
}

#ifdef CONFIG_AG7100_VITESSE_8601_PHY

#include "vsc8601_phy.h"

#ifndef AG7100_PHY_RUNTIME

#define ag7100_phy_setup		vsc8601_phy_setup
#define ag7100_get_link_status		vsc8601_phy_get_link_status
#define ag7100_do_ioctl			ag7100_do_ioctl_notsupp

#endif /* AG7100_PHY_RUNTIME */
#endif /* CONFIG_AG7100_VITESSE_8601_PHY */

#if defined(CONFIG_AG7100_VITESSE_8601_7395_PHY)

#include "vsc8601_phy.h"
#include "vsc73xx.h"

static inline int 
vsc8601_vsc73xx_phy_setup(int unit)
{
  if (0==unit) {
    return vsc8601_phy_setup(unit);
  } else { 
    if (1 == unit) {
      return vsc73xx_setup(unit);
    }
  }
  return -1;
}

static inline unsigned int
vsc8601_vsc73xx_get_link_status(int unit, int *link, int *fdx, ag7100_phy_speed_t *speed)
{
  if (0==unit)
    return vsc8601_phy_get_link_status(unit, link, fdx, speed);
  else 
    if (0 == in_interrupt())
      return vsc73xx_get_link_status(unit, link, fdx, speed);
  
  return -1;
}

#ifndef AG7100_PHY_RUNTIME

#define ag7100_phy_setup(unit)		vsc8601_vsc73xx_phy_setup(unit)
#define ag7100_get_link_status		vsc8601_vsc73xx_get_link_status
#define ag7100_do_ioctl			ag7100_do_ioctl_notsupp

#endif /* AG7100_PHY_RUNTIME */
#endif /* CONFIG_AG7100_VITESSE_8601_7395_PHY */

#if defined(CONFIG_AG7100_ICPLUS_PHY)

#include "ipPhy.h"

#ifndef AG7100_PHY_RUNTIME

#define ag7100_phy_setup(unit)          ip_phySetup(unit)
#define ag7100_get_link_status		ip_phyGetLinkStatus
#define ag7100_do_ioctl			ag7100_do_ioctl_notsupp

#endif /* AG7100_PHY_RUNTIME */
#endif /* CONFIG_AG7100_ICPLUS_PHY */

#if defined(CONFIG_AG7100_REALTEK_PHY)

#include "rtPhy.h"

#ifndef AG7100_PHY_RUNTIME

#define ag7100_phy_setup(unit)          rt_phySetup(unit)
#define ag7100_get_link_status		rt_phyGetLinkStatus
#define ag7100_do_ioctl			ag7100_do_ioctl_notsupp

#endif /* AG7100_PHY_RUNTIME */
#endif /* CONFIG_AG7100_REALTEK_PHY */

#if defined(CONFIG_AG7100_ADM6996FC_PHY)

#include "adm_phy.h"

#ifndef AG7100_PHY_RUNTIME

#define ag7100_phy_setup(unit)          adm_phySetup(unit)
#define ag7100_get_link_status		adm_phyGetLinkStatus
#define ag7100_do_ioctl			ag7100_do_ioctl_notsupp

#endif /* AG7100_PHY_RUNTIME */
#endif /* CONFIG_AG7100_ADM6996FC_PHY */

#if defined(CONFIG_AG7100_ATHRS26_PHY)

#include "athrs26_phy.h"

#ifndef AG7100_PHY_RUNTIME

#define ag7100_phy_setup(unit)          athrs26_phy_setup (unit)
#define ag7100_get_link_status		athrs26_get_link_status
#define ag7100_do_ioctl			athrs26_phy_do_ioctl

#endif /* AG7100_PHY_RUNTIME */
#endif /* CONFIG_AG7100_ATHRS26_PHY */

#if defined(CONFIG_AG7100_ATHRS16_PHY)

#include "athrs16_phy.h"

#ifndef AG7100_PHY_RUNTIME

#define ag7100_phy_setup(unit)          athrs16_phy_setup (unit)
#define ag7100_get_link_status		athrs16_get_link_status
#define ag7100_do_ioctl			athrs16_phy_do_ioctl

#endif /* AG7100_PHY_RUNTIME */
#endif /* CONFIG_AG7100_ATHRS16_PHY */

#if defined(CONFIG_AG7100_ATHRF1_PHY)

#include "athr_phy.h"

#ifndef AG7100_PHY_RUNTIME

#define ag7100_phy_setup(unit)          athr_phy_setup(unit)
#define ag7100_get_link_status		athr_phy_get_link_status
#define ag7100_do_ioctl			ag7100_do_ioctl_notsupp

#endif /* AG7100_PHY_RUNTIME */
#endif /* CONFIG_AG7100_ATHRF1_PHY */

#ifdef AG7100_PHY_RUNTIME

extern struct ag7100_phy_disp_t *ag7100_phy_disp[];

static inline int ag7100_phy_setup(int unit)
{
  return (*ag7100_phy_disp[unit]->phy_setup)(unit);
}

static inline unsigned int ag7100_get_link_status(int unit, int *link, int *fdx, ag7100_phy_speed_t *speed)
{
  return (*ag7100_phy_disp[unit]->get_link_status)(unit, link, fdx, speed);
}

static inline int
ag7100_do_ioctl(struct net_device *dev, struct ifreq *ifr, int cmd)
{
  ag7100_mac_t *mac = (ag7100_mac_t *)netdev_priv(dev);
  if (ag7100_phy_disp[mac->mac_unit]->ioctl)
    return (*ag7100_phy_disp[mac->mac_unit]->ioctl)(dev, ifr, cmd);
  else return -EOPNOTSUPP;
}

#endif /* AG7100_PHY_RUNTIME */

#endif /* _AG7100_PHY_H */
