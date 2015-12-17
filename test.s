	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 10
	.globl	_SeverityChr
	.align	4, 0x90
_SeverityChr:                           ## @SeverityChr
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp0:
	.cfi_def_cfa_offset 16
Ltmp1:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp2:
	.cfi_def_cfa_register %rbp
	cmpl	$4, %edi
	ja	LBB0_2
## BB#1:                                ## %switch.lookup
	shll	$3, %edi
	movabsq	$288926157124, %rax     ## imm = 0x4345574944
	movb	%dil, %cl
	shrq	%cl, %rax
	movsbl	%al, %eax
	popq	%rbp
	retq
LBB0_2:
	movl	$63, %eax
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_StderrSink_Send
	.align	4, 0x90
_StderrSink_Send:                       ## @StderrSink_Send
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp3:
	.cfi_def_cfa_offset 16
Ltmp4:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp5:
	.cfi_def_cfa_register %rbp
	pushq	%r15
	pushq	%r14
	pushq	%r13
	pushq	%r12
	pushq	%rbx
	subq	$4280, %rsp             ## imm = 0x10B8
Ltmp6:
	.cfi_offset %rbx, -56
Ltmp7:
	.cfi_offset %r12, -48
Ltmp8:
	.cfi_offset %r13, -40
Ltmp9:
	.cfi_offset %r14, -32
Ltmp10:
	.cfi_offset %r15, -24
	movq	%r9, -4216(%rbp)        ## 8-byte Spill
	movl	%r8d, -4228(%rbp)       ## 4-byte Spill
	movq	%rcx, -4224(%rbp)       ## 8-byte Spill
	movq	%rdx, -4240(%rbp)       ## 8-byte Spill
	movl	%esi, %r15d
	movq	___stack_chk_guard@GOTPCREL(%rip), %rax
	movq	(%rax), %rax
	movq	%rax, -48(%rbp)
	xorl	%edi, %edi
	callq	_time
	movq	%rax, -4152(%rbp)
	leaq	-4152(%rbp), %rdi
	leaq	-4208(%rbp), %rsi
	callq	_localtime_r
	movl	$63, %eax
	cmpl	$4, %r15d
	ja	LBB1_2
## BB#1:                                ## %switch.lookup.i
	shll	$3, %r15d
	movabsq	$288926157124, %rax     ## imm = 0x4345574944
	movb	%r15b, %cl
	shrq	%cl, %rax
	movsbl	%al, %eax
LBB1_2:                                 ## %SeverityChr.exit
	movl	%eax, -4244(%rbp)       ## 4-byte Spill
	movl	-4192(%rbp), %ebx
	incl	%ebx
	movl	-4196(%rbp), %r15d
	movl	-4200(%rbp), %r13d
	movl	-4208(%rbp), %r12d
	movl	-4204(%rbp), %r14d
	callq	_getpid
	cltq
	movq	-4240(%rbp), %rcx       ## 8-byte Reload
	movq	%rcx, 64(%rsp)
	movl	-4228(%rbp), %ecx       ## 4-byte Reload
	movl	%ecx, 56(%rsp)
	movq	-4224(%rbp), %rcx       ## 8-byte Reload
	movq	%rcx, 48(%rsp)
	movq	%rax, 40(%rsp)
	movl	%r12d, 32(%rsp)
	movl	%r14d, 24(%rsp)
	movl	%r13d, 16(%rsp)
	movl	%r15d, 8(%rsp)
	movl	%ebx, (%rsp)
	leaq	L_.str(%rip), %r8
	movl	$4096, %r12d            ## imm = 0x1000
	leaq	-4144(%rbp), %r15
	movl	$4096, %esi             ## imm = 0x1000
	movl	$0, %edx
	movl	$4096, %ecx             ## imm = 0x1000
	xorl	%eax, %eax
	movq	%r15, %rdi
	movl	-4244(%rbp), %r9d       ## 4-byte Reload
	callq	___snprintf_chk
	movslq	%eax, %rbx
	leaq	-4144(%rbp,%rbx), %rdi
	subq	%rbx, %r12
	xorl	%edx, %edx
	movq	$-1, %rcx
	movq	%r12, %rsi
	movq	-4216(%rbp), %r8        ## 8-byte Reload
	movq	16(%rbp), %r9
	callq	___vsnprintf_chk
	movq	___stderrp@GOTPCREL(%rip), %rcx
	movq	(%rcx), %rdi
	addl	%eax, %ebx
	leaq	L_.str1(%rip), %rsi
	xorl	%eax, %eax
	movl	%ebx, %edx
	movq	%r15, %rcx
	callq	_fprintf
	movq	___stack_chk_guard@GOTPCREL(%rip), %rax
	movq	(%rax), %rax
	cmpq	-48(%rbp), %rax
	jne	LBB1_4
