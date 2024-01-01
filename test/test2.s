        .global main
main:
        stmfd   sp!,{r11,lr}
        sub     sp,sp,#8
        push    { r3,r12,r14 }
        sub     sp,sp,#4
        mov     r0,#40
        bl      putint
        add     sp,sp,#4
        pop     { r3,r12,r14 }
        mov     r0,#1
        add     sp,sp,#8
        ldmfd   sp!,{r11,lr}
        bx      lr
