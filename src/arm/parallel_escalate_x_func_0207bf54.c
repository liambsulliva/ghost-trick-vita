asm void func_0207bf54(void)
{
    eors r2, r0, r1
    eormi r1, r1, #0x80000000
    dcd 0x4affff73
    subs r12, r0, r1
    eorlo r12, r12, #0x80000000
    sublo r0, r0, r12
    addlo r1, r1, r12
    mov r2, #0x80000000
    mov r3, r0, lsr #23
    orr r0, r2, r0, lsl #8
    ands r12, r3, #255
    cmpne r12, #255
    beq @L207c07c
    mov r12, r1, lsr #23
    orr r1, r2, r1, lsl #8
    ands r2, r12, #255
    beq @L207c0bc
@L207bf98:
    subs r12, r3, r12
    beq @L207bfe0
    rsb r2, r12, #32
    movs r2, r1, lsl r2
    mov r1, r1, lsr r12
    orrne r1, r1, #1
    subs r0, r0, r1
    bpl @L207c024
    ands r1, r0, #255
    add r0, r0, r0
    mov r0, r0, lsr #9
    orr r0, r0, r3, lsl #23
    tst r1, #128
    bxeq lr
    ands r1, r1, #127
    andeqs r1, r0, #1
    addne r0, r0, #1
    bx lr
@L207bfe0:
    subs r0, r0, r1
    beq @L207c188
    mov r2, r3, lsl #23
    and r2, r2, #0x80000000
    bic r3, r3, #0x100
    clz r12, r0
    movs r0, r0, lsl r12
    sub r3, r3, r12
    cmp r3, #0
    bgt @L207c014
    rsb r3, r3, #9
    orr r0, r2, r0, lsr r3
    bx lr
@L207c014:
    add r0, r0, r0
    orr r0, r2, r0, lsr #9
    orr r0, r0, r3, lsl #23
    bx lr
@L207c024:
    mov r2, r3, lsl #23
    and r2, r2, #0x80000000
    bic r3, r3, #0x100
    clz r12, r0
    movs r0, r0, lsl r12
    sub r3, r3, r12
    cmp r3, #0
    bgt @L207c050
    rsb r3, r3, #9
    orr r0, r2, r0, lsr r3
    bx lr
@L207c050:
    ands r1, r0, #255
    add r0, r0, r0
    orr r0, r2, r0, lsr #9
    orr r0, r0, r3, lsl #23
    bxeq lr
    tst r1, #128
    bxeq lr
    ands r1, r1, #127
    andeqs r1, r0, #1
    addne r0, r0, #1
    bx lr
@L207c07c:
    cmp r3, #0x100
    movge r2, #0x80000000
    movlt r2, #0
    ands r3, r3, #255
    beq @L207c0e4
    movs r0, r0, lsl #1
    bne @L207c1bc
    mov r12, r1, lsr #23
    mov r1, r1, lsl #9
    ands r12, r12, #255
    beq @L207c1b0
    cmp r12, #255
    blt @L207c1b0
    cmp r1, #0
    beq @L207c1c4
    b @L207c1bc
@L207c0bc:
    cmp r12, #0x100
    movge r2, #0x80000000
    movlt r2, #0
    and r3, r3, #255
    ands r12, r12, #255
    beq @L207c14c
@L207c0d4:
    eor r2, r2, #0x80000000
    movs r1, r1, lsl #1
    bne @L207c1bc
    b @L207c1b0
@L207c0e4:
    movs r0, r0, lsl #1
    beq @L207c11c
    mov r0, r0, lsr #1
    mov r3, #1
    mov r12, r1, lsr #23
    mov r1, r1, lsl #8
    ands r12, r12, #255
    beq @L207c14c
    cmp r12, #255
    beq @L207c0d4
    orr r1, r1, #0x80000000
    orr r3, r3, r2, lsr #23
    orr r12, r12, r2, lsr #23
    b @L207bf98
@L207c11c:
    mov r3, r1, lsr #23
    mov r0, r1, lsl #9
    ands r2, r3, #255
    beq @L207c140
    cmp r2, #255
    blt @L207c168
    cmp r0, #0
    bne @L207c1a8
    b @L207c1b0
@L207c140:
    cmp r0, #0
    beq @L207c188
    b @L207c168
@L207c14c:
    movs r1, r1, lsl #1
    beq @L207c170
    mov r1, r1, lsr #1
    mov r12, #1
    orr r12, r12, r2, lsr #23
    orr r3, r3, r2, lsr #23
    b @L207bf98
@L207c168:
    mov r0, r1
    bx lr
@L207c170:
    cmp r0, #0
    subges r3, r3, #1
    add r0, r0, r0
    orr r0, r2, r0, lsr #9
    orr r0, r0, r3, lsl #23
    bx lr
@L207c188:
    mov r0, #0
    bx lr
    cmp r0, #0
    subges r3, r3, #1
    add r0, r0, r0
    mov r0, r0, lsr #9
    orr r0, r0, r3, lsl #23
    bx lr
@L207c1a8:
    mvn r0, #0x80000000
    bx lr
@L207c1b0:
    mov r0, #0xff000000
    orr r0, r2, r0, lsr #1
    bx lr
@L207c1bc:
    mvn r0, #0x80000000
    bx lr
@L207c1c4:
    mvn r0, #0x80000000
    bx lr
}
