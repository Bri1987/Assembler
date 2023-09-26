        .arch armv7ve
        .text
        .syntax unified
        .arm
        .fpu    neon-vfpv4

        .global func
func:
        stmfd   sp!,{r4-r5,r11,lr}
        sub     sp,sp,#16
        mov     r4,r0
        movw    r3,#:lower16:g
        movt    r3,#:upper16:g
        ldr     r5,[r3]
        add     r4,r5,r4
        mov     r1,r4
        str     r1,[r3]
        ldr     r4,[r3]
        push    { r3,r12,r14 }
        sub     sp,sp,#4
        mov     r0,r4
        bl      putint
        add     sp,sp,#4
        pop     { r3,r12,r14 }
        ldr     r3,[r3]
        mov     r0,r3
        add     sp,sp,#16
        ldmfd   sp!,{r4-r5,r11,lr}
        bx      lr

        .global main
main:
        stmfd   sp!,{r4-r5,r11,lr}
        sub     sp,sp,#8
        push    { r3,r12,r14 }
        sub     sp,sp,#4
        bl      getint
        add     sp,sp,#4
        pop     { r3,r12,r14 }
        mov     r3,r0
        cmp     r3,#10
        ble     mainLABEL7
mainLABEL3:
        push    { r3,r12,r14 }
        sub     sp,sp,#4
        mov     r0,r3
        bl      func
        add     sp,sp,#4
        pop     { r3,r12,r14 }
        mov     r3,r0
        cmp     r3,#0
        beq     mainLABEL7
mainLABEL6:
        b       mainLABEL8
mainLABEL7:
mainLABEL8:
        push    { r3,r12,r14 }
        sub     sp,sp,#4
        bl      getint
        add     sp,sp,#4
        pop     { r3,r12,r14 }
        mov     r4,r0
        cmp     r4,#11
        ble     mainLABEL15
mainLABEL11:
        push    { r3,r12,r14 }
        sub     sp,sp,#4
        mov     r0,r4
        bl      func
        add     sp,sp,#4
        pop     { r3,r12,r14 }
        mov     r3,r0
        cmp     r3,#0
        beq     mainLABEL15
mainLABEL14:
        b       mainLABEL16
mainLABEL15:
mainLABEL16:
        push    { r3,r12,r14 }
        sub     sp,sp,#4
        bl      getint
        add     sp,sp,#4
        pop     { r3,r12,r14 }
        mov     r3,r0
        cmp     r3,#99
        bgt     mainLABEL19
        b       mainLABEL22
mainLABEL19:
        push    { r3,r12,r14 }
        sub     sp,sp,#4
        mov     r0,r3
        bl      func
        add     sp,sp,#4
        pop     { r3,r12,r14 }
        mov     r3,r0
        cmp     r3,#0
        beq     mainLABEL23
mainLABEL22:
        b       mainLABEL24
mainLABEL23:
mainLABEL24:
        push    { r3,r12,r14 }
        sub     sp,sp,#4
        bl      getint
        add     sp,sp,#4
        pop     { r3,r12,r14 }
        mov     r3,r0
        cmp     r3,#100
        bgt     mainLABEL27
        b       mainLABEL30
mainLABEL27:
        push    { r3,r12,r14 }
        sub     sp,sp,#4
        mov     r0,r3
        bl      func
        add     sp,sp,#4
        pop     { r3,r12,r14 }
        mov     r3,r0
        cmp     r3,#0
        beq     mainLABEL31
mainLABEL30:
        b       mainLABEL32
mainLABEL31:
mainLABEL32:
        push    { r3,r12,r14 }
        sub     sp,sp,#4
        mov     r0,#99
        bl      func
        add     sp,sp,#4
        pop     { r3,r12,r14 }
        mov     r3,r0
        cmp     r3,#0
        movne   r3,#1
        moveq   r3,#0
        movne   r3,#0
        moveq   r3,#1
        cmp     r3,#0
        beq     mainLABEL41
mainLABEL37:
        push    { r3,r12,r14 }
        sub     sp,sp,#4
        mov     r0,#100
        bl      func
        add     sp,sp,#4
        pop     { r3,r12,r14 }
        mov     r3,r0
        cmp     r3,#0
        beq     mainLABEL41
mainLABEL40:
        b       mainLABEL42
mainLABEL41:
mainLABEL42:
        mov     r0,#0
        add     sp,sp,#8
        ldmfd   sp!,{r4-r5,r11,lr}
        bx      lr
        .data
        .align  4
g:
        .long   0
