	.file	"mutex.c"
	.intel_syntax noprefix
	.text
.Ltext0:
	.globl	SLEEP_INTERVAL
	.section	.rodata
	.align 4
	.type	SLEEP_INTERVAL, @object
	.size	SLEEP_INTERVAL, 4
SLEEP_INTERVAL:
	.long	1
	.text
	.globl	compare_and_swap
	.type	compare_and_swap, @function
compare_and_swap:
.LFB0:
	.file 1 "mutex.c"
	.loc 1 16 0
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	mov	QWORD PTR [rbp-8], rdi
	mov	DWORD PTR [rbp-12], esi
	mov	DWORD PTR [rbp-16], edx
	.loc 1 17 0
	mov	rax, QWORD PTR [rbp-8]
	mov	edx, DWORD PTR [rbp-16]
	mov	DWORD PTR [rax], edx
	.loc 1 18 0
	mov	eax, 1
	.loc 1 19 0
	pop	rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	compare_and_swap, .-compare_and_swap
	.globl	lock
	.type	lock, @function
lock:
.LFB1:
	.loc 1 22 0
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 32
	mov	QWORD PTR [rbp-24], rdi
.L9:
	.loc 1 32 0
	mov	rax, QWORD PTR [rbp-24]
	mov	eax, DWORD PTR [rax]
	test	eax, eax
	jne	.L4
	.loc 1 34 0
	mov	rax, QWORD PTR [rbp-24]
	mov	edx, 1
	mov	esi, 0
	mov	rdi, rax
	call	compare_and_swap
	jmp	.L5
.L4:
	.loc 1 36 0
	mov	rax, QWORD PTR [rbp-24]
	mov	eax, DWORD PTR [rax]
	cmp	eax, 1
	jne	.L6
.LBB2:
	.loc 1 38 0
	mov	rax, QWORD PTR [rbp-24]
	mov	edx, 2
	mov	esi, 1
	mov	rdi, rax
	call	compare_and_swap
	mov	BYTE PTR [rbp-1], al
	.loc 1 39 0
	cmp	BYTE PTR [rbp-1], 0
	je	.L7
	.loc 1 39 0 is_stmt 0 discriminator 1
	jmp	.L10
.L7:
.LBE2:
	jmp	.L5
.L6:
	.loc 1 41 0 is_stmt 1
	mov	eax, 1
	mov	edi, eax
	call	sleep
	.loc 1 42 0
	jmp	.L9
.L5:
	jmp	.L9
.L10:
	.loc 1 44 0
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	lock, .-lock
	.globl	protected
	.bss
	.align 4
	.type	protected, @object
	.size	protected, 4
protected:
	.zero	4
	.comm	global_lock,4,4
	.globl	NUM_THREADS
	.section	.rodata
	.align 4
	.type	NUM_THREADS, @object
	.size	NUM_THREADS, 4
NUM_THREADS:
	.long	200
	.globl	INC_COUNT
	.align 4
	.type	INC_COUNT, @object
	.size	INC_COUNT, 4
INC_COUNT:
	.long	1
	.text
	.globl	faulty_lock
	.type	faulty_lock, @function
faulty_lock:
.LFB2:
	.loc 1 53 0
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 16
	mov	QWORD PTR [rbp-8], rdi
.L16:
	.loc 1 57 0
	mov	rax, QWORD PTR [rbp-8]
	mov	eax, DWORD PTR [rax]
	test	eax, eax
	jne	.L12
	.loc 1 57 0 is_stmt 0 discriminator 1
	mov	rax, QWORD PTR [rbp-8]
	mov	DWORD PTR [rax], 1
	.loc 1 64 0 is_stmt 1 discriminator 1
	jmp	.L16
.L12:
	.loc 1 58 0
	mov	rax, QWORD PTR [rbp-8]
	mov	eax, DWORD PTR [rax]
	cmp	eax, 1
	jne	.L14
	.loc 1 60 0
	mov	rax, QWORD PTR [rbp-8]
	mov	DWORD PTR [rax], 2
	.loc 1 61 0
	jmp	.L17
