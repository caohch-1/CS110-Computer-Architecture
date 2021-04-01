
#                               Project 1.2 RISC-V disassembler

#   You can suppose you will get the number of machine codes you should disassemble from 
#   "lines_of_machine_codes" and get the 32 bits-machine code from "machine_codes". 
#   Both of them are stored in file "input.S"

#   When we test your project, we will change the input files content.

# DO NOT REMOVE THIS LINE WHEN SUBMITTING
.import input.S

.data
# You can define your own data such as ".word", ".asciiz", ".byte"... here.
reg_x0: .asciiz "x0" # Init regs
reg_ra: .asciiz "ra"
reg_sp: .asciiz "sp"
reg_gp: .asciiz "gp" 
reg_tp: .asciiz "tp" # Init regs
reg_t0: .asciiz "t0"
reg_t1: .asciiz "t1"
reg_t2: .asciiz "t2"
reg_s0: .asciiz "s0" # Init regs
reg_s1: .asciiz "s1"
reg_a0: .asciiz "a0"
reg_a1: .asciiz "a1"
reg_a2: .asciiz "a2" # Init regs
reg_a3: .asciiz "a3"
reg_a4: .asciiz "a4"
reg_a5: .asciiz "a5"
reg_a6: .asciiz "a6" # Init regs
reg_a7: .asciiz "a7"
reg_s2: .asciiz "s2"
reg_s3: .asciiz "s3"
reg_s4: .asciiz "s4" # Init regs
reg_s5: .asciiz "s5"
reg_s6: .asciiz "s6"
reg_s7: .asciiz "s7"
reg_s8: .asciiz "s8" # Init regs
reg_s9: .asciiz "s9"
reg_s10: .asciiz "s10"
reg_s11: .asciiz "s11"
reg_t3: .asciiz "t3" # Init regs
reg_t4: .asciiz "t4"
reg_t5: .asciiz "t5"
reg_t6: .asciiz "t6"
comma: .asciiz ", " # Init symbols
left_parenthesis: .asciiz "("
right_parenthesis: .asciiz ")"
ins_add: .asciiz "add " # Init rtype
ins_or: .asciiz "or "
ins_slt: .asciiz "slt "
ins_sltu: .asciiz "sltu " 
ins_sll: .asciiz "sll "
ins_jalr: .asciiz "jalr " # Init itype1
ins_addi: .asciiz "addi "
ins_ori: .asciiz "ori "
ins_lb: .asciiz "lb " # Init itype2
ins_lbu: .asciiz "lbu "
ins_lw: .asciiz "lw "
ins_sb: .asciiz "sb " # Init stype
ins_sw: .asciiz "sw "
ins_beq: .asciiz "beq " # Init sbtype
ins_bne: .asciiz "bne "
ins_blt: .asciiz "blt "
ins_bge: .asciiz "bge "
ins_jal: .asciiz "jal " # Init ujtype
ins_lui: .asciiz "lui " # Init utype

.text
.globl main
main:
	lw s1 lines_of_machine_codes # s1 = lines_of_machine_codes
	la s2 machine_codes # s2 addr of machine_codes

	mv t1, zero # int i = 0

	main_loop:
	beq t1, s1, normal_return # if (i == lines_of_machine_codes): exit prog

	slli t2, t1, 2
	add t2, t2, s2 # t2 addr of machine_codes[i]
	lw t2, 0(t2) # int t2 = machine_codes[i]
	andi t3, t2, 0x7f # int opcode = machine_codes[i] & 0x7f;
	mv a1, t2 # a1 as arg of type functs

	judge_rtype:
	li t4, 0x33 # judge_type
	bne t3, t4, judge_itype1
	jal rtype
	addi t1, t1, 1 # i ++
	j main_loop

	judge_itype1:
	li t4, 0x67 # judge_type
	beq t3, t4, judge_itype1_sub # if (opcode == 0x67): itype1(machine_code)
	li t4, 0x13 # judge_type
	bne t3, t4, judge_itype2 # if (opcode != 0x13): check_if_itype2
	judge_itype1_sub:
	jal itype1
	addi t1, t1, 1
	j main_loop

	judge_itype2:
	li t4, 0x03 # judge_type
	bne t3, t4, judge_stype
	jal itype2
	addi t1, t1, 1 # i ++
	j main_loop

	judge_stype:
	li t4, 0x23 # judge_type
	bne t3, t4, judge_sbtype
	jal stype
	addi t1, t1, 1 # i ++
	j main_loop

	judge_sbtype:
	li t4, 0x63 # judge_type
	bne t3, t4, judge_ujtype
	jal sbtype
	addi t1, t1, 1 # i ++
	j main_loop

	judge_ujtype:
	li t4, 0x6f # judge_type
	bne t3, t4, judge_utype
	jal ujtype
	addi t1, t1, 1 # i ++
	j main_loop

	judge_utype:
	li t4, 0x37 # judge_type
	bne t3, t4, error_return
	jal utype
	addi t1, t1, 1 # i ++
	j main_loop

