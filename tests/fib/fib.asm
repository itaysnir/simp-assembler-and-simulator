	limm $sp, $zero, $zero, 2048	# set stack pointer at 2048
	lw $a0, $zero, $zero, 1024	# get x from address 1024
	jal $zero, $zero, $zero, fib	# calc $v0 = fib(x)
	sw $v0, $zero, $zero, 1025	# store fib(x) in 1025
	halt $zero, $zero, $zero, 0	# halt


fib:	limm $t0, $zero, $zero, 3	# $t0 = 3
	sub $sp, $sp, $t0, 0		# adjust stack for 3 items
	sw $s0, $sp, $zero, 2		# save $s0
	sw $ra, $sp, $zero, 1		# save return address
	sw $a0, $sp, $zero, 0		# save argument
        limm $t1, $zero, $zero, 1       # $t1 = 1
	bgt $zero, $a0, $t1, L1		# jump to L1 if x > 1
	add $v0, $a0, $zero, 0		# otherwise, fib(x) = x
	add $sp, $sp, $t0, 0		# pop 3 items from stack
	jr $ra, $zero, $zero, 0		# and return
L1:
	limm $t0, $zero, $zero, 1	# $t0 = 1
	sub $a0, $a0, $t0, 0		# calculate x - 1
	jal $zero, $zero, $zero, fib	# calc $v0=fib(x-1)
	add $s0, $v0, $zero, 0		# $s0 = fib(x-1)
	lw $a0, $sp, $zero, 0		# restore $a0 = x
	limm $t0, $zero, $zero, 2	# $t0 = 2
	sub $a0, $a0, $t0, 0		# calculate x - 2
	jal $zero, $zero, $zero, fib	# calc fib(x-2)
	add $v0, $v0, $s0, 0		# $v0 = fib(x-2) + fib(x-1)
	lw $a0, $sp, $zero, 0		# restore $a0
	lw $ra, $sp, $zero, 1		# restore $ra
	lw $s0, $sp, $zero, 2		# restore $s0
	limm $t0, $zero, $zero, 3	# $t0 = 3
	add $sp, $sp, $t0, 3		# pop 3 items from stack
	jr $ra, $zero, $zero, 0		# and return
	.word 1024 7
