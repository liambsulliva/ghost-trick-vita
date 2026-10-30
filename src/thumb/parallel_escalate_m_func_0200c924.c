extern int func_0200cd30(int found,int extra);
#define ENTRY_AT(object,index) (*(int *)((object) + (index) * 4 + 0x1520))
int func_0200c924(int object,int index,int extra)
{
    int result;
    int found;
    result = -1;
    found = ENTRY_AT(object,index);
    if (found != 0) {
        result = func_0200cd30(found,extra);
    }
    return result;
}
