/*
 * $QNXLicenseC:
 * Copyright 2008, 2017 QNX Software Systems.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"). You
 * may not reproduce, modify or distribute this software except in
 * compliance with the License. You may obtain a copy of the License
 * at: http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OF ANY KIND, either express or implied.
 *
 * This file may contain contributions from others, either as
 * contributors under the License or as licensors under other terms.
 * Please review this entire file for other proprietary rights or license
 * notices, as well as the QNX Development Suite License Guide at
 * http://licensing.qnx.com/license-guide/ for other information.
 * $
 */

#include <startup.h>
#include "common_x86/hv_guest.h"
#include "amd_misc.h"


//
// Initialize qtime structure in the system page.
// This code is hardware dependent and may have to be
// changed by end users.
//

/*
 ===============================================================================
 lapic_start
 lapic_diff

 These functions used to calibrate the TSC

*/
static unsigned lapic_start(void)
{
	_Uint32t reload = lapic_read(LAPIC_ICR);
	if (reload == 0)
	{
		/*
		 * First time called for this core
		 * Set the divisor to 1 and initialize the ICR value. We use
		 * a value which ensures that the counter will not roll-over
		 * within 1 msec as this causes the cailbrate_tsc() function
		 * (which assumes this) trouble. The real (1msec) timer value
		 * will be written when the kernel starts
		 */
		lapic_write(LAPIC_TDCR, lapic_read(LAPIC_TDCR) | 0xB);
		reload = 0x80000000;
	}
	/* (re)start the timer by reloading the initial count register */
	lapic_write(LAPIC_ICR, reload);
	/* return the elapsed count */
	return reload - lapic_read(LAPIC_CCR);
}
static unsigned lapic_diff(_Uint32t start)
{
	/* count down timer */
	const _Uint32t reload = lapic_read(LAPIC_ICR);
	const _Uint32t now = lapic_read(LAPIC_CCR);

	if(now <= start) return start - now;
	else return reload - (now - start);
}

/*
 ===============================================================================
 get_apic_timer_irq

 return the IRQ associated with <vector>

*/
static int get_apic_timer_irq(const unsigned vector)
{
	const struct intrinfo_entry * const intrinfo = lsp.intrinfo.p;
	const unsigned num_entries = lsp.intrinfo.size/sizeof(*intrinfo);
	int irq = -1;
	unsigned i;

	for (i=0; i<num_entries; i++)
	{
		const struct intrinfo_entry * const iip = &intrinfo[i];
		const unsigned first_vector = iip->cpu_intr_base;
		const unsigned last_vector = first_vector + iip->num_vectors - 1;

		if ((vector >= first_vector) && (vector <= last_vector))
		{
			irq = iip->vector_base + (vector - first_vector);
			break;
		}
	}
	return irq;
}

/*
 ===============================================================================
 amd_apic_timer_calib

 return the rate (in Hz) at which the LAPIC timer decrements

*/
static inline unsigned get_pit_timer0_count(void)
{
	unsigned ret;

	__asm__ __volatile__(
		"xorl	%%eax,%%eax;"
		"outb	%%al,$0x43;"
		"inb	$0x40,%%al;"
		"movb	%%al,%%ah;"
		"inb	$0x40,%%al;"
		"xchgb	%%al,%%ah;"
		: "=a" (ret)
	);
	return ret;
}

#define PIT_COUNTER_MASK			0xFFFF
#define PIT_TIMER_FREQUNCY			1193182UL		// 1.19318 MHz = 105/88 MHz

static uint64_t amd_apic_timer_calib(void)
{
	uint64_t apic_timer_start, apic_timer_now;
	uint64_t pit_start, pit_pre, pit_now;
	uint64_t pit_tadj, pit_diff;
	uint64_t calib_time_ms;
	uint64_t tmr_freq;

	/* set the LAPIC timer divisor to 1 */
	lapic_write(LAPIC_TDCR, lapic_read(LAPIC_TDCR) | 0xB);

	/* start the LAPIC timer */
	lapic_write(LAPIC_ICR, 0x80000000);

	/*
	 * initialize the 8254 timer chip for counter0, MODE2 and Binary mode
	 */
	out8(0x43, 0x34);
	out8(0x40, 0xff);
	out8(0x40, 0xff);

	/* get the start counter value */
	apic_timer_start = lapic_read(LAPIC_CCR);
	pit_start = get_pit_timer0_count() & PIT_COUNTER_MASK;

	pit_pre = pit_start;
	pit_tadj = 0;
	pit_diff = 0;

	/* take about 200s for LAPIC timer calibration */
	calib_time_ms = 200;
	while (1) {
		/* get a new data point */
		pit_now = get_pit_timer0_count() & PIT_COUNTER_MASK;

		/* check if the counter rolled thru 0 */
		if (pit_now > pit_pre) {
			pit_tadj += PIT_COUNTER_MASK + 1;
		}

		/* update the previous counter value */
		pit_pre = pit_now;

		/* update the PIT timer elapsed count */
		pit_diff = pit_tadj + pit_start - pit_now;

		/* check the calibration time */
		if (pit_diff > (calib_time_ms * PIT_TIMER_FREQUNCY / 1000)) {
			break;
		}
	}

	/* get the end counter value */
	apic_timer_now = lapic_read(LAPIC_CCR);
	pit_now = get_pit_timer0_count() & PIT_COUNTER_MASK;

	/* calculate the PIT timer elapsed count */
	if (pit_now > pit_pre) {
		pit_tadj += PIT_COUNTER_MASK + 1;
	}
	pit_diff = pit_tadj + pit_start - pit_now;

	/* calculate the frequency */
	tmr_freq = PIT_TIMER_FREQUNCY * (apic_timer_start - apic_timer_now) / pit_diff;

	/* stop the timers */
	lapic_write(LAPIC_ICR, 0);
	out8(0x43, 0x34);
	out8(0x40, 0);
	out8(0x40, 0);

	if (debug_flag > 1) {
		kprintf("%s: LAPIC timer calibration take about %d ms\n", __FUNCTION__, (pit_diff * 1000 / PIT_TIMER_FREQUNCY));
	}

	return tmr_freq;
}

