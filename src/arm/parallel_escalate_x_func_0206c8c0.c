typedef struct Pair6c8c0 {
    int first;
    int second;
} Pair6c8c0;

typedef struct Obj6c8c0 {
    char pad[0x18];
    int value;
} Obj6c8c0;

extern int data_020d0dc0;

Pair6c8c0 func_0206c8c0(Obj6c8c0 *object)
{
    Pair6c8c0 result;
    result.first = data_020d0dc0;
    result.second = object->value;
    return result;
}
