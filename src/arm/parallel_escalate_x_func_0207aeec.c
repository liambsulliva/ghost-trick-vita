asm void func_0207aeec(void)
{
    stmfd sp!, {r4, r5, r6, r7, lr}
    eor lr, r1, r3
    and lr, lr, #0x80000000
    mov r12, r1, lsr #0x14
    mov r1, r1, lsl #0xb
    orr r1, r1, r0, lsr #0x15
    mov r0, r0, lsl #0xb
    movs r6, r12, lsl #0x15
    cmnne r6, #0x200000
    beq @3
    orr r1, r1, #0x80000000
    bic r12, r12, #0x800
    mov r4, r3, lsr #0x14
    mov r3, r3, lsl #0xb
    orr r3, r3, r2, lsr #0x15
    mov r2, r2, lsl #0xb
    movs r5, r4, lsl #0x15
    cmnne r5, #0x200000
    beq @5
    orr r3, r3, #0x80000000
    bic r4, r4, #0x800
@1:
    add r12, r4, r12
    umull r5, r4, r0, r2
    umull r7, r6, r0, r3
    adds r4, r7, r4
    adc r6, r6, #0
    umull r7, r0, r1, r2
    adds r4, r7, r4
    adcs r0, r0, r6
    umull r7, r2, r1, r3
    adc r1, r2, #0
    adds r0, r0, r7
    adc r1, r1, #0
    orrs r4, r4, r5
    orrne r0, r0, #1
    cmp r1, #0
    blt @2
    sub r12, r12, #1
    adds r0, r0, r0
    adc r1, r1, r1
@2:
    add r12, r12, #2
    subs r12, r12, #0x400
    bmi @12
    beq @12
    mov r6, r12, lsl #0x14
    cmn r6, #0x100000
    bmi @19
    movs r2, r0, lsl #0x15
    mov r0, r0, lsr #0xb
    orr r0, r0, r1, lsl #0x15
    add r1, r1, r1
    orr r1, lr, r1, lsr #0xc
    orr r1, r1, r12, lsl #0x14
    ldmeqfd sp!, {r4, r5, r6, r7, lr}
    bxeq lr
    tst r2, #0x80000000
    ldmeqfd sp!, {r4, r5, r6, r7, lr}
    bxeq lr
    movs r2, r2, lsl #1
    andeqs r2, r0, #1
    ldmeqfd sp!, {r4, r5, r6, r7, lr}
    bxeq lr
    adds r0, r0, #1
    adc r1, r1, #0
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
@3:
    bics r12, r12, #0x800
    beq @6
    orrs r6, r0, r1, lsl #1
    bne @15
    mov r4, r3, lsr #0x14
    mov r3, r3, lsl #0xb
    orr r3, r3, r2, lsr #0x15
    mov r2, r2, lsl #0xb
    movs r5, r4, lsl #0x15
    beq @4
    cmn r5, #0x200000
    bne @14
    orrs r5, r2, r3, lsl #1
    beq @14
    b @15
@4:
    orrs r5, r3, r2
    beq @16
    b @14
@5:
    bics r4, r4, #0x800
    beq @10
    orrs r6, r2, r3, lsl #1
    bne @15
    b @14
@6:
    orrs r6, r0, r1, lsl #1
    beq @9
    mov r12, #1
    cmp r1, #0
    bne @7
    sub r12, r12, #0x20
    movs r1, r0
    mov r0, #0
    bmi @8
@7:
    clz r6, r1
    movs r1, r1, lsl r6
    rsb r6, r6, #0x20
    orr r1, r1, r0, lsr r6
    rsb r6, r6, #0x20
    mov r0, r0, lsl r6
    sub r12, r12, r6
@8:
    mov r4, r3, lsr #0x14
    mov r3, r3, lsl #0xb
    orr r3, r3, r2, lsr #0x15
    mov r2, r2, lsl #0xb
    movs r5, r4, lsl #0x15
    cmnne r5, #0x200000
    beq @5
    orr r3, r3, #0x80000000
    bic r4, r4, #0x800
    b @1
@9:
    mov r4, r3, lsr #0x14
    mov r3, r3, lsl #0xb
    orr r3, r3, r2, lsr #0x15
    mov r2, r2, lsl #0xb
    movs r5, r4, lsl #0x15
    beq @20
    cmn r5, #0x200000
    bne @20
    orrs r6, r2, r3, lsl #1
    beq @16
    b @15
@10:
    orrs r5, r2, r3, lsl #1
    beq @20
    mov r4, #1
    cmp r3, #0
    bne @11
    sub r4, r4, #0x20
    movs r3, r2
    mov r2, #0
    bmi @1
@11:
    clz r6, r3
    movs r3, r3, lsl r6
    rsb r6, r6, #0x20
    orr r3, r3, r2, lsr r6
    rsb r6, r6, #0x20
    mov r2, r2, lsl r6
    sub r4, r4, r6
    b @1
@12:
    cmn r12, #0x34
    beq @13
    bmi @18
    mov r2, r1
    mov r3, r0
    add r4, r12, #0x34
    cmp r4, #0x20
    movge r2, r3
    movge r3, #0
    subge r4, r4, #0x20
    rsb r5, r4, #0x20
    mov r2, r2, lsl r4
    orr r2, r2, r3, lsr r5
    movs r3, r3, lsl r4
    orrne r2, r2, #1
    rsb r12, r12, #0xc
    cmp r12, #0x20
    movge r0, r1
    movge r1, #0
    subge r12, r12, #0x20
    rsb r4, r12, #0x20
    mov r0, r0, lsr r12
    orr r0, r0, r1, lsl r4
    orr r1, lr, r1, lsr r12
    cmp r2, #0
    ldmeqfd sp!, {r4, r5, r6, r7, lr}
    bxeq lr
    tst r2, #0x80000000
    ldmeqfd sp!, {r4, r5, r6, r7, lr}
    bxeq lr
    movs r2, r2, lsl #1
    andeqs r2, r0, #1
    ldmeqfd sp!, {r4, r5, r6, r7, lr}
    bxeq lr
    adds r0, r0, #1
    adc r1, r1, #0
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
@13:
    orr r0, r0, r1, lsl #1
    b @17
@14:
    ldr r1, =0x7ff00000
    orr r1, lr, r1
    mov r0, #0
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
@15:
    mov r1, r3
    mvn r0, #0
    bic r1, r0, #0x80000000
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
@16:
    mvn r0, #0
    bic r1, r0, #0x80000000
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
@17:
    movs r2, r0
    mov r1, lr
    mov r0, #0
    addne r0, r0, #1
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
@18:
    mov r1, lr
    mov r0, #0
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
@19:
    ldr r1, =0x7ff00000
    orr r1, lr, r1
    mov r0, #0
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
@20:
    mov r1, lr
    mov r0, #0
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
}
