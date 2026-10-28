struct MotionQueue {
    unsigned char before_state[0xde];
    unsigned char state_low : 3;
    unsigned char state_clear : 1;
    unsigned char state_high : 4;
    unsigned char before_flags[0x61];
    unsigned char flags_low : 5;
    unsigned char first_flag : 1;
    unsigned char second_flag : 1;
    unsigned char flags_high : 1;
    unsigned char reserved[3];
    unsigned int motion_id;
    unsigned int motion_address;
};

void queue_motion(struct MotionQueue *object, unsigned int motion_address, unsigned int motion_id,
                  int first_flag, unsigned char second_flag)
{
    object->motion_address = motion_address;
    object->first_flag = (unsigned char)first_flag;
    object->second_flag = second_flag;
    object->motion_id = motion_id;
    object->state_clear = 0;
}
