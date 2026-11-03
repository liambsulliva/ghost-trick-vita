asm void func_01ff854c(void)
{
    stmfd sp!, {r4, r5, r6, r7, r8, lr}
    ldr r4, =0x2fffe60
    ldr r3, =0x1ff
    ldr r5, [r4]
    and r4, r0, r3
    bic r3, r5, #0x7000000
    ldr r5, =0x40001a4
    orr r3, r3, #0xa1000000
    rsb r12, r4, #0
@L1ff8570:
    ldr r4, [r5]
    tst r4, #0x80000000
    bne @L1ff8570
    ldr r7, =0x40001a1
    mov r4, #128
    strb r4, [r7]
    cmp r12, r2
    add r0, r0, r12
    ldmgefd sp!, {r4, r5, r6, r7, r8, pc}
    ldr r4, =0x4100010
    mov r6, #183
    mov r5, #0
@L1ff85a0:
    strb r6, [r7, #0x7]
    mov lr, r0, lsr #24
    strb lr, [r7, #0x8]
    mov lr, r0, lsr #16
    strb lr, [r7, #0x9]
    mov lr, r0, lsr #8
    strb lr, [r7, #0xa]
    strb r0, [r7, #0xb]
    strb r5, [r7, #0xc]
    strb r5, [r7, #0xd]
    strb r5, [r7, #0xe]
    str r3, [r7, #0x3]
@L1ff85d0:
    ldr r8, [r7, #0x3]
    tst r8, #0x800000
    beq @L1ff85f4
    cmp r12, #0
    ldr lr, [r4]
    blt @L1ff85f0
    cmp r12, r2
    strlt lr, [r1, r12]
@L1ff85f0:
    add r12, r12, #4
@L1ff85f4:
    tst r8, #0x80000000
    bne @L1ff85d0
    cmp r12, r2
    add r0, r0, #0x200
    blt @L1ff85a0
    ldmfd sp!, {r4, r5, r6, r7, r8, pc}
}
