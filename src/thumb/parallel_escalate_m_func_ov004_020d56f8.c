extern void *func_0202f25c(void);
extern unsigned short func_02071e20(void);
extern unsigned short func_02071e30(void);
extern void func_0207303c(void *, void *, int);
extern void func_0207193c(int);
extern void func_ov004_020d57b0(int, int);
extern void func_02071dbc(void);
extern void func_02071220(int);
extern void func_0206f588(void *, void *, int, int, int, int);
extern unsigned char *func_02008860(void);
extern void func_0203f910(int);
extern unsigned short data_020c9e48[];
extern char data_020c9e64;
extern char data_020c9e74;
extern unsigned int data_ov004_020e45e4[];

void func_ov004_020d56f8(void)
{
    unsigned short *puVar1;
    unsigned char *puVar5;
    unsigned short uVar4;
    unsigned int *piVar3;
    int iVar2;
    int zero;

    puVar5 = func_0202f25c();
    iVar2 = 0xaac;
    puVar1 = data_020c9e48;
    func_0207303c(puVar1, puVar5 + iVar2, 0x54);
    puVar5 = func_0202f25c();
    uVar4 = func_02071e20();
    *(unsigned int *)(puVar5 + iVar2 + 0x54) = (unsigned int)uVar4;
    puVar5 = func_0202f25c();
    uVar4 = func_02071e30();
    *(unsigned int *)(puVar5 + iVar2 + 0x58) = (unsigned int)uVar4;
    func_0207193c(3);
    func_ov004_020d57b0(0x6800000, 0x40000);
    func_02071dbc();
    func_02071220(3);
    zero = 0;
    *(unsigned int *)puVar1 = 0x0001010d;
    puVar1[4] = 0x4403;
    puVar1[4] = puVar1[4] | 0x84;
    puVar1[5] = 0x4a02;
    puVar1[5] = puVar1[5] | 0x84;
    piVar3 = data_ov004_020e45e4;
    func_0206f588(&data_020c9e64, piVar3, 0x80, 0x60, zero, zero);
    func_0206f588(&data_020c9e74, piVar3, 0x80, 0x60, zero, zero);
    puVar5 = func_02008860();
    func_0203f910((int)puVar5);
}
