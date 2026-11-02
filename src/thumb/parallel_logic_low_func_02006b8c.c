struct State {
    unsigned int before[4];
    int data;
    int ready;
};

extern struct State data_020b18a0;

void func_02006b8c(void)
{
    int off;

    off = 0x4cbc;
    *(unsigned int *)(data_020b18a0.data + off) = 0;
    *(unsigned int *)(data_020b18a0.data + (off + 4)) = 0;
    *(unsigned int *)(data_020b18a0.data + (off + 8)) = 0;
    *(unsigned char *)(data_020b18a0.data + (off - 2)) = 0;
    *(unsigned char *)(data_020b18a0.data + (off - 4)) = 0;
    *(unsigned int *)(data_020b18a0.data + (off + 12)) = 0;
    *(unsigned int *)(data_020b18a0.data + (off + 16)) = 2;
    *(unsigned char *)(data_020b18a0.data + (off - 3)) = 0;
    *(unsigned int *)(data_020b18a0.data + (off + 64)) = 0;
    *(unsigned int *)(data_020b18a0.data + (off + 68)) = 0;
    *(unsigned int *)(data_020b18a0.data + (off + 72)) = 0;
    *(unsigned int *)(data_020b18a0.data + (off + 76)) = 0;
    *(unsigned int *)(data_020b18a0.data + (off + 80)) = 0;
    *(unsigned int *)(data_020b18a0.data + (off + 84)) = 0;
    *(unsigned int *)(data_020b18a0.data + (off + 88)) = 0;
}
