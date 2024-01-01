        .global main
putint:
        sub     sp, sp, #4
        add     sp, sp, #4
        bx      lr
main:
        push    {r11, lr}
        mov     r11, sp
        sub     sp, sp, #8
        mov     r0, #0
        str     r0, [sp, #4]
        mov     r0, #10
        str     r0, [sp]
        bl      putint
        ldr     r0, [sp]
        mov     sp, r11
        pop     {r11, lr}
        bx      lr
