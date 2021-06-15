	.text
	.text
	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
	movq	$12, %r8
	movq	$3, %r9
	imulq	%r8, %r9
	movq	%r9, %rdi
	call	printint
	movq	$18, %r8
	movq	$2, %r9
	movq	$4, %r10
	imulq	%r9, %r10
	subq	%r10, %r8
	movq	%r8, %rdi
	call	printint
	movq	$1, %r8
	movq	$2, %r9
	addq	%r8, %r9
	movq	$9, %r8
	addq	%r9, %r8
	movq	$5, %r9
	movq	$2, %r10
	movq	%r9,%rax
	cqo
	idivq	%r10
	movq	%rax,%r9
	subq	%r9, %r8
	movq	$3, %r9
	movq	$5, %r10
	imulq	%r9, %r10
	addq	%r8, %r10
	movq	%r10, %rdi
	call	printint
L1:
	popq	%rbp
	ret
