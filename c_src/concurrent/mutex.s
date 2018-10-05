	.file	"mutex.c"
	.intel_syntax noprefix
	.text
.Ltext0:
	.p2align 4,,15
	.globl	incers
	.type	incers, @function
incers:
.LFB12:
	.file 1 "mutex.c"
	.loc 1 77 0
	.cfi_startproc
.LVL0:
	add	DWORD PTR protected[rip], 1000
	.loc 1 87 0
	ret
	.cfi_endproc
.LFE12:
	.size	incers, .-incers
	.p2align 4,,15
	.globl	compare_and_swap
	.type	compare_and_swap, @function
compare_and_swap:
.LFB8:
	.loc 1 16 0
	.cfi_startproc
.LVL1:
	.loc 1 17 0
	mov	DWORD PTR [rdi], edx
	.loc 1 19 0
	mov	eax, 1
	ret
	.cfi_endproc
.LFE8:
	.size	compare_and_swap, .-compare_and_swap
	.p2align 4,,15
	.globl	lock
	.type	lock, @function
lock:
.LFB9:
	.loc 1 22 0
	.cfi_startproc
.LVL2:
	push	rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	mov	eax, DWORD PTR [rdi]
	.loc 1 22 0
	mov	rbx, rdi
.LVL3:
	.p2align 4,,10
	.p2align 3
.L4:
	.loc 1 32 0
	test	eax, eax
	jne	.L5
.LVL4:
.LBB7:
.LBB8:
	.loc 1 17 0
	mov	DWORD PTR [rbx], 1
.LVL5:
.L8:
.LBE8:
.LBE7:
.LBB9:
.LBB10:
.LBB11:
	mov	DWORD PTR [rbx], 2
.LBE11:
.LBE10:
.LBE9:
	.loc 1 44 0
	pop	rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
.LVL6:
	ret
.LVL7:
	.p2align 4,,10
	.p2align 3
.L5:
	.cfi_restore_state
	.loc 1 36 0
	cmp	eax, 1
	je	.L8
	.loc 1 41 0
	mov	edi, 1
	call	sleep
.LVL8:
	mov	eax, DWORD PTR [rbx]
	jmp	.L4
	.cfi_endproc
.LFE9:
	.size	lock, .-lock
	.p2align 4,,15
	.globl	faulty_lock
	.type	faulty_lock, @function
faulty_lock:
.LFB10:
	.loc 1 53 0
	.cfi_startproc
.LVL9:
	push	rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	mov	eax, DWORD PTR [rdi]
	.loc 1 53 0
	mov	rbx, rdi
.LVL10:
	.p2align 4,,10
	.p2align 3
.L13:
	.loc 1 57 0
	test	eax, eax
	jne	.L14
	.loc 1 57 0 is_stmt 0 discriminator 1
	mov	DWORD PTR [rbx], 1
.L17:
	.loc 1 60 0 is_stmt 1
	mov	DWORD PTR [rbx], 2
	.loc 1 66 0
	pop	rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
.LVL11:
	ret
.LVL12:
	.p2align 4,,10
	.p2align 3
.L14:
	.cfi_restore_state
	.loc 1 58 0
	cmp	eax, 1
	je	.L17
	.loc 1 63 0
	mov	edi, 1
	call	sleep
.LVL13:
	mov	eax, DWORD PTR [rbx]
	jmp	.L13
	.cfi_endproc
.LFE10:
	.size	faulty_lock, .-faulty_lock
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"mutex.c"
.LC1:
	.string	"0"
	.text
	.p2align 4,,15
	.globl	unlock
	.type	unlock, @function
unlock:
.LFB11:
	.loc 1 71 0
	.cfi_startproc
.LVL14:
	.loc 1 72 0
	cmp	DWORD PTR [rdi], 2
	jne	.L21
	.loc 1 72 0 discriminator 1
	mov	DWORD PTR [rdi], 0
	ret
.L21:
	.loc 1 71 0
	push	rax
	.cfi_def_cfa_offset 16
	.loc 1 73 0
	mov	ecx, OFFSET FLAT:__PRETTY_FUNCTION__.2914
	mov	edx, 73
	mov	esi, OFFSET FLAT:.LC0
	mov	edi, OFFSET FLAT:.LC1
.LVL15:
	call	__assert_fail
.LVL16:
	.cfi_endproc
.LFE11:
	.size	unlock, .-unlock
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC3:
	.string	"Total: %d, Failed: %d, Correct: %d, Percent Success: %f\n"
	.text
	.p2align 4,,15
	.globl	print_stats
	.type	print_stats, @function
print_stats:
.LFB13:
	.loc 1 96 0
	.cfi_startproc
.LVL17:
	mov	rsi, rdi
	.loc 1 97 0
	cvtsi2sd	xmm0, edi
	sar	rsi, 32
.LVL18:
	mov	ecx, edi
	mov	eax, 1
	cvtsi2ss	xmm1, esi
	mov	edx, esi
	sub	edx, edi
	mov	edi, OFFSET FLAT:.LC3
.LVL19:
	mulsd	xmm0, QWORD PTR .LC2[rip]
	unpcklps	xmm1, xmm1
	cvtps2pd	xmm1, xmm1
	divsd	xmm0, xmm1
	jmp	printf
.LVL20:
	.cfi_endproc
.LFE13:
	.size	print_stats, .-print_stats
	.section	.rodata.str1.8
	.align 8
.LC4:
	.string	"Iteration: %d, Check Failed!: protected (%d) != NUM_THREADS*INC_COUNT (%d)\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB14:
	.loc 1 102 0
	.cfi_startproc
.LVL21:
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	.loc 1 105 0
	xor	esi, esi
	mov	edi, OFFSET FLAT:global
	.loc 1 102 0
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	push	r15
	.cfi_offset 15, -24
	.loc 1 103 0
	xor	r15d, r15d
	.loc 1 102 0
	push	r14
	push	r13
	.cfi_offset 14, -32
	.cfi_offset 13, -40
.LBB12:
	.loc 1 107 0
	xor	r13d, r13d
