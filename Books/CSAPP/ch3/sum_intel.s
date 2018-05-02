	.file	"sum.c"
	.intel_syntax noprefix
	.text
	.globl	sum
	.type	sum, @function
sum:
.LFB0:
	.cfi_startproc
	lea	eax, [rdi+rsi]
	add	DWORD PTR accum[rip], eax
	ret
	.cfi_endproc
.LFE0:
	.size	sum, .-sum
	.globl	accum
	.bss
	.align 4
	.type	accum, @object
	.size	accum, 4
accum:
	.zero	4
	.ident	"GCC: (Ubuntu 6.2.0-3ubuntu11~14.04) 6.2.0 20160901"
	.section	.note.GNU-stack,"",@progbits
