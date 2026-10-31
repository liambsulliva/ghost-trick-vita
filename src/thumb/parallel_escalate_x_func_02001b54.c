struct Key
{
    unsigned int idx : 24;
    unsigned int hi : 8;
};

extern char data_020b18a0;
extern struct Key *func_02000d9c();
extern int func_02000f80();

#define GLOBAL_OBJ() (*(char **)(*(char **)((char *)&data_020b18a0 + 0x10) + 0x1580))

int func_02001b54(unsigned int param_1)
{
    int total;
    unsigned short i;
    char *rec;
    struct Key *other;
    int node;
    int r0v;

    total = 0;
    rec = (char *)func_02000d9c(GLOBAL_OBJ(), param_1, *(unsigned int *)(*(char **)(GLOBAL_OBJ() + 0x98) + 0x10));
    if (rec == 0) {
        return total - 5;
    }
    for (i = 0; i < 4; i = i + 1) {
        if (*(unsigned short *)(rec + 4 + i * 2) != 0xffff) {
            other = func_02000d9c(GLOBAL_OBJ(), *(unsigned short *)(rec + 4 + i * 2), *(unsigned int *)(*(char **)(GLOBAL_OBJ() + 0x98) + 0x14));
            if ((other->hi & 1) != 0) {
                r0v = func_02000f80(GLOBAL_OBJ(), rec, other, i);
                if ((unsigned int)(r0v + 3) <= 1) {
                    return r0v;
                }
                total = (total + r0v + 0x1f) & ~0x1fu;
            } else {
                node = (int)(*(char **)(GLOBAL_OBJ() + 0x90) + other->idx * 0x10);
                if (*(unsigned int *)(node + 0x14) == 0) {
                    total = total + ((*(unsigned int *)(node + 0x10) + 0x1f) & ~0x1fu);
                }
            }
        }
    }
    *(unsigned char *)(*(char **)((char *)&data_020b18a0 + 0x10) + 0x1b7d) = 0;
    return total + 0x20;
}
