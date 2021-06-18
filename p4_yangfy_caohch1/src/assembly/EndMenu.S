.section .data
str:
.string "Retry"

str1:
.string "Quit"
# save string into str

# this fuction is to draw the dead menu


.section .text
.globl EndMenu
.type EndMenu, @function
EndMenu:
    # save the address of calling function to return at last
    addi sp, sp, -4
    sw ra, 0(sp)


    
    # LCD_ShowString(40, 50, (u8 *) ("Retry"), RED);
    # LCD_ShowString(95, 50, (u8 *) ("Quit"), YELLOW);
    # LCD_DrawRectangle(35, 45, 80, 70, WHITE);
    
    # Retry
    li a0, 40
    li a1, 50
    la a2, str
    li a3, 0xF800
    jal LCD_ShowString
    
    # Quit
    li a0, 95
    li a1, 50
    la a2, str1
    li a3, 0xFFE0
    jal LCD_ShowString
    
    # the rectangle show which will be chosen by user
    li a0, 35
    li a1, 45
    li a2, 80
    li a3, 70
    li a4, 0xFFFF
    jal LCD_DrawRectangle
    # return to the fuction for execute
    lw ra, 0(sp)
    addi sp, sp, 4
    ret
    
