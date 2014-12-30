	.file	"multiply.c"
	.text
.globl Multiply
	.type	Multiply, @function
Multiply:
.LFB19:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -136(%rbp)
	movq	%rsi, -144(%rbp)
	movq	%rdx, -152(%rbp)
	movl	%ecx, -156(%rbp)
	movl	%r8d, -160(%rbp)
	movl	%r9d, -164(%rbp)
	movq	-136(%rbp), %rax
	movq	%rax, -128(%rbp)
	movq	-144(%rbp), %rax
	movq	%rax, -120(%rbp)
	movq	-152(%rbp), %rax
	movq	%rax, -112(%rbp)
	movl	$0, -104(%rbp)
	jmp	.L2
.L3:
	mov	-104(%rbp), %eax
	salq	$2, %rax
	addq	-112(%rbp), %rax
	movl	$0, (%rax)
	addl	$1, -104(%rbp)
.L2:
	movl	-164(%rbp), %eax
	movl	-156(%rbp), %edx
	leal	(%rdx,%rax), %eax
	cmpl	-104(%rbp), %eax
	ja	.L3
	movl	$0, -104(%rbp)
	jmp	.L4
.L7:
	movl	$0, -100(%rbp)
	jmp	.L5
.L6:
	mov	-104(%rbp), %eax
	salq	$2, %rax
	addq	-128(%rbp), %rax
	movl	(%rax), %eax
	mov	%eax, %eax
	andl	$65535, %eax
	movq	%rax, -88(%rbp)
	mov	-100(%rbp), %eax
	salq	$2, %rax
	addq	-120(%rbp), %rax
	movl	(%rax), %eax
	mov	%eax, %eax
	andl	$65535, %eax
	movq	%rax, -80(%rbp)
	mov	-104(%rbp), %eax
	salq	$2, %rax
	addq	-128(%rbp), %rax
	movl	(%rax), %eax
	shrl	$16, %eax
	mov	%eax, %eax
	movq	%rax, -72(%rbp)
	mov	-100(%rbp), %eax
	salq	$2, %rax
	addq	-120(%rbp), %rax
	movl	(%rax), %eax
	shrl	$16, %eax
	mov	%eax, %eax
	movq	%rax, -64(%rbp)
	movq	-72(%rbp), %rax
	movq	-88(%rbp), %rdx
	leaq	(%rdx,%rax), %rax
	movq	%rax, -56(%rbp)
	movq	-64(%rbp), %rax
	movq	-80(%rbp), %rdx
	leaq	(%rdx,%rax), %rax
	movq	%rax, -48(%rbp)
	movq	-56(%rbp), %rax
	imulq	-48(%rbp), %rax
	movq	%rax, -24(%rbp)
	movq	-72(%rbp), %rax
	imulq	-64(%rbp), %rax
	movq	%rax, -32(%rbp)
	movq	-88(%rbp), %rax
	imulq	-80(%rbp), %rax
	movq	%rax, -16(%rbp)
	movq	-32(%rbp), %rax
	movq	-24(%rbp), %rdx
	movq	%rdx, %rcx
	subq	%rax, %rcx
	movq	%rcx, %rax
	subq	-16(%rbp), %rax
	movq	%rax, -24(%rbp)
	movq	-16(%rbp), %rax
	shrq	$16, %rax
	movq	%rax, -8(%rbp)
	andq	$65535, -16(%rbp)
	movq	-8(%rbp), %rax
	addq	%rax, -24(%rbp)
	movq	-24(%rbp), %rax
	shrq	$16, %rax
	movq	%rax, -8(%rbp)
	andq	$65535, -24(%rbp)
	movq	-8(%rbp), %rax
	addq	%rax, -32(%rbp)
	movq	-32(%rbp), %rax
	shrq	$16, %rax
	movq	%rax, -40(%rbp)
	andq	$65535, -32(%rbp)
	salq	$16, -24(%rbp)
	movq	-16(%rbp), %rax
	addq	%rax, -24(%rbp)
	movl	-100(%rbp), %eax
	movl	-104(%rbp), %edx
	leal	(%rdx,%rax), %eax
	mov	%eax, %eax
	salq	$2, %rax
	addq	-112(%rbp), %rax
	movl	(%rax), %eax
	mov	%eax, %eax
	addq	%rax, -24(%rbp)
	movl	-100(%rbp), %eax
	movl	-104(%rbp), %edx
	leal	(%rdx,%rax), %eax
	mov	%eax, %eax
	salq	$2, %rax
	addq	-112(%rbp), %rax
	movq	-24(%rbp), %rdx
	movl	%edx, (%rax)
	movl	-100(%rbp), %eax
	movl	-104(%rbp), %edx
	leal	(%rdx,%rax), %eax
	addl	$1, %eax
	mov	%eax, %eax
	salq	$2, %rax
	addq	-112(%rbp), %rax
	movl	-100(%rbp), %edx
	movl	-104(%rbp), %ecx
	leal	(%rcx,%rdx), %edx
	addl	$1, %edx
	mov	%edx, %edx
	salq	$2, %rdx
	addq	-112(%rbp), %rdx
	movl	(%rdx), %ecx
	movq	-32(%rbp), %rdx
	leal	(%rcx,%rdx), %edx
	movl	%edx, (%rax)
	movl	-100(%rbp), %eax
	movl	-104(%rbp), %edx
	leal	(%rdx,%rax), %eax
	addl	$1, %eax
	mov	%eax, %eax
	salq	$2, %rax
	addq	-112(%rbp), %rax
	movl	-100(%rbp), %edx
	movl	-104(%rbp), %ecx
	leal	(%rcx,%rdx), %edx
	addl	$1, %edx
	mov	%edx, %edx
	salq	$2, %rdx
	addq	-112(%rbp), %rdx
	movl	(%rdx), %edx
	movq	-40(%rbp), %rcx
	sall	$16, %ecx
	addl	%ecx, %edx
	movl	%edx, (%rax)
	movl	-100(%rbp), %eax
	movl	-104(%rbp), %edx
	leal	(%rdx,%rax), %eax
	addl	$1, %eax
	mov	%eax, %eax
	salq	$2, %rax
	addq	-112(%rbp), %rax
	movl	-100(%rbp), %edx
	movl	-104(%rbp), %ecx
	leal	(%rcx,%rdx), %edx
	addl	$1, %edx
	mov	%edx, %edx
	salq	$2, %rdx
	addq	-112(%rbp), %rdx
	movl	(%rdx), %ecx
	movq	-24(%rbp), %rdx
	shrq	$32, %rdx
	leal	(%rcx,%rdx), %edx
	movl	%edx, (%rax)
	addl	$1, -100(%rbp)
.L5:
	movl	-100(%rbp), %eax
	cmpl	-164(%rbp), %eax
	jb	.L6
	addl	$1, -104(%rbp)
.L4:
	movl	-104(%rbp), %eax
	cmpl	-156(%rbp), %eax
	jb	.L7
	movl	-156(%rbp), %eax
	movl	-164(%rbp), %edx
	addl	%eax, %edx
	movq	24(%rbp), %rax
	movl	%edx, (%rax)
	movl	-156(%rbp), %eax
	movl	-164(%rbp), %edx
	leal	(%rdx,%rax), %eax
	movl	%eax, %edx
	sall	$5, %edx
	movq	32(%rbp), %rax
	movl	%edx, (%rax)
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE19:
	.size	Multiply, .-Multiply
	.ident	"GCC: (GNU) 4.4.7 20120313 (Red Hat 4.4.7-4)"
	.section	.note.GNU-stack,"",@progbits
