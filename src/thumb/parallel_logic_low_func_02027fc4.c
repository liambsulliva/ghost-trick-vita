extern int func_02028148(int object);
extern int func_02008828(int object,int value);
extern void func_02059234(int object,unsigned int value);

void func_02027fc4(int object,unsigned int flag)
{
    int p;
    unsigned char value;

    p = func_02028148(object);
    if (p != 0) {
        if (func_02008828(p,0x020cf8b4) != 0) {
            value = flag & 1;
            func_02059234(p,value);
        }
    }
}
