typedef struct Query6c704 {
    int key;
    int value;
} Query6c704;

typedef struct Obj6c704 {
    char pad[0x10];
    Query6c704 *query;
    int state;
} Obj6c704;

extern int func_0206b9dc(Obj6c704 *obj, int a, int b);

int func_0206c704(Obj6c704 *obj, int key, int value)
{
    Query6c704 query;
    int one;
    obj->query = &query;
    query.key = key;
    query.value = value;
    one = 1;
    if (func_0206b9dc(obj, 0, one) != 0) {
        value = query.value;
    } else {
        value = one - 2;
        if (obj->state != 6) {
            value = query.value;
        }
    }
    return value;
}
