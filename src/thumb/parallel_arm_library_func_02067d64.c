extern void func_02067dd0(unsigned int *);
extern void func_0200b7c0(unsigned int *);

void func_02067d64(unsigned int *object)
{
    if (object[4] != 0) {
        object[4] = 0;
        func_02067dd0(object);
    }
    func_0200b7c0(object + 10);
}
