extern void func_02039498(unsigned char *obj, int x, int y);

void func_02039414(unsigned char *param_1)
{
    switch (*(signed char *)(param_1 + 0xde)) {
    case -1:
        func_02039498(param_1, 0x50000, (int)0xfff8c000u);
        return;
    case 0:
        func_02039498(param_1, 0x14000, (int)0xfffc4000u);
        return;
    case 1:
        func_02039498(param_1, 0x8000, (int)0xfffe0000u);
        return;
    case 2:
        func_02039498(param_1, 0, 0);
        return;
    case 3:
        func_02039498(param_1, 0x8000, 0x20000);
        return;
    case 4:
        func_02039498(param_1, 0x14000, 0x3c000);
        return;
    case 5:
        func_02039498(param_1, 0x50000, 0x74000);
        break;
    }
}
