	.file	"tailcall.c"
	.intel_syntax noprefix
	.text
	.globl	recursive_loop
	.type	recursive_loop, @function
recursive_loop:
.LFB0:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 32
	mov	DWORD PTR -20[rbp], edi
	mov	DWORD PTR -24[rbp], esi
	cmp	DWORD PTR -20[rbp], 0
	jg	.L2
	mov	eax, DWORD PTR -24[rbp]
	jmp	.L3
.L2:
	mov	edx, DWORD PTR -24[rbp]
	mov	eax, DWORD PTR -20[rbp]
	add	eax, edx
	mov	DWORD PTR -8[rbp], eax
	mov	eax, DWORD PTR -20[rbp]
	sub	eax, 1
	mov	DWORD PTR -4[rbp], eax
	mov	edx, DWORD PTR -8[rbp]
	mov	eax, DWORD PTR -4[rbp]
	mov	esi, edx
	mov	edi, eax
	call	recursive_loop
.L3:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	recursive_loop, .-recursive_loop
	.section	.rodata
.LC0:
	.string	"%d\n"
	.text
	.globl	f
	.type	f, @function
f:
.LFB1:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 32
	mov	DWORD PTR -4[rbp], edi
	mov	DWORD PTR -8[rbp], esi
	mov	DWORD PTR -12[rbp], edx
	mov	DWORD PTR -16[rbp], ecx
	mov	DWORD PTR -20[rbp], r8d
	mov	ecx, DWORD PTR -4[rbp]
	mov	edx, 351843721
	mov	eax, ecx
	imul	edx
	sar	edx, 13
	mov	eax, ecx
	sar	eax, 31
	sub	edx, eax
	mov	eax, edx
	imul	eax, eax, 100000
	sub	ecx, eax
	mov	eax, ecx
	test	eax, eax
	jne	.L5
	mov	eax, DWORD PTR -4[rbp]
	mov	esi, eax
	lea	rdi, .LC0[rip]
	mov	eax, 0
	call	printf@PLT
.L5:
	add	DWORD PTR -4[rbp], 1
	mov	edi, DWORD PTR -20[rbp]
	mov	ecx, DWORD PTR -16[rbp]
	mov	edx, DWORD PTR -12[rbp]
	mov	esi, DWORD PTR -8[rbp]
	mov	eax, DWORD PTR -4[rbp]
	mov	r8d, edi
	mov	edi, eax
	call	f
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	f, .-f
	.globl	f_opt
	.type	f_opt, @function
f_opt:
.LFB2:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 16
	mov	DWORD PTR -4[rbp], edi
	add	DWORD PTR -4[rbp], 1
	mov	ecx, DWORD PTR -4[rbp]
	mov	edx, 351843721
	mov	eax, ecx
	imul	edx
	sar	edx, 13
	mov	eax, ecx
	sar	eax, 31
	sub	edx, eax
	mov	eax, edx
	imul	eax, eax, 100000
	sub	ecx, eax
	mov	eax, ecx
	test	eax, eax
	jne	.L8
	mov	eax, DWORD PTR -4[rbp]
	mov	esi, eax
	lea	rdi, .LC0[rip]
	mov	eax, 0
	call	printf@PLT
.L8:
	mov	eax, DWORD PTR -4[rbp]
#APP
# 33 "tailcall.c" 1
	movq %rbp, %rsp;
	popq %rbp;
	movl eax, %edi;
	
# 0 "" 2
#NO_APP
	lea	rax, f_opt[rip]
	jmp	rax
	.cfi_endproc
.LFE2:
	.size	f_opt, .-f_opt
	.section	.rodata
.LC1:
	.string	"The result is %d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB3:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	mov	edi, 0
	call	f_opt
	mov	esi, eax
	lea	rdi, .LC1[rip]
	mov	eax, 0
	call	printf@PLT
	mov	eax, 0
	pop	rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 7.3.0-16ubuntu3) 7.3.0"
	.section	.note.GNU-stack,"",@progbits