.L14:
	.loc 1 63 0
	mov	eax, 1
	mov	edi, eax
	call	sleep
	.loc 1 64 0
	jmp	.L16
.L17:
	.loc 1 66 0
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	faulty_lock, .-faulty_lock
	.comm	global,40,32
	.section	.rodata
.LC0:
	.string	"mutex.c"
.LC1:
	.string	"0"
	.text
	.globl	unlock
	.type	unlock, @function
unlock:
.LFB3:
	.loc 1 71 0
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 16
	mov	QWORD PTR [rbp-8], rdi
	.loc 1 72 0
	mov	rax, QWORD PTR [rbp-8]
	mov	eax, DWORD PTR [rax]
	cmp	eax, 2
	jne	.L19
	.loc 1 72 0 is_stmt 0 discriminator 1
	mov	rax, QWORD PTR [rbp-8]
	mov	DWORD PTR [rax], 0
	jmp	.L21
.L19:
	.loc 1 73 0 is_stmt 1
	mov	ecx, OFFSET FLAT:__PRETTY_FUNCTION__.2888
	mov	edx, 73
	mov	esi, OFFSET FLAT:.LC0
	mov	edi, OFFSET FLAT:.LC1
	call	__assert_fail
.L21:
	.loc 1 74 0
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	unlock, .-unlock
	.globl	inc
	.type	inc, @function
inc:
.LFB4:
	.loc 1 77 0
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	mov	QWORD PTR [rbp-8], rdi
	.loc 1 78 0
	mov	eax, DWORD PTR protected[rip]
	add	eax, 1
	mov	DWORD PTR protected[rip], eax
	.loc 1 79 0
	pop	rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE4:
	.size	inc, .-inc
	.globl	incers
	.type	incers, @function
incers:
.LFB5:
	.loc 1 82 0
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	mov	QWORD PTR [rbp-24], rdi
.LBB3:
	.loc 1 83 0
	mov	DWORD PTR [rbp-4], 0
	jmp	.L24
.L25:
	.loc 1 87 0 discriminator 2
	mov	eax, DWORD PTR protected[rip]
	add	eax, 1
	mov	DWORD PTR protected[rip], eax
	.loc 1 83 0 discriminator 2
	add	DWORD PTR [rbp-4], 1
.L24:
	.loc 1 83 0 is_stmt 0 discriminator 1
	mov	eax, 1
	cmp	DWORD PTR [rbp-4], eax
	jl	.L25
.LBE3:
	.loc 1 92 0 is_stmt 1
	pop	rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	incers, .-incers
	.section	.rodata
	.align 8
.LC3:
	.string	"Total: %d, Failed: %d, Correct: %d, Percent Success: %f\n"
	.text
	.globl	print_stats
	.type	print_stats, @function
print_stats:
.LFB6:
	.loc 1 101 0
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 16
	mov	QWORD PTR [rbp-16], rdi
	.loc 1 102 0
	mov	eax, DWORD PTR [rbp-16]
	cvtsi2sd	xmm0, eax
	movsd	xmm1, QWORD PTR .LC2[rip]
	mulsd	xmm0, xmm1
	mov	eax, DWORD PTR [rbp-12]
	cvtsi2ss	xmm1, eax
	unpcklps	xmm1, xmm1
	cvtps2pd	xmm1, xmm1
	divsd	xmm0, xmm1
	mov	edx, DWORD PTR [rbp-16]
	mov	ecx, DWORD PTR [rbp-12]
	mov	eax, DWORD PTR [rbp-16]
	sub	ecx, eax
	mov	esi, ecx
	mov	eax, DWORD PTR [rbp-12]
	mov	ecx, edx
	mov	edx, esi
	mov	esi, eax
	mov	edi, OFFSET FLAT:.LC3
	mov	eax, 1
	call	printf
	.loc 1 104 0
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	print_stats, .-print_stats
	.section	.rodata
	.align 8
