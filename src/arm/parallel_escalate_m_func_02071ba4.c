extern unsigned short data_020d0e92;
extern int func_02071b6c(unsigned short *pVRam);

asm int func_02071ba4(void)
{
    mov r2, #0x4000000
    ldr r1, [r2]
    ldr r0, =data_020d0e92
    bic r1, r1, #0x40000000
    ldr r12, =func_02071b6c
    str r1, [r2]
    bx r12
}
