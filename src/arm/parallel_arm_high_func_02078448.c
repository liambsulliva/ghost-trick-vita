extern int func_020783c8();

int func_02078448(unsigned int param_1,unsigned int param_2,unsigned int param_3)
{
    unsigned int mapped;

    switch (param_1) {
    case 1:
        mapped = 1;
        break;
    case 3:
        mapped = 2;
        break;
    case 2:
        mapped = 3;
        break;
    default:
        mapped = 0;
        break;
    }
    if (mapped == 0) {
        return 65535;
    }
    return func_020783c8(mapped,0,0,param_2,param_3);
}