.LC4:
	.string	"Iteration: %d, Check Failed!: protected (%d) != NUM_THREADS*INC_COUNT (%d)\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB7:
	.loc 1 107 0
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	push	r15
	push	r14
	push	r13
	push	r12
	push	rbx
	sub	rsp, 56
	.cfi_offset 15, -24
	.cfi_offset 14, -32
	.cfi_offset 13, -40
	.cfi_offset 12, -48
	.cfi_offset 3, -56
	.loc 1 108 0
	mov	DWORD PTR [rbp-96], 0
	mov	DWORD PTR [rbp-92], 0
	.loc 1 110 0
	mov	esi, 0
	mov	edi, OFFSET FLAT:global
	call	pthread_mutex_init
.LBB4:
	.loc 1 112 0
	mov	DWORD PTR [rbp-52], 0
	jmp	.L28
.L35:
.LBB5:
	mov	rax, rsp
	mov	rbx, rax
	.loc 1 113 0
	mov	DWORD PTR global_lock[rip], 0
	.loc 1 114 0
	mov	DWORD PTR protected[rip], 0
	.loc 1 116 0
	mov	eax, 200
	cdqe
	sub	rax, 1
	mov	QWORD PTR [rbp-72], rax
	mov	eax, 200
	cdqe
	mov	r12, rax
	mov	r13d, 0
	mov	eax, 200
	cdqe
	mov	r14, rax
	mov	r15d, 0
	mov	eax, 200
	cdqe
	sal	rax, 3
	lea	rdx, [rax+7]
	mov	eax, 16
	sub	rax, 1
	add	rax, rdx
	mov	esi, 16
	mov	edx, 0
	div	rsi
	imul	rax, rax, 16
	sub	rsp, rax
	mov	rax, rsp
	add	rax, 7
	shr	rax, 3
	sal	rax, 3
	mov	QWORD PTR [rbp-80], rax
.LBB6:
	.loc 1 118 0
	mov	DWORD PTR [rbp-56], 0
	jmp	.L29
.L30:
	.loc 1 118 0 is_stmt 0 discriminator 2
	mov	rax, QWORD PTR [rbp-80]
	mov	edx, DWORD PTR [rbp-56]
	movsx	rdx, edx
	sal	rdx, 3
	add	rax, rdx
	mov	ecx, 0
	mov	edx, OFFSET FLAT:inc
	mov	esi, 0
	mov	rdi, rax
	call	pthread_create
	add	DWORD PTR [rbp-56], 1
.L29:
	.loc 1 118 0 discriminator 1
	mov	eax, 200
	cmp	DWORD PTR [rbp-56], eax
	jl	.L30
.LBE6:
.LBB7:
	.loc 1 120 0 is_stmt 1
	mov	DWORD PTR [rbp-60], 0
	jmp	.L31
.L32:
	.loc 1 120 0 is_stmt 0 discriminator 2
	mov	rax, QWORD PTR [rbp-80]
	mov	edx, DWORD PTR [rbp-60]
	movsx	rdx, edx
	mov	rax, QWORD PTR [rax+rdx*8]
	mov	esi, 0
	mov	rdi, rax
	call	pthread_join
	add	DWORD PTR [rbp-60], 1
.L31:
	.loc 1 120 0 discriminator 1
	mov	eax, 200
	cmp	DWORD PTR [rbp-60], eax
	jl	.L32
.LBE7:
	.loc 1 122 0 is_stmt 1
	mov	edx, 200
	mov	eax, 1
	imul	edx, eax
	mov	eax, DWORD PTR protected[rip]
	cmp	edx, eax
	je	.L33
	.loc 1 123 0
	mov	edx, 200
	mov	eax, 1
	mov	ecx, edx
	imul	ecx, eax
	mov	edx, DWORD PTR protected[rip]
	mov	eax, DWORD PTR [rbp-52]
	mov	esi, eax
	mov	edi, OFFSET FLAT:.LC4
	mov	eax, 0
	call	printf
	jmp	.L34
.L33:
	.loc 1 125 0
	mov	eax, DWORD PTR [rbp-96]
	add	eax, 1
	mov	DWORD PTR [rbp-96], eax
.L34:
	.loc 1 126 0
	mov	eax, DWORD PTR [rbp-92]
	add	eax, 1
	mov	DWORD PTR [rbp-92], eax
	mov	rsp, rbx
