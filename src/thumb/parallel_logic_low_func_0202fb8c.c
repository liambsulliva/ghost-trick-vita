extern int func_0201ee30(int object,unsigned int table);
extern int func_0202fb48(int object,int found,int extra);

int func_0202fb8c(int object,int name,int extra)
{
    int found;

    found = func_0201ee30(name,(unsigned int)0x020bbbb4u);
    if (found != 0) {
        return func_0202fb48(object,found,extra);
    }
    return 0;
}
