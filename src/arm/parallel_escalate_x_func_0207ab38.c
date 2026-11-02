asm void func_0207ab38(void)
{
    stmfd sp!, {r4, lr}
    eors r12, r1, r3
    eormi r3, r3, #0x80000000
    dcd 0x4affff33
    subs r12, r0, r2
    sbcs lr, r1, r3
    bhs @L207ab68
    eor lr, lr, #0x80000000
    adds r2, r2, r12
    adc r3, r3, lr
    subs r0, r0, r12
    sbc r1, r1, lr
@L207ab68:
    mov lr, #0x80000000
    mov r12, r1, lsr #20
    orr r1, lr, r1, lsl #11
    orr r1, r1, r0, lsr #21
    mov r0, r0, lsl #11
    movs r4, r12, lsl #21
    cmnne r4, #0x200000
    beq @L207ad6c
    mov r4, r3, lsr #20
    orr r3, lr, r3, lsl #11
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #11
    movs lr, r4, lsl #21
    beq @L207adb4
@L207aba0:
    subs r4, r12, r4
    beq @L207ac48
    cmp r4, #32
    ble @L207abdc
    cmp r4, #56
    movge r4, #63
    sub r4, r4, #32
    rsb lr, r4, #32
    orrs lr, r2, r3, lsl lr
    mov r2, r3, lsr r4
    orrne r2, r2, #1
    subs r0, r0, r2
    sbcs r1, r1, #0
    bmi @L207ac04
    b @L207acf4
@L207abdc:
    rsb lr, r4, #32
    movs lr, r2, lsl lr
    rsb lr, r4, #32
    mov r2, r2, lsr r4
    orr r2, r2, r3, lsl lr
    mov r3, r3, lsr r4
    orrne r2, r2, #1
    subs r0, r0, r2
    sbcs r1, r1, r3
    bpl @L207acf4
@L207ac04:
    movs r2, r0, lsl #21
    mov r0, r0, lsr #11
    orr r0, r0, r1, lsl #21
    add r1, r1, r1
    mov r1, r1, lsr #12
    orr r1, r1, r12, lsl #20
    tst r2, #0x80000000
    ldmeqfd sp!, {r4, lr}
    bxeq lr
    movs r2, r2, lsl #1
    andeqs r2, r0, #1
    ldmeqfd sp!, {r4, lr}
    bxeq lr
    adds r0, r0, #1
    adc r1, r1, #0
    ldmfd sp!, {r4, lr}
    bx lr
@L207ac48:
    subs r0, r0, r2
    sbc r1, r1, r3
    orrs lr, r1, r0
    beq @L207aed8
    mov lr, r12, lsl #20
    and lr, lr, #0x80000000
    bic r12, r12, #0x800
    cmp r1, #0
    bmi @L207acd0
    bne @L207ac80
    sub r12, r12, #32
    movs r1, r0
    mov r0, #0
    bmi @L207ac9c
@L207ac80:
    clz r4, r1
    movs r1, r1, lsl r4
    rsb r4, r4, #32
    orr r1, r1, r0, lsr r4
    rsb r4, r4, #32
    mov r0, r0, lsl r4
    sub r12, r12, r4
@L207ac9c:
    cmp r12, #0
    bgt @L207acd8
    rsb r12, r12, #12
    cmp r12, #32
    movge r0, r1
    movge r1, #0
    subge r12, r12, #32
    rsb r4, r12, #32
    mov r0, r0, lsr r12
    orr r0, r0, r1, lsl r4
    orr r1, lr, r1, lsr r12
    ldmfd sp!, {r4, lr}
    bx lr
@L207acd0:
    cmp r1, #0
    subges r12, r12, #1
@L207acd8:
    mov r0, r0, lsr #11
    orr r0, r0, r1, lsl #21
    add r1, r1, r1
    orr r1, lr, r1, lsr #12
    orr r1, r1, r12, lsl #20
    ldmfd sp!, {r4, lr}
    bx lr
@L207acf4:
    mov lr, r12, lsl #20
    and lr, lr, #0x80000000
    bic r12, r12, #0x800
    cmp r1, #0
    bne @L207ad18
    sub r12, r12, #32
    movs r1, r0
    mov r0, #0
    bmi @L207ad34
@L207ad18:
    clz r4, r1
    movs r1, r1, lsl r4
    rsb r4, r4, #32
    orr r1, r1, r0, lsr r4
    rsb r4, r4, #32
    mov r0, r0, lsl r4
    sub r12, r12, r4
@L207ad34:
    cmp r12, #0
    orrgt r12, r12, lr, lsr #20
    bgt @L207ac04
    rsb r12, r12, #12
    cmp r12, #32
    movge r0, r1
    movge r1, #0
    subge r12, r12, #32
    rsb r4, r12, #32
    mov r0, r0, lsr r12
    orr r0, r0, r1, lsl r4
    orr r1, lr, r1, lsr r12
    ldmfd sp!, {r4, lr}
    bx lr
@L207ad6c:
    cmp r12, #0x800
    movge lr, #0x80000000
    movlt lr, #0
    bics r12, r12, #0x800
    beq @L207add8
    orrs r4, r0, r1, lsl #1
    bne @L207aeb4
    mov r4, r3, lsr #20
    mov r3, r3, lsl #11
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #11
    movs r4, r4, lsl #21
    beq @L207aea0
    cmn r4, #0x200000
    bne @L207aea0
    orrs r4, r2, r3, lsl #1
    beq @L207aec8
    b @L207aeb4
@L207adb4:
    cmp r4, #0x800
    movge lr, #0x80000000
    movlt lr, #0
    bic r12, r12, #0x800
    bics r4, r4, #0x800
    beq @L207ae50
    orrs r4, r2, r3, lsl #1
    bne @L207aeb4
    b @L207aea0
@L207add8:
    orrs r4, r0, r1, lsl #1
    beq @L207ae18
    mov r12, #1
    bic r1, r1, #0x80000000
    mov r4, r3, lsr #20
    mov r3, r3, lsl #11
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #11
    movs r4, r4, lsl #21
    cmnne r4, #0x200000
    mov r4, r4, lsr #21
    orr r4, r4, lr, lsr #20
    beq @L207adb4
    orr r3, r3, #0x80000000
    orr r12, r12, lr, lsr #20
    b @L207aba0
@L207ae18:
    mov r12, r3, lsr #20
    mov r1, r3, lsl #11
    orr r1, r1, r2, lsr #21
    mov r0, r2, lsl #11
    movs r4, r12, lsl #21
    beq @L207ae44
    cmn r4, #0x200000
    bne @L207ae6c
    orrs r4, r0, r1, lsl #1
    bne @L207aeb8
    b @L207aea0
@L207ae44:
    orrs r4, r0, r1, lsl #1
    beq @L207aed8
    b @L207ae6c
@L207ae50:
    orrs r4, r2, r3, lsl #1
    beq @L207ae7c
    mov r4, #1
    bic r3, r3, #0x80000000
    orr r12, r12, lr, lsr #20
    orr r4, r4, lr, lsr #20
    b @L207aba0
@L207ae6c:
    mov r1, r3
    mov r0, r2
    ldmfd sp!, {r4, lr}
    bx lr
@L207ae7c:
    cmp r1, #0
    subges r12, r12, #1
    mov r0, r0, lsr #11
    orr r0, r0, r1, lsl #21
    add r1, r1, r1
    orr r1, lr, r1, lsr #12
    orr r1, r1, r12, lsl #20
    ldmfd sp!, {r4, lr}
    bx lr
@L207aea0:
    ldr r1, [pc, #0x40]
    orr r1, lr, r1
    mov r0, #0
    ldmfd sp!, {r4, lr}
    bx lr
@L207aeb4:
    mov r1, r3
@L207aeb8:
    mvn r0, #0
    bic r1, r0, #0x80000000
    ldmfd sp!, {r4, lr}
    bx lr
@L207aec8:
    mvn r0, #0
    bic r1, r0, #0x80000000
    ldmfd sp!, {r4, lr}
    bx lr
@L207aed8:
    mov r1, #0
    mov r0, #0
    ldmfd sp!, {r4, lr}
    bx lr
    dcd 0x7ff00000
}