.LBE12:
	.loc 1 102 0
	push	r12
	push	rbx
	sub	rsp, 8
	.cfi_offset 12, -48
	.cfi_offset 3, -56
	.loc 1 105 0
	call	pthread_mutex_init
.LVL22:
	.p2align 4,,10
	.p2align 3
.L33:
.LBB18:
.LBB13:
	mov	r14, rsp
	.loc 1 111 0
	sub	rsp, 96
	.loc 1 108 0
	mov	DWORD PTR global_lock[rip], 0
	.loc 1 109 0
	mov	DWORD PTR protected[rip], 0
.LVL23:
	.loc 1 111 0
	mov	r12, rsp
.LVL24:
	xor	ebx, ebx
.LVL25:
	.p2align 4,,10
	.p2align 3
.L27:
	lea	rdi, [r12+rbx]
.LBB14:
	.loc 1 113 0 discriminator 2
	xor	ecx, ecx
	xor	esi, esi
	mov	edx, OFFSET FLAT:incers
	add	rbx, 8
	call	pthread_create
.LVL26:
	cmp	rbx, 80
	jne	.L27
	.loc 1 113 0 is_stmt 0
	xor	bl, bl
	.p2align 4,,10
	.p2align 3
.L29:
.LBE14:
.LBB15:
	.loc 1 115 0 is_stmt 1 discriminator 2
	mov	rdi, QWORD PTR [r12+rbx]
	xor	esi, esi
	add	rbx, 8
	call	pthread_join
.LVL27:
	cmp	rbx, 80
	jne	.L29
.LBE15:
	.loc 1 117 0
	mov	edx, DWORD PTR protected[rip]
	cmp	edx, 10000
	je	.L30
	.loc 1 118 0
	mov	ecx, 10000
	mov	esi, r13d
	mov	edi, OFFSET FLAT:.LC4
	xor	eax, eax
	call	printf
.LVL28:
.L31:
.LBE13:
	.loc 1 107 0
	add	r13d, 1
.LVL29:
.LBB16:
	mov	rsp, r14
.LVL30:
.LBE16:
	cmp	r13d, 10000
	jne	.L33
.LBE18:
	.loc 1 124 0
	movabs	rax, 42949672960000
	mov	edi, r15d
	or	rdi, rax
	call	print_stats
.LVL31:
	.loc 1 126 0
	xor	edi, edi
	call	pthread_exit
.LVL32:
	.p2align 4,,10
	.p2align 3
.L30:
.LBB19:
.LBB17:
	.loc 1 120 0
	add	r15d, 1
.LVL33:
	jmp	.L31
.LBE17:
.LBE19:
	.cfi_endproc
.LFE14:
	.size	main, .-main
	.section	.rodata
	.type	__PRETTY_FUNCTION__.2914, @object
	.size	__PRETTY_FUNCTION__.2914, 7
__PRETTY_FUNCTION__.2914:
	.string	"unlock"
	.comm	global,40,32
	.globl	INC_COUNT
	.align 4
	.type	INC_COUNT, @object
	.size	INC_COUNT, 4
INC_COUNT:
	.long	1000
	.globl	NUM_THREADS
	.align 4
	.type	NUM_THREADS, @object
	.size	NUM_THREADS, 4
NUM_THREADS:
	.long	10
	.comm	global_lock,4,4
	.globl	protected
	.bss
	.align 16
	.type	protected, @object
	.size	protected, 4
protected:
	.zero	4
	.globl	SLEEP_INTERVAL
	.section	.rodata
	.align 4
	.type	SLEEP_INTERVAL, @object
	.size	SLEEP_INTERVAL, 4
SLEEP_INTERVAL:
	.long	1
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC2:
	.long	0
	.long	1079574528
	.text