/*
 ===============================================================================
 get_apic_timer_freq

 returns the rate (in Hz) at which the LAPIC timer decrements

*/
static uint64_t get_apic_timer_freq(void)
{
	uint64_t tmr_freq = timer_freq;	/* set by command line option */

	if (tmr_freq == 0)
	{
		if (!hvg_lapic_frequency(&tmr_freq)) {
			extern unsigned get_processor_base_freq(void);

			if (is_amd_ryzen()) {
				if (debug_flag > 1) {
					kprintf("%s: calibrate LAPIC timer for AMD Ryzen\n", __FUNCTION__);
				}
				tmr_freq = amd_apic_timer_calib();
			}
			else {
				/*
				 * The LAPIC timer will be deemed not supported for processors in
				 * which the LAPIC base frequency is not constant in lower P-states. This is
				 * because this condition is not currently detectable by the kernel and will
				 * therefore mess up its internal time keeping
				 */
				uint32_t const eax = x86_cpuid(0x6, NULL);
				if (debug_flag > 1)
				{
					kprintf("CPUID:0x6, eax: %x\n", eax);
				}
				if ((eax & (1u << 2)) != 0)
				{
					tmr_freq = get_processor_base_freq();
				}
				else
				{
					kprintf("LAPIC Timer not supported\n");
				}
			}
		}
	}
	return tmr_freq;
}

/*
 ===============================================================================
 init_qtime_lapic

 This function attempts to set up the Local APIC timer to be the system clock
 for the calling processor (which is CPU 0, the boot processor)

 Returns: 0 on success, otherwise -1 on any error

*/
int init_qtime_lapic(void)
{
	int r = -1;

	ASSERT(lapic_startup_vaddr != NULL);
	ASSERT(lapic_callout_vaddr != NULL);

	struct qtime_entry *qtime = alloc_qtime();

	if (qtime == NULL)
	{
		if (debug_flag) kprintf("ERROR: Unable to allocate qtime_entry\n");
	}
	else
	{
		/* stop the timer */
		lapic_write(LAPIC_ICR, 0);

		/* determine timer frequency */
		uint64_t apic_timer_freq = get_apic_timer_freq();
		if (debug_flag > 1) kprintf("LAPIC timer freq: %u Hz\n", apic_timer_freq);

		if (apic_timer_freq > 0)
		{
			/* set the divisor to 1 */
			lapic_write(LAPIC_TDCR, lapic_read(LAPIC_TDCR) | 0xB);

			const _Uint32t tdcr = lapic_read(LAPIC_TDCR);
			const _Uint32t div_bits = (tdcr & 3) | ((tdcr & 8) >> 1);
			const _Uint32t div = (div_bits == 7) ? 1 : (2u << div_bits);
			if (debug_flag > 1) {
				kprintf("LAPIC timer freq: %u Hz, tdcr: 0x%x, div_bits: 0x%x, divisor: %u\n",
					apic_timer_freq, tdcr, div_bits, div);
			}
			apic_timer_freq /= div;

			invert_timer_freq(qtime, apic_timer_freq);

			/*
			 * Ensure the mode is periodic. lapic_configure() should have already set
			 * this but we'll check anyway. We don't unmask now. That will be done when
			 * procnto attaches to the IRQ
			 */
			_Uint32t lvt_timer = lapic_read(LAPIC_LTR);
			if (((lvt_timer >> 17) & 3) != 1)
			{
				if (debug_flag) kprintf("LAPIC timer mode is %u, should be 1", (lvt_timer >> 17) & 3);
				struct {
					_Uint32t ebx;
					_Uint32t edx;
					_Uint32t ecx;
				} regs;
				x86_cpuid(1, (void *)&regs);
				const unsigned tsc_deadline_supported = ((regs.ecx & (1u << 24)) != 0);
				/* if TSC-deadline mode is supported, clear that mode bit, otherwise preserve it */
				if (tsc_deadline_supported) lvt_timer &= ~(1u << 18);
				lapic_write(LAPIC_LTR, lvt_timer | LVT_TIMER_MODE_PERIODIC);
			}

			timer_start = lapic_start;
			timer_diff  = lapic_diff;

			qtime->cycles_per_sec = 0;
			set_cycles_per_sec(qtime);

			/*
			 * set the reload value for a 1 msec tick but don't program the
			 * ICR (see comments in timer_start() above)
			 */
//			lapic_write(LAPIC_ICR, apic_timer_freq / 1000);

			/* set the timer IRQ and ensure its handled by CPU 0 (ie. this CPU) */
			qtime->intr = get_apic_timer_irq(lapic_read(LAPIC_LTR) & LVT_VECTOR_MASK);

			if (debug_flag) {
				kprintf("LAPIC on CPU 0 (ID=0x%x) selected for system clock on IRQ 0x%x\n", lapic_id(), qtime->intr);
			}
			r = 0;	// success
		}
	}
	return r;
}

/*
 ===============================================================================
                  M I S C   S U P P O R T   R O U T I N E S
 ===============================================================================
*/