# normal return
normal_return:
	li a0, 17
	li a1, 0
	ecall

# wrong return, since all valid inputs it shouldn't be used
error_return:
	li a0, 17
	li a1, -1
	ecall

# void rtype(int machine_code)
# here input arg machine_code is a1
rtype:
	addi sp, sp, -36 # store regs
	sw t1, 0(sp)
	sw t2, 4(sp)
	sw t3, 8(sp)
	sw t4, 12(sp) # store regs
	sw t5, 16(sp)
	sw t6, 20(sp)
	sw a0, 24(sp)
	sw a1, 28(sp) # store regs
	sw ra, 32(sp)

	li t6, 0xfe000000
	and t1, a1, t6 # int funct7 = machine_code & 0xfe000000
	srli t1, t1, 25 # funct7 = funct7 >> 25

	li t6, 0x01f00000
	and t2, a1, t6 # int rs2 = machine_code & 0x01f00000
	srli t2, t2, 20 # rs2 = rs2 >> 20

	li t6, 0x000f8000
	and t3, a1, t6 # int rs1 = machine_code & 0x000f8000
	srli t3, t3, 15 # rs1 = rs1 >> 15

	li t6, 0x00007000
	and t4, a1, t6 # int funct3 = machine_code & 0x00007000
	srli t4, t4, 12 # funct3 = funct3 >> 12

	li t6, 0x00000f80
	and t5, a1, t6 # int rd = machine_code & 0x00000f80
	srli t5, t5, 7 # rd = rd >> 7

	rtype_add:
	li t6, 0x0 # check_funct3
	bne t4, t6, rtype_or
	li a0, 4 # print "add "
	la a1, ins_add
	ecall
	j rtype_print

	rtype_or:
	li t6, 0x6 # check_funct3
	bne t4, t6, rtype_slt
	li a0, 4 # print "or "
	la a1, ins_or
	ecall
	j rtype_print

	rtype_slt:
	li t6, 0x2 # check_funct3
	bne t4, t6, rtype_sltu
	li a0, 4 # print "slt "
	la a1, ins_slt
	ecall
	j rtype_print

	rtype_sltu:
	li t6, 0x3 # check_funct3
	bne t4, t6, rtype_sll
	li a0, 4 # print "sltu "
	la a1, ins_sltu
	ecall
	j rtype_print

	rtype_sll:
	li t6, 0x1 # check_funct3
	bne t4, t6, error_return
	li a0, 4 # print "sll "
	la a1, ins_sll
	ecall
	j rtype_print


	rtype_print:
	mv a1, t5 # Arg a1 = rd
	jal print_reg
	jal print_comma
	mv a1, t3 # Arg a1 = rs1
	jal print_reg
	jal print_comma
	mv a1, t2 # Arg a1 = rs2
	jal print_reg
	jal print_newline

	lw t1, 0(sp) # restore regs
	lw t2, 4(sp)
	lw t3, 8(sp)
	lw t4, 12(sp)
	lw t5, 16(sp) # restore regs
	lw t6, 20(sp)
	lw a0, 24(sp)
	lw a1, 28(sp)
	lw ra, 32(sp) # restore regs
	addi sp, sp, 36

	ret


