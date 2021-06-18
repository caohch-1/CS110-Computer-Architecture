# DO NOT REMOVE THIS LINE WHEN SUBMITTING
.import input.S

.text
.globl	main
main:
	lw s1, len # int s1 = len
	lw s2, input_len # int s2 = input_len
	li s3, -1 # As a pop flag
	li s4, -2 # As a print flag
	la s5, input # s5 addr of input
	la s6, heap # a6 addr of heap
	li s7 2
	
	
	li t4, 0 # int t4 = 0

	main_loop:

	beq s2, zero, normal_exit

	slli t5, t4, 2 # for(t4 = 0; t4 < input_len; t4++)
	add t6, t5, s5 # t6 addr of input[t4]
	lw a1, 0(t6) # int a1 = input[t4]

	blt a1, x0, main_step1 # if a1 <= x0, to mian_step1
	jal min_heap_insert # a1 > 0, so insert
	j main_exit


	main_step1:
	bne a1, s3, main_step2 # if a1 != -1, check if print
	jal heap_extract_min  # a1 == -1, pop()
	j main_exit

	main_step2:
	bne a1, s4, error_exit # if a1 != -2, invliad input
	jal print_heap # a1 == -2, print()
	j main_exit

	main_exit:
	addi t4, t4, 1 # iter_ele ++
	bne t4, s2, main_loop # if iter_ele != input_len: do loop
	beq t4, s2, normal_exit # else: exit program



# Meet -2 and Print
print_heap:
	addi sp, sp, -20 #Store to stack
	sw a0, 0(sp) #Store to stack
	sw a1, 4(sp) #Store to stack
	sw t1, 8(sp) #Store to stack
	sw t2, 12(sp) #Store to stack
	sw t3, 16(sp) #Store to stack

	li t1, 0 # int t1 = 0
	
	
	print_loop: # for(t1 = 0; t1 < len; t1 ++)
	beq t1, s1, print_exit
	slli t2, t1, 2 
	add t3, t2, s6 # t3 addr of heap[t1]
	lw t3, 0(t3) # int t3 = heap[t1]

	li a0, 1 # print_int ecall
	mv a1, t3 # print t3
	ecall

	li a0, 11 # print_character ecall
	li a1, ' ' # print whitespace
	ecall

	addi t1, t1, 1 # t1 ++
	bne t1, s1, print_loop # if t1!=len: do loop

	lw t3, 16(sp) #Restore from stack
	lw t2, 12(sp) #Restore from stack
	lw t1, 8(sp) #Restore from stack
	lw a1, 4(sp) #Restore from stack
	lw a0, 0(sp) #Restore from stack
	addi sp, sp, 20

	print_exit:
	li a0, 11 # print_character ecall
	li a1, '\n' # print \n
	ecall

	ret

# Pop hepler function, here a2 is arg as i
min_heapify:
	addi, sp, sp, -52 #Store to stack
	sw t1, 0(sp) #Store to stack
	sw t2, 4(sp) #Store to stack
	sw t3, 8(sp) #Store to stack
	sw t4, 12(sp) #Store to stack
	sw t5, 16(sp) #Store to stack
	sw t6, 20(sp) #Store to stack
	sw t0, 24(sp) #Store to stack
	sw a2, 28(sp) #Store to stack
	sw a3, 32(sp) #Store to stack
	sw a4, 36(sp) #Store to stack
	sw a5, 40(sp) #Store to stack
	sw a6, 44(sp) #Store to stack
	sw ra, 48(sp) #Store to stack

	mv t1, a2
	slli t1, t1, 1
	addi t1, t1, 1 # t1 = l = 2 * i + 1

	mv t2, a2
	slli t2, t2, 1
	addi t2, t2, 2 # t2 = r = 2 * i + 2

	li t3, 0 # t3 = min

	slli t4, t1, 2 # byteoffset for heap[l]
	slli t5, t2, 2 # byteoffset for heap[r]
	slli t6, a2, 2 # byteoffset for heap[i]
	slli t0, t3, 2 # byteoffset for heap[min] 

	blt s1, t1, assign_min_i_for_min_heapify # if(len < l): do min = i
	add t4, t4, s6 # t4 addr of heap[l] 
	add t6, t6, s6 # t6 addr of heap[i]
	lw a4, 0(t4) # int a4 = heap[l]
	lw a6, 0(t6) # int a6 = heap[i]
	bge a4, a6, assign_min_i_for_min_heapify  # if(heap[l] >= heap[i]): do min = i
	mv t3, t1 # min = l
	mv a3, a4 # min updated,so update heap[min] (a3)
	slli t0, t3, 2 # update t0 addr of heap[min]
	add t0, t0, s6 # update t0 addr of heap[min]

	min_heapify_step1:
	blt s1, t2, min_heapify_step2 # if(len < r): skip this part
	add t5, t5, s6 # t5 addr of heap[r] 
	lw a5, 0(t5) # int a5 = heap[r]
	lw a3, 0(t0) # int a3 = heap[min]
	bge a5, a3, min_heapify_step2 # if(heap[r] >= heap[min]): skip this part
	mv t3, t2 # min = r
	mv a3, a5 # min updated,so update heap[min] (a3)
	slli t0, t3, 2 # update t0 addr of heap[min]
	add t0, t0, s6 # update t0 addr of heap[min]


	min_heapify_step2:
	beq t3, a2, min_heapify_exit # if min != i: return
	sw a3, 0(t6) # heap[i] = heap[min]
	sw a6, 0(t0) # heap[min] = heap[i]

	mv a2 t3 # let new arg(i) be min
	jal min_heapify # recurision
	j min_heapify_exit # recurision over
	

	assign_min_i_for_min_heapify:
	mv t3, a2 # min = i
	mv a3, a6 # min updated,so update heap[min] (a3)
	slli t0, t3, 2 # update t0 addr of heap[min]
	add t0, t0, s6 # update t0 addr of heap[min]
	j min_heapify_step1

	min_heapify_exit:
	lw ra, 48(sp) #Restore from stack
	lw a6, 44(sp) #Restore from stack
	lw a5, 40(sp) #Restore from stack
	lw a4, 36(sp) #Restore from stack
	lw a3, 32(sp) #Restore from stack
	lw a2, 28(sp) #Restore from stack
	lw t0, 24(sp) #Restore from stack
	lw t6, 20(sp) #Restore from stack
	lw t5, 16(sp) #Restore from stack
	lw t4, 12(sp) #Restore from stack
	lw t3, 8(sp) #Restore from stack
	lw t2, 4(sp) #Restore from stack
	lw t1, 0(sp) #Restore from stack
	addi sp, sp, 52 #Restore from stack
	ret



