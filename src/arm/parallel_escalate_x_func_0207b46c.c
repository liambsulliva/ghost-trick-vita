extern void func_0207b694(void);

asm void func_0207b46c(void)
{
    stmfd sp!, {r4, r5, r6, r7, r11, r12, lr}
    mov r4, r1
    orr r4, r4, #1
    b @1
    stmfd sp!, {r4, r5, r6, r7, r11, r12, lr}
    eor r4, r1, r3
    mov r4, r4, asr #1
    mov r4, r4, lsl #1
@1:
    orrs r5, r3, r2
    bne @2
    ldmfd sp!, {r4, r5, r6, r7, r11, r12, lr}
    bx lr
@2:
    mov r5, r0, lsr #0x1f
    add r5, r5, r1
    mov r6, r2, lsr #0x1f
    add r6, r6, r3
    orrs r6, r5, r6
    bne @3
    mov r1, r2
    bl func_0207b694
    ands r4, r4, #1
    movne r0, r1
    mov r1, r0, asr #0x1f
    ldmfd sp!, {r4, r5, r6, r7, r11, r12, lr}
    bx lr
@3:
    cmp r1, #0
    bge @4
    rsbs r0, r0, #0
    rsc r1, r1, #0
@4:
    cmp r3, #0
    bge @5
    rsbs r2, r2, #0
    rsc r3, r3, #0
@5:
    orrs r5, r1, r0
    beq @17
    mov r5, #0
    mov r6, #1
    cmp r3, #0
    bmi @7
@6:
    add r5, r5, #1
    adds r2, r2, r2
    adcs r3, r3, r3
    bpl @6
    add r6, r6, r5
@7:
    cmp r1, #0
    blt @9
@8:
    cmp r6, #1
    beq @9
    sub r6, r6, #1
    adds r0, r0, r0
    adcs r1, r1, r1
    bpl @8
@9:
    mov r7, #0
    mov r12, #0
    mov r11, #0
    b @11
@10:
    orr r12, r12, #1
    subs r6, r6, #1
    beq @14
    adds r0, r0, r0
    adcs r1, r1, r1
    adcs r7, r7, r7
@11:
    subs r0, r0, r2
    sbcs r1, r1, r3
    sbcs r7, r7, #0
    adds r12, r12, r12
    adc r11, r11, r11
    cmp r7, #0
    bge @10
@12:
    subs r6, r6, #1
    beq @13
    adds r0, r0, r0
    adcs r1, r1, r1
    adc r7, r7, r7
    adds r0, r0, r2
    adcs r1, r1, r3
    adc r7, r7, #0
    adds r12, r12, r12
    adc r11, r11, r11
    cmp r7, #0
    bge @10
    b @12
@13:
    adds r0, r0, r2
    adc r1, r1, r3
@14:
    ands r7, r4, #1
    moveq r0, r12
    moveq r1, r11
    beq @15
    subs r7, r5, #0x20
    movge r0, r1, lsr r7
    bge @18
    rsb r7, r5, #0x20
    mov r0, r0, lsr r5
    orr r0, r0, r1, lsl r7
    mov r1, r1, lsr r5
    b @15
    mov r0, r1, lsr r7
    mov r1, #0
@15:
    cmp r4, #0
    blt @16
    ldmfd sp!, {r4, r5, r6, r7, r11, r12, lr}
    bx lr
@16:
    rsbs r0, r0, #0
    rsc r1, r1, #0
    ldmfd sp!, {r4, r5, r6, r7, r11, r12, lr}
    bx lr
@17:
    mov r0, #0
@18:
    mov r1, #0
    cmp r4, #0
    blt @16
    ldmfd sp!, {r4, r5, r6, r7, r11, r12, lr}
    bx lr
}
