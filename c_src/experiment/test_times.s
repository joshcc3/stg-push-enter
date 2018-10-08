	.file	"test_times.c"
	.intel_syntax noprefix
	.comm	cas_lock,40,32
	.globl	LOCK_SLEEP_INTERVAL
	.data
	.align 16
	.type	LOCK_SLEEP_INTERVAL, @object
	.size	LOCK_SLEEP_INTERVAL, 16
LOCK_SLEEP_INTERVAL:
	.quad	0
	.quad	1000
	.text
	.globl	compare_and_swap
	.type	compare_and_swap, @function
compare_and_swap:
.LFB0:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 16
	mov	QWORD PTR [rbp-8], rdi
	mov	DWORD PTR [rbp-12], esi
	mov	DWORD PTR [rbp-16], edx
	mov	edi, OFFSET FLAT:cas_lock
	call	pthread_mutex_lock
	mov	rax, QWORD PTR [rbp-8]
	mov	eax, DWORD PTR [rax]
	cmp	eax, DWORD PTR [rbp-12]
	je	.L2
	mov	edi, OFFSET FLAT:cas_lock
	call	pthread_mutex_unlock
	mov	eax, 0
	jmp	.L3
.L2:
	mov	rax, QWORD PTR [rbp-8]
	mov	edx, DWORD PTR [rbp-16]
	mov	DWORD PTR [rax], edx
	mov	edi, OFFSET FLAT:cas_lock
	call	pthread_mutex_unlock
	mov	eax, 1
.L3:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	compare_and_swap, .-compare_and_swap
	.globl	lock_init
	.type	lock_init, @function
lock_init:
.LFB1:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	mov	QWORD PTR [rbp-8], rdi
	mov	rax, QWORD PTR [rbp-8]
	mov	DWORD PTR [rax], 0
	pop	rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	lock_init, .-lock_init
	.globl	lock
	.type	lock, @function
lock:
.LFB2:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 32
	mov	QWORD PTR [rbp-24], rdi
	mov	edi, OFFSET FLAT:cas_lock
	call	pthread_mutex_lock
	mov	rax, QWORD PTR [rbp-24]
	mov	eax, DWORD PTR [rax+4]
	lea	edx, [rax+1]
	mov	rax, QWORD PTR [rbp-24]
	mov	DWORD PTR [rax+4], edx
	mov	edi, OFFSET FLAT:cas_lock
	call	pthread_mutex_unlock
.L11:
	mov	rax, QWORD PTR [rbp-24]
	mov	eax, DWORD PTR [rax]
	test	eax, eax
	jne	.L6
	mov	rax, QWORD PTR [rbp-24]
	mov	edx, 1
	mov	esi, 0
	mov	rdi, rax
	call	compare_and_swap
	jmp	.L7
.L6:
	mov	rax, QWORD PTR [rbp-24]
	mov	eax, DWORD PTR [rax]
	cmp	eax, 1
	jne	.L8
	mov	rax, QWORD PTR [rbp-24]
	mov	edx, 2
	mov	esi, 1
	mov	rdi, rax
	call	compare_and_swap
	mov	BYTE PTR [rbp-1], al
	cmp	BYTE PTR [rbp-1], 0
	je	.L9
	jmp	.L12
.L9:
	jmp	.L7
.L8:
	mov	edi, OFFSET FLAT:cas_lock
	call	pthread_mutex_lock
	mov	rax, QWORD PTR [rbp-24]
	mov	eax, DWORD PTR [rax+8]
	lea	edx, [rax+1]
	mov	rax, QWORD PTR [rbp-24]
	mov	DWORD PTR [rax+8], edx
	mov	edi, OFFSET FLAT:cas_lock
	call	pthread_mutex_unlock
	mov	esi, 0
	mov	edi, OFFSET FLAT:LOCK_SLEEP_INTERVAL
	mov	eax, 0
	call	nanosleep
	jmp	.L11
.L7:
	jmp	.L11
.L12:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	lock, .-lock
	.section	.rodata
.LC0:
	.string	"test_times.c"
.LC1:
	.string	"0"
	.text
	.globl	unlock
	.type	unlock, @function
unlock:
.LFB3:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 16
	mov	QWORD PTR [rbp-8], rdi
	mov	rax, QWORD PTR [rbp-8]
	mov	eax, DWORD PTR [rax]
	cmp	eax, 2
	jne	.L14
	mov	rax, QWORD PTR [rbp-8]
	mov	DWORD PTR [rax], 0
	jmp	.L16
.L14:
	mov	ecx, OFFSET FLAT:__PRETTY_FUNCTION__.2897
	mov	edx, 86
	mov	esi, OFFSET FLAT:.LC0
	mov	edi, OFFSET FLAT:.LC1
	call	__assert_fail
.L16:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	unlock, .-unlock
	.section	.rodata
	.align 8
