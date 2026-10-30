extern int data_020b1748;
void func_02072a18(int, int, int, unsigned int, int);
void func_02072ea8(int, int, unsigned int);

static inline void copy_dma_or_cpu(int dmaNo, int src, int dest, unsigned int size)
{
    if (dmaNo != -1 && size > 28) {
        func_02072a18(dmaNo, src, dest, size, 1);
        return;
    }
    func_02072ea8(src, dest, size);
}

void func_02070118(int param_1, int param_2, unsigned int param_3)
{
    copy_dma_or_cpu(data_020b1748, param_1, param_2 + 0x5000000, param_3);
}
