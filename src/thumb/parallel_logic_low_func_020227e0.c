extern int func_02067fac(int object,int cursor);

int func_020227e0(int object,int value)
{
    int self;
    int which;
    int node;

    self = object;
    which = value;
    node = func_02067fac(object + 0x10,0);
    self = self + 0x10;
    goto test;
body:
    if (*(int *)(node + 8) == which) {
        goto done;
    }
    node = func_02067fac(self,node);
test:
    if (node != 0) {
        goto body;
    }
done:
    return node;
}
