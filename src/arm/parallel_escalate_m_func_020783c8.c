extern unsigned int func_020781f4(int packet,int count,unsigned int third,unsigned int fourth,unsigned int fifth);

asm unsigned int func_020783c8(unsigned int id,unsigned int value,unsigned int third,unsigned int fourth,unsigned int fifth)
{
    stmdb sp!, {lr}
    sub sp, sp, #12
    and r0, r0, #255
    orr lr, r0, #0x6100
    mov r0, r1, lsl #16
    ldr r1, =0x01010000
    orr lr, lr, #0x2000000
    orr r1, r1, r0, lsr #16
    str r1, [sp, #8]
    ldr r12, [sp, #16]
    str lr, [sp, #4]
    add r0, sp, #4
    mov r1, #2
    str r12, [sp]
    bl func_020781f4
    add sp, sp, #12
    ldmia sp!, {pc}
}
