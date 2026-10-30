class Item {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual unsigned char width();
    virtual unsigned char height();
};

struct Holder {
    unsigned char pad[0x10];
    Item *item;
    unsigned char pad2[0x50 - 0x14];
    unsigned char width;
    unsigned char height;
};
extern "C" {
extern char data_020b9d6c[];
Item *func_0200dbcc(char *table, int value);
void func_0201784c(Holder *holder, int mode);

void func_02017714(Holder *holder, int mode)
{
    Item *item;
    item = func_0200dbcc(data_020b9d6c, mode);
    if (item != 0 && holder->item != item) {
        holder->item = item;
        holder->width = item->width();
        holder->height = holder->item->height();
    }
    switch (mode) {
    case 0:
    case 1:
    case 2:
        func_0201784c(holder, mode);
        break;
    }
}
}
