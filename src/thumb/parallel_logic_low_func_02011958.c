extern void func_02014b4c(int object);
extern void func_02014a8c(int object,int a,int b,int c);

void func_02011958(int object,int a,int b)
{
    if (a != 0) {
        if (b != 0) {
            b = 1;
        }
        else {
            b = 0;
        }
        func_02014a8c(object + 0xf04,a,b,0);
        return;
    }
    func_02014b4c(object + 0xf04);
}
