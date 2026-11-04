const unsigned int data_020725b4[4] = {
    0x5a827999, 0x6ed9eba1, 0x8f1bbcdc, 0xca62c1d6
};

asm void func_020725c4(void)
{
    stmfd sp!, {r4, r5, r6, r7, r8, r9, r10, r11, r12, lr}
    mov r11, r0
    sub sp, sp, #64
    mov r12, sp
    mvn r3, #0xff00
    add r8, r11, #20
    mov r1, r12
    ldmia r8!, {r4, r5, r6, r7}
    mov r9, #16
@L20725e8:
    eor r2, r4, r4, ror #16
    and r2, r3, r2, lsr #8
    eor r4, r2, r4, ror #8
    eor r2, r5, r5, ror #16
    and r2, r3, r2, lsr #8
    eor r5, r2, r5, ror #8
    eor r2, r6, r6, ror #16
    and r2, r3, r2, lsr #8
    eor r6, r2, r6, ror #8
    eor r2, r7, r7, ror #16
    and r2, r3, r2, lsr #8
    eor r7, r2, r7, ror #8
    stmia r1!, {r4, r5, r6, r7}
    subs r9, r9, #4
    ldmneia r8!, {r4, r5, r6, r7}
    bne @L20725e8
    ldmia r11, {r4, r5, r6, r7, r8}
    ldr r12, [pc, #-128]
    mov r9, #0
@L2072634:
    and r10, r5, r6
    mvn lr, r5
    and lr, lr, r7
    orr r10, r10, lr
    add r10, r10, r12
    and lr, r9, #15
    ldr lr, [sp, lr, lsl #2]
    add r10, r10, r8
    add r10, r10, lr
    add r10, r10, r4, ror #27
    mov r8, r7
    mov r7, r6
    mov r6, r5, ror #2
    mov r5, r4
    mov r4, r10
    add r9, r9, #1
    cmp r9, #16
    bne @L2072634
@L207267c:
    and r10, r5, r6
    mvn lr, r5
    and lr, lr, r7
    orr r10, r10, lr
    add r10, r10, r12
    sub r2, r9, #16
    and r2, r2, #15
    sub lr, r9, #14
    and lr, lr, #15
    ldr r3, [sp, r2, lsl #2]
    ldr r1, [sp, lr, lsl #2]
    sub lr, r9, #8
    eor r3, r3, r1
    and lr, lr, #15
    ldr r1, [sp, lr, lsl #2]
    sub r2, r9, #3
    eor r3, r3, r1
    and r2, r2, #15
    ldr r1, [sp, r2, lsl #2]
    and r2, r9, #15
    eor r3, r3, r1
    mov r3, r3, ror #31
    str r3, [sp, r2, lsl #2]
    and lr, r9, #15
    ldr lr, [sp, lr, lsl #2]
    add r10, r10, r8
    add r10, r10, lr
    add r10, r10, r4, ror #27
    mov r8, r7
    mov r7, r6
    mov r6, r5, ror #2
    mov r5, r4
    mov r4, r10
    add r9, r9, #1
    cmp r9, #20
    bne @L207267c
    ldr r12, [pc, #-348]
@L2072710:
    eor r10, r5, r6
    eor r10, r10, r7
    add r10, r10, r12
    sub r2, r9, #16
    and r2, r2, #15
    sub lr, r9, #14
    and lr, lr, #15
    ldr r3, [sp, r2, lsl #2]
    ldr r1, [sp, lr, lsl #2]
    sub lr, r9, #8
    eor r3, r3, r1
    and lr, lr, #15
    ldr r1, [sp, lr, lsl #2]
    sub r2, r9, #3
    eor r3, r3, r1
    and r2, r2, #15
    ldr r1, [sp, r2, lsl #2]
    and r2, r9, #15
    eor r3, r3, r1
    mov r3, r3, ror #31
    str r3, [sp, r2, lsl #2]
    and lr, r9, #15
    ldr lr, [sp, lr, lsl #2]
    add r10, r10, r8
    add r10, r10, lr
    add r10, r10, r4, ror #27
    mov r8, r7
    mov r7, r6
    mov r6, r5, ror #2
    mov r5, r4
    mov r4, r10
    add r9, r9, #1
    cmp r9, #40
    bne @L2072710
    ldr r12, [pc, #-484]
@L207279c:
    and r10, r5, r6
    and lr, r5, r7
    orr r10, r10, lr
    and lr, r6, r7
    orr r10, r10, lr
    add r10, r10, r12
    sub r2, r9, #16
    and r2, r2, #15
    sub lr, r9, #14
    and lr, lr, #15
    ldr r3, [sp, r2, lsl #2]
    ldr r1, [sp, lr, lsl #2]
    sub lr, r9, #8
    eor r3, r3, r1
    and lr, lr, #15
    ldr r1, [sp, lr, lsl #2]
    sub r2, r9, #3
    eor r3, r3, r1
    and r2, r2, #15
    ldr r1, [sp, r2, lsl #2]
    and r2, r9, #15
    eor r3, r3, r1
    mov r3, r3, ror #31
    str r3, [sp, r2, lsl #2]
    and lr, r9, #15
    ldr lr, [sp, lr, lsl #2]
    add r10, r10, r8
    add r10, r10, lr
    add r10, r10, r4, ror #27
    mov r8, r7
    mov r7, r6
    mov r6, r5, ror #2
    mov r5, r4
    mov r4, r10
    add r9, r9, #1
    cmp r9, #60
    bne @L207279c
    ldr r12, [pc, #-632]
@L2072834:
    eor r10, r5, r6
    eor r10, r10, r7
    add r10, r10, r12
    sub r2, r9, #16
    and r2, r2, #15
    sub lr, r9, #14
    and lr, lr, #15
    ldr r3, [sp, r2, lsl #2]
    ldr r1, [sp, lr, lsl #2]
    sub lr, r9, #8
    eor r3, r3, r1
    and lr, lr, #15
    ldr r1, [sp, lr, lsl #2]
    sub r2, r9, #3
    eor r3, r3, r1
    and r2, r2, #15
    ldr r1, [sp, r2, lsl #2]
    and r2, r9, #15
    eor r3, r3, r1
    mov r3, r3, ror #31
    str r3, [sp, r2, lsl #2]
    and lr, r9, #15
    ldr lr, [sp, lr, lsl #2]
    add r10, r10, r8
    add r10, r10, lr
    add r10, r10, r4, ror #27
    mov r8, r7
    mov r7, r6
    mov r6, r5, ror #2
    mov r5, r4
    mov r4, r10
    add r9, r9, #1
    cmp r9, #80
    bne @L2072834
    ldmia r11, {r1, r2, r3, r9, r10}
    add r1, r1, r4
    add r2, r2, r5
    add r3, r3, r6
    add r9, r9, r7
    add r10, r10, r8
    stmia r11, {r1, r2, r3, r9, r10}
    add sp, sp, #64
    ldmfd sp!, {r4, r5, r6, r7, r8, r9, r10, r11, r12, pc}
}
