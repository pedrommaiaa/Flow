	.text
	.data
	.globl	str
str:	.quad	0
	.data
	.globl	a
a:	.long	0
	.data
	.globl	b
b:	.long	0
L2:
	.byte	110
	.byte	101
	.byte	103
	.byte	97
	.byte	116
	.byte	105
	.byte	111
	.byte	110
	.byte	32
	.byte	111
	.byte	102
	.byte	32
	.byte	48
	.byte	32
	.byte	98
	.byte	101
	.byte	99
	.byte	111
	.byte	109
	.byte	109
	.byte	105
	.byte	110
	.byte	103
	.byte	32
	.byte	49
	.byte	32
	.byte	105
	.byte	115
	.byte	32
	.byte	119
	.byte	111
	.byte	114
	.byte	107
	.byte	105
	.byte	110
	.byte	103
	.byte	10
	.byte	0
	.text
	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
	movq	$1, %r8
	movl	%r8d, a(%rip)
	movq	$0, %r8
	movl	%r8d, b(%rip)
	movslq	b(%rip), %r8
	test	%r8, %r8
	sete	%r8b
	movzbq	%r8b, %r8
	test	%r8, %r8
	je	L3
	leaq	L2(%rip), %r8
	movq	%r8, str(%rip)
L4:
	movq	str(%rip), %r8
	movzbq	(%r8), %r8
	test	%r8, %r8
	je	L5
	movq	str(%rip), %r8
	movzbq	(%r8), %r8
	movq	%r8, %rdi
	call	printchar
	movq	%rax, %r9
	movq	str(%rip), %r8
	incq	str(%rip)
	jmp	L4
L5:
L3:
	movslq	a(%rip), %r8
	test	%r8, %r8
	sete	%r8b
	movzbq	%r8b, %r8
	test	%r8, %r8
	je	L6
	movq	$24, %r8
	movq	%r8, %rdi
	call	printint
	movq	%rax, %r9
	jmp	L7
L6:
	movq	$10, %r8
	movq	%r8, %rdi
	call	printint
	movq	%rax, %r9
L7:
	movq	$0, %r8
	movl	%r8d, %eax
	jmp	L1
L1:
	popq	%rbp
	ret
