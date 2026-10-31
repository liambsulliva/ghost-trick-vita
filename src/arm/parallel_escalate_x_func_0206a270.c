typedef struct {
    int result;
    int pad[2];
    void *src;
    void *dest;
    unsigned int len;
    int pad2[1];
    unsigned int page_size;
    unsigned int sect_size;
} CommandArg;

typedef struct {
    CommandArg *cmd;
    volatile unsigned int flag;
    char pad0[0x4f4];
    unsigned int src;
    unsigned int dst;
    unsigned int len;
    char pad1[8];
    int req_type;
    int req_retry;
    int req_mode;
    char pad2[0x24];
    unsigned char backup_cache_page_buf[256];
} CardCommon;

extern CardCommon data_020d02a0;

extern void func_02000b64(unsigned int sym);
extern unsigned int func_0206a89c(void);
extern int func_0206a120(CardCommon *p, int req_type, int retry_count);
extern void func_02074844(void *start, unsigned int size);
extern void func_0207487c(void *start, unsigned int size);
extern void func_020748a0(void);
extern void func_0207303c(const void *src, void *dst, unsigned int size);

void func_0206a270(CardCommon *p)
{
    int req_type = p->req_type;
    int req_mode = p->req_mode;
    int retry_count = p->req_retry;
    unsigned int size = 256;

    func_02000b64(0x02000bac);
    if (req_type == 11) {
        size = func_0206a89c();
    } else if (req_type == 15) {
        size = data_020d02a0.cmd->sect_size;
    }
    for (;;) {
        const unsigned int len = (size < p->len) ? size : p->len;
        p->cmd->len = len;
        if ((p->flag & 0x40) != 0) {
            p->flag &= ~0x40;
            p->cmd->result = 7;
            break;
        }
        switch (req_mode) {
        case 0:
            func_02074844(data_020d02a0.backup_cache_page_buf, len);
            p->cmd->src = (void *)p->src;
            p->cmd->dest = data_020d02a0.backup_cache_page_buf;
            break;
        case 1:
        case 2:
            func_0207303c((const void *)p->src, data_020d02a0.backup_cache_page_buf, len);
            func_0207487c(data_020d02a0.backup_cache_page_buf, len);
            func_020748a0();
            p->cmd->src = data_020d02a0.backup_cache_page_buf;
            p->cmd->dest = (void *)p->dst;
            break;
        case 3:
            p->cmd->src = (void *)p->src;
            p->cmd->dest = (void *)p->dst;
            break;
        }
        if (!func_0206a120(p, req_type, retry_count))
            break;
        if (req_mode == 2) {
            if (!func_0206a120(p, 9, 1))
                break;
        } else if (req_mode == 0) {
            func_0207303c(data_020d02a0.backup_cache_page_buf, (void *)p->dst, len);
        }
        p->src += len;
        p->dst += len;
        p->len -= len;
        if (p->len == 0)
            break;
    }
}
