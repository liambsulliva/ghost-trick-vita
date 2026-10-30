extern void func_020751a0(void);

asm void func_0207512c(void)
{
    ldr r12, =0x020d11ac
    ldr r12, [r12]
    cmp r12, #0
    movne lr, pc
    bxne r12
    ldr r12, =0x02000000
    stmdb r12!, {r0, r1, r2, r3, sp, lr}
    and r0, sp, #1
    mov sp, r12
    mrs r1, cpsr
    and r1, r1, #31
    teq r1, #23
    bne @l168
    bl func_020751a0
    b @l174
@l168:
    teq r1, #27
    bne @l174
    bl func_020751a0
@l174:
    ldr r12, =0x020d11ac
    ldr r12, [r12]
    cmp r12, #0
@spin:
    beq @spin
@body:
    mov r0, r0
    b @body
    ldmfd sp!, {r0, r1, r2, r3, r12, lr}
    mov sp, r12
    bx lr
}
