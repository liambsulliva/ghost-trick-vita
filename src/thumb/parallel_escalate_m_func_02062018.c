void func_02062018(int param_1, int param_2, int param_3)
{
    *(volatile unsigned int *)0x0400048c = (unsigned short)param_1 | ((unsigned int)(unsigned short)param_2 << 16);
    *(volatile unsigned int *)0x0400048c = (unsigned short)param_3;
}
