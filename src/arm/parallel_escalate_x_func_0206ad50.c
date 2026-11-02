extern unsigned int func_02074f48(void);

static inline int NotInItcm(unsigned int start, unsigned int len)
{
    int ok;

    ok = 1;
    if (start + len > 0x01ff8000 && start < 0x02000000) {
        ok = 0;
    }
    return ok;
}

static inline int NotInDtcm(unsigned int start, unsigned int len)
{
    int ok;

    ok = 1;
    if (start + len > func_02074f48() && start < func_02074f48() + 0x4000) {
        ok = 0;
    }
    return ok;
}

int func_0206ad50(unsigned int dma, unsigned int dst, unsigned int src, unsigned int len)
{
    int result;
    int tcm_ok;
    int dma_ok;

    result = 0;
    tcm_ok = 0;
    dma_ok = 0;
    if (dma <= 3 && len != 0 && (dst & 0x1f) == 0) {
        if (NotInItcm(dst, len)) {
            dma_ok = 1;
        }
    }
    if (dma_ok) {
        if (NotInDtcm(dst, len)) {
            tcm_ok = 1;
        }
    }
    if (tcm_ok && ((src | len) & 0x1ff) == 0) {
        result = 1;
    }
    return result;
}