# void itype1(int machine_code)
# here input arg machine_code is a1
itype1:
	addi sp, sp, -36 # store regs
	sw t1, 0(sp)
	sw t2, 4(sp)
	sw t3, 8(sp)
	sw t4, 12(sp) # store regs
	sw t5, 16(sp)
	sw t6, 20(sp)
	sw a0, 24(sp)
	sw a1, 28(sp) # store regs
	sw ra, 32(sp)

	li t6, 0xfff00000
	and t1, a1, t6 # int imm = machine_code & 0xfff00000
	srai t1, t1, 20 # imm = imm >> 20

	li t6, 0x000f8000
	and t3, a1, t6 # int rs1 = machine_code & 0x000f8000
	srli t3, t3, 15 # rs1 = rs1 >> 15

	li t6, 0x00007000
	and t4, a1, t6 # int funct3 = machine_code & 0x00007000
	srli t4, t4, 12 # funct3 = funct3 >> 12

	li t6, 0x00000f80
	and t5, a1, t6 # int rd = machine_code & 0x00000f80
	srli t5, t5, 7 # rd = rd >> 7

	li t6, 0x0000007f
	and t2, a1, t6 # int opcode = machine_code & 0x0000007f

	itype1_jalr:
	li t6, 0x67 # check_opcode
	bne t2, t6, itype1_addi
	li t6, 0x0 # check_funct3
	bne t4, t6, itype1_addi
	li a0, 4 # print "jalr "
	la a1, ins_jalr
	ecall
	j itype1_print

	itype1_addi:
	li t6, 0x13 # check_opcode
	bne t2, t6, itype1_ori
	li t6, 0x0 # check_funct3
	bne t4, t6, itype1_ori
	li a0, 4 # print "addi "
	la a1, ins_addi
	ecall
	j itype1_print

	itype1_ori:
	li t6, 0x13 # check_opcode
	bne t2, t6, error_return
	li t6, 0x6 # check_funct3
	bne t4, t6, error_return
	li a0, 4 # print "ori "
	la a1, ins_ori
	ecall
	j itype1_print

	itype1_print:
	mv a1, t5 # Arg a1 = rd
	jal print_reg
	jal print_comma
	mv a1, t3 # Arg a1 = rs1
	jal print_reg
	jal print_comma
	mv a1, t1 # Arg a1 = imm
	jal print_imm
	jal print_newline

	lw t1, 0(sp) # restore regs
	lw t2, 4(sp)
	lw t3, 8(sp)
	lw t4, 12(sp)
	lw t5, 16(sp) # restore regs
	lw t6, 20(sp)
	lw a0, 24(sp)
	lw a1, 28(sp)
	lw ra, 32(sp) # restore regs
	addi sp, sp, 36

	ret

# void itype2(int machine_code)
# here input arg machine_code is a1
itype2:
	addi sp, sp, -36 # store regs
	sw t1, 0(sp)
	sw t2, 4(sp) # Unused
	sw t3, 8(sp)
	sw t4, 12(sp) # store regs
	sw t5, 16(sp)
	sw t6, 20(sp)
	sw a0, 24(sp)
	sw a1, 28(sp) # store regs
	sw ra, 32(sp)

	li t6, 0xfff00000
	and t1, a1, t6 # int imm = machine_code & 0xfff00000
	srai t1, t1, 20 # imm = imm >> 20

	li t6, 0x000f8000
	and t3, a1, t6 # int rs1 = machine_code & 0x000f8000
	srli t3, t3, 15 # rs1 = rs1 >> 15

	li t6, 0x00007000
	and t4, a1, t6 # int funct3 = machine_code & 0x00007000
	srli t4, t4, 12 # funct3 = funct3 >> 12

	li t6, 0x00000f80
	and t5, a1, t6 # int rd = machine_code & 0x00000f80
	srli t5, t5, 7 # rd = rd >> 7

	itype2_lb:
	li t6, 0x0 # check_funct3
	bne t4, t6, itype2_lbu
	li a0, 4 # print "lb "
	la a1, ins_lb
	ecall
	j itype2_print

	itype2_lbu:
	li t6, 0x4 # check_funct3
	bne t4, t6, itype2_lw
	li a0, 4 # print "lbu "
	la a1, ins_lbu
	ecall
	j itype2_print

	itype2_lw:
	li t6, 0x2 # check_funct3
	bne t4, t6, error_return
	li a0, 4 # print "lw "
	la a1, ins_lw
	ecall
	j itype2_print

	itype2_print:
	mv a1, t5 # Arg a1 = rd
	jal print_reg
	jal print_comma
	mv a1, t1 # Arg a1 = imm
	jal print_imm
	jal print_left_parenthesis
	mv a1, t3 # Arg a1 = rs1
	jal print_reg
	jal print_right_parenthesis
	jal print_newline

	lw t1, 0(sp) # restore regs
	lw t2, 4(sp)
	lw t3, 8(sp)
	lw t4, 12(sp)
	lw t5, 16(sp) # restore regs
	lw t6, 20(sp)
	lw a0, 24(sp)
	lw a1, 28(sp)
	lw ra, 32(sp) # restore regs
	addi sp, sp, 36

	ret

