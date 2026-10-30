asm void func_020748d4(void)
{
    mov r12, #0x4000000
    ldr r1, [r12, #0x208]
    str r12, [r12, #0x208]
@10:
    ldrh r0, [r12, #6]
    cmp r0, #0
    bne @10
    str r1, [r12, #0x208]
    bx lr
}
