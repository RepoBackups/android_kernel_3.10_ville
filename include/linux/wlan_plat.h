/* include/linux/wlan_plat.h
 *
 * Copyright (C) 2010 Google, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#ifndef _LINUX_WLAN_PLAT_H_
#define _LINUX_WLAN_PLAT_H_

<<<<<<< HEAD
=======
#define WLAN_PLAT_NODFS_FLAG	0x01

>>>>>>> common/android-3.10.y
struct wifi_platform_data {
	int (*set_power)(int val);
	int (*set_reset)(int val);
	int (*set_carddetect)(int val);
	void *(*mem_prealloc)(int section, unsigned long size);
	int (*get_mac_addr)(unsigned char *buf);
<<<<<<< HEAD
	void *(*get_country_code)(char *ccode);
#ifdef CONFIG_BCMDHD_4335_MCC
	struct msm_bus_scale_pdata *bus_scale_table;
#endif
=======
	void *(*get_country_code)(char *ccode, u32 flags);
>>>>>>> common/android-3.10.y
};

#endif