.Letext0:
	.file 2 "/usr/include/bits/types.h"
	.file 3 "/usr/lib/gcc/x86_64-redhat-linux/4.8.5/include/stddef.h"
	.file 4 "/usr/include/bits/pthreadtypes.h"
	.file 5 "/usr/include/libio.h"
	.file 6 "/usr/include/stdio.h"
	.file 7 "/usr/include/unistd.h"
	.file 8 "/usr/include/pthread.h"
	.file 9 "/usr/include/assert.h"
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.long	0x9d2
	.value	0x4
	.long	.Ldebug_abbrev0
	.byte	0x8
	.uleb128 0x1
	.long	.LASF96
	.byte	0x1
	.long	.LASF97
	.long	.LASF98
	.long	.Ldebug_ranges0+0x80
	.quad	0
	.long	.Ldebug_line0
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.long	.LASF0
	.uleb128 0x2
	.byte	0x2
	.byte	0x7
	.long	.LASF1
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.long	.LASF2
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.long	.LASF3
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF4
	.uleb128 0x2
	.byte	0x2
	.byte	0x5
	.long	.LASF5
	.uleb128 0x3
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.long	.LASF6
	.uleb128 0x4
	.long	.LASF7
	.byte	0x2
	.byte	0x8c
	.long	0x5a
	.uleb128 0x4
	.long	.LASF8
	.byte	0x2
	.byte	0x8d
	.long	0x5a
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.long	.LASF9
	.uleb128 0x5
	.byte	0x8
	.uleb128 0x6
	.byte	0x8
	.long	0x86
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF10
	.uleb128 0x4
	.long	.LASF11
	.byte	0x3
	.byte	0xd4
	.long	0x3e
	.uleb128 0x6
	.byte	0x8
	.long	0x9e
	.uleb128 0x7
	.long	0x86
	.uleb128 0x4
	.long	.LASF12
	.byte	0x4
	.byte	0x3c
	.long	0x3e
	.uleb128 0x8
	.long	.LASF15
	.byte	0x38
	.byte	0x4
	.byte	0x3f
	.long	0xd1
	.uleb128 0x9
	.long	.LASF13
	.byte	0x4
	.byte	0x41
	.long	0xd1
	.uleb128 0x9
	.long	.LASF14
	.byte	0x4
	.byte	0x42
	.long	0x5a
	.byte	0
	.uleb128 0xa
	.long	0x86
	.long	0xe1
	.uleb128 0xb
	.long	0x77
	.byte	0x37
	.byte	0
	.uleb128 0x4
	.long	.LASF15
	.byte	0x4
	.byte	0x45
	.long	0xae
	.uleb128 0xc
	.long	.LASF19
	.byte	0x10
	.byte	0x4
	.byte	0x4b
	.long	0x111
	.uleb128 0xd
	.long	.LASF16
	.byte	0x4
	.byte	0x4d
	.long	0x111
	.byte	0
	.uleb128 0xd
	.long	.LASF17
	.byte	0x4
	.byte	0x4e
	.long	0x111
	.byte	0x8
	.byte	0
	.uleb128 0x6
	.byte	0x8
	.long	0xec
	.uleb128 0x4
	.long	.LASF18
	.byte	0x4
	.byte	0x4f
	.long	0xec
	.uleb128 0xc
	.long	.LASF20
	.byte	0x28
	.byte	0x4
	.byte	0x5c
	.long	0x18f
	.uleb128 0xd
	.long	.LASF21
	.byte	0x4
	.byte	0x5e
	.long	0x53
	.byte	0
	.uleb128 0xd
	.long	.LASF22
	.byte	0x4
	.byte	0x5f
	.long	0x37
	.byte	0x4
	.uleb128 0xd
	.long	.LASF23
	.byte	0x4
	.byte	0x60
	.long	0x53
	.byte	0x8
	.uleb128 0xd
	.long	.LASF24
	.byte	0x4
	.byte	0x62
	.long	0x37
	.byte	0xc
	.uleb128 0xd
	.long	.LASF25
	.byte	0x4
	.byte	0x66
	.long	0x53
	.byte	0x10
	.uleb128 0xd
	.long	.LASF26
	.byte	0x4
	.byte	0x68
	.long	0x4c
	.byte	0x14
	.uleb128 0xd
	.long	.LASF27
	.byte	0x4
	.byte	0x69
	.long	0x4c
	.byte	0x16
	.uleb128 0xd
	.long	.LASF28
	.byte	0x4
	.byte	0x6a
	.long	0x117
	.byte	0x18
	.byte	0
	.uleb128 0xe
	.byte	0x28
	.byte	0x4
	.byte	0x5a
	.long	0x1b9
	.uleb128 0x9
	.long	.LASF29
	.byte	0x4
	.byte	0x7c
	.long	0x122
	.uleb128 0x9
	.long	.LASF13
	.byte	0x4
	.byte	0x7d
	.long	0x1b9
	.uleb128 0x9
	.long	.LASF14
	.byte	0x4
	.byte	0x7e
	.long	0x5a
	.byte	0
	.uleb128 0xa
	.long	0x86
	.long	0x1c9
	.uleb128 0xb
	.long	0x77
	.byte	0x27
	.byte	0
	.uleb128 0x4
	.long	.LASF30
	.byte	0x4
	.byte	0x7f
	.long	0x18f
	.uleb128 0xe
	.byte	0x4
	.byte	0x4
	.byte	0x81
	.long	0x1f3
	.uleb128 0x9
	.long	.LASF13
	.byte	0x4
	.byte	0x83
	.long	0x1f3
	.uleb128 0x9
	.long	.LASF14
	.byte	0x4
	.byte	0x84
	.long	0x53
	.byte	0
	.uleb128 0xa
	.long	0x86
	.long	0x203
	.uleb128 0xb
	.long	0x77
	.byte	0x3
	.byte	0
	.uleb128 0x4
	.long	.LASF31
	.byte	0x4
	.byte	0x85
	.long	0x1d4
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.long	.LASF32
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.long	.LASF33
	.uleb128 0xc
	.long	.LASF34
	.byte	0xd8
	.byte	0x5
	.byte	0xf6
	.long	0x39d
	.uleb128 0xd
	.long	.LASF35
	.byte	0x5
	.byte	0xf7
	.long	0x53
	.byte	0
	.uleb128 0xd
	.long	.LASF36
	.byte	0x5
	.byte	0xfc
	.long	0x80
	.byte	0x8
	.uleb128 0xd
	.long	.LASF37
	.byte	0x5
	.byte	0xfd
	.long	0x80
	.byte	0x10
	.uleb128 0xd
	.long	.LASF38
	.byte	0x5
	.byte	0xfe
	.long	0x80
	.byte	0x18
	.uleb128 0xd
	.long	.LASF39
	.byte	0x5
	.byte	0xff
	.long	0x80
	.byte	0x20
	.uleb128 0xf
	.long	.LASF40
	.byte	0x5
	.value	0x100
	.long	0x80
	.byte	0x28
	.uleb128 0xf
	.long	.LASF41
	.byte	0x5
	.value	0x101
	.long	0x80
	.byte	0x30
	.uleb128 0xf
	.long	.LASF42
	.byte	0x5
	.value	0x102
	.long	0x80
	.byte	0x38
	.uleb128 0xf
	.long	.LASF43
	.byte	0x5
	.value	0x103
	.long	0x80
	.byte	0x40
	.uleb128 0xf
	.long	.LASF44
	.byte	0x5
	.value	0x105
	.long	0x80
	.byte	0x48
	.uleb128 0xf
	.long	.LASF45
	.byte	0x5
	.value	0x106
	.long	0x80
	.byte	0x50
	.uleb128 0xf
	.long	.LASF46
	.byte	0x5
	.value	0x107
	.long	0x80
	.byte	0x58
	.uleb128 0xf
	.long	.LASF47
	.byte	0x5
	.value	0x109
	.long	0x3d5
	.byte	0x60
	.uleb128 0xf
	.long	.LASF48
	.byte	0x5
	.value	0x10b
	.long	0x3db
	.byte	0x68
	.uleb128 0xf
	.long	.LASF49
	.byte	0x5
	.value	0x10d
	.long	0x53
	.byte	0x70
	.uleb128 0xf
	.long	.LASF50
	.byte	0x5
	.value	0x111
	.long	0x53
	.byte	0x74
	.uleb128 0xf
	.long	.LASF51
	.byte	0x5
	.value	0x113
	.long	0x61
	.byte	0x78
	.uleb128 0xf
	.long	.LASF52
	.byte	0x5
	.value	0x117
	.long	0x30
	.byte	0x80
	.uleb128 0xf
	.long	.LASF53
	.byte	0x5
	.value	0x118
	.long	0x45
	.byte	0x82
	.uleb128 0xf
	.long	.LASF54
	.byte	0x5
	.value	0x119
	.long	0x3e1
	.byte	0x83
	.uleb128 0xf
	.long	.LASF55
	.byte	0x5
	.value	0x11d
	.long	0x3f1
	.byte	0x88
	.uleb128 0xf
	.long	.LASF56
	.byte	0x5
	.value	0x126
	.long	0x6c
	.byte	0x90
	.uleb128 0xf
	.long	.LASF57
	.byte	0x5
	.value	0x12f
	.long	0x7e
	.byte	0x98
	.uleb128 0xf
	.long	.LASF58
	.byte	0x5
	.value	0x130
	.long	0x7e
	.byte	0xa0
	.uleb128 0xf
	.long	.LASF59
	.byte	0x5
	.value	0x131
	.long	0x7e
	.byte	0xa8
	.uleb128 0xf
	.long	.LASF60
	.byte	0x5
	.value	0x132
	.long	0x7e
	.byte	0xb0
	.uleb128 0xf
	.long	.LASF61
	.byte	0x5
	.value	0x133
	.long	0x8d
	.byte	0xb8
	.uleb128 0xf
	.long	.LASF62
	.byte	0x5
	.value	0x135
	.long	0x53
	.byte	0xc0
	.uleb128 0xf
	.long	.LASF63
	.byte	0x5
	.value	0x137
	.long	0x3f7
	.byte	0xc4
	.byte	0
	.uleb128 0x10
	.long	.LASF99
	.byte	0x5
	.byte	0x9b
	.uleb128 0xc
	.long	.LASF64
	.byte	0x18
	.byte	0x5
	.byte	0xa1
	.long	0x3d5
	.uleb128 0xd
	.long	.LASF65
	.byte	0x5
	.byte	0xa2
	.long	0x3d5
	.byte	0
	.uleb128 0xd
	.long	.LASF66
	.byte	0x5
	.byte	0xa3
	.long	0x3db
	.byte	0x8
	.uleb128 0xd
	.long	.LASF67
	.byte	0x5
	.byte	0xa7
	.long	0x53
	.byte	0x10
	.byte	0
	.uleb128 0x6
	.byte	0x8
	.long	0x3a4
	.uleb128 0x6
	.byte	0x8
	.long	0x21c
	.uleb128 0xa
	.long	0x86
	.long	0x3f1
	.uleb128 0xb
	.long	0x77
	.byte	0
	.byte	0
	.uleb128 0x6
	.byte	0x8
	.long	0x39d
	.uleb128 0xa
	.long	0x86
	.long	0x407
	.uleb128 0xb
	.long	0x77
	.byte	0x13
	.byte	0
	.uleb128 0xc
	.long	.LASF68
	.byte	0x4
	.byte	0x1
	.byte	0x9
	.long	0x420
	.uleb128 0xd
	.long	.LASF69
	.byte	0x1
	.byte	0xa
	.long	0x53
	.byte	0
	.byte	0
	.uleb128 0x4
	.long	.LASF68
	.byte	0x1
	.byte	0xb
	.long	0x407
	.uleb128 0xc
	.long	.LASF70
	.byte	0x8
	.byte	0x1
	.byte	0x5a
	.long	0x450
	.uleb128 0xd
	.long	.LASF71
	.byte	0x1
	.byte	0x5b
	.long	0x53
	.byte	0
	.uleb128 0xd
	.long	.LASF72
	.byte	0x1
	.byte	0x5c
	.long	0x53
	.byte	0x4
	.byte	0
	.uleb128 0x4
	.long	.LASF70
	.byte	0x1
	.byte	0x5d
	.long	0x42b
	.uleb128 0x11
	.long	.LASF90
	.byte	0x1
	.byte	0xf
	.long	0x489
	.byte	0x1
	.long	0x489
	.uleb128 0x12
	.long	.LASF73
	.byte	0x1
	.byte	0xf
	.long	0x490
	.uleb128 0x13
	.string	"a"
	.byte	0x1
	.byte	0xf
	.long	0x53
	.uleb128 0x13
	.string	"b"
	.byte	0x1
	.byte	0xf
	.long	0x53
	.byte	0
	.uleb128 0x2
	.byte	0x1
	.byte	0x2
	.long	.LASF74
	.uleb128 0x6
	.byte	0x8
	.long	0x420
	.uleb128 0x14
	.long	.LASF100
	.byte	0x1
	.byte	0x4c
	.long	0x7e
	.quad	.LFB12
	.quad	.LFE12-.LFB12
	.uleb128 0x1
	.byte	0x9c
	.long	0x4c5
	.uleb128 0x15
	.long	.LASF75
	.byte	0x1
	.byte	0x4c
	.long	0x7e
	.uleb128 0x1
	.byte	0x55
	.byte	0
	.uleb128 0x16
	.long	0x45b
	.quad	.LFB8
	.quad	.LFE8-.LFB8
	.uleb128 0x1
	.byte	0x9c
	.long	0x4f6
	.uleb128 0x17
	.long	0x46b
	.uleb128 0x1
	.byte	0x55
	.uleb128 0x17
	.long	0x476
	.uleb128 0x1
	.byte	0x54
	.uleb128 0x17
	.long	0x47f
	.uleb128 0x1
	.byte	0x51
	.byte	0
	.uleb128 0x18
	.long	.LASF73
	.byte	0x1
	.byte	0x15
	.quad	.LFB9
	.quad	.LFE9-.LFB9
	.uleb128 0x1
	.byte	0x9c
	.long	0x5c1
	.uleb128 0x19
	.long	.LASF73
	.byte	0x1
	.byte	0x15
	.long	0x490
	.long	.LLST0
	.uleb128 0x1a
	.long	0x45b
	.quad	.LBB7
	.quad	.LBE7-.LBB7
	.byte	0x1
	.byte	0x22
	.long	0x559
	.uleb128 0x1b
	.long	0x47f
	.long	.LLST1
	.uleb128 0x1b
	.long	0x476
	.long	.LLST2
	.uleb128 0x1b
	.long	0x46b
	.long	.LLST3
	.byte	0
	.uleb128 0x1c
	.quad	.LBB9
	.quad	.LBE9-.LBB9
	.long	0x5ad
	.uleb128 0x1d
	.long	.LASF78
	.byte	0x1
	.byte	0x26
	.long	0x489
	.uleb128 0x1e
	.long	0x45b
	.quad	.LBB10
	.quad	.LBE10-.LBB10
	.byte	0x1
	.byte	0x26
	.uleb128 0x1b
	.long	0x47f
	.long	.LLST4
	.uleb128 0x1b
	.long	0x476
	.long	.LLST5
	.uleb128 0x1b
	.long	0x46b
	.long	.LLST6
	.byte	0
	.byte	0
	.uleb128 0x1f
	.quad	.LVL8
	.long	0x8e4
	.uleb128 0x20
	.uleb128 0x1
	.byte	0x55
	.uleb128 0x1
	.byte	0x31
	.byte	0
	.byte	0
	.uleb128 0x18
	.long	.LASF76
	.byte	0x1
	.byte	0x34
	.quad	.LFB10
	.quad	.LFE10-.LFB10
	.uleb128 0x1
	.byte	0x9c
	.long	0x601
	.uleb128 0x19
	.long	.LASF73
	.byte	0x1
	.byte	0x34
	.long	0x490
	.long	.LLST7
	.uleb128 0x1f
	.quad	.LVL13
	.long	0x8e4
	.uleb128 0x20
	.uleb128 0x1
	.byte	0x55
	.uleb128 0x1
	.byte	0x31
	.byte	0
	.byte	0
	.uleb128 0x18
	.long	.LASF77
	.byte	0x1
	.byte	0x46
	.quad	.LFB11
	.quad	.LFE11-.LFB11
	.uleb128 0x1
	.byte	0x9c
	.long	0x67c
	.uleb128 0x19
	.long	.LASF73
	.byte	0x1
	.byte	0x46
	.long	0x490
	.long	.LLST8
	.uleb128 0x21
	.long	.LASF79
	.long	0x68c
	.uleb128 0x9
	.byte	0x3
	.quad	__PRETTY_FUNCTION__.2914
	.uleb128 0x1f
	.quad	.LVL16
	.long	0x8fa
	.uleb128 0x20
	.uleb128 0x1
	.byte	0x55
	.uleb128 0x9
	.byte	0x3
	.quad	.LC1
	.uleb128 0x20
	.uleb128 0x1
	.byte	0x54
	.uleb128 0x9
	.byte	0x3
	.quad	.LC0
	.uleb128 0x20
	.uleb128 0x1
	.byte	0x51
	.uleb128 0x2
	.byte	0x8
	.byte	0x49
	.uleb128 0x20
	.uleb128 0x1
	.byte	0x52
	.uleb128 0x9
	.byte	0x3
	.quad	__PRETTY_FUNCTION__.2914
	.byte	0
	.byte	0
	.uleb128 0xa
	.long	0x86
	.long	0x68c
	.uleb128 0xb
	.long	0x77
	.byte	0x6
	.byte	0
	.uleb128 0x7
	.long	0x67c
	.uleb128 0x18
	.long	.LASF80
	.byte	0x1
	.byte	0x5f
	.quad	.LFB13
	.quad	.LFE13-.LFB13
	.uleb128 0x1
	.byte	0x9c
	.long	0x6d8
	.uleb128 0x22
	.string	"st"
	.byte	0x1
	.byte	0x5f
	.long	0x450
	.long	.LLST9
	.uleb128 0x23
	.quad	.LVL20
	.long	0x91a
	.uleb128 0x20
	.uleb128 0x1
	.byte	0x55
	.uleb128 0x9
	.byte	0x3
	.quad	.LC3
	.byte	0
	.byte	0
	.uleb128 0x24
	.long	.LASF81
	.byte	0x1
	.byte	0x65
	.long	0x53
	.quad	.LFB14
	.quad	.LFE14-.LFB14
	.uleb128 0x1
	.byte	0x9c
	.long	0x83a
	.uleb128 0x25
	.string	"st"
	.byte	0x1
	.byte	0x67
	.long	0x450
	.long	.LLST10
	.uleb128 0x26
	.long	.Ldebug_ranges0+0
	.long	0x7dd
	.uleb128 0x27
	.string	"x"
	.byte	0x1
	.byte	0x6b
	.long	0x53
	.uleb128 0x1
	.byte	0x5d
	.uleb128 0x28
	.long	.Ldebug_ranges0+0x40
	.uleb128 0x25
	.string	"ts"
	.byte	0x1
	.byte	0x6f
	.long	0x83a
	.long	.LLST11
	.uleb128 0x1c
	.quad	.LBB14
	.quad	.LBE14-.LBB14
	.long	0x781
	.uleb128 0x25
	.string	"i"
	.byte	0x1
	.byte	0x71
	.long	0x53
	.long	.LLST12
	.uleb128 0x1f
	.quad	.LVL26
	.long	0x931
	.uleb128 0x20
	.uleb128 0x1
	.byte	0x55
	.uleb128 0x7
	.byte	0x77
	.sleb128 0
	.byte	0x73
	.sleb128 0
	.byte	0x22
	.byte	0x38
	.byte	0x1c
	.uleb128 0x20
	.uleb128 0x1
	.byte	0x54
	.uleb128 0x1
	.byte	0x30
	.uleb128 0x20
	.uleb128 0x1
	.byte	0x51
	.uleb128 0x9
	.byte	0x3
	.quad	incers
	.uleb128 0x20
	.uleb128 0x1
	.byte	0x52
	.uleb128 0x1
	.byte	0x30
	.byte	0
	.byte	0
	.uleb128 0x1c
	.quad	.LBB15
	.quad	.LBE15-.LBB15
	.long	0x7b3
	.uleb128 0x29
	.string	"i"
	.byte	0x1
	.byte	0x73
	.long	0x53
	.uleb128 0x1f
	.quad	.LVL27
	.long	0x97b
	.uleb128 0x20
	.uleb128 0x1
	.byte	0x54
	.uleb128 0x1
	.byte	0x30
	.byte	0
	.byte	0
	.uleb128 0x1f
	.quad	.LVL28
	.long	0x91a
	.uleb128 0x20
	.uleb128 0x1
	.byte	0x55
	.uleb128 0x9
	.byte	0x3
	.quad	.LC4
	.uleb128 0x20
	.uleb128 0x1
	.byte	0x54
	.uleb128 0x2
	.byte	0x7d
	.sleb128 0
	.uleb128 0x20
	.uleb128 0x1
	.byte	0x52
	.uleb128 0x3
	.byte	0xa
	.value	0x2710
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x2a
	.quad	.LVL22
	.long	0x99c
	.long	0x802
	.uleb128 0x20
	.uleb128 0x1
	.byte	0x55
	.uleb128 0x9
	.byte	0x3
	.quad	global
	.uleb128 0x20
	.uleb128 0x1
	.byte	0x54
	.uleb128 0x2
	.byte	0x7d
	.sleb128 0
	.byte	0
	.uleb128 0x2a
	.quad	.LVL31
	.long	0x691
	.long	0x826
	.uleb128 0x20
	.uleb128 0x1
	.byte	0x55
	.uleb128 0xe
	.byte	0x7f
	.sleb128 0
	.byte	0xc
	.long	0xffffffff
	.byte	0x1a
	.byte	0xa
	.value	0x9c40
	.byte	0x4e
	.byte	0x24
	.byte	0x21
	.byte	0
	.uleb128 0x1f
	.quad	.LVL32
	.long	0x9c8
	.uleb128 0x20
	.uleb128 0x1
	.byte	0x55
	.uleb128 0x1
	.byte	0x30
	.byte	0
	.byte	0
	.uleb128 0xa
	.long	0xa3
	.long	0x84b
	.uleb128 0x2b
	.long	0x77
	.uleb128 0x1
	.byte	0x39
	.byte	0
	.uleb128 0x2c
	.long	.LASF82
	.byte	0x6
	.byte	0xa8
	.long	0x3db
	.uleb128 0x2c
	.long	.LASF83
	.byte	0x6
	.byte	0xa9
	.long	0x3db
	.uleb128 0x2d
	.long	.LASF84
	.byte	0x1
	.byte	0xd
	.long	0x876
	.uleb128 0x9
	.byte	0x3
	.quad	SLEEP_INTERVAL
	.uleb128 0x7
	.long	0x53
	.uleb128 0x2d
	.long	.LASF85
	.byte	0x1
	.byte	0x2e
	.long	0x53
	.uleb128 0x9
	.byte	0x3
	.quad	protected
	.uleb128 0x2d
	.long	.LASF86
	.byte	0x1
	.byte	0x2f
	.long	0x420
	.uleb128 0x9
	.byte	0x3
	.quad	global_lock
	.uleb128 0x2d
	.long	.LASF87
	.byte	0x1
	.byte	0x31
	.long	0x876
	.uleb128 0x9
	.byte	0x3
	.quad	NUM_THREADS
	.uleb128 0x2d
	.long	.LASF88
	.byte	0x1
	.byte	0x32
	.long	0x876
	.uleb128 0x9
	.byte	0x3
	.quad	INC_COUNT
	.uleb128 0x2d
	.long	.LASF89
	.byte	0x1
	.byte	0x44
	.long	0x1c9
	.uleb128 0x9
	.byte	0x3
	.quad	global
	.uleb128 0x2e
	.long	.LASF91
	.byte	0x7
	.value	0x1bc
	.long	0x37
	.long	0x8fa
	.uleb128 0x2f
	.long	0x37
	.byte	0
	.uleb128 0x30
	.long	.LASF101
	.byte	0x9
	.byte	0x44
	.long	0x91a
	.uleb128 0x2f
	.long	0x98
	.uleb128 0x2f
	.long	0x98
	.uleb128 0x2f
	.long	0x37
	.uleb128 0x2f
	.long	0x98
	.byte	0
	.uleb128 0x2e
	.long	.LASF92
	.byte	0x6
	.value	0x16a
	.long	0x53
	.long	0x931
	.uleb128 0x2f
	.long	0x98
	.uleb128 0x31
	.byte	0
	.uleb128 0x32
	.long	.LASF93
	.byte	0x8
	.byte	0xf4
	.long	0x53
	.long	0x955
	.uleb128 0x2f
	.long	0x955
	.uleb128 0x2f
	.long	0x95b
	.uleb128 0x2f
	.long	0x966
	.uleb128 0x2f
	.long	0x7e
	.byte	0
	.uleb128 0x6
	.byte	0x8
	.long	0xa3
	.uleb128 0x6
	.byte	0x8
	.long	0x961
	.uleb128 0x7
	.long	0xe1
	.uleb128 0x6
	.byte	0x8
	.long	0x96c
	.uleb128 0x33
	.long	0x7e
	.long	0x97b
	.uleb128 0x2f
	.long	0x7e
	.byte	0
	.uleb128 0x2e
	.long	.LASF94
	.byte	0x8
	.value	0x105
	.long	0x53
	.long	0x996
	.uleb128 0x2f
	.long	0xa3
	.uleb128 0x2f
	.long	0x996
	.byte	0
	.uleb128 0x6
	.byte	0x8
	.long	0x7e
	.uleb128 0x2e
	.long	.LASF95
	.byte	0x8
	.value	0x2f0
	.long	0x53
	.long	0x9b7
	.uleb128 0x2f
	.long	0x9b7
	.uleb128 0x2f
	.long	0x9bd
	.byte	0
	.uleb128 0x6
	.byte	0x8
	.long	0x1c9
	.uleb128 0x6
	.byte	0x8
	.long	0x9c3
	.uleb128 0x7
	.long	0x203
	.uleb128 0x34
	.long	.LASF102
	.byte	0x8
	.byte	0xfd
	.uleb128 0x2f
	.long	0x7e
	.byte	0
	.byte	0
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.uleb128 0x55
	.uleb128 0x17
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x10
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0x26
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0x17
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xe
	.uleb128 0x17
	.byte	0x1
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xf
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x10
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x11
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x20
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x12
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x13
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x14
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2117
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x15
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x16
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2117
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x17
	.uleb128 0x5
	.byte	0
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x18
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2117
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x19
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x1a
	.uleb128 0x1d
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x58
	.uleb128 0xb
	.uleb128 0x59
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1b
	.uleb128 0x5
	.byte	0
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x1c
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1d
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1e
	.uleb128 0x1d
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x58
	.uleb128 0xb
	.uleb128 0x59
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x1f
	.uleb128 0x4109
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x31
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x20
	.uleb128 0x410a
	.byte	0
	.uleb128 0x2
	.uleb128 0x18
	.uleb128 0x2111
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x21
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x34
	.uleb128 0x19
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x22
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x23
	.uleb128 0x4109
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x2115
	.uleb128 0x19
	.uleb128 0x31
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x24
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2117
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x25
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x26
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x55
	.uleb128 0x17
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x27
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x28
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x55
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x29
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x2a
	.uleb128 0x4109
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x2b
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x2c
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3c
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x2d
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x2e
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3c
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x2f
	.uleb128 0x5
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x30
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x3c
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x31
	.uleb128 0x18
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x32
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3c
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x33
	.uleb128 0x15
	.byte	0x1
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x34
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x3c
	.uleb128 0x19
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_loc,"",@progbits
.Ldebug_loc0:
.LLST0:
	.quad	.LVL2
	.quad	.LVL3
	.value	0x1
	.byte	0x55
	.quad	.LVL3
	.quad	.LVL6
	.value	0x1
	.byte	0x53
	.quad	.LVL6
	.quad	.LVL7
	.value	0x4
	.byte	0xf3
	.uleb128 0x1
	.byte	0x55
	.byte	0x9f
	.quad	.LVL7
	.quad	.LFE9
	.value	0x1
	.byte	0x53
	.quad	0
	.quad	0
