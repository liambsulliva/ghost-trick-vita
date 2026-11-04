asm void func_ov004_020d87d4(void)
{
    ldrh r3, [r0], #2
    ldrh r4, [r0], #2
    orr r3, r3, r4, lsl #16
    sub r12, pc, #0x294
    mov r4, r3, lsr #26
    ldr r5, [r12, r4, lsl #2]
    str r5, [r2]
    mov r4, r3, lsr #20
    and r4, r4, #63
    ldr r5, [r12, r4, lsl #2]
    str r5, [r2, #4]
    sub r12, pc, #0x1b4
    mov r4, r3, lsr #15
    and r4, r4, #31
    ldr r5, [r12, r4, lsl #2]
    str r5, [r2, #8]
    sub r12, pc, #0x148
    mov r4, r3, lsr #10
    and r4, r4, #31
    ldr r5, [r12, r4, lsl #2]
    str r5, [r2, #12]
    sub r12, pc, #220
    mov r4, r3, lsr #6
    and r4, r4, #15
    ldr r5, [r12, r4, lsl #2]
    str r5, [r2, #16]
    sub r12, pc, #176
    mov r4, r3, lsr #3
    and r4, r4, #7
    ldr r5, [r12, r4, lsl #2]
    str r5, [r2, #24]
    sub r12, pc, #164
    and r3, r3, #7
    ldr r5, [r12, r3, lsl #2]
    str r5, [r2, #28]
    ldrh r3, [r0], #2
    ldrh r4, [r0], #2
    orr r3, r3, r4, lsl #16
    mov r4, r3, lsr #26
    str r4, [r2, #60]
    mov r4, r3, lsr #20
    and r4, r4, #63
    str r4, [r2, #56]
    mov r4, r3, lsr #14
    and r4, r4, #63
    str r4, [r2, #52]
    mov r4, r3, lsr #8
    and r4, r4, #63
    str r4, [r2, #48]
    mov r4, r3, lsr #6
    and r4, r4, #3
    str r4, [r2, #44]
    mov r4, r3, lsr #4
    and r4, r4, #3
    str r4, [r2, #40]
    mov r4, r3, lsr #2
    and r4, r4, #3
    str r4, [r2, #36]
    and r3, r3, #3
    str r3, [r2, #32]
    ldrh r3, [r0], #2
    ldrh r4, [r0], #2
    orr r3, r3, r4, lsl #16
    mov r4, r3, lsr #29
    str r4, [r2, #64]
    mov r4, r3, lsr #26
    and r4, r4, #7
    str r4, [r2, #68]
    mov r4, r3, lsr #23
    and r4, r4, #7
    str r4, [r2, #72]
    mov r4, r3, lsr #20
    and r4, r4, #7
    str r4, [r2, #76]
    mov r4, r3, lsr #17
    and r4, r4, #7
    str r4, [r2, #80]
    mov r4, r3, lsr #14
    and r4, r4, #7
    str r4, [r2, #84]
    mov r4, r3, lsr #11
    and r4, r4, #7
    str r4, [r2, #88]
    mov r4, r3, lsr #8
    and r4, r4, #7
    str r4, [r2, #92]
    mov r4, r3, lsr #5
    and r4, r4, #7
    str r4, [r2, #96]
    mov r4, r3, lsr #2
    and r4, r4, #7
    str r4, [r2, #100]
    ldrh r5, [r0], #2
    ldrh r4, [r0], #2
    orr r5, r5, r4, lsl #16
    mov r4, r5, lsr #29
    str r4, [r2, #104]
    mov r4, r5, lsr #26
    and r4, r4, #7
    str r4, [r2, #108]
    mov r4, r5, lsr #23
    and r4, r4, #7
    str r4, [r2, #112]
    mov r4, r5, lsr #20
    and r4, r4, #7
    str r4, [r2, #116]
    mov r4, r5, lsr #17
    and r4, r4, #7
    str r4, [r2, #120]
    mov r4, r5, lsr #14
    and r4, r4, #7
    str r4, [r2, #124]
    mov r4, r5, lsr #11
    and r4, r4, #7
    str r4, [r2, #128]
    mov r4, r5, lsr #8
    and r4, r4, #7
    str r4, [r2, #132]
    mov r4, r5, lsr #5
    and r4, r4, #7
    str r4, [r2, #136]
    mov r4, r5, lsr #2
    and r4, r4, #7
    str r4, [r2, #140]
    and r3, r3, #3
    mov r4, r5, lsr #1
    and r4, r4, #1
    orr r4, r4, r3, lsl #1
    str r4, [r2, #144]
    ldrh r3, [r0], #2
    ldrh r4, [r0], #2
    orr r3, r3, r4, lsl #16
    mov r4, r3, lsr #29
    str r4, [r2, #148]
    mov r4, r3, lsr #26
    and r4, r4, #7
    str r4, [r2, #152]
    mov r4, r3, lsr #23
    and r4, r4, #7
    str r4, [r2, #156]
    mov r4, r3, lsr #20
    and r4, r4, #7
    str r4, [r2, #160]
    mov r4, r3, lsr #17
    and r4, r4, #7
    str r4, [r2, #164]
    mov r4, r3, lsr #14
    and r4, r4, #7
    str r4, [r2, #168]
    mov r4, r3, lsr #11
    and r4, r4, #7
    str r4, [r2, #172]
    mov r4, r3, lsr #8
    and r4, r4, #7
    str r4, [r2, #176]
    mov r4, r3, lsr #5
    and r4, r4, #7
    str r4, [r2, #180]
    mov r4, r3, lsr #2
    and r4, r4, #7
    str r4, [r2, #184]
    ldrh r6, [r0], #2
    ldrh r4, [r0], #2
    orr r6, r6, r4, lsl #16
    mov r4, r6, lsr #29
    str r4, [r2, #188]
    mov r4, r6, lsr #26
    and r4, r4, #7
    str r4, [r2, #192]
    mov r4, r6, lsr #23
    and r4, r4, #7
    str r4, [r2, #196]
    mov r4, r6, lsr #20
    and r4, r4, #7
    str r4, [r2, #200]
    mov r4, r6, lsr #17
    and r4, r4, #7
    str r4, [r2, #204]
    mov r4, r6, lsr #14
    and r4, r4, #7
    str r4, [r2, #208]
    mov r4, r6, lsr #11
    and r4, r4, #7
    str r4, [r2, #212]
    mov r4, r6, lsr #8
    and r4, r4, #7
    str r4, [r2, #216]
    mov r4, r6, lsr #5
    and r4, r4, #7
    str r4, [r2, #220]
    mov r4, r6, lsr #2
    and r4, r4, #7
    str r4, [r2, #224]
    and r3, r3, #3
    mov r4, r6, lsr #1
    and r4, r4, #1
    orr r4, r4, r3, lsl #1
    str r4, [r2, #228]
    ldrh r3, [r0], #2
    ldrh r4, [r0], #2
    orr r3, r3, r4, lsl #16
    mov r4, r3, lsr #29
    str r4, [r2, #232]
    mov r4, r3, lsr #26
    and r4, r4, #7
    str r4, [r2, #236]
    mov r4, r3, lsr #23
    and r4, r4, #7
    str r4, [r2, #240]
    mov r4, r3, lsr #20
    and r4, r4, #7
    str r4, [r2, #244]
    mov r4, r3, lsr #17
    and r4, r4, #7
    str r4, [r2, #248]
    mov r4, r3, lsr #14
    and r4, r4, #7
    str r4, [r2, #252]
    mov r4, r3, lsr #11
    and r4, r4, #7
    str r4, [r2, #0x100]
    mov r4, r3, lsr #8
    and r4, r4, #7
    str r4, [r2, #0x104]
    mov r4, r3, lsr #5
    and r4, r4, #7
    str r4, [r2, #0x108]
    mov r4, r3, lsr #2
    and r4, r4, #7
    str r4, [r2, #0x10c]
    ldrh r7, [r0], #2
    ldrh r4, [r0], #2
    orr r7, r7, r4, lsl #16
    mov r4, r7, lsr #29
    str r4, [r2, #0x110]
    mov r4, r7, lsr #26
    and r4, r4, #7
    str r4, [r2, #0x114]
    mov r4, r7, lsr #23
    and r4, r4, #7
    str r4, [r2, #0x118]
    mov r4, r7, lsr #20
    and r4, r4, #7
    str r4, [r2, #0x11c]
    mov r4, r7, lsr #17
    and r4, r4, #7
    str r4, [r2, #0x120]
    mov r4, r7, lsr #14
    and r4, r4, #7
    str r4, [r2, #0x124]
    mov r4, r7, lsr #11
    and r4, r4, #7
    str r4, [r2, #0x128]
    mov r4, r7, lsr #8
    and r4, r4, #7
    str r4, [r2, #0x12c]
    mov r4, r7, lsr #5
    and r4, r4, #7
    str r4, [r2, #0x130]
    mov r4, r7, lsr #2
    and r4, r4, #7
    str r4, [r2, #0x134]
    and r3, r3, #3
    mov r4, r7, lsr #1
    and r4, r4, #1
    orr r4, r4, r3, lsl #1
    str r4, [r2, #0x138]
    ldrh r3, [r0], #2
    ldrh r4, [r0], #2
    orr r3, r3, r4, lsl #16
    mov r4, r3, lsr #29
    str r4, [r2, #0x13c]
    mov r4, r3, lsr #26
    and r4, r4, #7
    str r4, [r2, #0x140]
    mov r4, r3, lsr #23
    and r4, r4, #7
    str r4, [r2, #0x144]
    mov r4, r3, lsr #20
    and r4, r4, #7
    str r4, [r2, #0x148]
    mov r4, r3, lsr #17
    and r4, r4, #7
    str r4, [r2, #0x14c]
    mov r4, r3, lsr #14
    and r4, r4, #7
    str r4, [r2, #0x150]
    mov r4, r3, lsr #11
    and r4, r4, #7
    str r4, [r2, #0x154]
    mov r4, r3, lsr #8
    and r4, r4, #7
    str r4, [r2, #0x158]
    mov r4, r3, lsr #5
    and r4, r4, #7
    str r4, [r2, #0x15c]
    mov r4, r3, lsr #2
    and r4, r4, #7
    str r4, [r2, #0x160]
    ldrh r8, [r0], #2
    ldrh r4, [r0], #2
    orr r8, r8, r4, lsl #16
    mov r4, r8, lsr #29
    str r4, [r2, #0x164]
    mov r4, r8, lsr #26
    and r4, r4, #7
    str r4, [r2, #0x168]
    mov r4, r8, lsr #23
    and r4, r4, #7
    str r4, [r2, #0x16c]
    mov r4, r8, lsr #20
    and r4, r4, #7
    str r4, [r2, #0x170]
    mov r4, r8, lsr #17
    and r4, r4, #7
    str r4, [r2, #0x174]
    mov r4, r8, lsr #14
    and r4, r4, #7
    str r4, [r2, #0x178]
    mov r4, r8, lsr #11
    and r4, r4, #7
    str r4, [r2, #0x17c]
    mov r4, r8, lsr #8
    and r4, r4, #7
    str r4, [r2, #0x180]
    mov r4, r8, lsr #5
    and r4, r4, #7
    str r4, [r2, #0x184]
    mov r4, r8, lsr #2
    and r4, r4, #7
    str r4, [r2, #0x188]
    and r3, r3, #3
    mov r4, r8, lsr #1
    and r4, r4, #1
    orr r4, r4, r3, lsl #1
    str r4, [r2, #0x18c]
    add r12, pc, #36
    and r5, r5, #1
    and r6, r6, #1
    and r7, r7, #1
    and r8, r8, #1
    orr r8, r8, r7, lsl #1
    orr r8, r8, r6, lsl #2
    orr r8, r8, r5, lsl #3
    ldr r4, [r12, r8, lsl #2]
    str r4, [r2, #20]
    mov pc, lr
}
