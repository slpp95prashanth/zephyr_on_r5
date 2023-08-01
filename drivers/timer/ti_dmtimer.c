/* Copyright (C) 2023 BeagleBoard.org Foundation
 * Copyright (C) 2023 S Prashanth
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT ti_am654_dmtimer

#include <zephyr/device.h>
#include <zephyr/drivers/timer/system_timer.h>
#include <zephyr/irq.h>
#include <zephyr/sys_clock.h>
#include <zephyr/kernel.h>

#include <zephyr/drivers/timer/ti_dmtimer.h>

#define CYC_PER_TICK  ((uint32_t)(sys_clock_hw_cycles_per_sec() \
				/ CONFIG_SYS_CLOCK_TICKS_PER_SEC))

static struct k_spinlock lock;

void ti_dmtimer_isr(void *data)
{
	k_spinlock_key_t key = k_spin_lock(&lock);

	sys_write32(BIT(TI_DM_TIMER_IRQENABLE_SET_OVF_EN_FLAG_SHIFT),
							TI_DM_TIMER_IRQSTATUS);
	sys_write32(0xffffffff - CYC_PER_TICK, TI_DM_TIMER_TLDR);
	sys_write32(0xffffffff - CYC_PER_TICK, TI_DM_TIMER_TCRR);
	sys_write32(BIT(TI_DM_TIMER_TCLR_ST_SHIFT), TI_DM_TIMER_TCLR);

	k_spin_unlock(&lock, key);

	sys_clock_announce(1);
}

void sys_clock_set_timeout(int32_t ticks, bool idle)
{
}

uint32_t sys_clock_cycle_get_32(void)
{
	k_spinlock_key_t key = k_spin_lock(&lock);

	uint32_t tcrr_count = sys_read32(TI_DM_TIMER_TCRR);

	k_spin_unlock(&lock, key);

	return (tcrr_count - (0xffffffff - CYC_PER_TICK));
}

unsigned int sys_clock_elapsed(void)
{
	return 0;
}

static int sys_clock_driver_init(void)
{
	IRQ_CONNECT(TI_DM_TIMER_IRQ_NUM, TI_DM_TIMER_IRQ_PRIO,
		ti_dmtimer_isr, NULL, TI_DM_TIMER_IRQ_FLAGS);

	sys_write32(BIT(TI_DM_TIMER_IRQENABLE_SET_OVF_EN_FLAG_SHIFT),
							TI_DM_TIMER_IRQENABLE_SET);

	sys_write32(0xffffffff - CYC_PER_TICK, TI_DM_TIMER_TLDR);
	sys_write32(0xffffffff - CYC_PER_TICK, TI_DM_TIMER_TCRR);

	sys_write32(0, TI_DM_TIMER_TPIR);
	sys_write32(0, TI_DM_TIMER_TNIR);

	sys_write32(BIT(TI_DM_TIMER_TCLR_ST_SHIFT), TI_DM_TIMER_TCLR);

	irq_enable(TI_DM_TIMER_IRQ_NUM);

	return 0;
}

SYS_INIT(sys_clock_driver_init, PRE_KERNEL_2,
	CONFIG_SYSTEM_CLOCK_INIT_PRIORITY);
