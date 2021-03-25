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
	slli t5, t4, 2 # int t5 byteoffset of input
	add t6, t5, s5 # int* t6 = t5(input)
	lw a1, 0(t6) # int a1 = *t6 as argument

	blt x0, a1, main_step1 # if a1 < x0, to mian_step1
	jal min_heap_insert
	j main_exit


	main_step1:
	bne a1, s3, main_step2 # if a1 == -1, pop(s5)
	jal heap_extract_min  
	j main_exit

	main_step2:
	beq a1, s4, error_exit # if a1 == -2, print(s5)
	jal print_heap
	j main_exit

	main_exit:
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

# Pop hepler function, here a2 is arg as i
min_heapify:
	addi, sp, sp, -52
	sw t1, 0(sp)
	sw t2, 4(sp)
	sw t3, 8(sp)
	sw t4, 12(sp)
	sw t5, 16(sp)
	sw t6, 20(sp)
	sw t0, 24(sp)
	sw a2, 28(sp)
	sw a3, 32(sp)
	sw a4, 36(sp)
	sw a5, 40(sp)
	sw a6, 44(sp)
	sw ra, 48(sp)

	mv t1, a2
	slli t1, t1, 1
	addi t1, t1, 1 # t1 = l = 2 * i + 1

	mv t2, a2
	slli t2, t2, 1
	addi t2, t2, 1 # t2 = r = 2 * i + 1

	li t3, 0 # t3 = min

	slli t4, t1, 2 # byteoffset for heap[l]
	slli t5, t2, 2 # byteoffset for heap[r]
	slli t6, a2, 2 # byteoffset for heap[i]
	slli t0, t3, 2 # byteoffset for heap[min] 

	add t4, t4, s6 # t4 addr of heap[l] 
	add t5, t5, s6 # t5 addr of heap[r] 
	add t6, t6, s6 # t6 addr of heap[i] 
	add t0, t0, s6 # t0 addr of heap[min] 

	lw a4, 0(t4) # a4 = heap[l]
	lw a5, 0(t5) # a5 = heap[r]
	lw a6, 0(t6) # a6 = heap[i]
	lw a3, 0(t0) # a3 = heap[min]
	

	blt s1, t1, assign_min_l_for_min_heapify
	bge a4, a6, assign_min_l_for_min_heapify
	mv t3, a2 # min = i

	min_heapify_step1:
	blt s1, t2, assign_min_r_for_min_heapify
	bge a5, a3, assign_min_r_for_min_heapify

	beq t3, a2, min_heapify_exit
	sw a3, 0(t6) # heap[i] = heap[min]
	sw a6, 0(t0) # heap[min] = heap[i]

	mv a2 t3 # let arg i be min
	jal min_heapify # recurision
	j min_heapify_exit
	


	assign_min_l_for_min_heapify:
	mv t3, t1 # min = l
	j min_heapify_step1

	assign_min_r_for_min_heapify:
	mv t3, t2 # min = r
	j min_heapify_step1

	min_heapify_exit:
	lw ra, 48(sp)
	lw a6, 44(sp)
	lw a5, 40(sp)
	lw a4, 36(sp)
	lw a3, 32(sp)
	lw a2, 28(sp)
	lw t0, 24(sp)
	lw t6, 20(sp)
	lw t5, 16(sp)
	lw t4, 12(sp)
	lw t3, 8(sp)
	lw t2, 4(sp)
	lw t1, 0(sp)
	ret



# Pop
heap_extract_min:
	addi sp, sp, -20 #Store to stack
	sw t1, 0(sp)
	sw t2, 4(sp)
	sw a0, 8(sp)
	sw a1, 12(sp)
	sw ra, 16(sp)


	beq s1, zero, error_exit

	lw t1, 0(s6) # int t1= min = heap[0]
	addi t2, s1, -1 # int t2 = len - 1
	slli t2, t2, 2 # byteoffset
	add t2, t2, s6 # t2 is now addr of heap[len - 1]
	lw t2, 0(t2) # t2 is now heap[len - 1]
	addi s1, s1, -1

	li a2, 0 # as 2rd arg of min_heapify
	jal min_heapify

	li a0, 1 # print_int ecall
	mv a1, t1 # print t1=min
	ecall

	li a0, 11 # print_character ecall
	li a1, '\n' # print \n
	ecall

	lw ra, 16(sp)
	lw a1, 12(sp) # Restore from stack
	lw a0, 8(sp)
	lw t2, 4(sp)
	lw t1, 0(sp)
	addi sp, sp, 16

	ret

# Insert helper function
heap_decrease_key:
	addi sp, sp, -24 #Store to stack
	sw t1, 0(sp)
	sw t2, 4(sp)
	sw t3, 8(sp)
	sw t4, 12(sp)
	sw t5, 16(sp)
	sw t6, 20(sp)

	mv t1, s1 # t1 = s1 = i

	
	slli t2, t1, 2 # t2 byteoffset
	add t2, t2, s6 # t2 is addr of heap[i]
	lw t4, 0(t2) # t4 is heap[i]

	srli t5, t1, 1 # t5 = i/2
	slli t6, t5, 2 # t6 byteoffset
	add t6, t6, s6 # t6 is addr of heap[i / 2]
	lw t3, 0(t6) # t3 is heap[i / 2]

	sw a1, 0(t2) # heap[i] = key

	heap_decrease_key_loop:
	bge x0, t1, heap_decrease_key_exit
	bge t4, t3, heap_decrease_key_exit

	mv t0, t4 # t0 = temp = heap[i]
	sw t3, 0(t2) # heap[i] = heap[i / 2]
	sw t0, 0(t6) # heap[i / 2] = temp
	
	srli t1, t1, 1 # i ++
	slli t2, t1, 2 # t2 byteoffset
	add t2, t2, s6 # t2 is addr of heap[i]
	lw t4, 0(t2) # t4 is heap[i]

	srli t5, t1, 1 # t5 = i/2
	slli t6, t5, 2 # t6 byteoffset
	add t6, t6, s6 # t6 is addr of heap[i / 2]
	lw t3, 0(t6) # t3 is heap[i / 2]

	j heap_decrease_key_loop
	
	heap_decrease_key_exit:
	lw t6, 20(sp)
	lw t5, 16(sp)
	lw t4, 12(sp)
	lw t3, 8(sp)
	lw t2, 4(sp)
	lw t1, 0(sp)
	addi sp, sp, 24
	ret

# Insert
min_heap_insert:
	addi sp, sp, -8
	sw t0, 0(sp)
	sw ra, 4(sp)

	li t0, 32 # t0 = 32
	beq s1, t0, error_exit # if len == 32: exit(-1)
	jal heap_decrease_key
	addi s1, s1, 1 # s1 ++ which means len ++

	lw ra, 4(sp)
	lw t0, 0(sp)
	addi sp, sp, 8
	ret


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
