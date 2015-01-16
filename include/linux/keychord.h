/*
 *  Key chord input driver
 *
 * Copyright (C) 2008 Google, Inc.
 * Author: Mike Lockwood <lockwood@android.com>
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

<<<<<<< HEAD:arch/arm/mach-msm/include/mach/system.h
/* low level hardware reset hook -- for example, hitting the
 * PSHOLD line on the PMIC to hard reset the system
 */
extern void (*msm_hw_reset_hook)(void);

void msm_set_i2c_mux(bool gpio, int *gpio_clk, int *gpio_dat);

=======
#ifndef __LINUX_KEYCHORD_H_
#define __LINUX_KEYCHORD_H_

#include <uapi/linux/keychord.h>

#endif	/* __LINUX_KEYCHORD_H_ */
>>>>>>> common/android-3.10.y:include/linux/keychord.h
