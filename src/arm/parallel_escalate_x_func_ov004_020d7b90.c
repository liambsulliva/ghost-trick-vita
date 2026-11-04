asm void func_ov004_020d7b90(void)
{
    dcd 0xea000202
    dcd 0x00000000
    dcd 0x00000000
    dcd 0x00000000
    mov r6, #32
    add r7, r3, #0x200
    mov r8, #0
    mov r9, #0
    mov r10, #0
    mov r11, #0
@L20d7bb8:
    stmia r7!, {r8, r9, r10, r11}
    subs r6, r6, #1
    bne @L20d7bb8
    mov pc, lr
}
