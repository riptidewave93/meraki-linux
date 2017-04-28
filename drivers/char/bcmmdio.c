
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/mii.h>

#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/platform_device.h>

struct bcm_mdio {
    volatile u32 control;
    volatile u32 data;
};

enum bcm_mdio_command {
    MDIO_READ = 2,
    MDIO_WRITE = 1,
};

struct bcm_mdio_op {
    enum bcm_mdio_command cmd;
    u8 addr:5;
    u8 reg:5;
    u16 data;
};

static struct bcm_mdio *bcm;

#define MDIO_DATA_START      (1 << 30)
#define MDIO_DATA_OP_SHIFT   28
#define MDIO_DATA_ADDR_SHIFT 23
#define MDIO_DATA_REG_SHIFT  18

#define MDIO_CTRL_EXT    (1 << 9)
#define MDIO_CTRL_BUSY   (1 << 8)
#define MDIO_CTRL_PRE    (1 << 7)
#define MDIO_CTRL_MDCDIV 33   //defined in datasheet. Reference clock is 66Mhz

int bcm_command(struct bcm_mdio *bcm, struct bcm_mdio_op *op)
{
    u32 data =
        (op->cmd << MDIO_DATA_OP_SHIFT)
        | MDIO_DATA_START
        | (op->addr << MDIO_DATA_ADDR_SHIFT)
        | (op->reg << MDIO_DATA_REG_SHIFT)
        | (op->data)
        | (2 << 16);

    bcm->data = data;

    while (bcm->control & MDIO_CTRL_BUSY)
        ;

    if (op->cmd == MDIO_READ)
        op->data = bcm->data & 0xffff;

    return 0;
}

long bcm_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    struct bcm_mdio_op op;

    struct mii_ioctl_data *mii = if_mii((struct ifreq *)arg);


    op.addr = mii->phy_id;
    op.reg = mii->reg_num;

    switch(cmd) {
        case SIOCGMIIREG:
            op.cmd = MDIO_READ;
            bcm_command(bcm, &op);
            mii->val_out = op.data;
            break;
        case SIOCSMIIREG:
            op.cmd = MDIO_WRITE;
            op.data = mii->val_in;
            bcm_command(bcm, &op);
            break;
        default:
            return -EOPNOTSUPP;
    }

    return 0;
}

static struct file_operations bcm_fops = {
    .owner = THIS_MODULE,
    .unlocked_ioctl = bcm_ioctl,
};

static struct miscdevice bcm_miscdev = {
    .fops       = &bcm_fops,
    .nodename   = "bcm_mdio",
    .name       = "bcm_mdio",
    .minor      = MISC_DYNAMIC_MINOR,
};

static int __devinit bcm_mdio_probe(struct platform_device *pdev) {
    bcm = ioremap(0x18032000, 8);

    if (!bcm)
        return -EINVAL;

    //set up clock rate and external access
    bcm->control = MDIO_CTRL_EXT | MDIO_CTRL_PRE | MDIO_CTRL_MDCDIV;
    return misc_register(&bcm_miscdev);
}

static int __devexit bcm_mdio_remove(struct platform_device *pdev) {
    iounmap(0x18032000);

    return 0;
}

static const struct of_device_id of_bcm_mdio_match[] = {
	{ .compatible = "bcm-mdio-ctrl", },
	{},
};

static struct platform_driver bcm_mdio_driver = {
    .driver = {
        .name = "bcm_mdio_ctrl",
        .owner = THIS_MODULE,
        .of_match_table = of_bcm_mdio_match,
    },
    .probe = bcm_mdio_probe,
    .remove = __devexit_p(bcm_mdio_remove),
};


static int __init bcm_mdio_init(void) {
    return platform_driver_register(&bcm_mdio_driver);
}

static void __exit bcm_mdio_exit(void) {
    platform_driver_unregister(&bcm_mdio_driver);
}

module_init(bcm_mdio_init);
module_exit(bcm_mdio_exit);

MODULE_AUTHOR("Justin Delegarde");
MODULE_DESCRIPTION("Driver for Broadcom MDIO");
MODULE_LICENSE("GPL");
