        .arch armv7ve
        .text
        .syntax unified
        .arm
        .fpu    neon-vfpv4

        .global main
main:
        stmfd   sp!,{r4-r5,r11,lr}
        sub     sp,sp,#48
        movw    r5,#:lower16:test
        movt    r5,#:upper16:test
        mov     r3,sp
        mov     r1,#4
        str     r1,[r3]
        add     r3,sp,#4
        mov     r1,#3
        str     r1,[r3]
        add     r3,sp,#8
        mov     r1,#9
        str     r1,[r3]
        add     r3,sp,#12
        mov     r1,#2
        str     r1,[r3]
        add     r3,sp,#16
        mov     r1,#0
        str     r1,[r3]
        add     r3,sp,#20
        mov     r1,#1
        str     r1,[r3]
        add     r3,sp,#24
        mov     r1,#6
        str     r1,[r3]
        add     r3,sp,#28
        mov     r1,#5
        str     r1,[r3]
        add     r3,sp,#32
        mov     r1,#7
        str     r1,[r3]
        add     r3,sp,#36
        mov     r1,#8
        str     r1,[r3]
        mov     r3,r5
        mov     r1,#3
        str     r1,[r3]
        mov     r3,#10
mainLABEL13:
        cmp     r3,#10
        bge     mainLABEL22
mainLABEL16:
        mov     r1,sp
        add     r4,r1,r3,lsl 2
        ldr     r4,[r4]
        push    { r3,r12,r14 }
        sub     sp,sp,#4
        mov     r0,r4
        bl      putint
        add     sp,sp,#4
        pop     { r3,r12,r14 }
        push    { r3,r12,r14 }
        sub     sp,sp,#4
        mov     r0,#10
        bl      putch
        add     sp,sp,#4
        pop     { r3,r12,r14 }
        add     r3,r3,#1
        b       mainLABEL13
mainLABEL22:
        mov     r3,r5
        ldr     r3,[r3]
        mov     r0,r3
        add     sp,sp,#48
        ldmfd   sp!,{r4-r5,r11,lr}
        bx      lr
        .data
        .align  4
test:
        .long   1
        .long   2
        .zero   8
