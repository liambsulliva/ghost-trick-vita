struct Bits { unsigned char a : 1; unsigned char b : 1; unsigned char c : 1; unsigned char rest : 5; };
extern void *func_0200bbe8(void *);
extern int func_0200ba44(void *);
extern int func_0200ba2c(void *);
extern void *func_02009c1c(void);
extern void func_0202f1bc(void *, void *, int, int, int);
void func_ov006_020d8220(unsigned char *p)
{
    if (((struct Bits *)(p + 0xc2))->c) {
        unsigned short *data = (unsigned short *)func_0200bbe8(p + 0x28);
        int length = func_0200ba44(p + 0x28);
        int offset = func_0200ba2c(p + 0x28);
        int i;
        for (i = 1; i < length >> 1; i++) data[i] = 0x4210;
        func_0202f1bc(func_02009c1c(), data, offset << 5, length, 0x12);
    }
}
