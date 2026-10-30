extern void func_020411d0(int object);
extern void func_02040600(int object);
extern void func_02031270(int object);

static inline void delete_object(int object)
{
    if (object != 0) {
        func_02040600(object);
        func_02031270(object);
    }
}

void func_020405d8(char *self)
{
    int object;

    if (*(int *)(self + 0x38) != 0) {
        func_020411d0(*(int *)(self + 0x38));
        object = *(int *)(self + 0x38);
        if (object != 0) {
            delete_object(object);
            *(int *)(self + 0x38) = 0;
        }
    }
}
