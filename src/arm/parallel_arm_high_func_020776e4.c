void func_020768a8(void);
void func_020768bc(void);
void func_02074860(unsigned char *p,unsigned int n);

void func_020776e4(unsigned char *object,unsigned int index,unsigned int value)
{
    func_020768a8();
    *(unsigned int *)(object + index * 4 + 60) = value;
    func_02074860(object + 60 + index * 4,4);
    func_020768bc();
}
