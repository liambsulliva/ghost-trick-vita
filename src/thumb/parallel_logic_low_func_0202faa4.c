extern int func_0201ee30(int name,unsigned int table);
extern int func_0202fb48(int object,int found,int start);

int func_0202faa4(int object,int name)
{
    int found;
    int start;

    found = func_0201ee30(name,(unsigned int)0x020bbbb4u);
    if (found != 0) {
        start = 0;
        start = ~start;
        if (func_0202fb48(object,found,start) != 0) {
            return 1;
        }
        return 0;
    }
    return 0;
}
