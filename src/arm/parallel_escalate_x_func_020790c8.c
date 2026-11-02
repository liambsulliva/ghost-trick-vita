asm int func_020790c8(register const char *s1, register const char *s2)
{
    stmfd sp!, {r3, lr}
    and r12, r0, #3
    and r2, r1, #3
    teq r12, r2
    mov r2, #0
    bne @bytes
    cmp r12, #0
    beq @words
    cmp r12, #4
    bhs @words
@head:
    ldrb r3, [r0, r2]
    ldrb lr, [r1, r2]
    cmp r3, lr
    bne @diff1
    cmp r3, #0
    bne @next1
@diff1:
    sub r0, r3, lr
    ldmfd sp!, {r3, pc}
@next1:
    add r2, r2, #1
    add r3, r2, r12
    cmp r3, #4
    blo @head
@words:
    ldr r3, =0x7f7f7f7f
@wloop:
    ldr lr, [r0, r2]
    ldr r12, [r1, r2]
    cmp lr, r12
    bne @bytes
    and r12, lr, r3
    add r12, r12, r3
    orr r12, r12, lr
    orr r12, r12, r3
    mvns r12, r12
    addeq r2, r2, #4
    beq @wloop
@bytes:
    ldrb r3, [r0, r2]
    ldrb r12, [r1, r2]
    cmp r3, r12
    bne @diff2
    cmp r3, #0
    bne @next2
@diff2:
    sub r0, r3, r12
    ldmfd sp!, {r3, pc}
@next2:
    add r2, r2, #1
    b @bytes
    ldmfd sp!, {r3, pc}
}