.LLST1:
	.quad	.LVL4
	.quad	.LVL5
	.value	0x2
	.byte	0x31
	.byte	0x9f
	.quad	0
	.quad	0
.LLST2:
	.quad	.LVL4
	.quad	.LVL5
	.value	0x2
	.byte	0x30
	.byte	0x9f
	.quad	0
	.quad	0
.LLST3:
	.quad	.LVL4
	.quad	.LVL5
	.value	0x1
	.byte	0x53
	.quad	0
	.quad	0
.LLST4:
	.quad	.LVL5
	.quad	.LVL7
	.value	0x2
	.byte	0x32
	.byte	0x9f
	.quad	0
	.quad	0
.LLST5:
	.quad	.LVL5
	.quad	.LVL7
	.value	0x2
	.byte	0x31
	.byte	0x9f
	.quad	0
	.quad	0
.LLST6:
	.quad	.LVL5
	.quad	.LVL6
	.value	0x1
	.byte	0x53
	.quad	.LVL6
	.quad	.LVL7
	.value	0x4
	.byte	0xf3
	.uleb128 0x1
	.byte	0x55
	.byte	0x9f
	.quad	0
	.quad	0
.LLST7:
	.quad	.LVL9
	.quad	.LVL10
	.value	0x1
	.byte	0x55
	.quad	.LVL10
	.quad	.LVL11
	.value	0x1
	.byte	0x53
	.quad	.LVL11
	.quad	.LVL12
	.value	0x4
	.byte	0xf3
	.uleb128 0x1
	.byte	0x55
	.byte	0x9f
	.quad	.LVL12
	.quad	.LFE10
	.value	0x1
	.byte	0x53
	.quad	0
	.quad	0
