extern void func_ov004_020d87d4(void);
extern void func_ov004_020d8d80(void);

asm void func_ov004_020d8f5c(void)
{
    stmfd sp!, {r4, r5, r6, r7, r8, r9, r10, r11, r12, lr}
    ldr r1, [r0, #0x4]
    add r2, r0, #8
    ldr r10, [r0, #0x1cc]
    ldr r0, [r0]
    bl func_ov004_020d87d4
    mov r3, r1
    add r4, r2, #64
    add r5, pc, #0x178
    mov r6, #0
    ldr r7, [r2, #0x30]
    add r7, r5, r7, lsl #5
    ldr r8, [r2, #0x20]
    bl func_ov004_020d8d80
    ldr r7, [r2, #0x34]
    add r7, r5, r7, lsl #5
    ldr r8, [r2, #0x24]
    bl func_ov004_020d8d80
    ldr r7, [r2, #0x38]
    add r7, r5, r7, lsl #5
    ldr r8, [r2, #0x28]
    bl func_ov004_020d8d80
    ldr r7, [r2, #0x3c]
    add r7, r5, r7, lsl #5
    ldr r8, [r2, #0x2c]
    bl func_ov004_020d8d80
    ldr r11, [pc, #-128]
    ldr r12, [pc, #-128]
    add r3, r2, #0x190
    ldr r9, [r2, #0x1b4]
    ldr r0, [pc, #-136]
    ldr r4, [pc, #-136]
    mov lr, #0x100
@L20d8fe0:
    ldrsh r5, [r1]
    ldr r6, [r2, #0x1c]
    ldr r7, [r3, #0x1c]
    mla r8, r6, r7, r12
    sub r5, r5, r8, asr #15
    mla r8, r6, r5, r12
    add r8, r7, r8, asr #15
    str r8, [r3, #0x20]
    ldr r6, [r2, #0x18]
    ldr r7, [r3, #0x18]
    mla r8, r6, r7, r12
    sub r5, r5, r8, asr #15
    mla r8, r6, r5, r12
    add r8, r7, r8, asr #15
    str r8, [r3, #0x1c]
    ldr r6, [r2, #0x14]
    ldr r7, [r3, #0x14]
    mla r8, r6, r7, r12
    sub r5, r5, r8, asr #15
    mla r8, r6, r5, r12
    add r8, r7, r8, asr #15
    str r8, [r3, #0x18]
    ldr r6, [r2, #0x10]
    ldr r7, [r3, #0x10]
    mla r8, r6, r7, r12
    sub r5, r5, r8, asr #15
    mla r8, r6, r5, r12
    add r8, r7, r8, asr #15
    str r8, [r3, #0x14]
    ldr r6, [r2, #0xc]
    ldr r7, [r3, #0xc]
    mla r8, r6, r7, r12
    sub r5, r5, r8, asr #15
    mla r8, r6, r5, r12
    add r8, r7, r8, asr #15
    str r8, [r3, #0x10]
    ldr r6, [r2, #0x8]
    ldr r7, [r3, #0x8]
    mla r8, r6, r7, r12
    sub r5, r5, r8, asr #15
    mla r8, r6, r5, r12
    add r8, r7, r8, asr #15
    str r8, [r3, #0xc]
    ldr r6, [r2, #0x4]
    ldr r7, [r3, #0x4]
    mla r8, r6, r7, r12
    sub r5, r5, r8, asr #15
    mla r8, r6, r5, r12
    add r8, r7, r8, asr #15
    str r8, [r3, #0x8]
    ldr r6, [r2]
    ldr r7, [r3]
    mla r8, r6, r7, r12
    sub r5, r5, r8, asr #15
    mla r8, r6, r5, r12
    add r8, r7, r8, asr #15
    str r8, [r3, #0x4]
    str r5, [r3]
    mla r6, r9, r11, r12
    add r9, r5, r6, asr #15
    add r8, r9, r9
    cmp r8, r0
    movlt r8, r0
    cmp r8, r4
    movgt r8, r4
    strh r8, [r1], r10
    subs lr, lr, #1
    bne @L20d8fe0
    str r9, [r3, #0x24]
    ldmfd sp!, {r4, r5, r6, r7, r8, r9, r10, r11, r12, lr}
    bx lr
}
