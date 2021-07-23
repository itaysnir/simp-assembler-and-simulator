limm $sp, $zero, $zero, 2048 # set stack pointer at 2048
jal $zero, $zero, $zero, mult	# call mult

mult:
limm $t3, $zero, $zero, 1024 # array location in memory
limm $t0, $zero, $zero, 2	# $t0 = 2
sub $sp, $sp, $t0, 0		# adjust stack for 2 items
sw $s0, $sp, $zero, 0		# save $s0 (i)
sw $s1, $sp, $zero, 1		# save $s1 (j)
limm $s0, $zero, $zero, 1       # i = 1
limm $s1, $zero, $zero, 1       # j = 1
limm $t0, $zero, $zero, 10  #save 10
limm $t1, $zero, $zero, 1  #save 1

for1:
bgt $zero, $s0, $t0, exit		# jump to exit if i > 10
limm $s1, $zero, $zero, 1		# j=1
beq $zero, $zero, $zero, for2	# start nested loop

for1cont:
add $s0, $s0, $t1, 0			# finished iteration, proceed to next, i = i+1
beq $zero, $zero, $zero, for1	# continue loop

for2:
bgt $zero, $s1, $t0, for1cont		# go back to for1cont if j > 10
sub $a0, $s0, $t1, 0		# calculate i - 1
sub $t2, $s1, $t1, 0		# calculate j - 1
add $a1, $t2, $t2, 0		# calculate 2(j-1)
add $a1, $a1, $a1, 0		# calculate 4(j-1)
add $a1, $a1, $t2, 0		# calculate 5(j-1)
add $a1, $a1, $a1, 0		# calculate 10(j-1)
add $a1, $a1, $a0, 0		# calculate 10(j-1)+(i-1)
add $a1, $a1, $t3, 0		# calculate 1024+10(j-1)+(i-1)
limm $t2, $zero, $zero, 1   # n = 1 (defined helper variable for the multipy function)
limm $v0, $zero, $zero, 0	# i*j = 0 (this will be the result of the mult function)
beq $zero, $zero, $zero, multiply	#multipy i*j

for2cont:
sw $v0, $a1, $zero, 0		# save the result in memory
add $s1, $s1, $t1, 0		# j = j+1
beq $zero, $zero, $zero, for2	#loop

multiply:
bgt $zero, $t2, $s1, for2cont		#if n>j return to for2cont
add $v0, $v0, $s0, 0				# v0 = v0 + i	 
add $t2, $t2, $t1, 0				# n = n+1
beq $zero, $zero, $zero, multiply	#loop


exit: 
lw $s0, $sp, $zero, 0		# restore $s0
lw $s1, $sp, $zero, 1		# restore $s1
limm $t0, $zero, $zero, 2	# $t0 = 2
add $sp, $sp, $t0, 2		# pop 2 items from stack
halt $zero, $zero, $zero, 0	# halt