.LBE5:
	.loc 1 112 0
	add	DWORD PTR [rbp-52], 1
.L28:
	.loc 1 112 0 is_stmt 0 discriminator 1
	cmp	DWORD PTR [rbp-52], 9999
	jle	.L35
.LBE4:
	.loc 1 129 0 is_stmt 1
	mov	rax, QWORD PTR [rbp-96]
	mov	rdi, rax
	call	print_stats
	.loc 1 131 0
	mov	edi, 0
	call	pthread_exit
	.cfi_endproc
.LFE7:
	.size	main, .-main
	.section	.rodata
	.type	__PRETTY_FUNCTION__.2888, @object
	.size	__PRETTY_FUNCTION__.2888, 7
__PRETTY_FUNCTION__.2888:
	.string	"unlock"
	.align 8
.LC2:
	.long	0
	.long	1079574528
	.text
.Letext0:
	.file 2 "/usr/include/bits/pthreadtypes.h"
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.long	0x4da
	.value	0x4
	.long	.Ldebug_abbrev0
	.byte	0x8
	.uleb128 0x1
	.long	.LASF50
	.byte	0x1
	.long	.LASF51
	.long	.LASF52
	.quad	.Ltext0
	.quad	.Letext0-.Ltext0
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
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.long	.LASF7
	.uleb128 0x4
	.byte	0x8
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF8
	.uleb128 0x5
	.long	.LASF11
	.byte	0x2
	.byte	0x3c
	.long	0x42
	.uleb128 0x6
	.long	.LASF13
	.byte	0x10
	.byte	0x2
	.byte	0x4b
	.long	0xa5
	.uleb128 0x7
	.long	.LASF9
	.byte	0x2
	.byte	0x4d
	.long	0xa5
	.byte	0
	.uleb128 0x7
	.long	.LASF10
	.byte	0x2
	.byte	0x4e
	.long	0xa5
	.byte	0x8
	.byte	0
	.uleb128 0x8
	.byte	0x8
	.long	0x80
	.uleb128 0x5
	.long	.LASF12
	.byte	0x2
	.byte	0x4f
	.long	0x80
	.uleb128 0x6
	.long	.LASF14
	.byte	0x28
	.byte	0x2
	.byte	0x5c
	.long	0x123
	.uleb128 0x7
	.long	.LASF15
	.byte	0x2
	.byte	0x5e
	.long	0x57
	.byte	0
	.uleb128 0x7
	.long	.LASF16
	.byte	0x2
	.byte	0x5f
	.long	0x3b
	.byte	0x4
	.uleb128 0x7
	.long	.LASF17
	.byte	0x2
	.byte	0x60
	.long	0x57
	.byte	0x8
	.uleb128 0x7
	.long	.LASF18
	.byte	0x2
	.byte	0x62
	.long	0x3b
	.byte	0xc
	.uleb128 0x7
	.long	.LASF19
	.byte	0x2
	.byte	0x66
	.long	0x57
	.byte	0x10
	.uleb128 0x7
	.long	.LASF20
	.byte	0x2
	.byte	0x68
	.long	0x50
	.byte	0x14
	.uleb128 0x7
	.long	.LASF21
	.byte	0x2
	.byte	0x69
	.long	0x50
	.byte	0x16
	.uleb128 0x7
	.long	.LASF22
	.byte	0x2
	.byte	0x6a
	.long	0xab
	.byte	0x18
	.byte	0
	.uleb128 0x9
	.byte	0x28
	.byte	0x2
	.byte	0x5a
	.long	0x14d
	.uleb128 0xa
	.long	.LASF23
	.byte	0x2
	.byte	0x7c
	.long	0xb6
	.uleb128 0xa
	.long	.LASF24
	.byte	0x2
	.byte	0x7d
	.long	0x14d
	.uleb128 0xa
	.long	.LASF25
	.byte	0x2
	.byte	0x7e
	.long	0x5e
	.byte	0
	.uleb128 0xb
	.long	0x6e
	.long	0x15d
	.uleb128 0xc
	.long	0x65
	.byte	0x27
	.byte	0
	.uleb128 0x5
	.long	.LASF26
	.byte	0x2
	.byte	0x7f
	.long	0x123
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.long	.LASF27
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.long	.LASF28
	.uleb128 0x6
	.long	.LASF29
	.byte	0x4
	.byte	0x1
	.byte	0x9
	.long	0x18f
	.uleb128 0x7
	.long	.LASF30
	.byte	0x1
	.byte	0xa
	.long	0x57
	.byte	0
	.byte	0
	.uleb128 0x5
	.long	.LASF29
	.byte	0x1
	.byte	0xb
	.long	0x176
	.uleb128 0x6
	.long	.LASF31
	.byte	0x8
	.byte	0x1
	.byte	0x5f
	.long	0x1bf
	.uleb128 0x7
	.long	.LASF32
	.byte	0x1
	.byte	0x60
	.long	0x57
	.byte	0
	.uleb128 0x7
	.long	.LASF33
	.byte	0x1
	.byte	0x61
	.long	0x57
	.byte	0x4
	.byte	0
	.uleb128 0x5
	.long	.LASF31
	.byte	0x1
	.byte	0x62
	.long	0x19a
	.uleb128 0xd
	.long	.LASF38
	.byte	0x1
	.byte	0xf
	.long	0x212
	.quad	.LFB0
	.quad	.LFE0-.LFB0
	.uleb128 0x1
	.byte	0x9c
	.long	0x212
	.uleb128 0xe
	.long	.LASF34
	.byte	0x1
	.byte	0xf
	.long	0x219
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0xf
	.string	"a"
	.byte	0x1
	.byte	0xf
	.long	0x57
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0xf
	.string	"b"
	.byte	0x1
	.byte	0xf
	.long	0x57
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.byte	0
	.uleb128 0x2
	.byte	0x1
	.byte	0x2
	.long	.LASF35
	.uleb128 0x8
	.byte	0x8
	.long	0x18f
	.uleb128 0x10
	.long	.LASF34
	.byte	0x1
	.byte	0x15
	.quad	.LFB1
	.quad	.LFE1-.LFB1
	.uleb128 0x1
	.byte	0x9c
	.long	0x26b
	.uleb128 0xe
	.long	.LASF34
	.byte	0x1
	.byte	0x15
	.long	0x219
	.uleb128 0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0x11
	.quad	.LBB2
	.quad	.LBE2-.LBB2
	.uleb128 0x12
	.long	.LASF41
	.byte	0x1
	.byte	0x26
	.long	0x212
	.uleb128 0x2
	.byte	0x91
	.sleb128 -17
	.byte	0
	.byte	0
	.uleb128 0x10
	.long	.LASF36
	.byte	0x1
	.byte	0x34
	.quad	.LFB2
	.quad	.LFE2-.LFB2
	.uleb128 0x1
	.byte	0x9c
	.long	0x297
	.uleb128 0xe
	.long	.LASF34
	.byte	0x1
	.byte	0x34
	.long	0x219
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.byte	0
	.uleb128 0x10
	.long	.LASF37
	.byte	0x1
	.byte	0x46
	.quad	.LFB3
	.quad	.LFE3-.LFB3
	.uleb128 0x1
	.byte	0x9c
	.long	0x2d6
	.uleb128 0xe
	.long	.LASF34
	.byte	0x1
	.byte	0x46
	.long	0x219
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x13
	.long	.LASF53
	.long	0x2e6
	.uleb128 0x9
	.byte	0x3
	.quad	__PRETTY_FUNCTION__.2888
	.byte	0
	.uleb128 0xb
	.long	0x6e
	.long	0x2e6
	.uleb128 0xc
	.long	0x65
	.byte	0x6
	.byte	0
	.uleb128 0x14
	.long	0x2d6
	.uleb128 0x15
	.string	"inc"
	.byte	0x1
	.byte	0x4c
	.long	0x6c
	.quad	.LFB4
	.quad	.LFE4-.LFB4
	.uleb128 0x1
	.byte	0x9c
	.long	0x31b
	.uleb128 0xe
	.long	.LASF39
	.byte	0x1
	.byte	0x4c
	.long	0x6c
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.byte	0
	.uleb128 0xd
	.long	.LASF40
	.byte	0x1
	.byte	0x51
	.long	0x6c
	.quad	.LFB5
	.quad	.LFE5-.LFB5
	.uleb128 0x1
	.byte	0x9c
	.long	0x369
	.uleb128 0xe
	.long	.LASF39
	.byte	0x1
	.byte	0x51
	.long	0x6c
	.uleb128 0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0x11
	.quad	.LBB3
	.quad	.LBE3-.LBB3
	.uleb128 0x16
	.string	"i"
	.byte	0x1
	.byte	0x53
	.long	0x57
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.byte	0
	.byte	0
	.uleb128 0x10
	.long	.LASF42
	.byte	0x1
	.byte	0x64
	.quad	.LFB6
	.quad	.LFE6-.LFB6
	.uleb128 0x1
	.byte	0x9c
	.long	0x394
	.uleb128 0xf
	.string	"st"
	.byte	0x1
	.byte	0x64
	.long	0x1bf
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.byte	0
	.uleb128 0x17
	.long	.LASF43
	.byte	0x1
	.byte	0x6a
	.long	0x57
	.quad	.LFB7
	.quad	.LFE7-.LFB7
	.uleb128 0x1
	.byte	0x9c
	.long	0x446
	.uleb128 0x16
	.string	"st"
	.byte	0x1
	.byte	0x6c
	.long	0x1bf
	.uleb128 0x3
	.byte	0x91
	.sleb128 -112
	.uleb128 0x11
	.quad	.LBB4
	.quad	.LBE4-.LBB4
	.uleb128 0x16
	.string	"x"
	.byte	0x1
	.byte	0x70
	.long	0x57
	.uleb128 0x3
	.byte	0x91
	.sleb128 -68
	.uleb128 0x11
	.quad	.LBB5
	.quad	.LBE5-.LBB5
	.uleb128 0x16
	.string	"ts"
	.byte	0x1
	.byte	0x74
	.long	0x446
	.uleb128 0x4
	.byte	0x91
	.sleb128 -96
	.byte	0x6
	.uleb128 0x18
	.quad	.LBB6
	.quad	.LBE6-.LBB6
	.long	0x424
	.uleb128 0x16
	.string	"i"
	.byte	0x1
	.byte	0x76
	.long	0x57
	.uleb128 0x3
	.byte	0x91
	.sleb128 -72
	.byte	0
	.uleb128 0x11
	.quad	.LBB7
	.quad	.LBE7-.LBB7
	.uleb128 0x16
	.string	"i"
	.byte	0x1
	.byte	0x78
	.long	0x57
	.uleb128 0x3
	.byte	0x91
	.sleb128 -76
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0xb
	.long	0x75
	.long	0x45a
	.uleb128 0x19
	.long	0x65
	.uleb128 0x4
	.byte	0x91
	.sleb128 -88
	.byte	0x6
	.byte	0
	.uleb128 0x1a
	.long	.LASF44
	.byte	0x1
	.byte	0xd
	.long	0x46f
	.uleb128 0x9
	.byte	0x3
	.quad	SLEEP_INTERVAL
	.uleb128 0x14
	.long	0x57
	.uleb128 0x1a
	.long	.LASF45
	.byte	0x1
	.byte	0x2e
	.long	0x57
	.uleb128 0x9
	.byte	0x3
	.quad	protected
	.uleb128 0x1a
	.long	.LASF46
	.byte	0x1
	.byte	0x2f
	.long	0x18f
	.uleb128 0x9
	.byte	0x3
	.quad	global_lock
	.uleb128 0x1a
	.long	.LASF47
	.byte	0x1
	.byte	0x31
	.long	0x46f
	.uleb128 0x9
	.byte	0x3
	.quad	NUM_THREADS
	.uleb128 0x1a
	.long	.LASF48
	.byte	0x1
	.byte	0x32
	.long	0x46f
	.uleb128 0x9
	.byte	0x3
	.quad	INC_COUNT
	.uleb128 0x1a
	.long	.LASF49
	.byte	0x1
	.byte	0x44
	.long	0x15d
	.uleb128 0x9
	.byte	0x3
	.quad	global
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
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
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
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x5
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
	.uleb128 0x6
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
	.uleb128 0x7
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
	.uleb128 0x8
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x9
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
	.uleb128 0xa
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
	.uleb128 0xb
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xd
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
	.uleb128 0xe
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
	.uleb128 0xf
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
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x10
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
	.uleb128 0x2116
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x11
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.byte	0
	.byte	0
	.uleb128 0x12
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
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x13
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
	.uleb128 0x14
	.uleb128 0x26
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x15
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0x8
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
	.uleb128 0x16
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
	.uleb128 0x17
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
	.uleb128 0x2116
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x18
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
	.uleb128 0x19
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x1a
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
	.byte	0
	.section	.debug_aranges,"",@progbits
	.long	0x2c
	.value	0x2
	.long	.Ldebug_info0
	.byte	0x8
	.byte	0
	.value	0
	.value	0
	.quad	.Ltext0
	.quad	.Letext0-.Ltext0
	.quad	0
	.quad	0
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.debug_str,"MS",@progbits,1
.LASF45:
	.string	"protected"
