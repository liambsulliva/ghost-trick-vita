extern void func_01ff854c(void);

asm unsigned int func_01ff848c(register int value)
{
    stmfd sp!, {r4, lr}
    mov r4, r0
    cmp r4, #0
    movne r0, #0
    ldreq r0, =0x02fffc2c
    ldreq r0, [r0]
    stmfd sp!, {r0}
    cmp r0, #32768
    ldrhs r1, =0x02fffe00
    movhs r2, #352
    blhs func_01ff854c
    ldr r12, =0x02fffe00
    ldr r0, [r12, #32]
    ldr r1, [r12, #40]
    ldr r2, [r12, #44]
    ldr r3, [sp]
    add r0, r0, r3
    subs r3, r0, #32768
    movlt r0, #32768
    sublt r1, r1, r3
    addlt r2, r2, r3
    cmp r2, #0
    blgt func_01ff854c
    ldr r12, =0x02fffe00
    ldr r0, [r12, #48]
    ldr r1, [r12, #56]
    ldr r2, [r12, #60]
    ldr r3, [sp]
    add r0, r0, r3
    cmp r2, #0
    blgt func_01ff854c
    ldmfd sp!, {r0}
    mov r1, #0
@outer:
    mov r0, #0
@inner:
    orr r2, r1, r0
    mcr p15, 0, r2, c7, c10, 2
    add r0, r0, #32
    cmp r0, #1024
    blt @inner
    adds r1, r1, #0x40000000
    bne @outer
    mov r0, #0
    mcr p15, 0, r0, c7, c6, 0
    mcr p15, 0, r0, c7, c5, 0
    mcr p15, 0, r0, c7, c10, 4
    ldmfd sp!, {r4, pc}
}
