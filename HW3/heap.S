# DO NOT REMOVE THIS LINE WHEN SUBMITTING
.import input.S

.text
.globl	main
main:
	mv	a1, zero
	li	a0, 17
	ecall


# Meet -2 and Print
print_heap:
	la t2, heap # int* t2 = heap
	mv t1, zero # int t1 = 0

	add t3, t1, t2 # int* t3 = t2 + t1
	lw t4, 0(t3) #




# Meet Error and Exit
error_exit:
	li a0, 17 # exit2 ecall
	li a1, -1 # exit code -1
	ecall