.LC3:
	.string	"Lock Attempts: %d, Number of times Contended: %d, Lock Contention%: %f\n"
	.text
	.globl	pprint_lock_stats
	.type	pprint_lock_stats, @function
pprint_lock_stats:
.LFB4:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 16
	mov	QWORD PTR [rbp-16], rdi
	mov	eax, DWORD PTR [rbp-12]
	cvtsi2sd	xmm0, eax
	mov	eax, DWORD PTR [rbp-16]
	cvtsi2sd	xmm1, eax
	divsd	xmm0, xmm1
	movsd	xmm1, QWORD PTR .LC2[rip]
	mulsd	xmm0, xmm1
	mov	edx, DWORD PTR [rbp-12]
	mov	eax, DWORD PTR [rbp-16]
	mov	esi, eax
	mov	edi, OFFSET FLAT:.LC3
	mov	eax, 1
	call	printf
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE4:
	.size	pprint_lock_stats, .-pprint_lock_stats
	.section	.rodata
	.align 8
.LC6:
	.string	"RAW: Clock Ticks - User time: %li, System Time: %li\n"
	.align 8
.LC7:
	.string	"User time: %f ms, System Time: %f ms\n"
	.text
	.globl	pprint_tms
	.type	pprint_tms, @function
pprint_tms:
.LFB5:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 48
	mov	QWORD PTR [rbp-40], rdi
	mov	edi, 2
	call	sysconf
	cvtsi2sd	xmm0, rax
	movsd	xmm1, QWORD PTR .LC4[rip]
	divsd	xmm0, xmm1
	movsd	QWORD PTR [rbp-8], xmm0
	mov	rax, QWORD PTR [rbp-40]
	mov	rax, QWORD PTR [rax]
	cvtsi2sd	xmm0, rax
	divsd	xmm0, QWORD PTR [rbp-8]
	movsd	xmm1, QWORD PTR .LC5[rip]
	divsd	xmm0, xmm1
	movsd	QWORD PTR [rbp-16], xmm0
	mov	rax, QWORD PTR [rbp-40]
	mov	rax, QWORD PTR [rax+8]
	cvtsi2sd	xmm0, rax
	divsd	xmm0, QWORD PTR [rbp-8]
	movsd	xmm1, QWORD PTR .LC5[rip]
	divsd	xmm0, xmm1
	movsd	QWORD PTR [rbp-24], xmm0
	mov	rax, QWORD PTR [rbp-40]
	mov	rdx, QWORD PTR [rax+8]
	mov	rax, QWORD PTR [rbp-40]
	mov	rax, QWORD PTR [rax]
	mov	rsi, rax
	mov	edi, OFFSET FLAT:.LC6
	mov	eax, 0
	call	printf
	mov	rdx, QWORD PTR [rbp-24]
	mov	rax, QWORD PTR [rbp-16]
	mov	QWORD PTR [rbp-48], rdx
	movsd	xmm1, QWORD PTR [rbp-48]
	mov	QWORD PTR [rbp-48], rax
	movsd	xmm0, QWORD PTR [rbp-48]
	mov	edi, OFFSET FLAT:.LC7
	mov	eax, 2
	call	printf
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	pprint_tms, .-pprint_tms
	.globl	main_1
	.type	main_1, @function
main_1:
.LFB6:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 48
	mov	QWORD PTR [rbp-16], 1
	mov	QWORD PTR [rbp-8], 0
	lea	rax, [rbp-16]
	mov	esi, 0
	mov	rdi, rax
	mov	eax, 0
	call	nanosleep
	lea	rax, [rbp-48]
	mov	rdi, rax
	call	times
	lea	rax, [rbp-48]
	mov	rdi, rax
	call	pprint_tms
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	main_1, .-main_1
	.comm	global,12,4
	.globl	large_sum
	.bss
	.align 8
	.type	large_sum, @object
	.size	large_sum, 8
large_sum:
	.zero	8
	.section	.rodata
.LC8:
	.string	"CHILD: Progressed to %d\n"
	.text
	.globl	child_fun
	.type	child_fun, @function
child_fun:
.LFB7:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 32
	mov	QWORD PTR [rbp-24], rdi
	mov	DWORD PTR [rbp-4], 0
	jmp	.L21
.L23:
	mov	edi, OFFSET FLAT:global
	call	lock
	mov	eax, DWORD PTR [rbp-4]
	movsx	rdx, eax
	mov	rax, QWORD PTR large_sum[rip]
	add	rax, rdx
	mov	QWORD PTR large_sum[rip], rax
	mov	edi, OFFSET FLAT:global
	call	unlock
	mov	ecx, DWORD PTR [rbp-4]
	mov	edx, 1759218605
	mov	eax, ecx
	imul	edx
	sar	edx, 12
	mov	eax, ecx
	sar	eax, 31
	sub	edx, eax
	mov	eax, edx
	imul	eax, eax, 10000
	sub	ecx, eax
	mov	eax, ecx
	test	eax, eax
	jne	.L22
	mov	eax, DWORD PTR [rbp-4]
	mov	esi, eax
	mov	edi, OFFSET FLAT:.LC8
	mov	eax, 0
	call	printf
