#include "mouse.h"

int mouse_hook_bit;
uint8_t data;

void (mouse_enable) (){
    uint8_t ack = 0;
    while(ack != MOUSE_ACK){
        sys_outb(KBD_CMD_BUF, KBC_MOUSE_COM);
        sys_outb(KBD_OUT_BUF, STREAM_MODE);
        util_sys_inb(KBD_OUT_BUF, &ack);
        printf("ack: %d \n", ack);
    }
    ack = 0;
    while(ack != MOUSE_ACK){
        sys_outb(KBD_CMD_BUF, KBC_MOUSE_COM);
        sys_outb(KBD_OUT_BUF, ENABLE_DATA);
        util_sys_inb(KBD_OUT_BUF, &ack);
        printf("ack: %d \n", ack);
    }
}

int mouse_subscribe_int(uint8_t *bit_no) {
    mouse_hook_bit = *bit_no;
    if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_bit) == OK) {
        return 0;
    }
    else return 1;
}

int (mouse_unsubscribe_int) () {
  if(sys_irqrmpolicy(&mouse_hook_bit) != OK) return 1;
  return 0;
}

void (assemble_bytes)(int *counter, uint8_t pac[3]){  //assemble bytes in pac[]
    if(*(counter) == 0){  //data & FIRST_BYTE_CHECK & 
        //printf("assmeble0: %d\n", (counter));
        pac[0] = data;
        if(data & BIT(3)) (*counter)++;
    }
    else{
        if(*(counter) == 1){
            //printf("assmeble1: %d\n", (counter));
            pac[1] = data;
            (*counter)++;
        }
        else if(*(counter) == 2){
            //printf("assmeble2: %d\n", (counter));
            pac[2] = data;
            (*counter)++;
        }
    }
    
    
}

void (build_packet)(int *counter, uint8_t pac[3], uint32_t *cnt){  //builds the struct packet to use 
    if(*(counter) == 3){
        //printf("in couter 3\n");
        struct packet pac_struct;

        for(int i = 0; i < 3; i++){
            pac_struct.bytes[i] = pac[i];
        }

        bool rb = pac[0] & MOUSE_RB;
        bool mb = pac[0] & MOUSE_MB;
        bool lb = pac[0] & MOUSE_LB;
        bool xov = pac[0] & MOUSE_XOV;
        bool yov = pac[0] & MOUSE_YOV;

        pac_struct.rb = rb;
        pac_struct.mb = mb;
        pac_struct.lb = lb;
        pac_struct.x_ov = xov;
        pac_struct.y_ov = yov;

        uint8_t msb_x = pac[0] << 3;
        msb_x >>= 7;

        uint8_t msb_y = pac[0] << 2;
        msb_y >>= 7;

        uint16_t x, y;
        //uint16_t all_ones = -1; 
        uint16_t mask = 0xFF00;

        if(msb_x)
            //x = pac[1] & all_ones;
            x = pac[1] | mask;
        else
            x = pac[1];

        if(msb_y)
            //y = pac[2] & all_ones;
            y = pac[2] | mask;
        else
            y = pac[2];

        pac_struct.delta_x = x;
        pac_struct.delta_y = y;

        //printf("before print packet\n");

        mouse_print_packet(&pac_struct);

        //printf("after print packet\n");

        (*cnt)--;
        (*counter) = 0;
    }
}

void (build_packet_nolim)(int *counter, uint8_t pac[3]){  //builds the struct packet to use 
    if(*(counter) == 3){
        //printf("in couter 3\n");
        struct packet pac_struct;

        for(int i = 0; i < 3; i++){
            pac_struct.bytes[i] = pac[i];
        }

        bool rb = pac[0] & MOUSE_RB;
        bool mb = pac[0] & MOUSE_MB;
        bool lb = pac[0] & MOUSE_LB;
        bool xov = pac[0] & MOUSE_XOV;
        bool yov = pac[0] & MOUSE_YOV;

        pac_struct.rb = rb;
        pac_struct.mb = mb;
        pac_struct.lb = lb;
        pac_struct.x_ov = xov;
        pac_struct.y_ov = yov;

        uint8_t msb_x = pac[0] << 3;
        msb_x >>= 7;

        uint8_t msb_y = pac[0] << 2;
        msb_y >>= 7;

        uint16_t x, y;
        //uint16_t all_ones = -1; 
        uint16_t mask = 0xFF00;

        if(msb_x)
            //x = pac[1] & all_ones;
            x = pac[1] | mask;
        else
            x = pac[1];

        if(msb_y)
            //y = pac[2] & all_ones;
            y = pac[2] | mask;
        else
            y = pac[2];

        pac_struct.delta_x = x;
        pac_struct.delta_y = y;

        //printf("before print packet\n");

        mouse_print_packet(&pac_struct);

        //printf("after print packet\n");

        (*counter) = 0;
    }
}

void (build_packet_ret)(int *counter, uint8_t pac[3], struct packet *p){  //builds the struct packet to use 
    if(*(counter) == 3){
        //printf("in couter 3\n");
        struct packet pac_struct;

        for(int i = 0; i < 3; i++){
            pac_struct.bytes[i] = pac[i];
        }

        bool rb = pac[0] & MOUSE_RB;
        bool mb = pac[0] & MOUSE_MB;
        bool lb = pac[0] & MOUSE_LB;
        bool xov = pac[0] & MOUSE_XOV;
        bool yov = pac[0] & MOUSE_YOV;

        pac_struct.rb = rb;
        pac_struct.mb = mb;
        pac_struct.lb = lb;
        pac_struct.x_ov = xov;
        pac_struct.y_ov = yov;

        uint8_t msb_x = pac[0] << 3;
        msb_x >>= 7;

        uint8_t msb_y = pac[0] << 2;
        msb_y >>= 7;

        uint16_t x, y;
        //uint16_t all_ones = -1; 
        uint16_t mask = 0xFF00;

        if(msb_x)
            //x = pac[1] & all_ones;
            x = pac[1] | mask;
        else
            x = pac[1];

        if(msb_y)
            //y = pac[2] & all_ones;
            y = pac[2] | mask;
        else
            y = pac[2];

        pac_struct.delta_x = x;
        pac_struct.delta_y = y;

        //printf("before print packet\n");

        mouse_print_packet(&pac_struct);

        *p = pac_struct;
        //printf("after print packet\n");

        (*counter) = 0;
    }
}


void (mouse_ih)(){
  util_sys_inb(KBD_OUT_BUF, & data);
}

void (disable_mouse_data)(){
    sys_outb(KBD_CMD_BUF, KBC_MOUSE_COM);
    sys_outb(KBD_OUT_BUF, DISABLE_DATA);
    sys_outb(KBD_CMD_BUF, KBC_MOUSE_COM);
    sys_outb(KBD_OUT_BUF, REMOTE_MODE);
}

bool (check_but)(struct packet p){
    return (p.rb | p.lb | p.mb);
}
