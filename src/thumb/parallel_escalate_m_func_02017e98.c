struct Owner {
    unsigned char pad[0x78];
    int first;
    int second;
    unsigned char pad2[0x8d - 0x80];
    unsigned char third;
    unsigned char fourth;
};
extern void func_0201d74c(void *part);
extern void *func_0201003c(void);
extern void func_02017ebc(void *object);

void func_02017e98(struct Owner *owner)
{
    func_0201d74c((char *)owner + 0xac);
    owner->first = 0;
    owner->third = 0;
    owner->second = 0;
    owner->fourth = 0;
    func_02017ebc(func_0201003c());
}
