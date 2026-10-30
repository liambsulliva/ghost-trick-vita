struct Alloc {
    unsigned char used[0x20];
    unsigned char live[0x20];
    unsigned int next;
};

extern int func_02022498(struct Alloc *alloc, int index);
extern void func_020224e0(struct Alloc *alloc, unsigned char *bits, unsigned int index);
extern void func_020224f8(struct Alloc *alloc, unsigned char *bits, unsigned int index);

int func_02022400(struct Alloc *alloc)
{
    unsigned int i;
    int index;

    for (i = 0; i < 0x100; i++) {
        if (func_02022498(alloc, alloc->next) == 0) {
            index = alloc->next;
            alloc->next = (unsigned char)(index + 1);
            func_020224e0(alloc, alloc->used, index);
            func_020224f8(alloc, alloc->live, index);
            return index;
        }
        alloc->next++;
    }
    return -1;
}
