typedef int code();
extern void *func_02007bd8(void);
extern int *func_02007b70(void *system);
extern int func_0200a4a0(int base,int x,int y);
extern int func_0200a7dc(int sprite,int x,int y,int buffer);
extern int *func_02009c1c(void);
extern void func_0202f1bc(int *table,int a,int b,int c,int d);

void func_0200a120(int base,int x,int y,int z)
{
    int result;
    int *allocator;
    int buffer;

    result = *(int *)(base + y * 0x100 + x * 4 + 0x7a04);
    if (result == 0) {
        if (*(unsigned char *)(base + 0xaa42) != 0) {
            result = func_0200a4a0(base,x,y);
        }
        else {
            allocator = func_02007b70(func_02007bd8());
            buffer = (*(code **)(*allocator + 0xc))(allocator,0x100);
            result = func_0200a7dc(base + *(unsigned char *)(base + (x + y * 0x40) + 0x6e02) * 0x10,x,y,buffer);
            *(unsigned short *)(base + 0xaa2e) = *(unsigned short *)(base + 0xaa2e) + 1;
        }
    }
    if (result != 0) {
        func_0202f1bc(func_02009c1c(),result,z << 8,0x100,0xb);
    }
}