.LLST8:
	.quad	.LVL14
	.quad	.LVL15
	.value	0x1
	.byte	0x55
	.quad	.LVL15
	.quad	.LFE11
	.value	0x4
	.byte	0xf3
	.uleb128 0x1
	.byte	0x55
	.byte	0x9f
	.quad	0
	.quad	0
.LLST9:
	.quad	.LVL17
	.quad	.LVL17
	.value	0x1
	.byte	0x55
	.quad	.LVL17
	.quad	.LVL18
	.value	0x5
	.byte	0x55
	.byte	0x93
	.uleb128 0x4
	.byte	0x93
	.uleb128 0x4
	.quad	.LVL18
	.quad	.LVL19
	.value	0x6
	.byte	0x55
	.byte	0x93
	.uleb128 0x4
	.byte	0x54
	.byte	0x93
	.uleb128 0x4
	.quad	.LVL19
	.quad	.LVL20-1
	.value	0x5
	.byte	0x52
	.byte	0x93
	.uleb128 0x4
	.byte	0x93
	.uleb128 0x4
	.quad	0
	.quad	0
.LLST10:
	.quad	.LVL21
	.quad	.LVL22
	.value	0x8
	.byte	0x30
	.byte	0x9f
	.byte	0x93
	.uleb128 0x4
	.byte	0x30
	.byte	0x9f
	.byte	0x93
	.uleb128 0x4
	.quad	.LVL22
	.quad	.LVL33
	.value	0x5
	.byte	0x5f
	.byte	0x93
	.uleb128 0x4
	.byte	0x93
	.uleb128 0x4
	.quad	.LVL33
	.quad	.LFE14
	.value	0x5
	.byte	0x5f
	.byte	0x93
	.uleb128 0x4
	.byte	0x93
	.uleb128 0x4
	.quad	0
	.quad	0
