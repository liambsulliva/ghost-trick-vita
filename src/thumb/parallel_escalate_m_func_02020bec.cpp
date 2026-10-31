class Target02020bec {
};

typedef void (Target02020bec::*Handler2_02020bec)(int *record, int index);
typedef void (Target02020bec::*Handler1_02020bec)(int *record);

struct Record02020bec {
    int a;
    int b;
    int c;
};

struct Owner02020bec {
    char before[8];
    Target02020bec *target;
    Record02020bec *records;
    char middle[0x1c - 0x10];
    Handler2_02020bec handler;
    char after[0x2c - 0x24];
    int index;
};

extern "C" {

int func_02020254(Owner02020bec *object);
int func_0202037c(Owner02020bec *object);
int func_0202038c(Owner02020bec *object);
void func_020083b0(int *dst, int *src);

void func_02020bec(Owner02020bec *object, int *record)
{
    Handler2_02020bec handler2;
    Handler1_02020bec handler1;

    if (func_02020254(object) == 0xe) {
        if (func_0202037c(object) != 0) {
            if (func_0202038c(object) != 0) {
                handler2 = object->handler;
                (object->target->*handler2)(record, object->index);
            } else {
                handler1 = reinterpret_cast<Handler1_02020bec>(object->handler);
                (object->target->*handler1)(record);
            }
        } else {
            func_020083b0((int *)&object->records[object->index], record);
        }
    }
}

}
