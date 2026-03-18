/*
 * Copyright (c) 2014, 2020, 2022, 2023, 2025 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define FIRST_HARDWARE_INTERRUPT	0x30
// Use a hardcoded value to ensure we handle the P6 and Pentium restriction
// that the bottom 4 bits of the vector have to be all one's.
#define APIC_SPURIOUS_INTERRUPT		0x2f

#define PARASIZE		16
#define ROUNDPA(_x)		ROUND ((_x), PARASIZE)
#define CPU_VENDOR_ID_SIZE		(3 * sizeof(uint32_t) + 1)	/* sum of the size of (EBX, ECX, EDX) and '\0' */

/*
 * ACCESS_REG
 *
 * This macro can be used to generically access a specific field within a structure,
 * like 'boot_regs'
 * Example
 * ACCESS_REG(boot_reg, ax) could be safely used to obtain either the 'eax' or 'rax'
 * register depending on whether the code was compiled x86 or x86_64
 */
#if defined(__X86_64__)
	#define ACCESS_REG(base, reg) (base->r##reg)
#else
	#define ACCESS_REG(base, reg) (base->e##reg)
#endif

/*
 * This signature is used by both x86 and x86_64 bios.boot
 */
#define BIOS_SIGNATURE	0x5162696F

/*
 * The following defines specify a case range to be used in switch statements
 * Since they occur in several places, its less error prone to use macros if
 * additional ID's are added or changed
*/
#define INTEL_LEGACY_PIIX4_LPC_DIDS		0x7110
#define INTEL_POULSBO_SCH_LPC_DIDS		0x8119
#define INTEL_TUNNEL_CREEK_LPC_DIDS		0x8186
#define INTEL_BAYTRAIL_LPC_DIDS			0x0f1c
#define INTEL_RANGELEY_LPC_DIDS			0x1f38
#define INTEL_APOLLO_LAKE_LPC_DIDS		0x5ae8
#define INTEL_DENVERTON_LPC_DIDS		0x19dc
#define INTEL_ANDERSON_LAKE_LPC_DIDS		0xa24e
#define INTEL_SKYLAKE_SKU_UY_LPC_DIDS	0x9D40 ... 0x9D5F	// Skylake U and Y SKU's
#define INTEL_COFFEELAKE_LPC_DIDS		0x9D84
#define INTEL_TIGERLAKE_LPC_DIDS		0xA080 ... 0xA09F	// Tigerlake
#define INTEL_SKYLAKE_LPC_DIDS			0xA141 ... 0xA15F	// Skylake full featured (A, B, H, Q, Z SKU's)
#define INTEL_SERIES_3xx_2xx_PCH_LPC_DIDS	0xA300 ... 0xA31F
#define INTEL_TOLAPAI_LPC_DIDS          0x5031
#define INTEL_ELKHARTLAKE_LPC_DIDS		0x4B00
#define INTEL_RAPTORLAKE_LPC_DIDS		0x7A00 ... 0x7A1F
#define INTEL_ALDERLAKE_LPC_DIDS		0x7A80 ... 0x7A9F
#define INTEL_AMSTONLAKE_LPC_DIDS		0x548A
#define INTEL_SERIES_C62x_SUPER_SKUS_LPC_DIDS	0xA242 ... 0xA246
#define INTEL_SERIES_C62xA_SUPER_SKUS_LPC_DIDS	0xA24A ... 0xA24C
#define INTEL_SERIES_C62x_PROD_SKUS_LPC_DIDS	0xA1C1 ... 0xA1C7
#define INTEL_SERIES_C62xA_PROD_SKUS_LPC_DIDS	0xA1CA ... 0xA1CD

#define AMD_SB600_SB700_ACPI_DIDS		0x4385 ... 0x439D




//
// Function prototypes
//

void init_gdt(unsigned idt_size);

void x86_init_4m_region(int global);

uintptr_t x86_map(uintptr_t vaddr,paddr_t paddr,paddr_t size,int flags);
uintptr_t x86_4m_map(uintptr_t vaddr,paddr_t paddr,paddr_t size,int flags);
void x86_init_pgdir(void);
void x86_load_pgdir(unsigned cpu);

uint32_t cpu_family_model(void);
uint32_t cpu_family(void);
uint32_t cpu_model(void);
unsigned x86_cputype(void);
unsigned long x86_cpuid(int, void *);
int x86_cpuid_string(char *buf,int max);
unsigned long x86_cpuspeed(unsigned cputype);

void x86_scanmem(PADDR_T beg, PADDR_T end);

void init_reboot_callout(void);

void x86_init_pcbios(void);
int  x86_pcbios_shadow_rom(PADDR_T rom, size_t size);
void init_one_cpuinfo(unsigned cpunum);
void set_cycles_per_sec(struct qtime_entry *);
void x86_v86_add_mapping(PADDR_T start, PADDR_T len);
int	load_elf32mmu_4m(PADDR_T, Elf32_Ehdr *, Elf32_Phdr *);

/* system clock source functions */
int init_qtime_lapic(void);
int init_qtime_disable_hpet(void);

unsigned	lapic_id(void);

int find_ecam_base(paddr_t *const ecam_base, uint64_t *const size);