# void stype(int machine_code)
# here input arg machine_code is a1
stype:
	addi sp, sp, -36 # store regs
	sw t1, 0(sp)
	sw t2, 4(sp) 
	sw t3, 8(sp)
	sw t4, 12(sp) # store regs
	sw t5, 16(sp) # Unused
	sw t6, 20(sp)
	sw a0, 24(sp)
	sw a1, 28(sp) # store regs
	sw ra, 32(sp)

	li t6, 0xfe000000
	and t1, a1, t6 # int imm = machine_code & 0x01f00000
	srai t1, t1, 20 # imm[11:5]
	li t6, 0x00000f80
	and t6, a1, t6 # int temp = machine_code & 0x00000f80
	srli t6, t6, 7 # imm[4:0]
	or t1, t1, t6 # full imm

	li t6, 0x01f00000
	and t2, a1, t6 # int rs2 = machine_code & 0x01f00000
	srli t2, t2, 20 # rs2 = rs2 >> 20

	li t6, 0x000f8000
	and t3, a1, t6 # int rs1 = machine_code & 0x000f8000
	srli t3, t3, 15 # rs1 = rs1 >> 15

	li t6, 0x00007000
	and t4, a1, t6 # int funct3 = machine_code & 0x00007000
	srli t4, t4, 12 # funct3 = funct3 >> 12

	stype_sb:
	li t6, 0x0 # check_funct3
	bne t4, t6, stype_sw
	li a0, 4 # print "sb "
	la a1, ins_sb
	ecall
	j stype_print

	stype_sw:
	li t6, 0x2 # check_funct3
	bne t4, t6, error_return
	li a0, 4 # print "sw "
	la a1, ins_sw
	ecall
	j stype_print

	stype_print:
	mv a1, t2 # Arg a1 = rs2
	jal print_reg
	jal print_comma
	mv a1, t1 # Arg a1 = imm
	jal print_imm
	jal print_left_parenthesis
	mv a1, t3 # Arg a1 = rs1
	jal print_reg
	jal print_right_parenthesis
	jal print_newline


	lw t1, 0(sp) # restore regs
	lw t2, 4(sp)
	lw t3, 8(sp)
	lw t4, 12(sp)
	lw t5, 16(sp) # restore regs
	lw t6, 20(sp)
	lw a0, 24(sp)
	lw a1, 28(sp)
	lw ra, 32(sp) # restore regs
	addi sp, sp, 36

	ret

