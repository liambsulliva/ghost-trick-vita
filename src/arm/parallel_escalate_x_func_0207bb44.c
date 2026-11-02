asm void func_0207bb44(void)
{
    eor r2, r0, r1
    and r2, r2, #0x80000000
    mov r12, #0xff
    ands r3, r12, r0, lsr #0x17
    mov r0, r0, lsl #8
    cmpne r3, #0xff
    beq @2
    orr r0, r0, #0x80000000
    ands r12, r12, r1, lsr #0x17
    mov r1, r1, lsl #8
    cmpne r12, #0xff
    beq @4
    orr r1, r1, #0x80000000
@1:
    add r12, r3, r12
    umull r1, r3, r0, r1
    movs r0, r3
    addpl r0, r0, r0
    subpl r12, r12, #1
    subs r12, r12, #0x7f
    bmi @9
    cmp r12, #0xfe
    bge @14
    ands r3, r0, #0xff
    orr r0, r2, r0, lsr #8
    add r0, r0, r12, lsl #0x17
    tst r3, #0x80
    bxeq lr
    orrs r1, r1, r3, lsl #0x19
    andeqs r3, r0, #1
    addne r0, r0, #1
    bx lr
@2:
    cmp r3, #0
    beq @6
    movs r0, r0, lsl #1
    bne @12
    mov r12, r1, lsr #0x17
    mov r1, r1, lsl #9
    ands r12, r12, #0xff
    beq @3
    cmp r12, #0xff
    blt @11
    cmp r1, #0
    beq @11
    b @12
@3:
    cmp r1, #0
    beq @13
    b @11
@4:
    cmp r12, #0
    beq @8
@5:
    movs r1, r1, lsl #1
    bne @12
    b @11
@6:
    movs r0, r0, lsl #1
    beq @7
    mov r0, r0, lsr #1
    clz r3, r0
    movs r0, r0, lsl r3
    rsb r3, r3, #1
    mov r12, r1, lsr #0x17
    mov r1, r1, lsl #8
    ands r12, r12, #0xff
    beq @8
    cmp r12, #0xff
    beq @5
    orr r1, r1, #0x80000000
    b @1
@7:
    mov r12, r1, lsr #0x17
    mov r1, r1, lsl #9
    ands r12, r12, #0xff
    beq @17
    cmp r12, #0xff
    blt @17
    cmp r1, #0
    beq @13
    b @12
@8:
    movs r1, r1, lsl #1
    beq @17
    mov r1, r1, lsr #1
    clz r12, r1
    movs r1, r1, lsl r12
    rsb r12, r12, #1
    b @1
@9:
    cmn r12, #0x18
    beq @10
    bmi @16
    cmp r1, #0
    orrne r0, r0, #1
    mov r3, r0
    mov r0, r0, lsr #8
    rsb r12, r12, #0
    orr r0, r2, r0, lsr r12
    rsb r12, r12, #0x18
    movs r1, r3, lsl r12
    bxeq lr
    tst r1, #0x80000000
    bxeq lr
    movs r1, r1, lsl #1
    andeqs r1, r0, #1
    addne r0, r0, #1
    bx lr
@10:
    mov r0, r0, lsl #1
    b @15
@11:
    mov r0, #0xff000000
    orr r0, r2, r0, lsr #1
    bx lr
@12:
    mvn r0, #0x80000000
    bx lr
@13:
    mvn r0, #0x80000000
    bx lr
@14:
    mov r0, #0xff000000
    orr r0, r2, r0, lsr #1
    bx lr
@15:
    movs r1, r0
    mov r0, r2
    addne r0, r0, #1
    bx lr
@16:
    mov r0, r2
    bx lr
@17:
    mov r0, r2
    bx lr
}