.LLST11:
	.quad	.LVL24
	.quad	.LVL30
	.value	0x2
	.byte	0x77
	.sleb128 0
	.quad	.LVL30
	.quad	.LVL32
	.value	0x2
	.byte	0x7c
	.sleb128 0
	.quad	.LVL32
	.quad	.LFE14
	.value	0x2
	.byte	0x77
	.sleb128 0
	.quad	0
	.quad	0
.LLST12:
	.quad	.LVL24
	.quad	.LVL25
	.value	0x2
	.byte	0x30
	.byte	0x9f
	.quad	0
	.quad	0
	.section	.debug_aranges,"",@progbits
	.long	0x3c
	.value	0x2
	.long	.Ldebug_info0
	.byte	0x8
	.byte	0
	.value	0
	.value	0
	.quad	.Ltext0
	.quad	.Letext0-.Ltext0
	.quad	.LFB14
	.quad	.LFE14-.LFB14
	.quad	0
	.quad	0
	.section	.debug_ranges,"",@progbits
.Ldebug_ranges0:
	.quad	.LBB12
	.quad	.LBE12
	.quad	.LBB18
	.quad	.LBE18
	.quad	.LBB19
	.quad	.LBE19
	.quad	0
	.quad	0
	.quad	.LBB13
	.quad	.LBE13
	.quad	.LBB16
	.quad	.LBE16
	.quad	.LBB17
	.quad	.LBE17
	.quad	0
	.quad	0
	.quad	.Ltext0
	.quad	.Letext0
	.quad	.LFB14
	.quad	.LFE14
	.quad	0
	.quad	0
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.debug_str,"MS",@progbits,1
.LASF50:
	.string	"_flags2"