# void sbtype(int machine_code)
# here input arg machine_code is a1
sbtype:
	addi sp, sp, -36 # store regs
	sw t1, 0(sp)
	sw t2, 4(sp) 
	sw t3, 8(sp)
	sw t4, 12(sp) # store regs
	sw t5, 16(sp) # Unused
	sw t6, 20(sp)
	sw a0, 24(sp)
	sw a1, 28(sp) # store regs
	sw ra, 32(sp)

	li t6, 0x80000000
	and t1, a1, t6 # int imm = machine_code & 0x80000000
	srai t1, t1, 19 # imm[12]
	li t6, 0x00000080
	and t6, a1, t6 # int temp = machine_code & 0x00000080
	slli t6, t6, 4 # imm[11]
	or t1, t1, t6 # imm[12:11]
	li t6, 0x7e000000
	and t6, a1, t6 # int temp = machine_code & 0x7e000000
	srli t6, t6, 20 # imm[10:5]
	or t1, t1, t6 # imm[12:5]
	li t6, 0x00000f00
	and t6, a1, t6 # int temp = machine_code & 0x00000f00
	srli t6, t6, 7 # imm[4:1]
	or t1, t1, t6 # full imm

	li t6, 0x01f00000
	and t2, a1, t6 # int rs2 = machine_code & 0x01f00000
	srli t2, t2, 20 # rs2 = rs2 >> 20

	li t6, 0x000f8000
	and t3, a1, t6 # int rs1 = machine_code & 0x000f8000
	srli t3, t3, 15 # rs1 = rs1 >> 15

	li t6, 0x00007000
	and t4, a1, t6 # int funct3 = machine_code & 0x00007000
	srli t4, t4, 12 # funct3 = funct3 >> 12

	sbtype_beq:
	li t6, 0x0 # check_funct3
	bne t4, t6, sbtype_bne
	li a0, 4 # print "beq "
	la a1, ins_beq
	ecall
	j sbtype_print

	sbtype_bne:
	li t6, 0x1 # check_funct3
	bne t4, t6, sbtype_blt
	li a0, 4 # print "bne "
	la a1, ins_bne
	ecall
	j sbtype_print

	sbtype_blt:
	li t6, 0x4 # check_funct3
	bne t4, t6, sbtype_bge
	li a0, 4 # print "blt "
	la a1, ins_blt
	ecall
	j sbtype_print

	sbtype_bge:
	li t6, 0x5 # check_funct3
	bne t4, t6, error_return
	li a0, 4 # print "bge "
	la a1, ins_bge
	ecall
	j sbtype_print

	sbtype_print:
	mv a1, t3 # Arg a1 = rs1
	jal print_reg
	jal print_comma
	mv a1, t2 # Arg a1 = rs2
	jal print_reg
	jal print_comma
	mv a1, t1 # Arg a1 = imm
	jal print_imm
	jal print_newline

	lw t1, 0(sp) # restore regs
	lw t2, 4(sp)
	lw t3, 8(sp)
	lw t4, 12(sp)
	lw t5, 16(sp) # restore regs
	lw t6, 20(sp)
	lw a0, 24(sp)
	lw a1, 28(sp)
	lw ra, 32(sp) # restore regs
	addi sp, sp, 36

	ret

# void ujtype(int machine_code)
# here input arg machine_code is a1
ujtype:
	addi sp, sp, -36 # store regs
	sw t1, 0(sp)
	sw t2, 4(sp) # Unused
	sw t3, 8(sp) # Unused
	sw t4, 12(sp) # Unused
	sw t5, 16(sp)
	sw t6, 20(sp)
	sw a0, 24(sp)
	sw a1, 28(sp) # store regs
	sw ra, 32(sp)

	li t6, 0x80000000
	and t1, a1, t6 # int imm = machine_code & 0x80000000
	srai t1, t1, 11 # imm[20]
	li t6, 0x000ff000
	and t6, a1, t6 # int temp = machine_code & 0x000ff000, imm[19:12]
	or t1, t1, t6 # imm[20:12]
	li t6, 0x00100000
	and t6, a1, t6 # int temp = machine_code & 0x00100000
	srli t6, t6, 9 # imm[11]
	or t1, t1, t6 # imm[20:11]
	li t6, 0x7fe00000
	and t6, a1, t6 # int temp = machine_code & 0x7fe00000
	srli t6, t6, 20 # imm[10:1]
	or t1, t1, t6 # full imm

	li t6, 0x00000f80
	and t5, a1, t6 # int rd = machine_code & 0x00000f80
	srli t5, t5, 7 # rd = rd >> 7

	li a0, 4 # print "jal "
	la a1, ins_jal
	ecall
	mv a1, t5 # Arg a1 = rd
	jal print_reg
	jal print_comma
	mv a1, t1 # Arg a1 = imm
	jal print_imm
	jal print_newline


	lw t1, 0(sp) # restore regs
	lw t2, 4(sp)
	lw t3, 8(sp)
	lw t4, 12(sp)
	lw t5, 16(sp) # restore regs
	lw t6, 20(sp)
	lw a0, 24(sp)
	lw a1, 28(sp)
	lw ra, 32(sp) # restore regs
	addi sp, sp, 36

	ret

