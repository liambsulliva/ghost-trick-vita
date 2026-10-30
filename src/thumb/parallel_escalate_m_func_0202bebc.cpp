struct Data0202bebc {
    char pad[0x1e8];
    int table;
    unsigned int count;
};

struct Field0202bebc {
    unsigned char value;
};

class Obj0202bebc {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void slot7();
    virtual void slot8();
    virtual void slot9();
    virtual void slot10(Obj0202bebc *object, int key, int index);
    char pad04[8];
    short key;
    char pad0e[0x62];
    void *owner;
    char pad74[0xc];
    Data0202bebc *data;
};

struct Self0202bebc {
    char pad[0x80];
    Data0202bebc *data;
};

extern "C" {
extern char data_020ca210;
extern char data_020ca264;
extern char data_020ca280;
extern int func_0202b258(int key, int table, unsigned int count);
extern int func_0202bf50(Field0202bebc *object);
extern Obj0202bebc *func_0202b208(void *object);
extern Obj0202bebc *func_0202b21c(void *object);
extern Obj0202bebc *func_0202b230(void *object);
extern void func_0202b53c(Obj0202bebc *object);

Obj0202bebc *func_0202bebc(Self0202bebc *self, int key)
{
    int index;
    int kind;
    Obj0202bebc *object;

    index = func_0202b258(key, self->data->table, self->data->count);
    if (index == -1) {
        return 0;
    }
    kind = func_0202bf50((Field0202bebc *)(self->data->table + index * 16));
    object = 0;
    if (kind == 2) {
        object = func_0202b208(&data_020ca264);
    } else if (kind == 7) {
        object = func_0202b21c(&data_020ca210);
    } else if (kind == 0x27) {
        object = func_0202b230(&data_020ca280);
    }
    if (object != 0) {
        object->key = key;
        func_0202b53c(object);
        object->owner = self;
        object->data = self->data;
        object->slot10(object, key, index);
    }
    return object;
}
}
