.section .data
str:
.string "Retro Snake"

str1:
.string "Play"

str2:
.string "Help"



.section .text
.globl Menu_init
.type Menu_init, @function
Menu_init:
    # save the address of calling function to return at last
    addi sp, sp, -4
    sw ra, 0(sp)

    # LCD_DrawCircle(10, 10, 5, WHITE);
# but for this,maybe red
# but for this,maybe gblue
    li a0, 10
    li a1, 10
    li a2, 5
    li a3, 0xFFFF
    jal LCD_DrawCircle
# LCD_DrawCircle(x, y, 5, WHITE);
# but for this,maybe red
# but for this,maybe gblue
    li a0, 20
    li a1, 10
    li a2, 5
    li a3, 0xF800
    jal LCD_DrawCircle
    li a0, 30
    li a1, 10
    li a2, 5
    li a3, 0x07FF
# LCD_DrawCircle(x, y, 5, WHITE);
# but for this,maybe red
# but for this,maybe gblue
    jal LCD_DrawCircle
    li a0, 40
    li a1, 10
    li a2, 5
    li a3, 0xFFFF
# LCD_DrawCircle(x, y, 5, WHITE);
# but for this,maybe red
# but for this,maybe gblue
    jal LCD_DrawCircle
    li a0, 50
    li a1, 10
    li a2, 5
    li a3, 0xF800
# LCD_DrawCircle(x, y, 5, WHITE);
# but for this,maybe red
# but for this,maybe gblue
    jal LCD_DrawCircle
    li a0, 60
    li a1, 10
    li a2, 5
    li a3, 0x07FF
# LCD_DrawCircle(x, y, 5, WHITE);
# but for this,maybe red
# but for this,maybe gblue
    jal LCD_DrawCircle
    li a0, 70
    li a1, 10
    li a2, 5
    li a3, 0xFFFF
# LCD_DrawCircle(x, y, 5, WHITE);
# but for this,maybe red
# but for this,maybe gblue
    jal LCD_DrawCircle
    li a0, 80
    li a1, 10
    li a2, 5
    li a3, 0xF800
# LCD_DrawCircle(x, y, 5, WHITE);
# but for this,maybe red
# but for this,maybe gblue
    jal LCD_DrawCircle
    li a0, 90
    li a1, 10
    li a2, 5
    li a3, 0x07FF
# LCD_DrawCircle(x, y, 5, WHITE);
# but for this,maybe red
# but for this,maybe gblue
    jal LCD_DrawCircle
    li a0, 100
    li a1, 10
    li a2, 5
    li a3, 0xFFFF
# LCD_DrawCircle(x, y, 5, WHITE);
# but for this,maybe red
# but for this,maybe gblue
    jal LCD_DrawCircle
    li a0, 110
    li a1, 10
    li a2, 5
    li a3, 0xF800
# LCD_DrawCircle(x, y, 5, WHITE);
# but for this,maybe red
# but for this,maybe gblue
    jal LCD_DrawCircle
    li a0, 120
    li a1, 10
    li a2, 5
    li a3, 0xFFFF
# LCD_DrawCircle(x, y, 5, WHITE);
# but for this,maybe gblue
# but for this,maybe red
    jal LCD_DrawCircle
    li a0, 130
    li a1, 10
    li a2, 5
    li a3, 0x07FF
# LCD_DrawCircle(x, 10, 5, WHITE);
# but for this,maybe red
# but for this,maybe gblue
    jal LCD_DrawCircle
    li a0, 140
    li a1, 10
    li a2, 5
    li a3, 0xF800
# LCD_DrawCircle(x, 10, 5, WHITE);
# but for this,maybe red
# but for this,maybe gblue
    jal LCD_DrawCircle
    li a0, 150
    li a1, 10
    li a2, 5
    li a3, 0xFFFF
# LCD_DrawCircle(10, y, 5, WHITE);
# but for this,maybe red
# but for this,maybe gblue
    jal LCD_DrawCircle
    li a0, 10
    li a1, 20
    li a2, 5
    li a3, 0x07FF
# LCD_DrawCircle(10, y, 5, WHITE);
# but for this,maybe red
# but for this,maybe gblue
    jal LCD_DrawCircle
    li a0, 10
    li a1, 30
    li a2, 5
    li a3, 0xF800
# LCD_DrawCircle(10, y, 5, WHITE);
# but for this,maybe red
# but for this,maybe gblue
    jal LCD_DrawCircle
    li a0, 10
    li a1, 40
    li a2, 5
    li a3, 0xFFFF
# LCD_DrawCircle(10, y, 5, WHITE);
# but for this,maybe red
# but for this,maybe gblue
    jal LCD_DrawCircle
    li a0, 10
    li a1, 50
    li a2, 5
    li a3, 0x07FF
# LCD_DrawCircle(10, y, 5, WHITE);
# but for this,maybe red
# but for this,maybe gblue
    jal LCD_DrawCircle
    li a0, 10
    li a1, 60
    li a2, 5
    li a3, 0xF800
# LCD_DrawCircle(10, y, 5, WHITE);
# but for this,maybe red
# but for this,maybe gblue
    jal LCD_DrawCircle
    li a0, 10
    li a1, 70
    li a2, 5
    li a3, 0xFFFF
# LCD_DrawCircle(10, y 5, WHITE);
# but for this,maybe red
# but for this,maybe gblue
    jal LCD_DrawCircle
    li a0, 150
    li a1, 20
    li a2, 5
    li a3, 0x07FF
# LCD_DrawCircle(10, y, 5, WHITE);
# but for this,maybe red
# but for this,maybe gblue
    jal LCD_DrawCircle
    li a0, 150
    li a1, 30
    li a2, 5
    li a3, 0xF800
