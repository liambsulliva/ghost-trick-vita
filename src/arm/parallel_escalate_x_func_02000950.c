asm void func_02000950(register void *bottom)
{
    cmp r0, #0
    beq @60
    stmfd sp!, {r4-r7}
    ldmdb r0, {r1, r2}
    add r2, r0, r2
    sub r3, r0, r1, lsr #24
    bic r1, r1, #0xff000000
    sub r1, r0, r1
    mov r4, r2
@10:
    cmp r3, r1
    ble @50
    ldrb r5, [r3, #-1]!
    mov r6, #8
@20:
    subs r6, r6, #1
    blt @10
    tst r5, #0x80
    bne @30
    ldrb r0, [r3, #-1]!
    strb r0, [r2, #-1]!
    b @40
@30:
    ldrb r12, [r3, #-1]!
    ldrb r7, [r3, #-1]!
    orr r7, r7, r12, lsl #8
    bic r7, r7, #0xf000
    add r7, r7, #2
    add r12, r12, #0x20
@35:
    ldrb r0, [r2, r7]
    strb r0, [r2, #-1]!
    subs r12, r12, #0x10
    bge @35
@40:
    cmp r3, r1
    mov r5, r5, lsl #1
    bgt @20
@50:
    mov r0, #0
    bic r3, r1, #0x1f
@55:
    mcr p15, 0, r0, c7, c10, 4
    mcr p15, 0, r3, c7, c5, 1
    mcr p15, 0, r3, c7, c14, 1
    add r3, r3, #0x20
    cmp r3, r4
    blt @55
    ldmfd sp!, {r4-r7}
@60:
    bx lr
}
