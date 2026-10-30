class Product {
public:
    virtual ~Product();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual int Make(int index,int first);
};
extern "C" {
struct Factory {
    Product *(**table)(struct Factory *factory);
};
extern char data_020bbb34;
extern struct Factory *func_0201ee6c(unsigned int id,void *name);
int func_0200dbcc(int *slots,int index)
{
    struct Factory *factory;
    Product *product;
    if (index < 3) {
        if (slots[index] == 0) {
            factory = func_0201ee6c(0x038e4166,&data_020bbb34);
            if (factory != 0) {
                product = factory->table[0](factory);
                if (product != 0) {
                    slots[index] = product->Make(index,index == 0);
                    delete product;
                }
            }
        }
        return slots[index];
    }
    return 0;
}
}