.LASF26:
	.string	"pthread_mutex_t"
.LASF30:
	.string	"state"
.LASF23:
	.string	"__data"
.LASF1:
	.string	"short unsigned int"
.LASF32:
	.string	"correct"
.LASF31:
	.string	"stats"
.LASF39:
	.string	"null"
.LASF49:
	.string	"global"
.LASF53:
	.string	"__PRETTY_FUNCTION__"
.LASF40:
	.string	"incers"
.LASF17:
	.string	"__owner"
.LASF19:
	.string	"__kind"
.LASF47:
	.string	"NUM_THREADS"
.LASF15:
	.string	"__lock"
.LASF27:
	.string	"long long unsigned int"
.LASF11:
	.string	"pthread_t"
.LASF0:
	.string	"unsigned char"
.LASF24:
	.string	"__size"
.LASF14:
	.string	"__pthread_mutex_s"
.LASF3:
	.string	"long unsigned int"
.LASF37:
	.string	"unlock"
.LASF42:
	.string	"print_stats"
.LASF46:
	.string	"global_lock"
.LASF36:
	.string	"faulty_lock"
.LASF29:
	.string	"lock_t"
.LASF13:
	.string	"__pthread_internal_list"
.LASF21:
	.string	"__elision"
.LASF43:
	.string	"main"