# but for this,maybe red
# LCD_DrawCircle(x, y, 5, WHITE);
# but for this,maybe gblue
    jal LCD_DrawCircle
    li a0, 150
    li a1, 40
    li a2, 5
    li a3, 0xFFFF
# LCD_DrawCircle(x, y, 5, WHITE);
# but for this,maybe red
# but for this,maybe gblue
    jal LCD_DrawCircle
    li a0, 150
    li a1, 50
    li a2, 5
    li a3, 0x07FF
# LCD_DrawCircle(x, y, 5, WHITE)
# but for this,maybe red
# but for this,maybe gblue
    jal LCD_DrawCircle
    li a0, 150
    li a1, 60
    li a2, 5
    li a3, 0xF800
# LCD_DrawCircle(x, y, 5, WHITE);
# but for this,maybe red
# but for this,maybe gblue
    jal LCD_DrawCircle
    li a0, 150
    li a1, 80
    li a2, 5
    li a3, 0xFFFF
# LCD_DrawCircle(x, y, 5, WHITE);
# but for this,maybe red
# but for this,maybe gblue
    jal LCD_DrawCircle
    li a0, 20
    li a1, 80
    li a2, 5
    li a3, 0xF800
# LCD_DrawCircle(x, y, 5, WHITE);
# but for this,maybe red
# but for this,maybe gblue
    jal LCD_DrawCircle
    li a0, 30
    li a1, 80
    li a2, 5
    li a3, 0x07FF
# LCD_DrawCircle(x, y, 5, WHITE);
# but for this,maybe red
# but for this,maybe gblue
    jal LCD_DrawCircle
    li a0, 40
    li a1, 80
    li a2, 5
    li a3, 0xFFFF
# LCD_DrawCircle(x, y, 5, WHITE);
# but for this,maybe red
# but for this,maybe gblue
    jal LCD_DrawCircle
    li a0, 50
    li a1, 80
    li a2, 5
    li a3, 0xF800
# LCD_DrawCircle(x, y, 5, WHITE);
# but for this,maybe red
# but for this,maybe gblue
    jal LCD_DrawCircle
    li a0, 60
    li a1, 80
    li a2, 5
    li a3, 0xFFFF
# LCD_DrawCircle(x, y, 5, WHITE);
# but for this,maybe red
# but for this,maybe gblue
    jal LCD_DrawCircle
    li a0, 70
    li a1, 80
    li a2, 5
    li a3, 0x07FF
# LCD_DrawCircle(x, y, 5, WHITE);
# but for this,maybe red
# but for this,maybe gblue
    jal LCD_DrawCircle
    li a0, 80
    li a1, 80
    li a2, 5
    li a3, 0xF800
# LCD_DrawCircle(x, y, 5, WHITE);
# but for this,maybe red
# but for this,maybe gblue
    jal LCD_DrawCircle
    li a0, 90
    li a1, 80
    li a2, 5
    li a3, 0xFFFF
# LCD_DrawCircle(x, y, 5, WHITE);
# but for this,maybe red
# but for this,maybe gblue
    jal LCD_DrawCircle
    li a0, 100
    li a1, 80
    li a2, 5
    li a3, 0x07FF
# LCD_DrawCircle(10, 10, 5, WHITE);
# but for this,maybe red
# but for this,maybe gblue
    jal LCD_DrawCircle
    li a0, 110
    li a1, 80
    li a2, 5
    li a3, 0xF800
# LCD_DrawCircle(x, y, 5, WHITE);
# but for this,maybe red
# but for this,maybe gblue
    jal LCD_DrawCircle
    li a0, 120
    li a1, 80
    li a2, 5
    li a3, 0xFFFF
# LCD_DrawCircle(x, y, 5, WHITE);
# but for this,maybe red
# but for this,maybe gblue
    jal LCD_DrawCircle
    li a0, 130
    li a1, 80
    li a2, 5
    li a3, 0x07FF
# LCD_DrawCircle(x, y, 5, WHITE);
# but for this,maybe red
# but for this,maybe gblue
    jal LCD_DrawCircle
    li a0, 140
    li a1, 80
    li a2, 5
    li a3, 0xF800
# LCD_DrawCircle(1x, y, 5, WHITE);
# but for this,maybe red
# but for this,maybe gblue
    jal LCD_DrawCircle
    li a0, 150
    li a1, 80
    li a2, 5
    li a3, 0xF800
# LCD_DrawCircle(x, y, 5, WHITE);
# but for this,maybe red
# but for this,maybe gblue
    jal LCD_DrawCircle
    li a0, 10
    li a1, 80
    li a2, 5
    li a3, 0xF800
# LCD_DrawCircle(x, y, 5, WHITE);
# but for this,maybe red
# but for this,maybe gblue
    jal LCD_DrawCircle
    li a0, 150
    li a1, 70
    li a2, 5
    li a3, 0xF800
    jal LCD_DrawCircle

    # title is Retro snake
    li a0, 40
    li a1, 20
    la a2, str
    li a3, 0x07E0
    jal LCD_ShowString

    # has a selection "play"
    li a0, 40
    li a1, 50
    la a2, str1
    li a3, 0xF800
    jal LCD_ShowString
    
    # has a selection "help"
    li a0, 90
    li a1, 50
    la a2, str2
    li a3, 0xFFE0
    jal LCD_ShowString

    # has a selection of Rectangle
    li a0, 35
    li a1, 45
    li a2, 75
    li a3, 70
    li a4, 0xFFFF
    jal LCD_DrawRectangle
    # return to the fuction for execute
    lw ra, 0(sp)
    addi sp, sp, 4
    ret
