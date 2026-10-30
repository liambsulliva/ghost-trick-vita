extern int func_02078074();
extern unsigned int data_020d2ec0;

void func_0207816c(unsigned int param_1,unsigned int param_2,unsigned int param_3)
{
    unsigned int shifted;
    unsigned int index;
    unsigned int value;

    shifted = (param_2 & 0x7f00) << 8;
    value = param_2 & 255;
    index = shifted >> 16;
    if (param_3 != 0) {
        func_02078074(index == 97 || index == 98 ? 1 : 2,index,param_3,shifted);
        return;
    }
    index = index - 96;
    switch (index) {
    case 1:
        index = ((unsigned int *)&data_020d2ec0)[14];
        if (index != 0) {
            *(unsigned short *)index = (unsigned short)value;
        }
        value = 0;
        break;
    case 0:
        value = 0;
        break;
    case 2:
        break;
    case 3:
        index = (unsigned int)&data_020d2ec0;
        param_3 = 1;
        ((unsigned int *)index)[3] = param_3;
        break;
    default:
        break;
    }
    func_02078074(value,index,param_3,shifted);
}
