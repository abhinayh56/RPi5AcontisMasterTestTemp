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




.text

.extern        cpu_features
.extern        cpu_features2
.extern        cpu_features3
.extern        cpu_features_set

    .globl        x86_cputype
x86_cputype:
    .globl        x86_64_cputype
x86_64_cputype:
// 586 and forward have an instruction to get machine type.
// The 'major' number is stored in ah.
// Return the major number *5 + 86
    push %rbx
    /* get feature bits */
    movl      $0x00000001,%eax
    cpuid
    andb      $0x0f,%ah
    push      %rax    // save family info for the return value(e.g 586, 686)
    movl      %edx,cpu_features
    movl      %ecx,cpu_features2
    movb      $0x01,cpu_features_set
    movl      $0x07,%eax
    movl      $0x00,%ecx
    cpuid
    movl      %ebx,cpu_features3
    /* get TSC bits (e.g invariant avilability) */
    movl      $0x80000000,%eax    // Get Highest Extended Function Supported
    cpuid
    cmpl      $0x80000007,%eax    // support 'Advanced Power Management Info'?
    jl        1f
    movl      $0x80000007,%eax    // if it does, check 'invariant TSC availability'
    cpuid
    movl      %edx,cpu_tsc
1:
    pop       %rax
    movb      %ah,%al
    xorb      %ah,%ah
    imulw     $0x0064,%ax
    addw      $0x0056,%ax
    pop       %rbx
    ret
