struct State {
    unsigned int before[4];
    int data;
};

extern struct State data_020b18a0;
extern void func_020010a0(void *a, int size);
extern int *func_02000d9c(int obj, int key, int c);
extern int func_02000d64(int obj, int id, unsigned char *entry, int size, int flag);

void func_02005f7c(int ch, int key)
{
    int *src;
    unsigned char *base;
    unsigned char *entry;
    int obj;
    int ret;
    unsigned char head;
    unsigned char next;
    unsigned char prev;

    base = (unsigned char *)(data_020b18a0.data + 0x3d28 + ch * 0x3a4);
    head = base[0x3a1];
    next = (head + 1) & 7;
    if (next != base[0x3a0]) {
        entry = base + 0x120 + head * 0x50;
        func_020010a0(entry, 0x50);
        obj = *(int *)(data_020b18a0.data + 0x1580);
        src = func_02000d9c(obj, key, *(int *)(*(int *)(obj + 0x98) + 0x24));
        if (src != 0) {
            ret = func_02000d64(*(int *)(data_020b18a0.data + 0x1580), src[0], entry, 0x40, 0);
            if ((unsigned int)(ret + 3) > 1) {
                if (base[0x3a0] != base[0x3a1]) {
                    prev = (base[0x3a1] - 1) & 7;
                    *(base + prev * 0x50 + 0x139) = 0;
                }
                *(unsigned short *)(entry + 0x40) = key;
                *(int *)(entry + 0x44) = src[0];
                entry[0x4c] = ((unsigned char *)src)[4];
                entry[0x4d] = ((unsigned char *)src)[5];
                entry[0x4e] = ((unsigned char *)src)[6];
                entry[0x4f] = ((unsigned char *)src)[7];
                *(int *)(entry + 0x48) = *(int *)(*(int *)(*(int *)(data_020b18a0.data + 0x1580) + 0x90) + src[0] * 16 + 0xc);
                base[0x3a1] = next;
            }
        }
    }
}
