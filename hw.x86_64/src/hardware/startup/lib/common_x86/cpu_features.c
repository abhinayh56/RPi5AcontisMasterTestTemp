/*
 * Copyright (c) 2008, 2022, BlackBerry Limited.
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





#include <startup.h>

/* cpu feature bits set by x86_cputype() */
unsigned cpu_features;		/* feature bits on EDX, from CPUID(eax=0x01) */
unsigned cpu_features2;	/* feature bits on ECX, from CPUID(eax=0x01) */
unsigned cpu_features3;	/* feature bits on EBX, from CPUID(eax=0x07, ecx=0x00) */
unsigned cpu_tsc;	/* feature bits on EDX, from CPUID(eax=0x80000007) */
uint8_t cpu_features_set;	/* It is set if cpu_feature is not zero */