# void utype(int machine_code)
# here input arg machine_code is a1
utype:
	addi sp, sp, -36 # store regs
	sw t1, 0(sp)
	sw t2, 4(sp) # Unused
	sw t3, 8(sp) # Unused
	sw t4, 12(sp) # Unused
	sw t5, 16(sp)
	sw t6, 20(sp)
	sw a0, 24(sp)
	sw a1, 28(sp) # store regs
	sw ra, 32(sp)

	li t6, 0xfffff000
	and t1, a1, t6  # int imm = machine_code & 0xfffff000
	srli t1, t1, 12 # full imm

	li t6, 0x00000f80
	and t5, a1, t6 # int rd = machine_code & 0x00000f80
	srli t5, t5, 7 # rd = rd >> 7

	li a0, 4 # print "lui "
	la a1, ins_lui
	ecall
	mv a1, t5 # Arg a1 = rd
	jal print_reg
	jal print_comma
	mv a1, t1 # Arg a1 = imm
	jal print_imm
	jal print_newline

	lw t1, 0(sp) # restore regs
	lw t2, 4(sp)
	lw t3, 8(sp)
	lw t4, 12(sp)
	lw t5, 16(sp) # restore regs
	lw t6, 20(sp)
	lw a0, 24(sp)
	lw a1, 28(sp)
	lw ra, 32(sp) # restore regs
	addi sp, sp, 36

	ret

# print imm
print_imm:
	addi sp, sp -4 # store regs
	sw a0, 0(sp)

	li a0, 1
	ecall

	lw a0, 0(sp) # restore regs
	addi sp, sp, 4

	ret

# print newline
print_newline:
	addi sp, sp -8 # store regs
	sw a1, 0(sp)
	sw a0, 4(sp)

	li a1, '\n'
	li a0, 11
	ecall

	lw a0, 4(sp) # restore regs
	lw a1, 0(sp)
	addi sp, sp, 8

	ret

# print comma
print_comma:
	addi sp, sp -8 # store regs
	sw a1, 0(sp)
	sw a0, 4(sp)

	la a1, comma
	li a0, 4
	ecall

	lw a0, 4(sp) # restore regs
	lw a1, 0(sp)
	addi sp, sp, 8

	ret

# print left parenthesis
print_left_parenthesis:
	addi sp, sp -8 # store regs
	sw a1, 0(sp)
	sw a0, 4(sp)

	la a1, left_parenthesis
	li a0, 4
	ecall

	lw a0, 4(sp) # restore regs
	lw a1, 0(sp)
	addi sp, sp, 8

	ret

# print right parenthesis
print_right_parenthesis:
	addi sp, sp -8 # store regs
	sw a1, 0(sp)
	sw a0, 4(sp)

	la a1, right_parenthesis
	li a0, 4
	ecall

	lw a0, 4(sp) # restore regs
	lw a1, 0(sp)
	addi sp, sp, 8

	ret

