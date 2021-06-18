.section .data
str:
.string "Press 0 to go left"

str1:
.string "Press 1 to go right"

str2:
.string "Eat    to lengthen"

str3:
.string "Eat    to shorten"

str4:
.string "Eat    to turn back"



.section .text
.globl HelpMenu_init
.type HelpMenu_init, @function
HelpMenu_init:
    # save the address of calling function to return at last
    addi sp, sp, -4
    sw ra, 0(sp)

    li a0, 0x0000
    jal LCD_Clear
    # LCD_ShowString(0, 0, (u8 *) ("Press 0 to go left"), WHITE);
    # LCD_ShowString(0, 16, (u8 *) ("Press 1 to go right"), WHITE);
    li a0, 0
    li a1, 0
    la a2, str
    li a3, 0xffff
    jal LCD_ShowString

    li a0, 0
    li a1, 16
    la a2, str1
    li a3, 0xffff
    jal LCD_ShowString

    # LCD_ShowString(0, 32, (u8 *) ("Eat    to lengthen"), GREEN);
    # LCD_DrawCircle(37, 38, 7, WHITE);
    li a0, 0
    li a1, 32
    la a2, str2
    li a3, 0x07E0
    jal LCD_ShowString

    
    li a0, 37
    li a1, 38
    li a2, 7
    li a3, 0xFFFF
    jal LCD_DrawCircle


    # LCD_ShowString(0, 48, (u8 *) ("Eat    to shorten"), GREEN);
    # LCD_DrawCircle(37, 54, 7, RED);
    li a0, 0
    li a1, 48
    la a2, str3
    li a3, 0x07E0
    jal LCD_ShowString


    li a0, 37
    li a1, 54
    li a2, 7
    li a3, 0xF800
    jal LCD_DrawCircle

    # LCD_ShowString(0, 64, (u8 *)("Eat    to turn back"), GREEN);
    # LCD_DrawCircle(37, 70, 7, GBLUE);
    li a0, 0
    li a1, 64
    la a2, str4
    li a3, 0x07E0
    jal LCD_ShowString

    li a0, 37
    li a1, 70
    li a2, 7
    li a3, 0x07FF
    jal LCD_DrawCircle
    # return to the fuction for execute 
    lw ra, 0(sp)
    addi sp, sp, 4
    ret