.LASF78:
	.string	"result"
.LASF84:
	.string	"SLEEP_INTERVAL"
.LASF88:
	.string	"INC_COUNT"
.LASF96:
	.string	"GNU C 4.8.5 20150623 (Red Hat 4.8.5-16) -masm=intel -mtune=generic -march=x86-64 -ggdb -O3 -std=c99"
.LASF69:
	.string	"state"
.LASF46:
	.string	"_IO_save_end"
.LASF5:
	.string	"short int"
.LASF11:
	.string	"size_t"
.LASF68:
	.string	"lock_t"
.LASF9:
	.string	"sizetype"
.LASF56:
	.string	"_offset"
.LASF19:
	.string	"__pthread_internal_list"
.LASF70:
	.string	"stats"
.LASF40:
	.string	"_IO_write_ptr"
.LASF35:
	.string	"_flags"
.LASF30:
	.string	"pthread_mutex_t"
.LASF16:
	.string	"__prev"
.LASF22:
	.string	"__count"
.LASF26:
	.string	"__spins"
.LASF55:
	.string	"_lock"
.LASF14:
	.string	"__align"
.LASF47:
	.string	"_markers"
.LASF37:
	.string	"_IO_read_end"
.LASF71:
	.string	"correct"
.LASF34:
	.string	"_IO_FILE"
