 .arch armv7ve
        .text
        .syntax unified
        .arm
        .fpu    neon-vfpv4

        .global equal
equal:
        stmfd   sp!,{r4-r5,r11,lr}
        sub     sp,sp,#24
        mov     r3,r1
        mov     r4,r0
        cmp     r4,r3
        bne     equalLABEL5
equalLABEL4:
        mov     r3,#1
        b       equalLABEL6
equalLABEL5:
        mov     r3,#0
equalLABEL6:
        mov     r0,r3
        add     sp,sp,#24
        ldmfd   sp!,{r4-r5,r11,lr}
        bx      lr

        .global dfs
dfs:
        stmfd   sp!,{r4-r12,lr}
        sub     sp,sp,#40
        mov     r5,r3
        mov     r6,r2
        mov     r4,r1
        mov     r7,r0
        movw    r0,#:lower16:dp
        movt    r0,#:upper16:dp
        str     r0,[sp,#24]
        ldr     r8,[sp,#84]
        ldr     r1,[sp,#24]
        movw    r2,#0xd9c0
        movt    r2,#0x2c
        mla     r3,r7,r2,r1
        movw    r2,#0x7de0
        movt    r2,#0x2
        mla     r3,r4,r2,r3
        movw    r2,#0x2370
        movt    r2,#0x0
        mla     r3,r6,r2,r3
        mov     r2,#504
        mla     r3,r5,r2,r3
        mov     r2,#28
        ldr     r0,[sp,#80]
        mla     r3,r0,r2,r3
        add     r3,r3,r8,lsl 2
        ldr     r3,[r3]
        movw    r2,#0xffff
        movt    r2,#0xffff
        cmp     r3,r2
        beq     dfsLABEL23
dfsLABEL15:
        ldr     r1,[sp,#24]
        movw    r2,#0xd9c0
        movt    r2,#0x2c
        mla     r3,r7,r2,r1
        movw    r2,#0x7de0
        movt    r2,#0x2
        mla     r3,r4,r2,r3
        movw    r2,#0x2370
        movt    r2,#0x0
        mla     r3,r6,r2,r3
        mov     r2,#504
        mla     r3,r5,r2,r3
        mov     r2,#28
        ldr     r0,[sp,#80]
        mla     r3,r0,r2,r3
        add     r3,r3,r8,lsl 2
        ldr     r3,[r3]
        b       dfsLABEL108
dfsLABEL23:
        add     r3,r7,r4
        add     r3,r3,r6
        add     r3,r3,r5
        ldr     r0,[sp,#80]
        add     r3,r3,r0
        cmp     r3,#0
        bne     dfsLABEL30
dfsLABEL29:
        mov     r3,#1
        b       dfsLABEL108
dfsLABEL30:
        cmp     r7,#0
        beq     dfsLABEL40
dfsLABEL32:
        push    { r3,r12,r14 }
        sub     sp,sp,#4
        mov     r0,r8
        mov     r1,#2
        bl      equal
        add     sp,sp,#4
        pop     { r3,r12,r14 }
        mov     r3,r0
        sub     r3,r7,r3
        sub     r9,r7,#1
        push    { r3,r12,r14 }
        sub     sp,sp,#4
        mov     r0,#1
        str     r0,[sp,#-4]!
        ldr     r0,[sp,#100]
        str     r0,[sp,#-4]!
        mov     r0,r9
        mov     r1,r4
        mov     r2,r6
        mov     r3,r5
        bl      dfs
        add     sp,sp,#12
        pop     { r3,r12,r14 }
        mov     r9,r0
        mul     r3,r3,r9
        add     r3,r3,#0
        movw    r1,#0xca07
        movt    r1,#0x3b9a
        mov     r0,r3
        sdiv    r2,r0,r1
        mls     r3,r2,r1,r0
        b       dfsLABEL42
dfsLABEL40:
        mov     r3,#0
dfsLABEL42:
        cmp     r4,#0
        beq     dfsLABEL54
dfsLABEL44:
        push    { r3,r12,r14 }
        sub     sp,sp,#4
        mov     r0,r8
        mov     r1,#3
        bl      equal
        add     sp,sp,#4
        pop     { r3,r12,r14 }
        mov     r9,r0
        sub     r9,r4,r9
        add     r11,r7,#1
        sub     r10,r4,#1
        push    { r3,r12,r14 }
        sub     sp,sp,#4
        mov     r0,#2
        str     r0,[sp,#-4]!
        ldr     r0,[sp,#100]
        str     r0,[sp,#-4]!
        mov     r0,r11
        mov     r1,r10
        mov     r2,r6
        mov     r3,r5
        bl      dfs
        add     sp,sp,#12
        pop     { r3,r12,r14 }
        mov     r10,r0
        mul     r9,r9,r10
        add     r3,r3,r9
        movw    r1,#0xca07
        movt    r1,#0x3b9a
        mov     r0,r3
        sdiv    r2,r0,r1
        mls     r3,r2,r1,r0
        b       dfsLABEL55
dfsLABEL54:
dfsLABEL55:
        cmp     r6,#0
        beq     dfsLABEL67
dfsLABEL57:
        push    { r3,r12,r14 }
        sub     sp,sp,#4
        mov     r0,r8
        mov     r1,#4
        bl      equal
        add     sp,sp,#4
        pop     { r3,r12,r14 }
        mov     r9,r0
        sub     r9,r6,r9
        add     r11,r4,#1
        sub     r10,r6,#1
        push    { r3,r12,r14 }
        sub     sp,sp,#4
        mov     r0,#3
        str     r0,[sp,#-4]!
        ldr     r0,[sp,#100]
        str     r0,[sp,#-4]!
        mov     r0,r7
        mov     r1,r11
        mov     r2,r10
        mov     r3,r5
        bl      dfs
        add     sp,sp,#12
        pop     { r3,r12,r14 }
        mov     r10,r0
        mul     r9,r9,r10
        add     r3,r3,r9
        movw    r1,#0xca07
        movt    r1,#0x3b9a
        mov     r0,r3
        sdiv    r2,r0,r1
        mls     r3,r2,r1,r0
        b       dfsLABEL68
dfsLABEL67:
dfsLABEL68:
        cmp     r5,#0
        beq     dfsLABEL80
dfsLABEL70:
        push    { r3,r12,r14 }
        sub     sp,sp,#4
        mov     r0,r8
        mov     r1,#5
        bl      equal
        add     sp,sp,#4
        pop     { r3,r12,r14 }
        mov     r9,r0
        sub     r11,r5,r9
        add     r10,r6,#1
        sub     r9,r5,#1
        push    { r3,r12,r14 }
        sub     sp,sp,#4
        mov     r0,#4
        str     r0,[sp,#-4]!
        ldr     r0,[sp,#100]
        str     r0,[sp,#-4]!
        mov     r0,r7
        mov     r1,r4
        mov     r2,r10
        mov     r3,r9
        bl      dfs
        add     sp,sp,#12
        pop     { r3,r12,r14 }
        mov     r9,r0
        mul     r9,r11,r9
        add     r3,r3,r9
        movw    r1,#0xca07
        movt    r1,#0x3b9a
        mov     r0,r3
        sdiv    r2,r0,r1
        mls     r3,r2,r1,r0
        b       dfsLABEL81
dfsLABEL80:
dfsLABEL81:
        ldr     r1,[sp,#80]
        cmp     r1,#0
        beq     dfsLABEL91
dfsLABEL83:
        add     r9,r5,#1
        ldr     r1,[sp,#80]
        sub     r10,r1,#1
        push    { r3,r12,r14 }
        sub     sp,sp,#4
        mov     r0,#5
        str     r0,[sp,#-4]!
        mov     r0,r10
        str     r0,[sp,#-4]!
        mov     r0,r7
        mov     r1,r4
        mov     r2,r6
        mov     r3,r9
        bl      dfs
        add     sp,sp,#12
        pop     { r3,r12,r14 }
        mov     r9,r0
        ldr     r1,[sp,#80]
        mul     r9,r1,r9
        add     r3,r3,r9
        movw    r1,#0xca07
        movt    r1,#0x3b9a
        mov     r0,r3
        sdiv    r2,r0,r1
        mls     r3,r2,r1,r0
        b       dfsLABEL92
dfsLABEL91:
dfsLABEL92:
        ldr     r1,[sp,#24]
        movw    r2,#0xd9c0
        movt    r2,#0x2c
        mla     r14,r7,r2,r1
        movw    r2,#0x7de0
        movt    r2,#0x2
        mla     r11,r4,r2,r14
        movw    r2,#0x2370
        movt    r2,#0x0
        mla     r9,r6,r2,r11
        mov     r2,#504
        mla     r12,r5,r2,r9
        mov     r2,#28
        ldr     r0,[sp,#80]
        mla     r1,r0,r2,r12
        str     r1,[sp,#16]
        ldr     r1,[sp,#16]
        add     r10,r1,r8,lsl 2
        movw    r1,#0xca07
        movt    r1,#0x3b9a
        mov     r0,r3
        sdiv    r2,r0,r1
        mls     r3,r2,r1,r0
        str     r3,[r10]
        ldr     r1,[sp,#24]
        movw    r2,#0xd9c0
        movt    r2,#0x2c
        mla     r0,r7,r2,r1
        movw    r2,#0x7de0
        movt    r2,#0x2
        mla     r0,r4,r2,r14
        movw    r2,#0x2370
        movt    r2,#0x0
        mla     r0,r6,r2,r11
        mov     r2,#504
        mla     r0,r5,r2,r9
        mov     r2,#28
        ldr     r0,[sp,#80]
        mla     r0,r0,r2,r12
        ldr     r1,[sp,#16]
        add     r0,r1,r8,lsl 2
        ldr     r3,[r10]
dfsLABEL108:
        mov     r0,r3
        add     sp,sp,#40
        ldmfd   sp!,{r4-r12,lr}
        bx      lr

        .global main
main:
        stmfd   sp!,{r4-r12,lr}
        sub     sp,sp,#16
        movw    r8,#:lower16:cns
        movt    r8,#:upper16:cns
        movw    r6,#:lower16:list
        movt    r6,#:upper16:list
        movw    r9,#:lower16:dp
        movt    r9,#:upper16:dp
        push    { r3,r12,r14 }
        sub     sp,sp,#4
        bl      getint
        add     sp,sp,#4
        pop     { r3,r12,r14 }
        mov     r7,r0
        mov     r5,#0
mainLABEL2:
        cmp     r5,#18
        bge     mainLABEL43
mainLABEL5:
        mov     r10,#0
mainLABEL6:
        cmp     r10,#18
        bge     mainLABEL41
mainLABEL9:
        mov     r4,#0
mainLABEL10:
        cmp     r4,#18
        bge     mainLABEL39
mainLABEL13:
        mov     r3,#0
mainLABEL14:
        cmp     r3,#18
        bge     mainLABEL37
mainLABEL17:
        mov     r11,#0
mainLABEL18:
        cmp     r11,#18
        bge     mainLABEL35
mainLABEL21:
        mov     r12,#0
mainLABEL22:
        cmp     r12,#7
        bge     mainLABEL33
mainLABEL25:
        movw    r2,#0xd9c0
        movt    r2,#0x2c
        mla     r14,r5,r2,r9
        movw    r2,#0x7de0
        movt    r2,#0x2
        mla     r14,r10,r2,r14
        movw    r2,#0x2370
        movt    r2,#0x0
        mla     r14,r4,r2,r14
        mov     r2,#504
        mla     r14,r3,r2,r14
        mov     r2,#28
        mla     r14,r11,r2,r14
        add     r14,r14,r12,lsl 2
        movw    r1,#0xffff
        movt    r1,#0xffff
        str     r1,[r14]
        add     r12,r12,#1
        b       mainLABEL22
mainLABEL33:
        add     r11,r11,#1
        b       mainLABEL18
mainLABEL35:
        add     r3,r3,#1
        b       mainLABEL14
mainLABEL37:
        add     r4,r4,#1
        b       mainLABEL10
mainLABEL39:
        add     r10,r10,#1
        b       mainLABEL6
mainLABEL41:
        add     r5,r5,#1
        b       mainLABEL2
mainLABEL43:
        mov     r3,#0
mainLABEL44:
        cmp     r3,r7
        bge     mainLABEL59
mainLABEL47:
        add     r4,r6,r3,lsl 2
        push    { r3,r12,r14 }
        sub     sp,sp,#4
        bl      getint
        add     sp,sp,#4
        pop     { r3,r12,r14 }
        mov     r5,r0
        str     r5,[r4]
        add     r0,r6,r3,lsl 2
        ldr     r5,[r4]
        add     r5,r8,r5,lsl 2
        add     r0,r6,r3,lsl 2
        ldr     r4,[r4]
        add     r4,r8,r4,lsl 2
        ldr     r4,[r4]
        add     r4,r4,#1
        str     r4,[r5]
        add     r3,r3,#1
        b       mainLABEL44
mainLABEL59:
        add     r3,r8,#4
        ldr     r4,[r3]
        add     r3,r8,#8
        ldr     r7,[r3]
        add     r3,r8,#12
        ldr     r5,[r3]
        add     r3,r8,#16
        ldr     r3,[r3]
        add     r6,r8,#20
        ldr     r6,[r6]
        push    { r3,r12,r14 }
        sub     sp,sp,#4
        mov     r0,#0
        str     r0,[sp,#-4]!
        mov     r0,r6
        str     r0,[sp,#-4]!
        mov     r3,r3
        mov     r0,r4
        mov     r1,r7
        mov     r2,r5
        bl      dfs
        add     sp,sp,#12
        pop     { r3,r12,r14 }
        mov     r3,r0
        push    { r3,r12,r14 }
        sub     sp,sp,#4
        mov     r0,r3
        bl      putint
        add     sp,sp,#4
        pop     { r3,r12,r14 }
        mov     r0,r3
        add     sp,sp,#16
        ldmfd   sp!,{r4-r12,lr}
        bx      lr
        .bss
        .align  4
cns:
        .zero   80
        .bss
        .align  4
list:
        .zero   800
        .bss
        .align  4
dp:
        .zero   52907904