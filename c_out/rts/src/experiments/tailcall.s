	.file	"tailcall.c"
	.intel_syntax noprefix
	.text
	.p2align 4,,15
	.globl	recursive_loop
	.type	recursive_loop, @function
recursive_loop:
.LFB7:
	.cfi_startproc
	test	edi, edi
	mov	eax, esi
	jle	.L2
	mov	ecx, edi
	mov	r8d, edi
	shr	ecx, 2
	lea	esi, [0+rcx*4]
	test	esi, esi
	je	.L3
	cmp	edi, 9
	jbe	.L3
	lea	edx, [rdi-1]
	movdqa	xmm3, XMMWORD PTR .LC0[rip]
	mov	DWORD PTR [rsp-20], edx
	lea	edx, [rdi-2]
	movd	xmm7, DWORD PTR [rsp-20]
	mov	DWORD PTR [rsp-16], edx
	lea	edx, [rdi-3]
	movd	xmm1, DWORD PTR [rsp-16]
	mov	DWORD PTR [rsp-16], edi
	mov	DWORD PTR [rsp-12], edx
	movd	xmm0, DWORD PTR [rsp-16]
	xor	edx, edx
	movd	xmm6, DWORD PTR [rsp-12]
	punpckldq	xmm0, xmm7
	punpckldq	xmm1, xmm6
	punpcklqdq	xmm0, xmm1
	pxor	xmm1, xmm1
	jmp	.L9
	.p2align 4,,10
	.p2align 3
.L30:
	movdqa	xmm0, xmm2
.L9:
	movdqa	xmm2, xmm0
	add	edx, 1
	paddd	xmm1, xmm0
	cmp	ecx, edx
	paddd	xmm2, xmm3
	ja	.L30
	movdqa	xmm4, xmm1
	sub	edi, esi
	psrldq	xmm4, 8
	paddd	xmm1, xmm4
	movdqa	xmm5, xmm1
	psrldq	xmm5, 4
	paddd	xmm1, xmm5
	movd	DWORD PTR [rsp-20], xmm1
	add	eax, DWORD PTR [rsp-20]
	cmp	esi, r8d
	je	.L2
.L3:
	lea	edx, [rdi-1]
	add	eax, edi
	test	edx, edx
	jle	.L2
	add	eax, edx
	mov	edx, edi
	sub	edx, 2
	je	.L2
	add	eax, edx
	mov	edx, edi
	sub	edx, 3
	je	.L2
	add	eax, edx
	mov	edx, edi
	sub	edx, 4
	je	.L2
	add	eax, edx
	mov	edx, edi
	sub	edx, 5
	je	.L2
	add	eax, edx
	mov	edx, edi
	sub	edx, 6
	je	.L2
	add	eax, edx
	mov	edx, edi
	sub	edx, 7
	je	.L2
	add	eax, edx
	lea	eax, [rdi-8+rax]
	ret
	.p2align 4,,10
	.p2align 3
.L2:
	rep ret
	.cfi_endproc
.LFE7:
	.size	recursive_loop, .-recursive_loop
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC1:
	.string	"This should not be printed"
.LC2:
	.string	"Just after label2"
	.text
	.p2align 4,,15
	.globl	jmp_func
	.type	jmp_func, @function
jmp_func:
.LFB8:
	.cfi_startproc
	cmp	BYTE PTR reg[rip], 0
	je	.L33
	mov	QWORD PTR jmp_ptr[rip], OFFSET FLAT:.L34
	xor	eax, eax
	ret
	.p2align 4,,10
	.p2align 3
.L33:
.L34:
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	mov	edi, OFFSET FLAT:.LC1
	call	puts
	mov	edi, OFFSET FLAT:.LC2
	call	puts
	mov	eax, 100
	add	rsp, 8
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE8:
	.size	jmp_func, .-jmp_func
	.section	.rodata.str1.1
.LC3:
	.string	"Arrived at goto"
	.text
	.p2align 4,,15
	.globl	goto_test
	.type	goto_test, @function
goto_test:
.LFB9:
	.cfi_startproc
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	mov	edi, OFFSET FLAT:.LC3
	call	puts
	jmp	[QWORD PTR jmp_ptr[rip]]
	.cfi_endproc
.LFE9:
	.size	goto_test, .-goto_test
	.p2align 4,,15
	.globl	main3
	.type	main3, @function
main3:
.LFB10:
	.cfi_startproc
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	xor	eax, eax
	call	jmp_func
	xor	eax, eax
	mov	BYTE PTR reg[rip], 0
	call	goto_test
	.cfi_endproc
.LFE10:
	.size	main3, .-main3
	.section	.rodata.str1.1
.LC4:
	.string	"%d\n"
	.text
	.p2align 4,,15
	.globl	main1
	.type	main1, @function
main1:
.LFB11:
	.cfi_startproc
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	mov	esi, 210
	mov	edi, OFFSET FLAT:.LC4
	xor	eax, eax
	call	printf
	xor	eax, eax
	add	rsp, 8
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE11:
	.size	main1, .-main1
	.section	.rodata.str1.1
.LC5:
	.string	"The value of val: %d"
	.text
	.p2align 4,,15
	.globl	simple_function
	.type	simple_function, @function
simple_function:
.LFB12:
	.cfi_startproc
	add	edi, esi
	push	rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	xor	eax, eax
	add	edx, edi
	mov	edi, OFFSET FLAT:.LC5
	add	ecx, edx
	add	r8d, ecx
	lea	ebx, [r8+r9]
	add	ebx, DWORD PTR [rsp+16]
	add	ebx, DWORD PTR [rsp+24]
	add	ebx, DWORD PTR [rsp+32]
	add	ebx, DWORD PTR [rsp+40]
	add	ebx, DWORD PTR [rsp+48]
	add	ebx, DWORD PTR [rsp+56]
	add	ebx, DWORD PTR [rsp+64]
	add	ebx, DWORD PTR [rsp+72]
	add	ebx, DWORD PTR [rsp+80]
	mov	esi, ebx
	call	printf
	mov	eax, ebx
	pop	rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE12:
	.size	simple_function, .-simple_function
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB13:
	.cfi_startproc
	sub	rsp, 8
	.cfi_def_cfa_offset 16
	mov	esi, 16
	mov	edi, OFFSET FLAT:.LC5
	xor	eax, eax
	call	printf
	xor	eax, eax
	add	rsp, 8
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE13:
	.size	main, .-main
	.comm	jmp_ptr,8,8
	.globl	reg
	.data
	.align 16
	.type	reg, @object
	.size	reg, 1
reg:
	.byte	1
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC0:
	.long	-4
	.long	-4
	.long	-4
	.long	-4
	.ident	"GCC: (GNU) 4.8.5 20150623 (Red Hat 4.8.5-16)"
	.section	.note.GNU-stack,"",@progbits
