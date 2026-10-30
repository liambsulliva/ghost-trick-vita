extern void func_02075f84(void);

void func_02072db0(unsigned int param_1, unsigned int param_2)
{
    int n;
    unsigned int cnt;
    unsigned int timing;

    for (n = 0; n < 3; n++) {
        if (n == param_1) {
            continue;
        }
        cnt = *(volatile unsigned int *)(0x040000b8 + n * 12);
        if ((cnt & 0x80000000) == 0) {
            continue;
        }
        timing = cnt & 0x38000000;
        if (timing == param_2) {
            continue;
        }
        if (timing == 0x8000000 && param_2 == 0x10000000) {
            continue;
        }
        if (timing == 0x10000000 && param_2 == 0x8000000) {
            continue;
        }
        if (timing == 0x18000000 || timing == 0x20000000 || timing == 0x28000000 || timing == 0x30000000 || timing == 0x38000000 || timing == 0x8000000 || timing == 0x10000000) {
            func_02075f84();
        }
    }
}
