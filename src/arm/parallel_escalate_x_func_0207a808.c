asm void func_0207a808(void)
{
    stmfd sp!, {r4, lr}
    eors r12, r1, r3
    eormi r3, r3, #0x80000000
    dcd 0x4a0000cb
    subs r12, r0, r2
    sbcs lr, r1, r3
    bhs @L207a834
    adds r2, r2, r12
    adc r3, r3, lr
    subs r0, r0, r12
    sbc r1, r1, lr
@L207a834:
    mov lr, #0x80000000
    mov r12, r1, lsr #20
    orr r1, lr, r1, lsl #11
    orr r1, r1, r0, lsr #21
    mov r0, r0, lsl #11
    movs r4, r12, lsl #21
    cmnne r4, #0x200000
    beq @L207a930
    mov r4, r3, lsr #20
    orr r3, lr, r3, lsl #11
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #11
    movs lr, r4, lsl #21
    beq @L207a978
@L207a86c:
    subs r4, r12, r4
    beq @L207a8c4
    cmp r4, #32
    ble @L207a8a8
    cmp r4, #56
    movge r4, #63
    sub r4, r4, #32
    rsb lr, r4, #32
    orrs lr, r2, r3, lsl lr
    mov r2, r3, lsr r4
    orrne r2, r2, #1
    adds r0, r0, r2
    adcs r1, r1, #0
    blo @L207a8ec
    b @L207a8d0
@L207a8a8:
    rsb lr, r4, #32
    movs lr, r2, lsl lr
    rsb lr, r4, #32
    mov r2, r2, lsr r4
    orr r2, r2, r3, lsl lr
    mov r3, r3, lsr r4
    orrne r2, r2, #1
@L207a8c4:
    adds r0, r0, r2
    adcs r1, r1, r3
    blo @L207a8ec
@L207a8d0:
    add r12, r12, #1
    and r4, r0, #1
    movs r1, r1, rrx
    orr r0, r4, r0, rrx
    mov lr, r12, lsl #21
    cmn lr, #0x200000
    beq @L207aafc
@L207a8ec:
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
@L207a930:
    cmp r12, #0x800
    movge lr, #0x80000000
    movlt lr, #0
    bics r12, r12, #0x800
    beq @L207a99c
    orrs r4, r0, r1, lsl #1
    bne @L207aad8
    mov r4, r3, lsr #20
    mov r3, r3, lsl #11
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #11
    movs r4, r4, lsl #21
    beq @L207aac4
    cmn r4, #0x200000
    bne @L207aac4
    orrs r4, r2, r3, lsl #1
    beq @L207aac4
    b @L207aad8
@L207a978:
    cmp r4, #0x800
    movge lr, #0x80000000
    movlt lr, #0
    bic r12, r12, #0x800
    bics r4, r4, #0x800
    beq @L207aa08
    orrs r4, r2, r3, lsl #1
    bne @L207aad8
    b @L207aac4
@L207a99c:
    orrs r4, r0, r1, lsl #1
    beq @L207a9dc
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
    beq @L207a978
    orr r3, r3, #0x80000000
    orr r12, r12, lr, lsr #20
    b @L207a86c
@L207a9dc:
    mov r12, r3, lsr #20
    mov r1, r3, lsl #11
    orr r1, r1, r2, lsr #21
    mov r0, r2, lsl #11
    movs r4, r12, lsl #21
    beq @L207aa90
    cmn r4, #0x200000
    bne @L207aa90
    orrs r4, r0, r1, lsl #1
    beq @L207aac4
    b @L207aadc
@L207aa08:
    orrs r4, r2, r3, lsl #1
    beq @L207aaa0
    mov r4, #1
    bic r3, r3, #0x80000000
    cmp r1, #0
    bpl @L207aa2c
    orr r12, r12, lr, lsr #20
    orr r4, r4, lr, lsr #20
    b @L207a86c
@L207aa2c:
    adds r0, r0, r2
    adcs r1, r1, r3
    blo @L207aa4c
    add r12, r12, #1
    and r4, r0, #1
    movs r1, r1, rrx
    mov r0, r0, rrx
    orr r0, r0, r4
@L207aa4c:
    cmp r1, #0
    subges r12, r12, #1
    movs r2, r0, lsl #21
    mov r0, r0, lsr #11
    orr r0, r0, r1, lsl #21
    add r1, r1, r1
    orr r1, lr, r1, lsr #12
    orr r1, r1, r12, lsl #20
    ldmeqfd sp!, {r4, lr}
    bxeq lr
    tst r2, #0x80000000
    ldmeqfd sp!, {r4, lr}
    bxeq lr
    movs r2, r2, lsl #1
    andeqs r2, r0, #1
    ldmeqfd sp!, {r4, lr}
    bxeq lr
@L207aa90:
    mov r1, r3
    mov r0, r2
    ldmfd sp!, {r4, lr}
    bx lr
@L207aaa0:
    cmp r1, #0
    subges r12, r12, #1
    mov r0, r0, lsr #11
    orr r0, r0, r1, lsl #21
    add r1, r1, r1
    orr r1, lr, r1, lsr #12
    orr r1, r1, r12, lsl #20
    ldmfd sp!, {r4, lr}
    bx lr
@L207aac4:
    ldr r1, [pc, #0x50]
    orr r1, lr, r1
    mov r0, #0
    ldmfd sp!, {r4, lr}
    bx lr
@L207aad8:
    mov r1, r3
@L207aadc:
    mvn r0, #0
    bic r1, r0, #0x80000000
    ldmfd sp!, {r4, lr}
    bx lr
    mvn r0, #0
    bic r1, r0, #0x80000000
    ldmfd sp!, {r4, lr}
    bx lr
@L207aafc:
    cmp r12, #0x800
    movge lr, #0x80000000
    movlt lr, #0
    ldr r1, [pc, #0xc]
    orr r1, lr, r1
    mov r0, #0
    ldmfd sp!, {r4, lr}
    bx lr
    dcd 0x7ff00000
}
