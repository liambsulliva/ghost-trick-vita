extern void func_ov004_020e41c8(void);
extern void func_ov004_020e43f4(void);

asm void func_ov004_020e4098(void)
{
    stmfd sp!, {r4, r5, r6, r7, r8, r9, lr}
    sub sp, sp, #24
    mov r4, r0
    ldr r5, [r4, #0x18]
    add r6, pc, #192
    ldrb r6, [r6, r5]
    add r7, pc, #221
    ldrb r7, [r7, r5]
    orr r6, r6, r7, lsl #8
    str r6, [sp, #0x10]
    ldr r5, [r4]
    ldr r6, [r4, #0x8]
    ldr r7, [r4, #0x10]
    ldr r8, [r4, #0x14]
    ldr r9, [r4, #0x1c]
    str r5, [sp]
    str r6, [sp, #0x4]
    str r7, [sp, #0x8]
    str r8, [sp, #0xc]
    add r9, r9, #64
    str r9, [sp, #0x14]
    mov r0, sp
    bl func_ov004_020e41c8
    ldr r0, [r4, #0x20]
    cmp r0, #1
    movne r0, sp
    dcd 0x1b00007e
    ldr r5, [r4, #0x4]
    ldr r6, [r4, #0xc]
    ldr r7, [r4, #0x10]
    ldr r8, [r4, #0x14]
    str r5, [sp]
    str r6, [sp, #0x4]
    mov r7, r7, asr #1
    mov r8, r8, asr #1
    str r7, [sp, #0x8]
    str r8, [sp, #0xc]
    mov r0, sp
    bl func_ov004_020e43f4
    ldr r0, [r4, #0x20]
    cmp r0, #1
    movne r0, sp
    dcd 0x1b0000ed
    ldr r5, [r4, #0x4]
    ldr r6, [r4, #0xc]
    add r5, r5, #128
    add r6, r6, #128
    str r5, [sp]
    str r6, [sp, #0x4]
    mov r0, sp
    bl func_ov004_020e43f4
    ldr r0, [r4, #0x20]
    cmp r0, #1
    movne r0, sp
    dcd 0x1b0000e1
    add sp, sp, #24
    ldmfd sp!, {r4, r5, r6, r7, r8, r9, pc}
}
