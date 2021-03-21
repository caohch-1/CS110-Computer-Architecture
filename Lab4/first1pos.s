.data
shouldben1:	.asciiz "Should be -1, and firstposshift and firstposmask returned: "
shouldbe0:	.asciiz "Should be 0 , and firstposshift and firstposmask returned: "
shouldbe16:	.asciiz "Should be 16, and firstposshift and firstposmask returned: "
shouldbe31:	.asciiz "Should be 31, and firstposshift and firstposmask returned: "

.text
main:
	la	a1, shouldbe31
	jal	print_str
	lui	a1, 0x80000	# should be 31
	jal	first1posshift
	mv	a1, a0
	jal	print_int
	jal	print_space

	lui	a1, 0x80000
	jal	first1posmask
	mv	a1, a0
	jal	print_int
	jal	print_newline

	la	a1, shouldbe16
	jal	print_str
	lui	a1, 0x0010	# should be 16
	jal	first1posshift
	mv	a1, a0
	jal	print_int
	jal	print_space

	lui	a1, 0x0010
	jal	first1posmask
	mv	a1, a0
	jal	print_int
	jal	print_newline

	la	a1, shouldbe0
	jal	print_str
	li	a1, 1		# should be 0
	jal	first1posshift
	mv	a1, a0
	jal	print_int
	jal	print_space

	li	a1, 1
	jal	first1posmask
	mv	a1, a0
	jal	print_int
	jal	print_newline

	la	a1, shouldben1
	jal	print_str
	mv	a1, zero		# should be -1
	jal	first1posshift
	mv	a1, a0
	jal	print_int
	jal	print_space

	mv	a1, zero
	jal	first1posmask
	mv	a1, a0
	jal	print_int
	jal	print_newline

	li	a0, 10
	ecall

first1posshift:
	### YOUR CODE HERE ###
    li t1 32 # t1 = 32
    loop:
    beqz t1 end # if t1 == 0: end
    srli t2 a1 31 # t2 = test_num >> 31
    slli a1 a1 1 # test_num = test_num << 1
    addi t1 t1 -1 # t1 -= 1
    beqz t2 loop # if t2 == 0: loop
    
    mv a0 t1 # a0 = t1
    ret
    
    end:	
    li a0 -1 # a0 = -1
    ret
    
    

first1posmask:
	### YOUR CODE HERE ###
    li t1 32 # t1 = 32
    li t2 0x80000000 #mask = 0x80000000
    loop2:
    beqz t1 end2 # if t1 == 0: end
    and t3 a1 t2 # t3= test_num & mask
    addi t1 t1 -1 # t1 -= 1
    srl t3 t3 t1 # t3 = t3 >> t1
    srli t2 t2 1 # mask = mask >> 1
    beqz t3 loop2 # if t3 == 0: loop2
    
    mv a0 t1 # a0 = t1
    ret
    
    end2:
    li a0 -1 # a0 = -1
    ret

print_int:
	mv	a1, a0
	li	a0, 1
	ecall
	jr	ra

print_str:
	li	a0, 4
	ecall
	jr	ra

print_space:
	li	a1, ' '
	li	a0, 11
	ecall
	jr	ra

print_newline:
	li	a1, '\n'
	li	a0, 11
	ecall
	jr	ra