//
// Variable prototypes
//
extern PADDR_T					gdt_paddr;
extern PADDR_T					idt_paddr;
extern PADDR_T					page_tables;
extern unsigned					cpu_features;		/* feature bits on EDX, from CPUID(eax=0x01) */
extern unsigned					cpu_features2;		/* feature bits on ECX, from CPUID(eax=0x01) */
extern unsigned					cpu_features3;		/* feature bits on EBX, from CPUID(eax=0x07, ecx=0x00) */
extern unsigned					cpu_tsc;		/* feature bits on EDX, from CPUID(eax=0x80000007) */
extern uint8_t					cpu_features_set;	/* It is set if cpu_feature is not zero */
extern uint8_t					acpi_ignore;
extern PADDR_T					lapic_paddr;
extern void						*lapic_callout_vaddr;
extern void						*lapic_startup_vaddr;
extern X86_PREF(CPU_REGISTERS)	*boot_regs;
extern int						nx_supported;
extern int						meltdown_active;

//
// board specific SMP stuff
//
// Intel MultiProcessor Specification versions
unsigned imps_smp_num_cpu(void);

// ACPI Specification versions
unsigned acpi_smp_num_cpu(void); // returns zero if ACPI not present

unsigned is_uefi_boot(void);

// reset register argument to pass to reboot callout
typedef struct
{
	uint32_t reg_width;	/* the width of the register (1 byte, 2 bytes, 4 bytes) */
	uint32_t value;		/* the value to reset the board */
	paddr_t  reg_addr;	/* the address of reset register */
} reboot_patch_args_t;

// APIC versions
void apic_set_paddr(PADDR_T p);
void apic_add_cpuid(unsigned num_cpus, unsigned id);
void apic_smp_init(struct smp_entry *, unsigned num_cpus);
int apic_smp_start(unsigned cpu, void (*start)(void));
unsigned apic_smp_adjust_num(unsigned);
void lapic_configure(int);
void init_intrinfo_apic(void);
void init_intrinfo_lapic(void);
int init_intrinfo_ioapic(int vector_base, const int _ioapic_id);
void fix_tsc(void);
uint64_t calibrate_tsc(unsigned long ns);
uint64_t get_invariant_tsc(void);
void add_ioapic(int ioapic_id, PADDR_T paddr, int gsi, ioapic_accessfnc_t *accessfncs);
unsigned ioapic_find(void);
unsigned ioapic_get_num(void);
unsigned ioapic_get_id_first(void);
unsigned ioapic_get_id_last(void);
int ioapic_get_id_next(unsigned ioapic_id);
int ioapic_get_gsibase(unsigned ioapic_id);
void *ioapic_get_vaddr(unsigned ioapic_id);
PADDR_T ioapic_get_paddr(unsigned ioapic_id);
const ioapic_accessfnc_t * ioapic_get_accessfnc(const unsigned ioapic_id);
unsigned ioapic_set_accessfnc(const unsigned ioapic_id, const ioapic_accessfnc_t * const ioapic_fnc);
unsigned int ioapic_get_autofill_profile_num(const unsigned ioapic_id);
unsigned ioapic_set_autofill_profile_num(const unsigned ioapic_id, const unsigned int profile_num);

/* ioapic_intsrc_polarity_get() returns -1 for active low, +1 for active high, 0 undetermined/error */
int ioapic_intsrc_polarity_get(const unsigned ioapic_id, const unsigned ioapic_entry_num);
/* ioapic_intsrc_polarity_set() returns the polarity set as above or 0 for any error */
int ioapic_intsrc_polarity_set(const unsigned ioapic_id, const unsigned ioapic_entry_num, const int polarity);
/* ioapic_intsrc_trigger_mode_get() returns 0 for level, 1 for edge and -1 on any error */
int ioapic_intsrc_trigger_mode_get(const unsigned ioapic_id, const unsigned ioapic_entry_num);
/* ioapic_intsrc_trigger_mode_set() returns the trigger set as above or -1 on any error */
int ioapic_intsrc_trigger_mode_set(const unsigned ioapic_id, const unsigned ioapic_entry_num, const unsigned trigger_mode);
/* ioapic_intsrc_cfg_check() returns 0 on success otherwise -1 */
int ioapic_intsrc_cfg_check(const unsigned ioapic_id);

unsigned ioapic_discover_acpi(void);
unsigned ioapic_discover_imps(void);
unsigned ioapic_auto_config(const unsigned ioapic_id, ioapic_cfg_t *ioapic_cfg, const unsigned num);
int ioapic_autofill_defaults(const unsigned ioapic_id, ioapic_cfg_t *ioapic_cfg, const unsigned num, unsigned profile_num);
unsigned find_top_cpu_intr(void);
unsigned find_next_cpu_intr(void);

void pcu_ilb_pirq_remap_adjust(void);
void itss_pirq_remap_adjust(void);
unsigned find_P2SB(void);

//
// Common debug routines
//
void init_vga(unsigned channel, const char *init, const char *defaults);
void put_vga(int c);
extern struct callout_rtn	display_char_vga;
extern struct callout_rtn	poll_key_pckbd;

//
// Common callouts
//
extern struct callout_rtn	reboot;
extern struct callout_rtn	reboot_pckbd;
extern struct callout_rtn	reboot_fastbios;
extern struct callout_rtn	reboot_poulsbo_nokbd;
extern struct callout_rtn	reboot_ich;

/* APIC callout patch data type */
typedef struct
{
	const void *	lapic;
	const void *	ioapic;
	uint32_t		ioapic_cpu_intr_base;
} apic_patch_data_t;
