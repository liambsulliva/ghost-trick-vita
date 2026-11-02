asm void func_0207bd24(void)
{
    eors r2, r0, r1
    eormi r1, r1, #0x80000000
    dcd 0x4a00008b
    subs r12, r0, r1
    sublo r0, r0, r12
    addlo r1, r1, r12
    mov r2, #0x80000000
    mov r3, r0, lsr #23
    orr r0, r2, r0, lsl #8
    ands r12, r3, #255
    cmpne r12, #255
    beq @L207bdc4
    mov r12, r1, lsr #23
    orr r1, r2, r1, lsl #8
    ands r2, r12, #255
    beq @L207be04
@L207bd64:
    subs r12, r3, r12
    beq @L207bd7c
    rsb r2, r12, #32
    movs r2, r1, lsl r2
    mov r1, r1, lsr r12
    orrne r1, r1, #1
@L207bd7c:
    adds r0, r0, r1
    blo @L207bd9c
    and r1, r0, #1
    orr r0, r1, r0, rrx
    add r3, r3, #1
    and r2, r3, #255
    cmp r2, #255
    beq @L207bf0c
@L207bd9c:
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
@L207bdc4:
    cmp r3, #0x100
    movge r2, #0x80000000
    movlt r2, #0
    ands r3, r3, #255
    beq @L207be28
    movs r0, r0, lsl #1
    bne @L207bf38
    mov r12, r1, lsr #23
    mov r1, r1, lsl #9
    ands r12, r12, #255
    beq @L207bf2c
    cmp r12, #255
    blt @L207bf2c
    cmp r1, #0
    beq @L207bf2c
    b @L207bf38
@L207be04:
    cmp r3, #0x100
    movge r2, #0x80000000
    movlt r2, #0
    and r3, r3, #255
    ands r12, r12, #255
    beq @L207be84
@L207be1c:
    movs r1, r1, lsl #1
    bne @L207bf38
    b @L207bf2c
@L207be28:
    movs r0, r0, lsl #1
    beq @L207be60
    mov r3, #1
    mov r0, r0, lsr #1
    mov r12, r1, lsr #23
    mov r1, r1, lsl #8
    ands r12, r12, #255
    beq @L207be84
    cmp r12, #255
    beq @L207be1c
    orr r1, r1, #0x80000000
    orr r3, r3, r2, lsr #23
    orr r12, r12, r2, lsr #23
    b @L207bd64
@L207be60:
    mov r3, r1, lsr #23
    mov r0, r1, lsl #9
    ands r3, r3, #255
    beq @L207beec
    cmp r3, #255
    blt @L207beec
    cmp r0, #0
    beq @L207bf2c
    b @L207bf24
@L207be84:
    movs r1, r1, lsl #1
    beq @L207bef4
    mov r1, r1, lsr #1
    mov r12, #1
    orr r3, r3, r2, lsr #23
    orr r12, r12, r2, lsr #23
    cmp r0, #0
    bmi @L207bd64
    adds r0, r0, r1
    blo @L207beb8
    and r1, r0, #1
    orr r0, r1, r0, rrx
    add r12, r12, #1
@L207beb8:
    cmp r0, #0
    subge r12, r12, #1
    ands r1, r0, #255
    add r0, r0, r0
    mov r0, r0, lsr #9
    orr r0, r0, r12, lsl #23
    bxeq lr
    tst r1, #128
    bxeq lr
    ands r1, r1, #127
    andeqs r1, r0, #1
    addne r0, r0, #1
    bx lr
@L207beec:
    mov r0, r1
    bx lr
@L207bef4:
    cmp r0, #0
    subges r3, r3, #1
    add r0, r0, r0
    orr r0, r2, r0, lsr #9
    orr r0, r0, r3, lsl #23
    bx lr
@L207bf0c:
    cmp r3, #0x100
    movge r2, #0x80000000
    movlt r2, #0
    mov r0, #0xff000000
    orr r0, r2, r0, lsr #1
    bx lr
@L207bf24:
    mvn r0, #0x80000000
    bx lr
@L207bf2c:
    mov r0, #0xff000000
    orr r0, r2, r0, lsr #1
    bx lr
@L207bf38:
    mvn r0, #0x80000000
    bx lr
}
