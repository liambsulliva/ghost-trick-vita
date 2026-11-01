typedef struct Obj0c470 {
    char pad[0x400];
    unsigned int entries[32][4];
    char pad2[0x482];
    unsigned char count;
} Obj0c470;

extern void func_0207303c(const void *src, void *dst, unsigned int size);

int func_0200c470(Obj0c470 *obj, unsigned int *entry)
{
    int n;
    unsigned int *last;
    unsigned int i;
    if (obj->count < 32) {
        n = obj->count;
        if (n < 2) {
            func_0207303c(entry, obj->entries[n], 16);
            return obj->count++;
        }
        last = obj->entries[n - 1];
        for (i = 0; i < 4; i++) {
            if (entry[i] != last[i]) {
                func_0207303c(entry, obj->entries[n], 16);
                return obj->count++;
            }
        }
        return n - 1;
    }
    return -1;
}
