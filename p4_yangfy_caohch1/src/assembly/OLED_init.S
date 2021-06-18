.section .text
.globl OLED_init
.type OLED_init, @function
OLED_init:
    # save the address of the calling function
    addi sp,sp,-4
    sw ra, 0(sp)
    # jump to IO_init()
    jal IO_init

    
    # clear the background with black color
    li a0, 0x0000
    jal LCD_Clear
    
    # read the address for return to its own function
    lw ra, 0(sp)
    addi sp, sp,4;
    ret