## BB#3:                                ## %SeverityChr.exit
	addq	$4280, %rsp             ## imm = 0x10B8
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
	retq
LBB1_4:                                 ## %SeverityChr.exit
	callq	___stack_chk_fail
	.cfi_endproc

	.globl	_StderrSink_Destroy
	.align	4, 0x90
_StderrSink_Destroy:                    ## @StderrSink_Destroy
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp11:
	.cfi_def_cfa_offset 16
Ltmp12:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp13:
	.cfi_def_cfa_register %rbp
	popq	%rbp
	jmp	_free                   ## TAILCALL
	.cfi_endproc

	.globl	_CreateStderrSink
	.align	4, 0x90
_CreateStderrSink:                      ## @CreateStderrSink
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp14:
	.cfi_def_cfa_offset 16
Ltmp15:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp16:
	.cfi_def_cfa_register %rbp
	pushq	%rbx
	pushq	%rax
Ltmp17:
	.cfi_offset %rbx, -24
	movl	%edi, %ebx
	movl	$32, %edi
	callq	_malloc
	movl	%ebx, (%rax)
	leaq	_StderrSink_Send(%rip), %rcx
	movq	%rcx, 8(%rax)
	movl	$1, %ecx
	xchgl	%ecx, 16(%rax)
	leaq	_StderrSink_Destroy(%rip), %rcx
	movq	%rcx, 24(%rax)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_DefaultConfig_GetSink
	.align	4, 0x90
_DefaultConfig_GetSink:                 ## @DefaultConfig_GetSink
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp18:
	.cfi_def_cfa_offset 16
Ltmp19:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp20:
	.cfi_def_cfa_register %rbp
	movq	24(%rdi), %rax
	lock
	incl	16(%rax)
	movq	24(%rdi), %rax
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_DefaultConfig_Destroy
	.align	4, 0x90
_DefaultConfig_Destroy:                 ## @DefaultConfig_Destroy
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp21:
	.cfi_def_cfa_offset 16
Ltmp22:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp23:
	.cfi_def_cfa_register %rbp
	pushq	%rbx
	pushq	%rax
Ltmp24:
	.cfi_offset %rbx, -24
	movq	%rdi, %rbx
	movq	24(%rbx), %rax
	testq	%rax, %rax
	je	LBB5_3
## BB#1:
	movl	$-1, %ecx
	lock
	xaddl	%ecx, 16(%rax)
	testl	%ecx, %ecx
	jne	LBB5_3
## BB#2:
	movq	24(%rbx), %rdi
	callq	*24(%rdi)
LBB5_3:
	movq	%rbx, %rdi
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	jmp	_free                   ## TAILCALL
	.cfi_endproc

	.globl	_CreateDefaultConfig
	.align	4, 0x90
_CreateDefaultConfig:                   ## @CreateDefaultConfig
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp25:
	.cfi_def_cfa_offset 16
Ltmp26:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp27:
	.cfi_def_cfa_register %rbp
	pushq	%rbx
	pushq	%rax
Ltmp28:
	.cfi_offset %rbx, -24
	movq	%rdi, %rbx
	movl	$32, %edi
	callq	_malloc
	movq	%rbx, 24(%rax)
	leaq	_DefaultConfig_GetSink(%rip), %rcx
	movq	%rcx, (%rax)
	movl	$1, %ecx
	xchgl	%ecx, 8(%rax)
	leaq	_DefaultConfig_Destroy(%rip), %rcx
	movq	%rcx, 16(%rax)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_main
	.align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp29:
	.cfi_def_cfa_offset 16
Ltmp30:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp31:
	.cfi_def_cfa_register %rbp
	pushq	%r15
	pushq	%r14
	pushq	%rbx
	pushq	%rax
Ltmp32:
	.cfi_offset %rbx, -40
Ltmp33:
	.cfi_offset %r14, -32
