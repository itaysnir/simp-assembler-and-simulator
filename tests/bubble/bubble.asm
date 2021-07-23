limm $a0, $zero, $zero, 15		#function argument- array size-1
limm $a1, $zero, $zero, 1024	#function argument- array location

limm $sp, $zero, $zero, 2048	#set stack pointer in memory
limm $t0, $zero, $zero, 5	# $t0 = 5
sub $sp, $sp, $t0, 0		# adjust stack for 5 items
sw $s0, $sp, $zero, 4		# save $s0
sw $s1, $sp, $zero, 3		# save $s1
sw $s2, $sp, $zero, 2		# save $s2
sw $a0, $sp, $zero, 1		# save $a0
sw $a1, $sp, $zero, 0		# save $a1
limm $s0, $zero, $zero, 0	# intialize i = 0
limm $s1, $zero, $zero, 0	# intialize j = 0
limm $t1, $zero, $zero, 1	# constant 1

for1:
bgt $zero, $s0, $a0, exit		# if i>15, exit
limm $s1, $zero, $zero, 0		# j=0
add $s2, $t1, $a0, 0			# save n = 16
sub $s2, $s2, $s0, 0			# save n-i
beq $zero, $zero, $zero, for2	# start nested loop

for1cont:
add $s0, $s0, $t1, 0			# finished iteration, proceed to next, i = i+1
beq $zero, $zero, $zero, for1	# continue loop

for2:
bgt $zero, $s1, $s2, for1cont	# if j > n-i, go to the original loop
add $t0, $a1, $s1, 0			# save a[j] address
add $v0, $t0, $t1, 0			# save a[j+1] address
lw $t2, $t0, $zero, 0			# load a[j]
lw $t3, $v0, $zero, 0			# load a[j+1]
bgt $zero, $t3, $t2, swap		# if a[j+1]>a[j], swap 

for2cont:
add $s1, $s1, $t1, 0			# j = j+1
beq $zero, $zero, $zero, for2	#loop

swap:
sw $t3, $t0, $zero, 0				# first swap
sw $t2, $v0, $zero, 0				# second swap
beq $zero, $zero, $zero, for2cont	# swapping done, return to loop

exit: 
lw $s0, $sp, $zero, 4		# restore $s0
lw $s1, $sp, $zero, 3		# restore $s1
lw $s2, $sp, $zero, 2		# restore $s2
lw $a0, $sp, $zero, 1		# restore $a0
lw $a1, $sp, $zero, 0		# restore $a1
limm $t0, $zero, $zero, 5	# $t0 = 5
add $sp, $sp, $t0, 5		# pop 5 items from stack
halt $zero, $zero, $zero, 0	# halt
.word 1024 -2
.word 1025 3
.word 1026 15
.word 1027 2
.word 1028 11
.word 1029 4
.word 1030 110
.word 1031 2
.word 1032 -3
.word 1033 14
.word 1034 117
.word 1035 15
.word 1036 343
.word 1037 32
.word 1038 -1
.word 1039 0
