extern char data_020ce3b8;
extern int func_02009900();
extern int func_0200fb4c();
extern void func_02014ddc();
extern void func_02014de4();
extern int func_020283d0();
extern void func_0202f7e8();
extern int func_02031254();
extern int func_0204026c();
extern void func_020406d8();
extern int func_020410fc();

void func_02040604(char *p, unsigned short *def)
{
    unsigned int a;
    unsigned int b;
    int obj;

    a = 0x8001;
    b = 0x8001 + 0xb;
    *(unsigned short **)(p + 0x3c) = def;
    *(unsigned short *)(p + 0x44) = 0x12f1;
    if (def != 0) {
        a = def[2];
        b = def[1];
        *(unsigned short *)(p + 0x44) = def[0];
    }
    if (*(unsigned char *)(p + 0x2c) != 0) {
        a = 0x8000;
    }
    if (*(int *)(p + 0x30) == 0) {
        obj = func_0204026c(&data_020ce3b8);
        *(int *)(p + 0x30) = obj;
        if (obj != 0) {
            func_020406d8(obj, a, 6);
            func_02014ddc(*(int *)(p + 0x30), func_0200fb4c(*(int *)(p + 0x30)) - 0x50000);
            func_0202f7e8(func_02009900(), *(int *)(p + 0x30), 10, 0);
        }
    }
    if (*(int *)(p + 0x34) == 0) {
        obj = func_0204026c(&data_020ce3b8);
        *(int *)(p + 0x34) = obj;
        if (obj != 0) {
            func_020406d8(obj, b, 6);
            func_02014de4(*(int *)(p + 0x34), func_020283d0(*(int *)(p + 0x34)) - 0x6000);
            func_0202f7e8(func_02009900(), *(int *)(p + 0x34), 10, 0);
        }
    }
    if (*(int *)(p + 0x38) == 0) {
        obj = func_02031254(0x10);
        if (obj != 0) {
            obj = func_020410fc(obj, *(unsigned short *)(p + 0x44));
        }
        *(int *)(p + 0x38) = obj;
    }
}