.LASF95:
	.string	"pthread_mutex_init"
.LASF85:
	.string	"protected"
.LASF17:
	.string	"__next"
.LASF25:
	.string	"__kind"
.LASF33:
	.string	"long long int"
.LASF94:
	.string	"pthread_join"
.LASF93:
	.string	"pthread_create"
.LASF6:
	.string	"long int"
.LASF92:
	.string	"printf"
.LASF52:
	.string	"_cur_column"
.LASF79:
	.string	"__PRETTY_FUNCTION__"
.LASF67:
	.string	"_pos"
.LASF31:
	.string	"pthread_mutexattr_t"
.LASF86:
	.string	"global_lock"
.LASF66:
	.string	"_sbuf"
.LASF51:
	.string	"_old_offset"
.LASF72:
	.string	"total"
.LASF0:
	.string	"unsigned char"
.LASF80:
	.string	"print_stats"
.LASF97:
	.string	"mutex.c"
.LASF60:
	.string	"__pad4"
.LASF4:
	.string	"signed char"
.LASF32:
	.string	"long long unsigned int"
.LASF100:
	.string	"incers"
.LASF2:
	.string	"unsigned int"
.LASF64:
	.string	"_IO_marker"
.LASF54:
	.string	"_shortbuf"
.LASF39:
	.string	"_IO_write_base"
.LASF91:
	.string	"sleep"
.LASF36:
	.string	"_IO_read_ptr"
.LASF13:
	.string	"__size"
.LASF43:
	.string	"_IO_buf_end"
.LASF90:
	.string	"compare_and_swap"
.LASF10:
	.string	"char"
.LASF24:
	.string	"__nusers"
.LASF81:
	.string	"main"
.LASF73:
	.string	"lock"
.LASF65:
	.string	"_next"
.LASF57:
	.string	"__pad1"
.LASF58:
	.string	"__pad2"
.LASF59:
	.string	"__pad3"
.LASF74:
	.string	"_Bool"
.LASF61:
	.string	"__pad5"
.LASF21:
	.string	"__lock"
.LASF1:
	.string	"short unsigned int"
.LASF102:
	.string	"pthread_exit"
.LASF20:
	.string	"__pthread_mutex_s"
.LASF3:
	.string	"long unsigned int"
.LASF41:
	.string	"_IO_write_end"
.LASF8:
	.string	"__off64_t"
.LASF27:
	.string	"__elision"
.LASF49:
	.string	"_fileno"
.LASF48:
	.string	"_chain"
.LASF18:
	.string	"__pthread_list_t"
.LASF62:
	.string	"_mode"
.LASF29:
	.string	"__data"
.LASF7:
	.string	"__off_t"
.LASF45:
	.string	"_IO_backup_base"
.LASF82:
	.string	"stdin"
.LASF77:
	.string	"unlock"
.LASF42:
	.string	"_IO_buf_base"
.LASF87:
	.string	"NUM_THREADS"
.LASF23:
	.string	"__owner"
.LASF38:
	.string	"_IO_read_base"
.LASF28:
	.string	"__list"
.LASF53:
	.string	"_vtable_offset"
.LASF63:
	.string	"_unused2"
.LASF75:
	.string	"null"
.LASF44:
	.string	"_IO_save_base"
.LASF89:
	.string	"global"
.LASF101:
	.string	"__assert_fail"
.LASF15:
	.string	"pthread_attr_t"
.LASF12:
	.string	"pthread_t"
.LASF98:
	.string	"/data/home/jcoutin/projects/stg-push-enter/c_src/concurrent"
.LASF83:
	.string	"stdout"
.LASF99:
	.string	"_IO_lock_t"
.LASF76:
	.string	"faulty_lock"
	.ident	"GCC: (GNU) 4.8.5 20150623 (Red Hat 4.8.5-16)"
	.section	.note.GNU-stack,"",@progbits
