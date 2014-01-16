BOOTSEG = 0x07c0
.code16
.text
.global start
start:
    # Normalize the start address
    jmpl    $BOOTSEG, $start2
start2:
    movw    %cs, %ax
    movw    %ax, %ds
    movw    %ax, %es
    movb    %ah,(msg+17)                 #替换字符串中最后一个.符号为系统蜂鸣声
    movw    $20,%cx                           #共显示20个字符，包括回车换行符
    movw    $0x1004,%dx                   #字符串将显示的位置0x10行0x0x04列
    movw    $0x00c,%bx                     #字符显示属性（红色）
    movw    $msg,%bp                       #指向要显示的字符串的首址
    movw    $0x1301,%ax                  #BIOS中断调用0x10,功能0x13,子功能0x01
    int    $0x10
loop:    jmp    loop                             #死循环 
msg:    .ascii  "hmy"
    .byte    3,10
.org    510
    .word     0xAA55

