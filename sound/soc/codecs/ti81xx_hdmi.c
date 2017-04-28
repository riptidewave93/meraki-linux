/*
 * ALSA SoC TI81XX HDMI dummy driver
 *
 *  This driver is used by TI81XX devices which have an on-chip HDMI controllers
 *  where no codec is needed.  This file provides stub codec that can be used
 *  in these configurations.
 *  Based on OMAP4 HDMI audio codec driver
 *
 * Author:      Deepu Raj <deepu.raj@ti.com>
 * Author:      Vaibhav Bedia <vaibhav.bedia@ti.com>
 * Copyright:   (C) 2011  Texas Instruments, India
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <sound/soc.h>
#include <sound/pcm.h>
#include <sound/initval.h>

static struct snd_soc_codec_driver soc_codec_hdmi;

/* Dummy dai driver for HDMI */
static struct snd_soc_dai_driver ti81xx_dai = {
	.name = "HDMI-DAI-CODEC",
	.playback = {
		.stream_name = "Playback",
		.channels_min = 2,
		.channels_max = 2,
		.rates = SNDRV_PCM_RATE_48000
			| SNDRV_PCM_RATE_32000
			| SNDRV_PCM_RATE_44100
			| SNDRV_PCM_RATE_96000
			| SNDRV_PCM_RATE_192000 ,
		.formats = SNDRV_PCM_FMTBIT_S16_LE
			| SNDRV_PCM_FMTBIT_S32_LE
			| SNDRV_PCM_FMTBIT_S24_LE,
	},
};

static int hdmi_codec_probe(struct platform_device *pdev)
{
	int ret;

	ret = snd_soc_register_codec(&pdev->dev, &soc_codec_hdmi,
					&ti81xx_dai, 1);
	if (ret < 0)
		printk(KERN_INFO " HDMI Codec Register Failed\n");

	return ret;
}

static int hdmi_codec_remove(struct platform_device *pdev)
{
	snd_soc_unregister_codec(&pdev->dev);
	return 0;
}

static struct platform_driver hdmi_codec_driver = {
	.probe		= hdmi_codec_probe,
	.remove		= hdmi_codec_remove,
	.driver		= {
		.name	= "hdmi-dummy-codec",
		.owner	= THIS_MODULE,
	},
};

static int __init hdmi_modinit(void)
{
	return platform_driver_register(&hdmi_codec_driver);
}

static void __exit hdmi_exit(void)
{
	platform_driver_unregister(&hdmi_codec_driver);
}

module_init(hdmi_modinit);
module_exit(hdmi_exit);

MODULE_AUTHOR("Vaibhav Bedia <vaibhav.bedia@ti.com>");
MODULE_DESCRIPTION(" HDMI Dummy codec Interface");
MODULE_LICENSE("GPL");
