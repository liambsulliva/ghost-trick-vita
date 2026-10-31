typedef enum {
    PXI_FIFO_SUCCESS = 0,
    PXI_FIFO_FAIL_SEND_ERR = -1,
    PXI_FIFO_FAIL_SEND_FULL = -2,
    PXI_FIFO_FAIL_RECV_ERR = -3,
    PXI_FIFO_FAIL_RECV_EMPTY = -4
} PXIFifoStatus;

typedef union {
    unsigned int raw;
    struct {
        unsigned int tag:5;
        unsigned int err:1;
        unsigned int data:26;
    } e;
} PXIFifoMessage;

typedef void (*PXIFifoCallback)(int tag, unsigned int data, int err);

extern PXIFifoCallback data_020d12a8[];
extern int func_02075c98(void);
extern int func_02075cac(int enabled);

#define reg_PXI_FIFO_CNT (*(volatile unsigned short *)0x04000184)
#define reg_PXI_SEND_FIFO (*(volatile unsigned int *)0x04000188)
#define reg_PXI_RECV_FIFO (*(volatile unsigned int *)0x04100000)

static inline PXIFifoStatus PXIi_GetFromFifo(unsigned int *data_buf)
{
    int enabled;

    if (reg_PXI_FIFO_CNT & 0x4000) {
        reg_PXI_FIFO_CNT |= (0x8000 | 0x4000);
        return PXI_FIFO_FAIL_RECV_ERR;
    }
    enabled = func_02075c98();
    if (reg_PXI_FIFO_CNT & 0x0100) {
        (void)func_02075cac(enabled);
        return PXI_FIFO_FAIL_RECV_EMPTY;
    }
    *data_buf = reg_PXI_RECV_FIFO;
    (void)func_02075cac(enabled);
    return PXI_FIFO_SUCCESS;
}

static inline PXIFifoStatus PXIi_SetToFifo(unsigned int data)
{
    int enabled;

    if (reg_PXI_FIFO_CNT & 0x4000) {
        reg_PXI_FIFO_CNT |= (0x8000 | 0x4000);
        return PXI_FIFO_FAIL_SEND_ERR;
    }
    enabled = func_02075c98();
    if (reg_PXI_FIFO_CNT & 0x0002) {
        (void)func_02075cac(enabled);
        return PXI_FIFO_FAIL_SEND_FULL;
    }
    reg_PXI_SEND_FIFO = data;
    (void)func_02075cac(enabled);
    return PXI_FIFO_SUCCESS;
}

void func_020762b8(void)
{
    PXIFifoMessage fifomessage;

    while (1) {
        PXIFifoStatus ret_code = PXIi_GetFromFifo(&fifomessage.raw);

        if (ret_code == PXI_FIFO_FAIL_RECV_EMPTY) {
            return;
        }
        if (ret_code == PXI_FIFO_FAIL_RECV_ERR) {
            continue;
        }
        {
            int tag = (int)fifomessage.e.tag;

            if (tag) {
                if (data_020d12a8[tag]) {
                    (data_020d12a8[tag])(tag, (unsigned int)fifomessage.e.data, (int)fifomessage.e.err);
                } else {
                    if (fifomessage.e.err) {
                    } else {
                        fifomessage.e.err = 1;
                        (void)PXIi_SetToFifo(fifomessage.raw);
                    }
                }
            }
        }
    }
}
