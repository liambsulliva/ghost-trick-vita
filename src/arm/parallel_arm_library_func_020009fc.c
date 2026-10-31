extern char BuildInfo[];
extern void AutoloadCallback(void);

asm void func_020009fc(void)
{
    ldr r0, =BuildInfo
    ldr r1, [r0]
    ldr r2, [r0, #4]
    ldr r3, [r0, #8]
@next:
    cmp r1, r2
    beq @done
    ldr r5, [r1], #4
    ldr r7, [r1], #4
    add r6, r5, r7
    mov r4, r5
@copy:
    cmp r4, r6
    ldrmi r7, [r3], #4
    strmi r7, [r4], #4
    bmi @copy
    ldr r7, [r1], #4
    add r6, r4, r7
    mov r7, #0
@zero:
    cmp r4, r6
    strlo r7, [r4], #4
    blo @zero
    bic r4, r5, #31
@cache:
    mcr p15, 0, r7, c7, c10, 4
    mcr p15, 0, r4, c7, c5, 1
    mcr p15, 0, r4, c7, c14, 1
    add r4, r4, #32
    cmp r4, r6
    blt @cache
    b @next
@done:
    b AutoloadCallback
}
