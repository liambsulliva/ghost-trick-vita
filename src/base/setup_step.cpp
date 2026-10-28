struct Step {
    unsigned char before[4];
    unsigned short field_4;
    unsigned char field_6;
};

struct Created {
    virtual ~Created();
};

extern "C" {
extern char data_020b4264;
int func_02069a68(void *object);
int func_02073a04(int value);
void *func_02008b0c(void *marker);
void func_02008b5c(void *created, void *part);
void func_02009750(void *object);
void func_0206a85c(int code);
void func_02009760(void *object);
void func_0200965c(void *object, int zero);

void func_02008c2c(struct Step *object)
{
    struct Created *created;
    object->field_4 = func_02073a04(func_02069a68(object));
    object->field_6 = 1;
    created = (struct Created *)func_02008b0c(&data_020b4264);
    if (created != 0) {
        func_02008b5c(created, (char *)object + 0x1c);
        if (created != 0) {
            delete created;
        }
    }
    func_02009750(object);
    func_0206a85c(0xd01);
    func_02009760(object);
    func_0200965c(object, 0);
}
}