# Pop
heap_extract_min:
	addi sp, sp, -20 #Store to stack
	sw t1, 0(sp) #Store to stack
	sw t2, 4(sp) #Store to stack
	sw a0, 8(sp) #Store to stack
	sw a1, 12(sp) #Store to stack
	sw ra, 16(sp) #Store to stack


	beq s1, zero, error_exit # if len == 0: underflow

	lw t1, 0(s6) # int t1 = heap[0] = min(for print)
	addi t2, s1, -1 # int t2 = len - 1
	slli t2, t2, 2 # t2 addr of heap[len - 1]
	add t2, t2, s6 # t2 addr of heap[len - 1]
	lw t2, 0(t2) # int t2 = heap[len - 1]
	sw t2, 0(s6) # heap[0] = heap[len - 1]
	addi s1, s1, -1 # len --

	li a2, 0 # Set 2rd arg(i) to 0
	jal min_heapify

	li a0, 1 # print_int ecall
	mv a1, t1 # print t1=min
	ecall

	li a0, 11 # print_character ecall
	li a1, '\n' # print \n
	ecall

	lw ra, 16(sp) # Restore from stack
	lw a1, 12(sp) # Restore from stack
	lw a0, 8(sp) # Restore from stack
	lw t2, 4(sp) # Restore from stack
	lw t1, 0(sp) # Restore from stack
	addi sp, sp, 20 # Restore from stack

	ret

# Insert helper function
heap_decrease_key:
	addi sp, sp, -24 #Store to stack
	sw t1, 0(sp) #Store to stack
	sw t2, 4(sp) #Store to stack
	sw t3, 8(sp) #Store to stack
	sw t4, 12(sp) #Store to stack
	sw t5, 16(sp) #Store to stack
	sw t6, 20(sp) #Store to stack

	mv t1, s1 # int t1 = len = i(useful in this funct)

	
	slli t2, t1, 2 # t2 addr of heap[i]
	add t2, t2, s6 # t2 addr of heap[i]
	sw a1, 0(t2) # heap[i] = key
	lw t4, 0(t2) # int t4 = heap[i]

	mv t5, t1 # int t5 = i
	addi t5, t5, -1 # t5 -= 1
	div t5, t5, s7 # int t5 = t5 / 2
	slli t6, t5, 2 # t6 addr of heap[i / 2]
	add t6, t6, s6 # t6 addr of heap[i / 2]
	lw t3, 0(t6) # int t3 = heap[i / 2]

	

	heap_decrease_key_loop:
	bge x0, t1, heap_decrease_key_exit # if (i <= 0): return
	bge t4, t3, heap_decrease_key_exit # if (heap[i] >= heap[i / 2]): return

	sw t3, 0(t2) # heap[i] = heap[i / 2]
	sw t4, 0(t6) # heap[i / 2] = temp
	
	addi t1, t1, -1
	div t1, t1, s7 # int t1 = i / 2
	slli t2, t1, 2 # t2 addr of heap[i]
	add t2, t2, s6 # t2 addr of heap[i]
	lw t4, 0(t2) # int t4 = heap[i]

	mv t5, t1 # int t5 = i
	addi t5, t5, -1 # t5 -= 1
	div t5, t5, s7 # int t5 = t5 / 2
	slli t6, t5, 2 # t6 addr of heap[i / 2]
	add t6, t6, s6 # t6 addr of heap[i / 2]
	lw t3, 0(t6) # int t3 = heap[i / 2]

	j heap_decrease_key_loop
	
	heap_decrease_key_exit:
	lw t6, 20(sp) # Restore from stack
	lw t5, 16(sp) # Restore from stack
	lw t4, 12(sp) # Restore from stack
	lw t3, 8(sp) # Restore from stack
	lw t2, 4(sp) # Restore from stack
	lw t1, 0(sp) # Restore from stack
	addi sp, sp, 24 # Restore from stack
	ret

# Insert
min_heap_insert:
	addi sp, sp, -8 #Store to stack
	sw t0, 0(sp) #Store to stack
	sw ra, 4(sp) #Store to stack

	li t0, 32 # t0 = 32
	beq s1, t0, error_exit # if len == 32: overflow
	jal heap_decrease_key
	addi s1, s1, 1 # len ++

	lw ra, 4(sp) # Restore from stack
	lw t0, 0(sp) # Restore from stack
	addi sp, sp, 8 # Restore from stack
	ret


# Meet Error and Exit
error_exit:
	li a0, 17 # exit1 ecall
	li a1, -1 # exit code -1
	ecall

# Normal Exit
normal_exit:
	li a0, 17 # exit1 ecall
	li a1, 0 # exit code 0
	ecall
