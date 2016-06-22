	.file	"kadai20.c"
	.def	___main;	.scl	2;	.type	32;	.endef
	.text
	.globl	_main
	.def	_main;	.scl	2;	.type	32;	.endef
_main:
LFB6:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	andl	$-16, %esp
	subl	$48, %esp
	call	___main
	movl	$0, 40(%esp)
	call	_getchar
	movl	%eax, 44(%esp)
	jmp	L2
L4:
	movl	40(%esp), %eax
	leal	1(%eax), %edx
	movl	%edx, 40(%esp)
	movl	44(%esp), %edx
	movb	%dl, 31(%esp,%eax)
	call	_getchar
	movl	%eax, 44(%esp)
L2:
	cmpl	$-1, 44(%esp)
	je	L3
	cmpl	$8, 40(%esp)
	jle	L4
L3:
	leal	31(%esp), %eax
	movl	%eax, (%esp)
	call	_puts
	movl	$0, %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE6:
	.ident	"GCC: (GNU) 4.8.1"
	.def	_getchar;	.scl	2;	.type	32;	.endef
	.def	_puts;	.scl	2;	.type	32;	.endef
