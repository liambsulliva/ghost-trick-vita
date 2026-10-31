struct Flags020592c8 {
    unsigned char a : 1;
    unsigned char b : 1;
};

extern void func_02008378(int *dst, unsigned char *a, int *b);
extern unsigned char *func_020083a0(unsigned char *p);
extern void func_020083b0(int *dst, int *src);
extern void func_0200f70c(int *p);
extern void func_0205958c(int *dst, void *p, unsigned int index);

void func_020592c8(char *obj)
{
    unsigned char *puVar2;
    unsigned char *puVar1;
    int pos[3];
    int base[3];
    int world[3];
    int local[3];

    func_0200f70c(pos);
    puVar2 = *(unsigned char **)(obj + 0xd8);
    if (puVar2 != 0) {
        puVar1 = func_020083a0(puVar2);
        func_0205958c(local, puVar2, *(unsigned char *)(obj + 0xdc));
        func_02008378(world, puVar1, local);
        func_020083b0(pos, world);
        func_0205958c(base, obj, *(unsigned char *)(obj + 0xdd));
        *(int *)(obj + 0xb0) = pos[0] - base[0];
        *(int *)(obj + 0xb4) = pos[1] - base[1];
        if (!((struct Flags020592c8 *)(obj + 0xde))->a &&
            ((struct Flags020592c8 *)(obj + 0xde))->b) {
            ((struct Flags020592c8 *)(obj + 0xde))->b = 0;
            *(int *)(obj + 0xd8) = 0;
        }
    }
}
