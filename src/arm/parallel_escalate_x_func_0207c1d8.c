asm void func_0207c1d8(void)
{
    stmfd sp!, {lr}
    mov r12, #255
    ands r3, r12, r0, lsr #23
    cmpne r3, #255
    beq @L207c3ac
    ands r12, r12, r1, lsr #23
    cmpne r12, #255
    beq @L207c3e8
    orr r1, r1, #0x800000
    orr r0, r0, #0x800000
    bic r2, r0, #0xff000000
    bic lr, r1, #0xff000000
@L207c208:
    cmp r2, lr
    movlo r2, r2, lsl #1
    sublo r3, r3, #1
    teq r0, r1
    sub r0, pc, #148
    ldrb r1, [r0, lr, lsr #15]
    rsb lr, lr, #0
    mov r0, lr, asr #1
    mul r0, r1, r0
    add r0, r0, #0x80000000
    mov r0, r0, lsr #6
    mul r0, r1, r0
    mov r0, r0, lsr #14
    mul r1, lr, r0
    sub r12, r3, r12
    mov r1, r1, lsr #12
    mul r1, r0, r1
    mov r0, r0, lsl #14
    add r0, r0, r1, lsr #15
    umull r1, r0, r2, r0
    mov r3, r0
    orrmi r0, r0, #0x80000000
    adds r12, r12, #126
    bmi @L207c4b0
    cmp r12, #254
    bge @L207c564
    add r0, r0, r12, lsl #23
    mov r12, r1, lsr #28
    cmp r12, #7
    beq @L207c38c
    add r0, r0, r1, lsr #31
    ldmfd sp!, {lr}
    bx lr
    dcd 0xfdfeffff
    dcd 0xf9fafbfc
    dcd 0xf5f6f7f8
    dcd 0xf1f2f3f4
    dcd 0xeeeff0f0
    dcd 0xeaebeced
    dcd 0xe7e8e9ea
    dcd 0xe4e5e6e6
    dcd 0xe1e2e2e3
    dcd 0xdedfdfe0
    dcd 0xdbdcdcdd
    dcd 0xd8d9d9da
    dcd 0xd5d6d7d7
    dcd 0xd2d3d4d4
    dcd 0xd0d0d1d2
    dcd 0xcdcececf
    dcd 0xcbcbcccc
    dcd 0xc8c9c9ca
    dcd 0xc6c6c7c8
    dcd 0xc3c4c5c5
    dcd 0xc1c2c2c3
    dcd 0xbfbfc0c0
    dcd 0xbdbdbebe
    dcd 0xbabbbcbc
    dcd 0xb8b9b9ba
    dcd 0xb6b7b7b8
    dcd 0xb4b5b5b6
    dcd 0xb2b3b3b4
    dcd 0xb0b1b1b2
    dcd 0xafafafb0
    dcd 0xadadaeae
    dcd 0xababacac
    dcd 0xa9aaaaaa
    dcd 0xa7a8a8a9
    dcd 0xa6a6a7a7
    dcd 0xa4a4a5a5
    dcd 0xa2a3a3a4
    dcd 0xa1a1a2a2
    dcd 0x9fa0a0a0
    dcd 0x9e9e9e9f
    dcd 0x9c9d9d9d
    dcd 0x9b9b9b9c
    dcd 0x999a9a9a
    dcd 0x98989999
    dcd 0x96979798
    dcd 0x95959696
    dcd 0x94949495
    dcd 0x92939393
    dcd 0x91919292
    dcd 0x90909191
    dcd 0x8f8f8f90
    dcd 0x8d8e8e8e
    dcd 0x8c8c8d8d
    dcd 0x8b8b8c8c
    dcd 0x8a8a8a8b
    dcd 0x8989898a
    dcd 0x88888888
    dcd 0x86878787
    dcd 0x85868686
    dcd 0x84858585
    dcd 0x83838484
    dcd 0x82828383
    dcd 0x81818282
    dcd 0x80808181
@L207c38c:
    mov r1, r3, lsl #1
    add r1, r1, #1
    rsb lr, lr, #0
    mul r1, lr, r1
    cmp r1, r2, lsl #24
    addmi r0, r0, #1
    ldmfd sp!, {lr}
    bx lr
@L207c3ac:
    eor lr, r0, r1
    and lr, lr, #0x80000000
    cmp r3, #0
    beq @L207c404
    movs r0, r0, lsl #9
    bne @L207c54c
    mov r12, r1, lsr #23
    mov r1, r1, lsl #9
    ands r12, r12, #255
    beq @L207c53c
    cmp r12, #255
    blt @L207c53c
    cmp r1, #0
    beq @L207c558
    b @L207c534
@L207c3e8:
    eor lr, r0, r1
    and lr, lr, #0x80000000
    cmp r12, #0
    beq @L207c468
@L207c3f8:
    movs r1, r1, lsl #9
    bne @L207c534
    b @L207c584
@L207c404:
    movs r2, r0, lsl #9
    beq @L207c438
    clz r3, r2
    movs r2, r2, lsl r3
    rsb r3, r3, #0
    mov r2, r2, lsr #8
    ands r12, r12, r1, lsr #23
    beq @L207c490
    cmp r12, #255
    beq @L207c3f8
    orr r1, r1, #0x800000
    bic lr, r1, #0xff000000
    b @L207c208
@L207c438:
    mov r12, r1, lsr #23
    mov r1, r1, lsl #9
    ands r12, r12, #255
    beq @L207c45c
    cmp r12, #255
    blt @L207c584
    cmp r1, #0
    beq @L207c584
    b @L207c534
@L207c45c:
    cmp r1, #0
    beq @L207c558
    b @L207c584
@L207c468:
    movs r12, r1, lsl #9
    beq @L207c53c
    mov lr, r12
    clz r12, lr
    movs lr, lr, lsl r12
    rsb r12, r12, #0
    mov lr, lr, lsr #8
    orr r0, r0, #0x800000
    bic r2, r0, #0xff000000
    b @L207c208
@L207c490:
    movs r12, r1, lsl #9
    beq @L207c53c
    mov lr, r12
    clz r12, lr
    movs lr, lr, lsl r12
    rsb r12, r12, #0
    mov lr, lr, lsr #8
    b @L207c208
@L207c4b0:
    and r0, r0, #0x80000000
    cmn r12, #24
    beq @L207c524
    bmi @L207c57c
    add r1, r12, #23
    mov r2, r2, lsl r1
    rsb r12, r12, #0
    mov r3, r3, lsr r12
    orr r0, r0, r3
    rsb lr, lr, #0
    mul r1, lr, r3
    cmp r1, r2
    ldmeqfd sp!, {lr}
    bxeq lr
    add r1, r1, lr
    cmp r1, r2
    beq @L207c518
    addmi r0, r0, #1
    subpl r1, r1, lr
    add r1, lr, r1, lsl #1
    cmp r1, r2, lsl #1
    and r3, r0, #1
    addmi r0, r0, #1
    addeq r0, r0, r3
    ldmfd sp!, {lr}
    bx lr
@L207c518:
    add r0, r0, #1
    ldmfd sp!, {lr}
    bx lr
@L207c524:
    cmn r2, lr
    addne r0, r0, #1
    ldmfd sp!, {lr}
    bx lr
@L207c534:
    mov r0, r1
    b @L207c54c
@L207c53c:
    mov r0, #0xff000000
    orr r0, lr, r0, lsr #1
    ldmfd sp!, {lr}
    bx lr
@L207c54c:
    mvn r0, #0x80000000
    ldmfd sp!, {lr}
    bx lr
@L207c558:
    mvn r0, #0x80000000
    ldmfd sp!, {lr}
    bx lr
@L207c564:
    tst r0, #0x80000000
    mov r0, #0xff000000
    movne r0, r0, asr #1
    moveq r0, r0, lsr #1
    ldmfd sp!, {lr}
    bx lr
@L207c57c:
    ldmfd sp!, {lr}
    bx lr
@L207c584:
    mov r0, lr
    ldmfd sp!, {lr}
    bx lr
}
