extern unsigned char data_027e0000;
extern void func_01ff8478(void);

asm void func_01ff83a8(void)
{
    mov r12, #0x4000000
    str r12, [r12, #0x208]
    ldr r1, =data_027e0000
    add r1, r1, #16320
    add r1, r1, #60
    mov r0, #0
    str r0, [r1]
    ldr r1, =0x04000180
@10:
    ldrh r0, [r1]
    and r0, r0, #15
    cmp r0, #1
    bne @10
    mov r0, #256
    strh r0, [r1]
    mov r0, #0
    ldr r3, =0x02ffff9c
    ldr r4, [r3]
    ldr r1, =0x02fffd80
    mov r2, #128
    bl func_01ff8478
    ldr r3, =0x02fffd9c
    str r4, [r3]
    ldr r1, =0x02ffff80
    mov r2, #24
    bl func_01ff8478
    ldr r1, =0x02ffff98
    strh r0, [r1]
    ldr r1, =0x02ffff9c
    mov r2, #100
    bl func_01ff8478
    ldr r1, =0x04000180
@20:
    ldrh r0, [r1]
    and r0, r0, #15
    cmp r0, #1
    beq @20
    mov r0, #0
    strh r0, [r1]
    ldr r3, =0x02fffe00
    ldr r12, [r3, #0x24]
    mov lr, r12
    ldr r11, =0x02ffff80
    ldmia r11, {r0-r10}
    mov r11, #0
    bx r12
}
