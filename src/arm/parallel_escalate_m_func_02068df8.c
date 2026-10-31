struct LnkVramMan {
    void *pFreeList;
};

extern int func_02068b38(struct LnkVramMan *pMan, void **ppBlockPoolList, unsigned int *pRetAddr, unsigned int szByte);
extern struct LnkVramMan data_020cff8c;
extern struct LnkVramMan data_020cff90;
extern void *data_020cff94;

static inline unsigned int MakeTexKey(unsigned int addr, unsigned int size, int b4x4)
{
    return ((size >> 4) << 16) | (0xffff & (addr >> 3)) | ((unsigned int)b4x4 << 31);
}

unsigned int func_02068df8(unsigned int szByte, int is4x4comp, unsigned int opt)
{
    unsigned int size;
    unsigned int addr;
    int bSuccess;

    if (szByte == 0) {
        size = 16;
    } else {
        size = (szByte + 15) & ~15;
    }
    if (size >= 0x7fff0) {
        return 0;
    }
    if (is4x4comp) {
        bSuccess = func_02068b38(&data_020cff90, &data_020cff94, &addr, size);
    } else {
        bSuccess = func_02068b38(&data_020cff8c, &data_020cff94, &addr, size);
    }
    if (bSuccess) {
        return MakeTexKey(addr, size, is4x4comp);
    } else {
        return 0;
    }
}
