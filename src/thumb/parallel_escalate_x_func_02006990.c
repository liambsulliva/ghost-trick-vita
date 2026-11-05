extern unsigned int data_020b18a0;
extern unsigned int func_02002ff4(int id);
extern unsigned int func_02003030(int id);
extern unsigned int func_0200306c(int id);
extern void func_02006954(void *fade, unsigned int from, unsigned int to, int mode, int frames);
extern void func_020079bc(int index, int volume);
extern unsigned int func_0207bd24(unsigned int a, unsigned int b);
extern unsigned int func_0207b3dc(unsigned int a);
extern unsigned int func_0207b424(unsigned int a);
extern unsigned int func_0207bb44(unsigned int a, unsigned int b);
extern unsigned int func_0207c1d8(unsigned int a, unsigned int b);
extern int func_0207ba84(unsigned int a);

asm int func_02006990(register int group, register int sound, register int bank, register unsigned int volume, int frames, int owner)
{
    push {r3, r4, r5, r6, r7, lr}
    sub sp, #0x18
    add r4, r1, #0x0
    ldr r1, [sp, #0x30]
    add r7, r2, #0x0
    str r1, [sp, #0x30]
    ldr r1, [sp, #0x34]
    add r6, r3, #0x0
    str r1, [sp, #0x34]
    mov r1, #0x0
    mvn r1, r1
    ldr r5, =0xffff
    cmp r0, r1
    beq @L9b2
    bl func_02002ff4
    and r5, r0
@L9b2:
    mov r0, #0x0
    mvn r0, r0
    cmp r4, r0
    beq @L9c2
    add r0, r4, #0x0
    bl func_02003030
    and r5, r0
@L9c2:
    mov r0, #0x0
    mvn r0, r0
    cmp r7, r0
    beq @L9d2
    add r0, r7, #0x0
    bl func_0200306c
    and r5, r0
@L9d2:
    mov r0, #0x2
    lsl r0, r0, #0x1e
    tst r0, r6
    beq @L9de
    mov r0, #0x1
    b @L9e0
@L9de:
    mov r0, #0x0
@L9e0:
    str r0, [sp, #0x8]
    ldr r0, =0x7fffffff
    mov r7, #0x0
    and r6, r0
    lsl r0, r6, #0x18
    lsr r0, r0, #0x18
    str r0, [sp, #0xc]
    mov r0, #0x8
    str r0, [sp, #0x14]
    ldr r0, =0x15d4
    str r0, [sp, #0x10]
    sub r0, #0x18
    str r0, [sp, #0x10]
@L9fa:
    ldr r1, =data_020b18a0
    mov r0, #0x4c
    ldr r2, [r1, #0x10]
    ldr r1, [sp, #0x10]
    mul r0, r7
    add r1, r2, r1
    add r2, r5, #0x0
    add r4, r1, r0
    asr r2, r7
    mov r1, #0x1
    tst r1, r2
    beq @Laa8
    ldr r2, [r4, #0x4]
    ldr r1, [sp, #0x8]
    cmp r1, r2
    bne @Laa8
    ldrb r2, [r4, #0x12]
    ldr r1, [sp, #0x34]
    cmp r2, r1
    beq @La26
    cmp r1, #0xff
    bne @Laa8
@La26:
    ldr r1, =data_020b18a0
    ldr r2, =0x15d4
    ldr r1, [r1, #0x10]
    add r2, r1, r2
    add r0, r2, r0
    ldr r2, =0x1a7c
    add r1, r1, r2
    ldrb r2, [r4, #0x11]
    lsl r2, r2, #0x3
    add r1, r1, r2
    str r1, [sp, #0x4]
    cmp r0, #0x0
    beq @Laa8
    ldr r2, [r4, #0x3c]
    ldr r1, [sp, #0x14]
    bic r2, r1
    str r2, [r4, #0x3c]
    mov r1, #0x3
    str r1, [r0, #0x14]
    add r2, r4, #0x0
    ldr r1, [sp, #0xc]
    add r2, #0x31
    strb r1, [r2]
    ldr r1, [sp, #0x30]
    cmp r1, #0x0
    beq @La70
    str r1, [sp]
    ldrb r1, [r4, #0xe]
    add r2, r6, #0x0
    mov r3, #0x2
    bl func_02006954
    ldr r1, [r4, #0x3c]
    mov r0, #0x8
    orr r0, r1
    str r0, [r4, #0x3c]
    b @Laa8
@La70:
    ldr r0, [sp, #0xc]
    strb r0, [r4, #0xe]
    add r0, r6, #0x0
    bl func_0207b3dc
    add r4, r0, #0x0
    ldr r0, [sp, #0x4]
    ldrb r0, [r0, #0x6]
    bl func_0207b424
    add r1, r0, #0x0
    add r0, r4, #0x0
    bl func_0207bb44
    ldr r1, =0x42fe0000
    bl func_0207c1d8
    add r1, r0, #0x0
    mov r0, #0x3f
    lsl r0, r0, #0x18
    bl func_0207bd24
    bl func_0207ba84
    add r1, r0, #0x0
    add r0, r7, #0x0
    bl func_020079bc
@Laa8:
    add r7, r7, #0x1
    cmp r7, #0x10
    blt @L9fa
    mov r0, #0x0
    add sp, #0x18
    pop {r3, r4, r5, r6, r7, pc}
}
