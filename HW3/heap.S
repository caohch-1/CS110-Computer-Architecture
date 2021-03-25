# DO NOT REMOVE THIS LINE WHEN SUBMITTING
.import input.S

.text
.globl	main
main:
	lw s1, len
	j print_heap


# Meet -2 and Print
print_heap:
	addi sp, sp, -8 #Store a0 and a1
	sw a0, 0(sp)
	sw a1, 4(sp)

	la t1, heap # int* t1 = heap
	mv t2, zero # int t2 = 0
	
	
	print_loop:
	
	slli t3, t2, 2 # int t3 = 4 * t2
	add t4, t3, t1 # int* t4 = t3(t1)
	lw t4, 0(t4) # int t4 = *t4

	li a0, 1 # print_int ecall
	mv a1, t4 # print t4
	ecall

	li a0, 11 # print_character ecall
	li a1, ' ' # print whitespace
	ecall

	addi t2, t2, 1 # t2 ++
	bne t2, s1, print_loop

	lw a1, 4(sp) #Restore a0 and a1
	lw a0, 0(sp)
	addi sp, sp, 8
	ret


# Meet Error and Exit
error_exit:
	li a0, 17 # exit1 ecall
	li a1, -1 # exit code -1
	ecall
