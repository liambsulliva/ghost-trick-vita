void func_0207254c(int param_1, unsigned int *param_2, unsigned char *param_3, unsigned int param_4)
{
    unsigned int crc;
    unsigned int count;
    unsigned char c;

    crc = *param_2;
    count = 0;
    if (count < param_4)
    {
        do
        {
            c = *param_3;
            param_3++;
            count++;
            crc = ((unsigned int *)param_1)[(crc ^ c) & 0xff] ^ (crc >> 8);
        } while (count < param_4);
    }
    *param_2 = crc;
}
