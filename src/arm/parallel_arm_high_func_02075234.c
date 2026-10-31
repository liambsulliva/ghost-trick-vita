extern void func_02074f5c(void);
extern void func_02074f6c(void);

asm void func_02075234(void)
{
    stmfd sp!, {r3, lr}
    ldr r0, =0x020d11ac
    ldr r0, [r0, #0xc]
    cmp r0, #0
    ldmeqfd sp!, {r3, pc}
    mrs r2, cpsr
    mov r0, sp
    ldr r1, =0x9f
    msr CPSR_fsxc, r1
    mov r1, sp
    mov sp, r0
    stmfd sp!, {r1, r2}
    bl func_02074f5c
    ldr r0, =0x020d11dc
    ldr r1, =0x020d11b0
    ldr r1, [r1]
    ldr r12, =0x020d11b8
    ldr r12, [r12]
    ldr lr, =0x02075284
    bx r12
    bl func_02074f6c
    ldmfd sp!, {r1, r2}
    mov sp, r1
    msr CPSR_fsxc, r2
    ldmfd sp!, {r3, pc}
}