.LASF2:
	.string	"unsigned int"
.LASF44:
	.string	"SLEEP_INTERVAL"
.LASF20:
	.string	"__spins"
.LASF41:
	.string	"result"
.LASF50:
	.string	"GNU C 4.8.5 20150623 (Red Hat 4.8.5-16) -masm=intel -mtune=generic -march=x86-64 -ggdb -std=c99"
.LASF51:
	.string	"mutex.c"
.LASF38:
	.string	"compare_and_swap"
.LASF48:
	.string	"INC_COUNT"
.LASF7:
	.string	"sizetype"
.LASF28:
	.string	"long long int"
.LASF52:
	.string	"/data/home/jcoutin/projects/stg-push-enter/c_src/concurrent"
.LASF8:
	.string	"char"
.LASF25:
	.string	"__align"
.LASF18:
	.string	"__nusers"
.LASF16:
	.string	"__count"
.LASF5:
	.string	"short int"
.LASF9:
	.string	"__prev"
.LASF12:
	.string	"__pthread_list_t"
.LASF33:
	.string	"total"
.LASF6:
	.string	"long int"
.LASF22:
	.string	"__list"
.LASF10:
	.string	"__next"
.LASF4:
	.string	"signed char"
.LASF34:
	.string	"lock"
.LASF35:
	.string	"_Bool"
	.ident	"GCC: (GNU) 4.8.5 20150623 (Red Hat 4.8.5-16)"
	.section	.note.GNU-stack,"",@progbits
