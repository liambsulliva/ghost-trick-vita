asm void func_02073174(register const unsigned char *src, register unsigned char *dest)
{
    stmfd sp!, {r4, r5, r6, r7, lr}
    ldr r5, [r0], #4
    mov r2, r5, lsr #8
    mov r7, #0
    tst r5, #15
    movne r7, #1
@size_loop:
    cmp r2, #0
    ble @done
    ldrb lr, [r0], #1
    mov r4, #8
@bit_loop:
    subs r4, r4, #1
    blt @size_loop
    tst lr, #128
    bne @ref
    ldrb r6, [r0], #1
    swpb r6, r6, [r1]
    add r1, r1, #1
    sub r2, r2, #1
    b @next
@ref:
    ldrb r5, [r0]
    cmp r7, #0
    moveq r6, #3
    beq @length
    tst r5, #224
    movne r6, #1
    bne @length
    add r0, r0, #1
    and r6, r5, #15
    mov r6, r6, lsl #4
    tst r5, #16
    beq @add17
    mov r6, r6, lsl #8
    ldrb r5, [r0], #1
    add r6, r6, r5, lsl #4
    add r6, r6, #256
@add17:
    add r6, r6, #17
    ldrb r5, [r0]
@length:
    add r3, r6, r5, asr #4
    add r0, r0, #1
    and r5, r5, #15
    mov r12, r5, lsl #8
    ldrb r6, [r0], #1
    orr r5, r6, r12
    add r12, r5, #1
    sub r2, r2, r3
@copy:
    ldrb r5, [r1, -r12]
    swpb r5, r5, [r1]
    add r1, r1, #1
    subs r3, r3, #1
    bgt @copy
@next:
    cmp r2, #0
    movgt lr, lr, lsl #1
    bgt @bit_loop
    b @size_loop
@done:
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
}
