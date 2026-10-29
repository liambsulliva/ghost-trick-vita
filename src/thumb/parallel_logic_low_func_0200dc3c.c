extern int func_02031254(int size);
extern int func_0200e31c(int mem,unsigned char index,int flag);

int func_0200dc3c(int param_1,int param_2)
{
    int *slots;
    int mem;

    if (param_2 < 3) {
        slots = (int *)(param_1 + 0xc);
        if (slots[param_2] == 0) {
            mem = func_02031254(0x5c);
            if (mem != 0) {
                mem = func_0200e31c(mem,(unsigned char)param_2,1);
            }
            slots[param_2] = mem;
        }
        return slots[param_2];
    }
    return 0;
}
