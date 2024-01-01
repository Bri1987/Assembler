        .global main
main:
        stmfd   sp!,{r11,lr}
        sub     sp,sp,#8
        mov     r0,#40
        add     sp,sp,#8
        ldmfd   sp!,{r11,lr}
        bx      lr