# void print_reg(int reg_num)
# here input arg reg_num is a1
print_reg:
	addi sp, sp, -16 # store regs
	sw t1, 0(sp)
	sw a0, 4(sp)
	sw a1, 8(sp)
	sw ra, 12(sp) # store regs


	x0_print:
	li t1, 0
	bne a1, t1, ra_print # print x0
	li a0, 4
	la a1, reg_x0
	ecall
	j print_exit

	ra_print:
	li t1, 1
	bne a1, t1, sp_print # print ra
	li a0, 4
	la a1, reg_ra
	ecall
	j print_exit

	sp_print:
	li t1, 2
	bne a1, t1, gp_print # print sp
	li a0, 4
	la a1, reg_sp
	ecall
	j print_exit

	gp_print:
	li t1, 3
	bne a1, t1, tp_print # print gp
	li a0, 4
	la a1, reg_gp
	ecall
	j print_exit

	tp_print:
	li t1, 4
	bne a1, t1, t0_print # print tp
	li a0, 4
	la a1, reg_tp
	ecall
	j print_exit

	t0_print:
	li t1, 5
	bne a1, t1, t1_print # print t0
	li a0, 4
	la a1, reg_t0
	ecall
	j print_exit

	t1_print:
	li t1, 6
	bne a1, t1, t2_print # print t1
	li a0, 4
	la a1, reg_t1
	ecall
	j print_exit

	t2_print:
	li t1, 7
	bne a1, t1, s0_print # print t2
	li a0, 4
	la a1, reg_t2
	ecall
	j print_exit

	s0_print:
	li t1, 8
	bne a1, t1, s1_print # print s0
	li a0, 4
	la a1, reg_s0
	ecall
	j print_exit

	s1_print:
	li t1, 9
	bne a1, t1, a0_print # print s1
	li a0, 4
	la a1, reg_s1
	ecall
	j print_exit

	a0_print:
	li t1, 10
	bne a1, t1, a1_print # print a0
	li a0, 4
	la a1, reg_a0
	ecall
	j print_exit

	a1_print:
	li t1, 11
	bne a1, t1, a2_print # print a1
	li a0, 4
	la a1, reg_a1
	ecall
	j print_exit

	a2_print:
	li t1, 12
	bne a1, t1, a3_print # print a2
	li a0, 4
	la a1, reg_a2
	ecall
	j print_exit

	a3_print:
	li t1, 13
	bne a1, t1, a4_print # print a3
	li a0, 4
	la a1, reg_a3
	ecall
	j print_exit

	a4_print:
	li t1, 14
	bne a1, t1, a5_print # print a4
	li a0, 4
	la a1, reg_a4
	ecall
	j print_exit

	a5_print:
	li t1, 15
	bne a1, t1, a6_print # print a5
	li a0, 4
	la a1, reg_a5
	ecall
	j print_exit

	a6_print:
	li t1, 16
	bne a1, t1, a7_print # print a6
	li a0, 4
	la a1, reg_a6
	ecall
	j print_exit

	a7_print:
	li t1, 17
	bne a1, t1, s2_print # print a7
	li a0, 4
	la a1, reg_a7
	ecall
	j print_exit

	s2_print:
	li t1, 18
	bne a1, t1, s3_print # print s2
	li a0, 4
	la a1, reg_s2
	ecall
	j print_exit

	s3_print:
	li t1, 19
	bne a1, t1, s4_print # print s3
	li a0, 4
	la a1, reg_s3
	ecall
	j print_exit

	s4_print:
	li t1, 20
	bne a1, t1, s5_print # print s4
	li a0, 4
	la a1, reg_s4
	ecall
	j print_exit

	s5_print:
	li t1, 21
	bne a1, t1, s6_print # print s5
	li a0, 4
	la a1, reg_s5
	ecall
	j print_exit

	s6_print:
	li t1, 22
	bne a1, t1, s7_print # print s6
	li a0, 4
	la a1, reg_s6
	ecall
	j print_exit

	s7_print:
	li t1, 23
	bne a1, t1, s8_print # print s7
	li a0, 4
	la a1, reg_s7
	ecall
	j print_exit

	s8_print:
	li t1, 24
	bne a1, t1, s9_print # print s8
	li a0, 4
	la a1, reg_s8
	ecall
	j print_exit

	s9_print:
	li t1, 25
	bne a1, t1, s10_print # print s9
	li a0, 4
	la a1, reg_s9
	ecall
	j print_exit

	s10_print:
	li t1, 26
	bne a1, t1, s11_print # print s10
	li a0, 4
	la a1, reg_s10
	ecall
	j print_exit

	s11_print:
	li t1, 27
	bne a1, t1, t3_print # print s11
	li a0, 4
	la a1, reg_s11
	ecall
	j print_exit

	t3_print:
	li t1, 28
	bne a1, t1, t4_print # print t3
	li a0, 4
	la a1, reg_t3
	ecall
	j print_exit

	t4_print:
	li t1, 29
	bne a1, t1, t5_print # print t4
	li a0, 4
	la a1, reg_t4
	ecall
	j print_exit

	t5_print:
	li t1, 30
	bne a1, t1, t6_print # print t5
	li a0, 4
	la a1, reg_t5
	ecall
	j print_exit

	t6_print:
	li t1, 31
	bne a1, t1, print_exit # print t6
	li a0, 4
	la a1, reg_t6
	ecall
	j print_exit

	print_exit:
	lw t1, 0(sp) # restore regs
	lw a0, 4(sp)
	lw a1, 8(sp)
	lw ra, 12(sp)
	addi sp, sp, 16 # restore regs
	ret