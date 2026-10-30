struct State020750b4 {
    unsigned int value;
    unsigned int pad;
    unsigned int raw;
    unsigned int extra;
};

extern struct State020750b4 data_020d11ac;
extern void func_0207512c(void);

void func_020750b4(void)
{
    unsigned int uVar4;
    unsigned int uVar3;
    unsigned int *puVar2;

    puVar2 = (unsigned int *)0x02fffd9c;
    uVar3 = *puVar2;
    uVar4 = 0;
    data_020d11ac.raw = uVar3;
    if ((0x2600000 <= uVar3) && (uVar3 < 0x2800000)) {
        uVar4 = 1;
    }
    if (uVar4 == 0) {
        uVar3 = 0;
    }
    data_020d11ac.value = uVar3;
    if (uVar3 == 0) {
        uVar4 = (unsigned int)func_0207512c;
        *puVar2 = uVar4;
        *(unsigned int *)((unsigned int)puVar2 & 0xff7fffff) = uVar4;
    }
    data_020d11ac.extra = 0;
}