.L22:
	sub	DWORD PTR [rbp-4], 1
.L21:
	cmp	DWORD PTR [rbp-4], -999999
	jge	.L23
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	child_fun, .-child_fun
	.section	.rodata
.LC9:
	.string	"MAIN: Progressed to %d\n"
.LC10:
	.string	"Sum result (0): %d\n"
.LC11:
	.string	"Clock Ticks: %li \n"
	.text
	.globl	main_test_lock_cont
	.type	main_test_lock_cont, @function
main_test_lock_cont:
.LFB8:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 64
	mov	edi, OFFSET FLAT:global
	call	lock_init
	lea	rax, [rbp-56]
	mov	ecx, 0
	mov	edx, OFFSET FLAT:child_fun
	mov	esi, 0
	mov	rdi, rax
	call	pthread_create
	mov	DWORD PTR [rbp-4], 0
	jmp	.L25
.L27:
	mov	edi, OFFSET FLAT:global
	call	lock
	mov	eax, DWORD PTR [rbp-4]
	movsx	rdx, eax
	mov	rax, QWORD PTR large_sum[rip]
	add	rax, rdx
	mov	QWORD PTR large_sum[rip], rax
	mov	edi, OFFSET FLAT:global
	call	unlock
	mov	ecx, DWORD PTR [rbp-4]
	mov	edx, 1759218605
	mov	eax, ecx
	imul	edx
	sar	edx, 12
	mov	eax, ecx
	sar	eax, 31
	sub	edx, eax
	mov	eax, edx
	imul	eax, eax, 10000
	sub	ecx, eax
	mov	eax, ecx
	test	eax, eax
	jne	.L26
	mov	eax, DWORD PTR [rbp-4]
	mov	esi, eax
	mov	edi, OFFSET FLAT:.LC9
	mov	eax, 0
	call	printf
.L26:
	add	DWORD PTR [rbp-4], 1
.L25:
	cmp	DWORD PTR [rbp-4], 999999
	jle	.L27
	mov	rax, QWORD PTR [rbp-56]
	mov	esi, 0
	mov	rdi, rax
	call	pthread_join
	mov	rax, QWORD PTR large_sum[rip]
	mov	rsi, rax
	mov	edi, OFFSET FLAT:.LC10
	mov	eax, 0
	call	printf
	mov	rdi, QWORD PTR global[rip+4]
	call	pprint_lock_stats
	lea	rax, [rbp-48]
	mov	rdi, rax
	call	times
	mov	rsi, rax
	mov	edi, OFFSET FLAT:.LC11
	mov	eax, 0
	call	printf
	lea	rax, [rbp-48]
	mov	rdi, rax
	call	pprint_tms
	mov	edi, 0
	call	pthread_exit
	.cfi_endproc
.LFE8:
	.size	main_test_lock_cont, .-main_test_lock_cont
	.section	.rodata
.LC13:
	.string	"---- %ld, %ld\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB9:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 64
	movabs	rdx, 4711630319722168320
	movabs	rax, 4696837146684686336
	mov	QWORD PTR [rbp-56], rdx
	movsd	xmm1, QWORD PTR [rbp-56]
	mov	QWORD PTR [rbp-56], rax
	movsd	xmm0, QWORD PTR [rbp-56]
	mov	edi, OFFSET FLAT:.LC13
	mov	eax, 2
	call	printf
	mov	QWORD PTR [rbp-8], 0
	mov	DWORD PTR [rbp-12], 0
	jmp	.L29
.L30:
	mov	eax, DWORD PTR [rbp-12]
	cdqe
	add	QWORD PTR [rbp-8], rax
	add	DWORD PTR [rbp-12], 1
.L29:
	cmp	DWORD PTR [rbp-12], 999999
	jle	.L30
	lea	rax, [rbp-48]
	mov	rdi, rax
	call	times
	lea	rax, [rbp-48]
	mov	rdi, rax
	call	pprint_tms
	mov	eax, 0
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	main, .-main
	.section	.rodata
	.type	__PRETTY_FUNCTION__.2897, @object
	.size	__PRETTY_FUNCTION__.2897, 7
__PRETTY_FUNCTION__.2897:
	.string	"unlock"
	.align 8
.LC2:
	.long	0
	.long	1079574528
	.align 8
.LC4:
	.long	0
	.long	1104006501
	.align 8
.LC5:
	.long	0
	.long	1093567616
	.ident	"GCC: (GNU) 4.8.5 20150623 (Red Hat 4.8.5-16)"
	.section	.note.GNU-stack,"",@progbits
