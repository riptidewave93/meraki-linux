/*
 *  Meraki small form-factor pluggable (SFP/SFP+) transceiver socket driver
 *
 *  Copyright (C) 2013 Cisco Systems, Inc.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
 */

#ifndef __MERAKI_SFP_H__
#define __MERAKI_SFP_H__

#include <linux/types.h>
#include <linux/module.h>

struct sfp_notify
{
    void (*insert)(unsigned int port, void* user_data);
    void (*remove)(unsigned int port, void* user_data);

    struct module* owner;
    void* user_data;

    struct list_head list;
};

// For information on these fields, see SFF-8472
// Note that the string fields are NOT null-terminated!
struct sfp_data
{
    uint8_t identifier;
    uint8_t ext_identifier;
    uint8_t connector;
    uint8_t eth_10g_comp;
    uint8_t sonet_comp_1;
    uint8_t sonet_comp_2;
    uint8_t eth_comp;
    uint8_t fc_1;
    uint8_t fc_2_and_pigtail;
    uint8_t fc_3;
    uint8_t fc_4;
    uint8_t encoding;
    uint8_t bitrate;   // Units of 100 Mbps
    uint8_t rate_id;
    uint8_t length_smf_km;
    uint8_t length_smf;
    uint8_t length_om2;
    uint8_t length_om1;
    uint8_t length_om4_cu;
    uint8_t length_om3;
    char vendor[16];   // NOT null-terminated!
    uint8_t reserved;
    uint8_t oui[3];
    char vendor_pn[16];   // NOT null-terminated!
    char vendor_rev[4];   // NOT null-terminated!
    uint16_t wavelength __attribute__((packed));
    uint8_t reserved_2;
    uint8_t base_cc;
    uint16_t options __attribute__ ((packed));
    uint8_t bitrate_max;
    uint8_t bitrate_min;
    char vendor_sn[16];   // NOT null-terminated!
    char date_code[8];    // NOT null-terminated!
    uint8_t diag_type;
    uint8_t enhanced_opts;
    uint8_t spec_compliance;
    uint8_t ext_cc;
    uint8_t vendor_data[32];
};

extern int register_sfp_notify(struct sfp_notify* notify);
extern int unregister_sfp_notify(struct sfp_notify* notify);
extern int get_sfp_data(unsigned int port, struct sfp_data* data);

#endif
