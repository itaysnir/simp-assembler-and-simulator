limm $sp, $zero, $zero, 2048	# set stack pointer at 2048
limm $t1, $zero, $zero, 1		# saved 1
limm $a0, $zero, $zero, 1024	# p = array's start address
limm $a1, $zero, $zero, 1039	# r = array's end address
limm $s1, $zero, $zero, 512		# set B[0] pointer at 512
limm $s2, $zero, $zero, 16		# array's size

limm $t0, $zero, $zero, 3		# $t0 = 3
sub $sp, $sp, $t0, 0		# adjust stack for 3 items
sw $s0, $sp, $zero, 0		# save $s0
sw $s1, $sp, $zero, 1		# save $s1
sw $s2, $sp, $zero, 2		# save $s2
jal $zero, $zero, $zero, mergesort		# call mergesort procedure
lw $s0, $sp, $zero, 0		# restore $s0
lw $s1, $sp, $zero, 1		# restore $s1
lw $s2, $sp, $zero, 2		# restore $s2
limm $t0, $zero, $zero, 3	# $t0 = 3
add $sp, $sp, $t0, 3		# pop 3 items from stack
halt $zero, $zero, $zero, 0		# exit

mergesort:
bgt $zero, $a0, $a1, exit1		#if p >r, exit
beq $zero, $a0, $a1, exit1		# if p = r, exit
beq $zero, $zero, $zero, continue	# else, proceed

exit1:
jr $ra, $zero, $zero, 0			# return

continue:
limm $gp, $zero, $zero, 8	# save parameters, $gp = 8
sub $sp, $sp, $gp, 0		# adjust stack for 8 items
sw $s2, $sp, $zero, 7		# save $s2
sw $s1, $sp, $zero, 6		# save $s1
sw $ra, $sp, $zero, 5		# save return address
sw $a0, $sp, $zero, 4		# save $a0
sw $a1, $sp, $zero, 3		# save $a1
sw $t0, $sp, $zero, 2		# save $t0
sw $s0, $sp, $zero, 1		# save $s0
sw $t2, $sp, $zero, 0		# save $t2

add $t0, $a0, $a1, 0		# $t0 = q = p + r
sra $t0, $t0, $t1, 0		# q = (p + r)/2 
add $s0, $t0, $t1, 0		# $s0 = q+1
add $a1, $t0, $zero, 0		# now, r = q
jal $zero, $zero, $zero, mergesort # calc mergesort between p and q
lw $ra, $sp, $zero, 5		# reload original return address
lw $a1, $sp, $zero, 3		# reload original value from stack	
add $a0, $s0, $zero, 0		# now, p = q + 1
jal $zero, $zero, $zero, mergesort # calc mergesort between q+1 and r
lw $ra, $sp, $zero, 5		# reload original return address
lw $a0, $sp, $zero, 4		# reload original value from stack

merge:
bgt $zero, $a0, $t0, secondloop	# at start, i = p. if i>q, continue to next loop
bgt $zero, $s0, $a1, secondloop	# at start, j = q+1. if j>r, continue to next loop
lw $t2, $a0, $zero, 0			# load A[i]
lw $v0, $s0, $zero, 0			# load A[j]
bgt $zero, $t2, $v0, else1		# if A[i] > A[j], do else1
sw $v0, $s1, $zero, 0			# B[k] = A[j]
add $s0, $s0, $t1, 0			# j = j+1
beq $zero, $zero, $zero, finish1	# skip the else

aftermerge:
lw $ra, $sp, $zero, 5 # restore return address
lw $s1, $sp, $zero, 6 # restore $s1
lw $s2, $sp, $zero, 7 # restore $s2
lw $a0, $sp, $zero, 4 # restore $a0
lw $a1, $sp, $zero, 3 # restore $a1
lw $t0, $sp, $zero, 2 # restore $t0
lw $s0, $sp, $zero, 1 # restore $s0
lw $t2, $sp, $zero, 0 # restore $t2
limm $v0, $zero, $zero, 8	# $v0 = 8
add $sp, $sp, $v0, 8		# pop 8 items from stack
jr $ra, $zero, $zero, 0 # return

else1:							
sw $t2, $s1, $zero, 0		# B[k] = A[i]
add $a0, $a0, $t1, 0		# i = i + 1
finish1:
add $s1, $s1, $t1, 0		# finished iteration, proceed to next, k = k+1
beq $zero, $zero, $zero, merge	# continue loop

secondloop:
bgt $zero, $a0, $t0, thirdloop			# if i>q, go to next loop
lw $t2, $a0, $zero, 0					# load A[i]
sw $t2, $s1, $zero, 0					# B[k] = A[i]
add $s1, $s1, $t1, 0					# k = k + 1
add $a0, $a0, $t1, 0					# i = i + 1
beq $zero, $zero, $zero, secondloop		# continue loop

thirdloop:
bgt $zero, $s0, $a1, getarray			# j>r
lw $t2, $s0, $zero, 0					# load A[j]
sw $t2, $s1, $zero, 0					# B[k] = A[j]
add $s1, $s1, $t1, 0					# k = k + 1
add $s0, $t1, $s0, 0					# j = j+1
beq $zero, $zero, $zero, thirdloop		# continue loop

getarray: 
lw $a0, $sp, $zero, 4			# restore original A[] start from stack (i)
lw $s1, $sp, $zero, 6			# restore original B[] start from stack (k)

fourthloop:
bgt $zero, $a0, $a1, aftermerge	# if i>r, break and jump to aftermerge
lw $t2, $s1, $zero, 0			# load B[k]
sw $t2, $a0, $zero, 0			# A[i] = B[k]
add $a0, $a0, $t1, 0			# i = i + 1
add $s1, $s1, $t1, 0			# k = k + 1	
beq $zero, $zero, $zero, fourthloop	# continue loop

.word 1024 -124
.word 1025 -1
.word 1026 2624
.word 1027 22
.word 1028 -19
.word 1029 321
.word 1030 110
.word 1031 2
.word 1032 -3
.word 1033 1999
.word 1034 -9999
.word 1035 2464
.word 1036 0x3
.word 1037 32
.word 1038 -1
.word 1039 0x15
