# DO NOT REMOVE THIS LINE WHEN SUBMITTING
.import input.S

.text
.globl	main
main:
	lw s1, len # int s1 = len
	lw s2, input_len # int s2 = input_len
	li s3, -1
	li s4, -2
	la s5, input # int* s5 = input
	la s6, heap
	
	
	li t4, 0 # int t4 = 0

	main_loop:
	slli t5, t4, 2 # int t5 = 4 * t4
	add t6, t5, s5 # int* t6 = t5(s5)
	lw a1, 0(t6) # int a1 = *t6 as argument

	bge a1, x0, min_heap_insert # if a1 >= x0, insert(s5, a1)
	addi t4, t4, 1
	bne t4, s2, main_loop
	beq t4, s2, normal_exit


	beq a1, s3, heap_extract_min # if a1 == -1, pop(s5)
	addi t4, t4, 1
	bne t4, s2, main_loop
	beq t4, s2, normal_exit

	beq a1, s4, print_heap # if a1 == -2, print(s5)
	addi t4, t4, 1
	bne t4, s2, main_loop
	beq t4, s2, normal_exit



# Meet -2 and Print
print_heap:
	addi sp, sp, -20 #Store to stack
	sw a0, 0(sp)
	sw a1, 4(sp)
	sw t1, 8(sp)
	sw t2, 12(sp)
	sw t3, 16(sp)

	li t1, 0 # int t1 = 0
	
	
	print_loop:
	
	slli t2, t1, 2 # int t2 = 4 * t1
	add t3, t2, s6 # int* t3 = t2(s6)
	lw t3, 0(t3) # int t3 = *t3

	li a0, 1 # print_int ecall
	mv a1, t3 # print t3
	ecall

	li a0, 11 # print_character ecall
	li a1, ' ' # print whitespace
	ecall

	addi t1, t1, 1 # t1 ++
	bne t1, s1, print_loop

	lw t3, 16(sp) #Restore from stack
	lw t2, 12(sp)
	lw t1, 8(sp)
	lw a1, 4(sp) 
	lw a0, 0(sp)
	addi sp, sp, 20
	ret


min_heapify:



heap_extract_min:



heap_decrease_key:
	mv t1, s1 # t1 = s1 = i
	slli t2, t1, 2 # t2 byteoffset
	add t3, t2, s6 # t3 is addr of heap[i]
	lw t4, 0(t3) # t4 is heap[i]

	srli t5, t1, 1 # t5 = i/2
	slli t6, t5, 2 # t6 byteoffset
	add t0, t6, s6 # t0 is addr of heap[i / 2]

	bge x0, t1, heap_decrease_key_exit
	bge t4, t0, heap_decrease_key_exit
	




	heap_decrease_key_exit:
	ret


min_heap_insert:
	jal heap_decrease_key
	addi s1, s1, 1 # s1 ++ which means len ++


# Meet Error and Exit
error_exit:
	li a0, 17 # exit1 ecall
	li a1, -1 # exit code -1
	ecall

# Normal Exit
normal_exit:
	li a0, 17 # exit1 ecall
	li a1, 0 # exit code -1
	ecall
