typedef struct {
    int m[16];
} MtxFx44;

void func_0206f6a8(const MtxFx44 *mtx)
{
    *(volatile int *)0x04000440 = 3;
    *(volatile int *)0x04000458 = mtx->m[0];
    *(volatile int *)0x04000458 = mtx->m[1];
    *(volatile int *)0x04000458 = mtx->m[2];
    *(volatile int *)0x04000458 = mtx->m[3];
    *(volatile int *)0x04000458 = mtx->m[4];
    *(volatile int *)0x04000458 = mtx->m[5];
    *(volatile int *)0x04000458 = mtx->m[6];
    *(volatile int *)0x04000458 = mtx->m[7];
    *(volatile int *)0x04000458 = mtx->m[8] << 4;
    *(volatile int *)0x04000458 = mtx->m[9] << 4;
    *(volatile int *)0x04000458 = mtx->m[10] << 4;
    *(volatile int *)0x04000458 = mtx->m[11] << 4;
    *(volatile int *)0x04000458 = mtx->m[12] << 4;
    *(volatile int *)0x04000458 = mtx->m[13] << 4;
    *(volatile int *)0x04000458 = mtx->m[14] << 4;
    *(volatile int *)0x04000458 = mtx->m[15] << 4;
}