Ltmp34:
	.cfi_offset %r15, -24
	leaq	L_.str2(%rip), %rdi
	callq	_GetLogger
	movq	%rax, %r14
	movl	$32, %edi
	callq	_malloc
	movq	%rax, %rbx
	movl	$1, (%rbx)
	leaq	_StderrSink_Send(%rip), %rax
	movq	%rax, 8(%rbx)
	movl	$1, %r15d
	movl	$1, %eax
	xchgl	%eax, 16(%rbx)
	leaq	_StderrSink_Destroy(%rip), %rax
	movq	%rax, 24(%rbx)
	movl	$32, %edi
	callq	_malloc
	movq	%rbx, 24(%rax)
	leaq	_DefaultConfig_GetSink(%rip), %rcx
	movq	%rcx, (%rax)
	xchgl	%r15d, 8(%rax)
	leaq	_DefaultConfig_Destroy(%rip), %rcx
	movq	%rcx, 16(%rax)
	movq	%rax, %rdi
	callq	_YalogSetConfig
	movl	(%r14), %eax
	testl	%eax, %eax
	jg	LBB7_2
## BB#1:
	leaq	L_.str3(%rip), %rdx
	leaq	L_.str4(%rip), %r8
	leaq	L_.str5(%rip), %r9
	movl	$0, %esi
	movl	$93, %ecx
	xorl	%eax, %eax
	movq	%r14, %rdi
	callq	_YalogSend
LBB7_2:
	movl	(%r14), %eax
	cmpl	$1, %eax
	jg	LBB7_4
## BB#3:
	leaq	L_.str3(%rip), %rdx
	leaq	L_.str4(%rip), %r8
	leaq	L_.str5(%rip), %r9
	movl	$1, %esi
	movl	$94, %ecx
	xorl	%eax, %eax
	movq	%r14, %rdi
	callq	_YalogSend
LBB7_4:
	movl	(%r14), %eax
	cmpl	$2, %eax
	jg	LBB7_6
## BB#5:
	leaq	L_.str3(%rip), %rdx
	leaq	L_.str4(%rip), %r8
	leaq	L_.str5(%rip), %r9
	movl	$2, %esi
	movl	$95, %ecx
	xorl	%eax, %eax
	movq	%r14, %rdi
	callq	_YalogSend
LBB7_6:
	xorl	%edi, %edi
	callq	_YalogSetConfig
	movl	(%r14), %eax
	testl	%eax, %eax
	jg	LBB7_8
## BB#7:
	leaq	L_.str3(%rip), %rdx
	leaq	L_.str4(%rip), %r8
	leaq	L_.str5(%rip), %r9
	movl	$0, %esi
	movl	$98, %ecx
	xorl	%eax, %eax
	movq	%r14, %rdi
	callq	_YalogSend
LBB7_8:
	movl	(%r14), %eax
	cmpl	$1, %eax
	jg	LBB7_10
## BB#9:
	leaq	L_.str3(%rip), %rdx
	leaq	L_.str4(%rip), %r8
	leaq	L_.str5(%rip), %r9
	movl	$1, %esi
	movl	$99, %ecx
	xorl	%eax, %eax
	movq	%r14, %rdi
	callq	_YalogSend
LBB7_10:
	movl	(%r14), %eax
	cmpl	$2, %eax
	jg	LBB7_12
## BB#11:
	leaq	L_.str3(%rip), %rdx
	leaq	L_.str4(%rip), %r8
	leaq	L_.str5(%rip), %r9
	movl	$2, %esi
	movl	$100, %ecx
	xorl	%eax, %eax
	movq	%r14, %rdi
	callq	_YalogSend
LBB7_12:
	xorl	%eax, %eax
	addq	$8, %rsp
	popq	%rbx
	popq	%r14
	popq	%r15
	popq	%rbp
	retq
	.cfi_endproc

	.section	__TEXT,__cstring,cstring_literals
L_.str:                                 ## @.str
	.asciz	"%c%02d%02d %02d:%02d:%02d %ld %s:%d] %s] "

L_.str1:                                ## @.str1
	.asciz	"%.*s\n"

L_.str2:                                ## @.str2
	.asciz	"123"

L_.str3:                                ## @.str3
	.asciz	"test.c"

L_.str4:                                ## @.str4
	.asciz	"%s"

L_.str5:                                ## @.str5
	.asciz	"Hello, World!"


.subsections_via_symbols
