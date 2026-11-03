typedef struct Screen5c10 {
    char pad0[0x9cc];
    int top;
    int bottom;
    char pad9d4[0xd5];
    unsigned char lowaa9 : 7;
    unsigned char ready : 1;
    unsigned char screens : 3;
    unsigned char highaaa : 5;
} Screen5c10;

extern volatile int data_020bbb50;
extern int func_ov004_020d5b78(Screen5c10 *p);
extern int func_ov004_020d5b8c(Screen5c10 *p);
extern void func_ov004_020d3e34(int a, int b, int c, int d);

void func_ov004_020d5c10(Screen5c10 *p)
{
    while (!((volatile Screen5c10 *)p)->ready) {
    }
    while (data_020bbb50 != 1) {
    }
    data_020bbb50 = 0;
    if (p->screens & 1) {
        func_ov004_020d3e34(p->top, func_ov004_020d5b78(p), 0x100, 0);
    }
    if (p->screens & 2) {
        func_ov004_020d3e34(p->bottom, func_ov004_020d5b8c(p), 0x100, 0);
    }
    p->ready = 0;
}
