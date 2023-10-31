        .global P
P:
        stmfd   sp!,{r4-r5,r11,lr}
        sub     sp,sp,#16
        mov     r3,r0
        movw    r5,#:lower16:g
        movt    r5,#:upper16:g
        cmp     r3,#0
        ble     PLABEL6
PLABEL3:
        mov     r1,#0
        str     r1,[r5]
        ldr     r4,[r5]
        sub     r3,r3,r4
        push    { r3,r12,r14 }
        sub     sp,sp,#4
        mov     r0,r3
        bl      P
        add     sp,sp,#4
        pop     { r3,r12,r14 }
PLABEL6:
        add     sp,sp,#16
        ldmfd   sp!,{r4-r5,r11,lr}
        bx      lr

        .global main
main:
        stmfd   sp!,{r11,lr}
        sub     sp,sp,#8
        push    { r3,r12,r14 }
        sub     sp,sp,#4
        mov     r0,#0
        bl      P
        add     sp,sp,#4
        pop     { r3,r12,r14 }
        mov     r0,#1
        add     sp,sp,#8
        ldmfd   sp!,{r11,lr}
        bx      lr
        .data
        .align  4
g:
        .long   0
