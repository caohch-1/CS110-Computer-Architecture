.section .text
.globl StartUI
.type StartUI, @function
StartUI:
	# save the address of calling function to return at last
    addi sp,sp,-4
    sw ra, 0(sp)
    jal start_menu
    
    # clear the back with black color
    li a0, 0x0000
    jal LCD_Clear

    # return to the fuction for execute
    lw ra, 0(sp)
    addi sp, sp,4;
    ret
